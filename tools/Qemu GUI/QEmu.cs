using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Xml;
using System.Xml.Serialization;
using System.IO;

namespace Qemu_GUI
{

    public enum Platforms
    {
        x86 = 0,
        x86_ISA,
        x64,
        x64_ISA,       
        ARM_integratorcp926,
        ARM_integratorcp1026,
        ARM_versatilepb,
        ARM_versatileab,
        PPC_g3bw,                
        PPC_mac99,
        PPC_prep
    }

    [XmlRoot("Settings")]
    public class QEmu : IDisposable 
    {



        private Misc m_Misc = new Misc();
        private Floppies m_Floppies = new Floppies();
        private Harddisks m_Harddisks = new Harddisks();
        private Audio m_Audio = new Audio();
        private Debug m_Debug = new Debug();
        private Paths m_Paths = new Paths();
        private CDROM m_CDROM = new CDROM();
        private Tools m_Tools = new Tools();
        private string m_LastError = "";

        public QEmu()
        {
        }

        public void Dispose()
        {
        }

        public bool CreateImage(string FileName, long Size, string Format)
        {
            long d = Size * 1024;
            string argv = " create -f " + Format + " \""+ FileName + "\" " + d.ToString(); ;
            Process p = new Process();
            p.StartInfo.FileName = this.Paths.QEmu + "\\" + "qemu-img.exe";
            p.StartInfo.WorkingDirectory = this.Paths.QEmu;
            p.StartInfo.Arguments = argv;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.EnableRaisingEvents = true;
            p.Exited += new EventHandler(mytask_output);
            try
            {
                p.Start();
                //m_LastError = p.StandardError.ReadToEnd();
                //if (m_LastError.Length > 0)
                //    return false;
            }
            catch
            {
                m_LastError = "qemu-img.exe not found!";
                return false;
            }
            return true;
        }

        public string GetLastError()
        {
            return m_LastError;
        }


        public bool MountImage()
        {
            Process p = new Process();
            p.StartInfo.FileName = this.Paths.VDK + "\\vdk.exe";
            p.StartInfo.WorkingDirectory = this.Paths.VDK;
            p.StartInfo.Arguments = "open * " + "\"" + this.Tools.vdk.Image + "\"" + @" /RW /L:" + this.Tools.vdk.DriveLetter.Substring(0, 1);
            Console.WriteLine(p.StartInfo.Arguments);  
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.EnableRaisingEvents = true;
            p.Exited += new EventHandler(mytask_output);
            try
            {
                p.Start();
                //m_LastError = p.StandardOutput.ReadToEnd();
                //if (m_LastError.Length > 0)
                //    return false;
            }
            catch
            {
                m_LastError = "vdk not found!";
                return false;
            }
            return true;
        }

        public bool UnmountImage()
        {
            Process p = new Process();
            p.StartInfo.FileName = this.Paths.VDK + "\\vdk.exe";
            p.StartInfo.WorkingDirectory = this.Paths.VDK;
            p.StartInfo.Arguments = "CLOSE * /F";
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;

            p.EnableRaisingEvents = true;
            p.Exited += new EventHandler(mytask_output);

            try
            {
                p.Start();
                //m_LastError = p.StandardError.ReadToEnd();
                //if (m_LastError.Length > 0)
                //    return false;
            }
            catch
            {
                m_LastError = "vdk not found!";
                return false;
            }
            return true;
        }

        public bool Start(Platforms Platform)
        {
            Process p = new Process();
            
            switch (Platform)
            {
                case Platforms.x86:
                    p.StartInfo.FileName = this.Paths.QEmu + "\\qemu.exe";
                    break;
                case Platforms.x86_ISA:
                    p.StartInfo.FileName = this.Paths.QEmu + "\\qemu-system-x86_64.exe";
                    break;
                case Platforms.x64:
                    p.StartInfo.FileName = this.Paths.QEmu + "\\qemu.exe";
                    break;
                case Platforms.x64_ISA:
                    p.StartInfo.FileName = this.Paths.QEmu + "\\qemu-system-x86_64.exe";
                    break;       
                case Platforms.ARM_integratorcp926:
                case Platforms.ARM_integratorcp1026:
                case Platforms.ARM_versatilepb:
                case Platforms.ARM_versatileab:
                    p.StartInfo.FileName = this.Paths.QEmu + "\\qemu-system-arm.exe";
                    break;
                case Platforms.PPC_g3bw:                
                case Platforms.PPC_mac99:
                case Platforms.PPC_prep:
                    p.StartInfo.FileName = this.Paths.QEmu + "\\qemu-system-ppc.exe";
                    break;
        
            }     
    
            p.StartInfo.WorkingDirectory = this.Paths.QEmu;
            p.StartInfo.Arguments = GetArgv();
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;

            p.EnableRaisingEvents = true;
            p.Exited += new EventHandler(mytask_output);
            

            try
            {
                p.Start();
              //  m_LastError = p.StandardError.ReadLine();
             //   if (m_LastError.Length > 0)
             //       return false;    
            }
            catch 
            {
                m_LastError = "qemu not found!";
                return false;
            }
            return true;
        }

