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
        string machineName = "ReactOS";
        int maxRetries = 3;
        int numStages = 3;
        int vmTimeout = 5 * 1000; // 120 secs

        IMachine rosVM;
        IVirtualBox vBox;

        enum ContinueType
        {
            EXIT_CHECKPOINT_REACHED,
            EXIT_CONTINUE,
            EXIT_DONT_CONTINUE
        }

        const String stageCheckpoint = "SYSREG_CHECKPOINT:THIRDBOOT_COMPLETE";

        public RegTester()
        {
            /* Create VBox instance */
            vBox = new VirtualBox.VirtualBox();
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
                            Console.WriteLine("[SYSREG] Timeout");
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
                        else if (stage == 2 && line.Contains(stageCheckpoint))
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

        public void RunTests()
        {
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
            //vmProgress.WaitForCompletion(-1);

            // TODO: Empty the HDD, prepare for the first run

            /* Empty the debug log file */
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

            /* Close VM session */
            vmSession.Close();

            /* Start main testing loop */
            for (int stage = 0; stage < numStages; stage++)
            {
                for (int retries = 0; retries < maxRetries; retries++)
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

                        ContinueType Ret = ProcessDebugOutput(vmSession, stage);

                        /* Kill the VM */
                        vmProgress = vmSession.Console.PowerDown();
                        vmProgress.WaitForCompletion(-1);

                        /* If we have a checkpoint to reach for success, assume that
                           the application used for running the tests (probably "rosautotest")
                           continues with the next test after a VM restart. */
                        if (stage == 2 && Ret == ContinueType.EXIT_CONTINUE)
                            Console.WriteLine("[SYSREG] Rebooting VM (retry {0})", retries + 1);
                        else
                            break;

                        /* Close VM session */
                        vmSession.Close();
                    }
                    catch (Exception exc)
                    {
                        Console.WriteLine("Running the VM failed with exception: " + exc.ToString());
                        break;
                    }

                    break;
                }

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
