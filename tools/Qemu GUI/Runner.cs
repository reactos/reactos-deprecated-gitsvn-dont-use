using System;
using System.IO;
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
        DebugForm output;
        public string temp_path;

        public Runner(Data In)
        {
            p = new Process();
            data = In;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.EnableRaisingEvents = true;
            p.Exited += new EventHandler(ProcessStop);

            //FIXME: remove when pipe client works
            temp_path = Application.StartupPath + "\\";
        }

        public bool StartQemu(Platforms Platform)
        {
            switch (Platform)
            {
                case Platforms.x86:
                case Platforms.x86_ISA:
                    p.StartInfo.FileName = data.Paths.Qemu + "\\qemu.exe";
                    break;
                case Platforms.x64:
                case Platforms.x64_ISA:
                    p.StartInfo.FileName = data.Paths.Qemu + "\\qemu-system-x86_64.exe";
                    break;
                case Platforms.ARM_integratorcp1026:
                case Platforms.ARM_integratorcp926:
                case Platforms.ARM_versatileab:
                case Platforms.ARM_versatilepb:
                    p.StartInfo.FileName = data.Paths.Qemu + "\\qemu-system-arm.exe";
                    break;
                case Platforms.PPC_g3bw:
                case Platforms.PPC_mac99:
                case Platforms.PPC_prep:
                    p.StartInfo.FileName = data.Paths.Qemu + "\\qemu-system-ppc.exe";
                    break;
                case Platforms.Sparc_sun4m:
                    p.StartInfo.FileName = data.Paths.Qemu + "\\qemu-system-sparc.exe";
                    break;
            }

            if (data.Debug.SerialPort.SRedirect)
            {
                /* create a random name */
                //FIXME: rewrite when pipe client works
                string filename = "serial" + DateTime.UtcNow.Ticks.ToString() + ".txt";
                data.Debug.SerialPort.FileName = temp_path + filename;
            }

            try
            {
                p.StartInfo.WorkingDirectory = data.Paths.Qemu;
                
                if (data.Debug.SerialPort.SRedirect)
                {
                   
                    output = new DebugForm();

                    /* create a random name */
                    //FIXME: rewrite when pipe client works
                    string filename = "serial" + DateTime.UtcNow.Ticks.ToString() + ".txt";
                    data.Debug.SerialPort.FileName = temp_path + filename;
                    if (File.Exists(data.Debug.SerialPort.FileName))
                    {
                        try
                        {
                            File.Delete(data.Debug.SerialPort.FileName);
                        }
                        catch { }
                    }
                }
                p.StartInfo.Arguments = data.GetArgv();
            }
            catch (Exception e)
            {
                MessageBox.Show("Invalid path or arguments. Your settings may be corrupt. \nException Information: " + e.Message, "Error");
                return false;
            }

            /* show the command line */
            ErrBuffer = "Path:" + Environment.NewLine + p.StartInfo.FileName.ToString() + Environment.NewLine + "Arguments:" + Environment.NewLine + data.GetArgv();

            try
            {
                p.Start();
                if (data.Debug.SerialPort.SRedirect)
                {
                    output = new DebugForm();
                    output.Listen(p, data.Debug.SerialPort.FileName);
                }
            }
            catch (Exception e)
            {
                ErrBuffer += Environment.NewLine + "Exception: " + e.Message;
                ErrorForm error = new ErrorForm();
                error.txtError.Text = ErrBuffer;
                error.Show();
                return false;
            }
            return true;
        }

        public bool CreateImage(string FileName, long Size, string Format)
        {
            long d = Size * 1024;
            string argv = " create -f " + Format + " \"" + FileName + "\" " + d.ToString();

            p.StartInfo.FileName = data.Paths.Qemu + "\\qemu-img.exe";
            p.StartInfo.WorkingDirectory = data.Paths.Qemu;
            p.StartInfo.Arguments = argv;
            try
            {
                p.Start();
            }
            catch(Exception e)
            {
                ErrBuffer += Environment.NewLine + "Error: " + e.Message;
                ErrorForm error = new ErrorForm();
                error.txtError.Text = ErrBuffer;
                error.txtError.Text += p.StandardError.ReadToEnd();
                error.ShowDialog();
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
            bool success = StartVdkService();

            if (success == true)
            {
                p.StartInfo.FileName = data.Paths.VDK + "\\vdk.exe";
                p.StartInfo.WorkingDirectory = data.Paths.VDK;
                p.StartInfo.Arguments = "open 0 " + "\"" + data.Tools.vdk.Image + "\" /RW /L:" + data.Tools.vdk.DriveLetter;

                try
                {
                    p.Start();
                }
                catch (Exception e)
                {
                    ErrBuffer = e.Message;
                    success = false;
                }
            }
            
            if (success == false)
            {
                ErrorForm error = new ErrorForm();
                error.txtError.Text = ErrBuffer;
                error.txtError.Text += p.StandardOutput.ReadToEnd();//vdk does not use stderr
                error.ShowDialog();
            }
            return success;
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
            catch (Exception e)
            {
                ErrBuffer += Environment.NewLine + "Error: " + e.Message;
                ErrorForm error = new ErrorForm();
                error.txtError.Text = ErrBuffer;
                error.txtError.Text += p.StandardError.ReadToEnd();
                error.ShowDialog();
                return false;
            }
            return true;
        }

        private bool StartVdkService()
        {
            string buffer;

            p.StartInfo.FileName = data.Paths.VDK + "\\vdk.exe";
            p.StartInfo.WorkingDirectory = data.Paths.VDK;
            p.StartInfo.Arguments = "start";
            try
            {
                p.Start();
            }
            catch (Exception e)
            {
                ErrBuffer = e.Message;
                return false;
            }

            buffer = p.StandardOutput.ReadToEnd();

            if (buffer.Contains("Failed to start the Virtual Disk Driver.") == true)
            {
                ErrBuffer = buffer;
                return false;
            }
            else
                return true;

        }

        public void ProcessStop(object sender, EventArgs e)
        {
            string buff = p.StandardError.ReadToEnd();
            ErrBuffer += Environment.NewLine + "Error:" + Environment.NewLine + buff;
            if (buff.Length > 0)
            {
                ErrorForm error = new ErrorForm();
                error.txtError.Text = ErrBuffer;
                error.ShowDialog();
            }
        }
    }

}