        private void mytask_output(object sender, EventArgs e)
        {
            Process ps = (Process)sender; 
           // add error handling here and do not freze the gui  
        }

        [XmlElement("Misc")]
        public Misc Misc
        {
            get { return m_Misc; }
            set { this.m_Misc = value; }
        }

        [XmlElement("Tools")]
        public Tools Tools
        {
            get { return m_Tools; }
            set { this.m_Tools = value; }
        }

        [XmlElement("Floppies")]
        public Floppies Floppies
        {
            get { return m_Floppies; }
            set { this.m_Floppies = value; }
        }

        [XmlElement("CDROM")]
        public CDROM CDROM
        {
            get { return m_CDROM; }
            set { this.m_CDROM = value; }
        }

        [XmlElement("Harddisks")]
        public Harddisks Harddisks
        {
            get { return m_Harddisks; }
            set { this.m_Harddisks = value; }
        }

        [XmlElement("Audio")]
        public Audio Audio
        {
            get { return m_Audio; }
            set { this.m_Audio = value; }
        }

        [XmlElement("Paths")]
        public Paths Paths
        {
            get { return m_Paths; }
            set { this.m_Paths = value; }
        }

        [XmlElement("Debug")]
        public Debug Debug
        {
            get { return m_Debug;}
            set { this.m_Debug = value;}
        }

        private string GetArgv()
        {
            string arg = "-L " + this.Paths.QEmu + " ";

            arg += Misc.ToString();
            arg += Floppies.ToString();  
            arg += Harddisks.ToString();  
            arg += CDROM.ToString();
            arg += Audio.ToString();  
            arg += Debug.ToString();

            return arg;
        }
        
    }

    

    public class Harddisks
    {
        private Drive[] m_HardDisks = new Drive[4];

        public Harddisks()
        {
            /* init array */
            for (int i = 0; i < 4; i++)
                m_HardDisks[i] = new Drive(); 
        }

        public override string ToString()
        {
            string buffer = "";
            for (int i = 0; i < 4; i++)
            {
                if (this.HDD[i].Enabled && (this.HDD[i].Path.Length > 0))
                    buffer += "-hd" + Convert.ToChar('a' + i) + " \"" + this.HDD[i].Path + "\" ";
            }
            return buffer;
        }

        [XmlElement("Harddisk")]
        public Drive[] HDD
        {
            get { return this.m_HardDisks; }
            set { this.m_HardDisks = value; }
        }
    }

    public class Floppies
    {
        private Drive[] m_Floppies = new Drive[2];

        public Floppies()
        {
            /* init array */
            for (int i = 0; i < 2; i++)
                m_Floppies[i] = new Drive();
        }

        public override string ToString()
        {
            string buffer = "";
            if (this.FDD[0].Enabled && (this.FDD[0].Path.Length > 0))
                buffer += "-fda \"" + this.FDD[0].Path + "\" ";

            if (this.FDD[1].Enabled && (this.FDD[1].Path.Length > 0))
                buffer += "-fdb \"" + this.FDD[1].Path + "\" ";

            return buffer;
        }

        [XmlElement("Floppy")]
        public Drive[] FDD
        {
            get { return this.m_Floppies; }
            set { this.m_Floppies = value; }
        }
    }

    public class Paths
    {
        private string m_QEmu = "";

        [XmlElement("QEmu")]
        public string QEmu
        {
            get
            { 
                return m_QEmu;
            }
            set 
            { 
                if (value != "")
                    m_QEmu = value;
                else
                    m_QEmu = ".";
            }
        }
        [XmlElement("VDK")]
        public string VDK = ".";

        public Paths()
        {
            m_QEmu = ".";
        }
    }

    public class Misc
    {
        [XmlElement("Machine")]
        public Platforms Machine;
        [XmlElement("Memory")]
        public int Memory;
        [XmlElement("CPUs")]
        public int CPUs;
        [XmlElement("BootFrom")]
        public string BootFrom = "";
        [XmlElement("SetClock")]
        public bool SetClock;
        [XmlElement("Fullscreen")]
        public bool Fullscreen;
        [XmlElement("VGA")]
        public bool VGA;
        [XmlElement("KQEmu")]
        public bool KQEmu;

