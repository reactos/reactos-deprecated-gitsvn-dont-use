using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Windows.Forms;

namespace Qemu_GUI
{
    public class Runner
    {
        private Data data;
        private string ErrBuffer = "";
        private Process p;

        public Runner(Data In)
        {
            p = new Process();
            data = In;
            p.StartInfo.RedirectStandardError = true;
            //p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.EnableRaisingEvents = true;
            p.Exited += new EventHandler(ListenerStop);
        }

        public bool StartQemu(Platforms Platform)
        {
            switch (Platform)
            {
                case Platforms.x86:
                case Platforms.x86_ISA:
                    p.StartInfo.FileName = data.Paths.QEmu + "\\qemu.exe";
                    break;
                case Platforms.x64:
                case Platforms.x64_ISA:
                    p.StartInfo.FileName = data.Paths.QEmu + "\\qemu-system-x86_64.exe";
                    break;
                case Platforms.ARM_integratorcp1026:
                case Platforms.ARM_integratorcp926:
                case Platforms.ARM_versatileab:
                case Platforms.ARM_versatilepb:
                    p.StartInfo.FileName = data.Paths.QEmu + "\\qemu-system-arm.exe";
                    break;
                case Platforms.PPC_g3bw:
                case Platforms.PPC_mac99:
                case Platforms.PPC_prep:
                    p.StartInfo.FileName = data.Paths.QEmu + "\\qemu-system-ppc.exe";
                    break;
                case Platforms.Sparc_sun4m:
                    p.StartInfo.FileName = data.Paths.QEmu + "\\qemu-system-sparc.exe";
                    break;
            }

            try
            {
                p.StartInfo.WorkingDirectory = data.Paths.QEmu;
                p.StartInfo.Arguments = data.GetArgv();
            }
            catch(Exception e)
            {
                MessageBox.Show("Invalid path or arguments.\nException Information: " + e.Message , "Error");
                return false;
            }

            /* show the command line */
            ErrBuffer = "Path:" + p.StartInfo.FileName.ToString() + " \n\rArguments:" + data.GetArgv();

            try
            {
                p.Start();
            }
            catch
            {
                ErrBuffer += "\n\rError: " + p.StartInfo.FileName + " not found!";
                MessageBox.Show(ErrBuffer, "Error");
                return false;
            }
            return true;
        }

        public bool CreateImage(string FileName, long Size, string Format)
        {
            long d = Size * 1024;
            string argv = " create -f " + Format + " \"" + FileName + "\" " + d.ToString(); ;

            p.StartInfo.FileName = data.Paths.QEmu + "\\" + "qemu-img.exe";
            p.StartInfo.WorkingDirectory = data.Paths.QEmu;
            p.StartInfo.Arguments = argv;
            try
            {
                p.Start();
            }
            catch
            {
                ErrBuffer += "\n\rError: " + p.StartInfo.FileName + " not found!";
                MessageBox.Show(ErrBuffer, "Error");
                return false;
            }
            return true;
        }

        public string GetErrorBuffer()
        {
            return ErrBuffer;
        }

        public bool MountImage()
        {
            p.StartInfo.FileName = data.Paths.VDK + "\\vdk.exe";
            p.StartInfo.WorkingDirectory = data.Paths.VDK;
            p.StartInfo.Arguments = "open 0 " + "\"" + data.Tools.vdk.Image + "\" /RW /L:" + data.Tools.vdk.DriveLetter;

            try
            {
                p.Start();
            }
            catch
            {
                ErrBuffer += "\n\rError: " + p.StartInfo.FileName + " not found!";
                MessageBox.Show(ErrBuffer, "Error");
                return false;
            }
            return true;
        }

        public bool UnmountImage()
        {
            p.StartInfo.FileName = data.Paths.VDK + "\\vdk.exe";
            p.StartInfo.WorkingDirectory = data.Paths.VDK;
            p.StartInfo.Arguments = "CLOSE * /F";

            try
            {
                p.Start();
            }
            catch
            {
                ErrBuffer += "\n\rError: " + p.StartInfo.FileName + " not found!";
                MessageBox.Show(ErrBuffer, "Error");
                return false;
            }
            return true;
        }

        public void ListenerStop(object sender, EventArgs e)
        {
            string buff = p.StandardError.ReadToEnd();
            ErrBuffer += "\n\rError:\n\r" + buff;
            if(buff.Length > 0)
                MessageBox.Show(ErrBuffer, "Program wrote to error stream!");
        }
    }

}
