/*
 * PROJECT:     ReactOS System Regression Testing Utility, Windows/VirtualBox version
 * LICENSE:     GNU GPLv2 or any later version as published by the Free Software Foundation
 * PURPOSE:     Processing the incoming debugging data
 * COPYRIGHT:   Copyright Aleksey Bragin <aleksey@reactos.org>. Based around ideas and code from
 *              sysreg created by Christoph von Wittich <christoph_vw@reactos.org> and
 *              Colin Finck <colin@reactos.org>
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;
using VirtualBox;

namespace sysreg3
{
    class RegTester
    {
        string machineName = "ReactOS Testbot";
        string dbgPortPath = "C:\\testbot.txt";
        int maxRetries = 3;
        int numStages = 3;
        int vmTimeout = 120 * 1000; // 120 secs

        ulong hddSize = 2048;

        IMachine rosVM;
        IVirtualBox vBox;

        enum ContinueType
        {
            EXIT_CHECKPOINT_REACHED,
            EXIT_CONTINUE,
            EXIT_DONT_CONTINUE
        }

        String[] stageCheckpoint;

        public RegTester()
        {
            /* Create VBox instance */
            vBox = new VirtualBox.VirtualBox();

            stageCheckpoint = new String[3];
            stageCheckpoint[0] = "It's the final countdown...";
            stageCheckpoint[1] = "It's the final countdown...";
            stageCheckpoint[2] = "SYSREG_CHECKPOINT:THIRDBOOT_COMPLETE";
        }

        private ContinueType ProcessDebugOutput(ISession vmSession, int stage)
        {
            ContinueType Result = ContinueType.EXIT_DONT_CONTINUE;

            /* Get the serial port */
            ISerialPort dbgPort = vmSession.Machine.GetSerialPort(0);

            Stream stream = new FileStream(dbgPort.Path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            StreamReader sr = new StreamReader(stream);

            try
            {
                string line;
                int kdbgHit = 0;
                bool quitLoop = false;

                while (!quitLoop)
                {
                    /* Poll the stream every 1 sec */
                    int waitingTime = 0;
                    while (sr.EndOfStream)
                    {
                        waitingTime += 1000;
                        Thread.Sleep(1000);

                        /* Peek a byte to update the EndOfStream value */
                        sr.Peek();

                        if (waitingTime >= vmTimeout)
                        {
                            /* We hit the timeout, quit */
                            Console.WriteLine("[SYSREG] timeout");
                            Result = ContinueType.EXIT_CONTINUE;
                            quitLoop = true;
                            break;
                        }
                    }

                    while ((line = sr.ReadLine()) != null)
                    {
                        Console.WriteLine(line);

                        /* Check for magic sequences */
                        if (line == "kdb:> ")
                        {
                            kdbgHit++;

                            if (kdbgHit == 1)
                            {
                                /* It happened for the first time, backtrace */
                                vmSession.Console.Keyboard.PutScancode(0x30); // b make
                                vmSession.Console.Keyboard.PutScancode(0xb0); // b release
                                vmSession.Console.Keyboard.PutScancode(0x14); // t make
                                vmSession.Console.Keyboard.PutScancode(0x94); // t release
                                vmSession.Console.Keyboard.PutScancode(0x1c); // Enter make
                                vmSession.Console.Keyboard.PutScancode(0x9c); // Enter release

                                continue;
                            }
                            else
                            {
                                /* It happened once again, no reason to continue */
                                Console.WriteLine();
                                Result = ContinueType.EXIT_CONTINUE;
                                quitLoop = true;
                                break;
                            }
                        }
                        else if (line.Contains("--- Press q"))
                        {
                            /* Send Return to get more data from Kdbg */
                            vmSession.Console.Keyboard.PutScancode(0x1c); // Enter make
                            vmSession.Console.Keyboard.PutScancode(0x9c); // Enter release
                            continue;
                        }
                        else if (line.Contains("SYSREG_ROSAUTOTEST_FAILURE"))
                        {
                            quitLoop = true;
                            break;
                        }
                        else if (line.Contains(stageCheckpoint[stage]))
                        {
                            Result = ContinueType.EXIT_CHECKPOINT_REACHED;
                            quitLoop = true;
                            break;
                        }
                    }
                }
            }
            catch (Exception e)
            {
                // Let the user know what went wrong.
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
            }
            finally
            {
                sr.Close();
            }

            return Result;
        }

        private void CreateHardDisk(Session vmSession)
        {
            IMedium rosHdd;
            IProgress progress;
            IStorageController controller;

            /* Create the hdd and storage */
            rosHdd = vBox.CreateHardDisk(null, "ReactOS Testbot.vdi");
            progress = rosHdd.CreateBaseStorage(hddSize, MediumVariant.MediumVariant_Standard);
            progress.WaitForCompletion(-1);

            /* Attach it to the vm */
            controller = rosVM.GetStorageControllerByInstance(0);
            vmSession.Machine.SaveSettings();
            vmSession.Machine.AttachDevice(controller.Name, 0, 0, DeviceType.DeviceType_HardDisk, rosHdd.Id);
            vmSession.Machine.SaveSettings();
        }

        private void EmptyHardDisk(Session vmSession)
        {
            IProgress progress;
            IStorageController controller;
            IMedium rosHdd;// = rosVM.GetMedium("IDE Controller", 0, 0);

            try
            {
                rosHdd = vBox.FindHardDisk("ReactOS Testbot.vdi");
            }
            catch (Exception exc)
            {
                /* Opening failed. Probably we need to create it */
                //Console.WriteLine("[SYSREG] Finding hdd failed: " + exc.ToString());
                CreateHardDisk(vmSession);
                return;
            }

            /* Delete the existing hdd */
            controller = rosVM.GetStorageControllerByInstance(0);
            vmSession.Machine.DetachDevice(controller.Name, 0, 0);
            vmSession.Machine.SaveSettings();

            progress = rosHdd.DeleteStorage();
            progress.WaitForCompletion(-1);
            rosHdd.Close();

            /* Create a new one */
            CreateHardDisk(vmSession);
        }

        private void EmptyDebugLog(Session vmSession)
        {
            try
            {
                ISerialPort dbgPort = vmSession.Machine.GetSerialPort(0);
                FileStream dbgFile = File.Open(dbgPort.Path, FileMode.Truncate);
                dbgFile.Close();
            }
            catch
            {
                /* Don't care about the exceptions here */
            }
        }

        private void ConfigVm(Session vmSession)
        {
            /* Check serial port */
            ISerialPort dbgPort = vmSession.Machine.GetSerialPort(0);

            if (dbgPort.Enabled == 0)
            {
                /* Create it */
                dbgPort.Enabled = 1;
                dbgPort.Path = dbgPortPath;
                dbgPort.HostMode = PortMode.PortMode_RawFile;
            }
        }

        public void RunTests()
        {
            ContinueType Ret = ContinueType.EXIT_DONT_CONTINUE;
            IProgress vmProgress;

            // TODO: Load settings

            /* Open the testing machine */
            Session vmSession = new Session();

            try
            {
                rosVM = vBox.FindMachine(machineName);
            }
            catch (Exception exc)
            {
                /* Opening failed. Probably we need to create it */
                Console.WriteLine("Opening the vm failed " + exc.ToString());
                return;
            }

            vBox.OpenSession(vmSession, rosVM.Id);

            /* Configure the virtual machine */
            ConfigVm(vmSession);

            /* Empty or create the HDD, prepare for the first run */
            EmptyHardDisk(vmSession);

            /* Empty the debug log file */
            EmptyDebugLog(vmSession);

            /* Close VM session */
            vmSession.Close();

            /* Start main testing loop */
            for (int stage = 0; stage < numStages; stage++)
            {
                int retries;
                for (retries = 0; retries < maxRetries; retries++)
                {
                    /* Start the VM */
                    try
                    {
                        vmProgress = vBox.OpenRemoteSession(vmSession, rosVM.Id, "gui", null);
                        vmProgress.WaitForCompletion(-1);

                        if (vmProgress.ResultCode != 0)
                        {
                            /* Print out error's text */
                            Console.WriteLine("Error starting VM: " + vmProgress.ErrorInfo.Text);

                            /* Close VM session */
                            vmSession.Close();
                            break;
                        }

                        Console.WriteLine(); Console.WriteLine(); Console.WriteLine();
                        Console.WriteLine("[SYSREG] Running stage {0}...", stage + 1);
                        Console.WriteLine("[SYSREG] Domain {0} started.\n", rosVM.Name);

                        Ret = ProcessDebugOutput(vmSession, stage);

                        /* Kill the VM */
                        vmProgress = vmSession.Console.PowerDown();
                        vmProgress.WaitForCompletion(-1);

                        /* If we have a checkpoint to reach for success, assume that
                           the application used for running the tests (probably "rosautotest")
                           continues with the next test after a VM restart. */
                        if (stage == 2 && Ret == ContinueType.EXIT_CONTINUE)
                            Console.WriteLine("[SYSREG] Rebooting VM (retry {0})", retries + 1);
                        else
                        {
                            /* Empty the debug log file */
                            EmptyDebugLog(vmSession);

                            /* Close VM session */
                            vmSession.Close();

                            break;
                        }
                    }
                    catch (Exception exc)
                    {
                        Console.WriteLine("[SYSREG] Running the VM failed with exception: " + exc.ToString());
                        //break;
                    }
                }

                /* Wait till the machine state is actually closed (no vmProgress alas) */
                //while (vmSession.State != SessionState.SessionState_Closed) Thread.Sleep(1000);

                /* Check for a maximum number of retries */
                if (retries == maxRetries)
                {
                    Console.WriteLine("[SYSREG] Maximum number of allowed retries exceeded, aborting!");
                    break;
                }

                /* Stop executing if asked so */
                if (Ret == ContinueType.EXIT_DONT_CONTINUE) break;
            }

            switch (Ret)
            {
                case ContinueType.EXIT_CHECKPOINT_REACHED:
                    Console.WriteLine("[SYSREG] Status: Reached the checkpoint!");
                    break;
                case ContinueType.EXIT_CONTINUE:
                    Console.WriteLine("[SYSREG] Status: Failed to reach the checkpoint!!");
                    break;
                case ContinueType.EXIT_DONT_CONTINUE:
                    Console.WriteLine("[SYSREG] Status: Testing process aborted!");
                    break;
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            RegTester regTester = new RegTester();
            regTester.RunTests();
        }
    }
}