        public override string ToString()
        {
            /* Memory settings */
            string buffer = "-m " + this.Memory.ToString() + " ";

            /* SMP settings */
            buffer += "-smp " + this.CPUs.ToString() + " ";

            /* Set clock */
            if (this.SetClock)
                buffer += "-localtime ";

            /* No vga output */
            if (this.VGA)
                buffer += "-nographic ";

            /* Fullscreen */
            if (this.Fullscreen)
                buffer += "-full-screen ";

            /* KQEmu */
            if (!this.KQEmu)
                buffer += "-no-kqemu ";

            /* Machine settings */
            switch (this.Machine)
            {
                case Platforms.x86:
                    buffer += "-M pc ";
                    break;
                case Platforms.x86_ISA:
                    buffer += "-M isapc ";
                    break;
                case Platforms.x64:
                    buffer += "-M pc ";
                    break;
                case Platforms.x64_ISA:
                    buffer += "-M isapc ";
                    break;
                case Platforms.ARM_integratorcp926:
                    buffer += "-M integratorcp926 ";
                    break;

                case Platforms.ARM_integratorcp1026:
                    buffer += "-M integratorcp1026 ";
                    break;

                case Platforms.ARM_versatilepb:
                    buffer += "-M versatilepb ";
                    break;
                case Platforms.ARM_versatileab:
                    buffer += "-M versatileab ";
                    break;
                case Platforms.PPC_g3bw:
                    buffer += "-M g3bw ";
                    break;  
                case Platforms.PPC_mac99:
                    buffer += "-M mac99 ";
                    break;
                case Platforms.PPC_prep:
                    buffer += "-M prep ";
                    break;
                    
     
            }    

            /* Boot options */
            switch (this.BootFrom)
            {
                case "Floppy":
                    buffer += "-boot a ";
                    break;
                case "Harddisk":
                    buffer += "-boot c ";
                    break;
                case "CD-ROM":
                    buffer += "-boot d ";
                    break;
                default:
                    break;
            }

            return buffer;
        }

        public Misc()
        {
        }
    }

    public class Tools
    {
        private VDK m_VDK = new VDK();

        [XmlElement("VDK")]
        public VDK vdk
        {
            get { return this.m_VDK; }
            set { this.m_VDK = value; }
        }
        
        public Tools()
        {
        }
    }

    public class VDK
    {
        [XmlElement("Image")]
        public string Image = "";
        [XmlElement("DriveLetter")]
        public string DriveLetter = "";

        public VDK()
        {
        }
    }

    public class Audio
    {
        [XmlElement("Soundblaster")]
        public bool Soundblaster;
        [XmlElement("OPL2")]
        public bool OPL2;
        [XmlElement("ES1370")]
        public bool ES1370;
        [XmlElement("Speaker")]
        public bool Speaker;
                 
        public Audio()
        {
        }

        public override string ToString() 
        {
            bool audio_on = (this.ES1370 | this.OPL2 | this.Soundblaster | this.Speaker);

            if (!audio_on)
                return "";

            string buffer = "-soundhw ";

            if (this.Speaker)
                buffer += "pcspk,";

            if (this.Soundblaster)
                buffer += "sb16,";

            if (this.OPL2)
                buffer += "adlib,";

            if (this.ES1370)
                buffer += "es1370,";

            return buffer.Substring(0, buffer.Length - 1) + " ";
        }
            
    }

    public class Debug
    {
        public ParPort ParallelPort = new ParPort();
        public SerPort SerialPort = new SerPort();

        [XmlElement("VBE3")]
        public bool VBE3;
        [XmlElement("GDBPort")]
        public int GDBPort;

        public override string ToString()
        {
            string buffer = "";

            /* Serial port */
            if (this.SerialPort.Redirect)
            {
                if (this.SerialPort.FileName.Length > 0)
                    buffer = "-serial file: \"" + this.SerialPort.FileName + "\" ";
            }

            /* Parallel port */
            if (this.ParallelPort.Redirect)
            {
                if (this.ParallelPort.FileName.Length > 0)
                    buffer += "-parallel  file: \"" + this.ParallelPort.FileName + "\" ";
            }

            /*  Standard VGA */
            if (this.VBE3)
                buffer += "-std-vga ";

            if (this.GDBPort != 1234)
                buffer += "-p " + this.GDBPort.ToString() + " ";

            return buffer;
        }

        public Debug()
        {
        }
    }

    public class SerPort
    {
        [XmlAttribute("Redirect")]
        public bool Redirect;
        [XmlAttribute("FileName")]
        public string FileName;

        public SerPort()
        {
        }
    }

    public class ParPort
    {
        [XmlAttribute("Redirect")]
        public bool Redirect;
        [XmlAttribute("FileName")]
        public string FileName;

        public ParPort()
        {
        }
    }

    public class CDROM
    {
        [XmlAttribute("Image")]
        public string Image;
        [XmlAttribute("UseFromHost")]
        public bool UseFromHost;
        [XmlAttribute("HostDrive")]
        public string HostDrive;
        [XmlAttribute("Enabled")]
        public bool Enabled;

        public override string ToString()
        {
            string buffer = "";
            if (this.Enabled)
            {
                if (this.UseFromHost)
                    buffer = "-cdrom " + this.HostDrive + " ";
                else if (this.Image.Length > 0)
                    buffer = "-cdrom \"" + this.Image + "\" ";
            }
            return buffer;
        }

        public CDROM()
        {
        }
    }

    public class Drive
    {
        [XmlAttribute("Path")] 
        public string Path;
        [XmlAttribute("Enabled")]
        public bool Enabled;
        
        public Drive()
        {
        }
    }
    
}
