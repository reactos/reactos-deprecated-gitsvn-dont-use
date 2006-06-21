using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Xml;
using System.Xml.Serialization;

namespace Qemu_GUI
{

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

        public QEmu()
        {
        }

        public void Dispose()
        {
        }

        public bool CreateImage(string FileName, long Size, string Format)
        {
            long d = Size * 1024;
            string argv = " create -f " + Format + " " + FileName + "." + Format + " " + d.ToString(); ;

            Process Qemu = new Process();
            Qemu.StartInfo.FileName = this.Paths.QEmu + "\\" + "qemu-img.exe";
            Qemu.StartInfo.WorkingDirectory = this.Paths.QEmu;
            Qemu.StartInfo.Arguments = argv;
            try
            {
                Qemu.Start();
            }
            catch
            {
                return false;
            }
            return true;
        }

        public bool Start()
        {
            Process Qemu = new Process();
            Qemu.StartInfo.FileName = this.Paths.QEmu + "\\" + "qemu.exe";
            Qemu.StartInfo.WorkingDirectory = this.Paths.QEmu;
            Qemu.StartInfo.Arguments = GetArgv();
            try
            {
                Qemu.Start();
            }
            catch 
            {
                return false;
            }
            return true;
        }

        [XmlElement("Misc")]
        public Misc Misc
        {
            get { return m_Misc; }
            set { this.m_Misc = value; }
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
            /*  bool audio_on = false; */

            /* Machine settings */
            if (!this.m_Misc.StandardPC)
                arg = arg + "-M isapc ";
            else
                arg = arg + "-M pc ";


            /* Floppy settings */
            if (this.Floppies.FDD[0].Enabled && (this.Floppies.FDD[0].Path.Length > 0))
                arg = arg + "-fda " + this.Floppies.FDD[0].Path + " ";

            if (this.Floppies.FDD[1].Enabled && (this.Floppies.FDD[1].Path.Length > 0))
                arg = arg + "-fdb " + this.Floppies.FDD[1].Path + " ";


            /* Harddisk settings */
            for (int i = 0; i < 4; i++)
            {
                if (this.Harddisks.HDD[i].Enabled && (this.Harddisks.HDD[i].Path.Length > 0))
                    arg = arg + "-hd" + ((char) i + 97) + " " + this.Harddisks.HDD[i].Path + " ";
            }


            /* CD-ROM */
            if (this.CDROM.Enabled)
            {
                if (this.CDROM.UseFromHost)
                    arg = arg + "-cdrom " + this.CDROM.HostDrive + " ";
                else if (this.CDROM.Image.Length >  0)
                    arg = arg + "-cdrom " + this.CDROM.Image + " ";
            }

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

        [XmlElement("Floppy")]
        public Drive[] FDD
        {
            get { return this.m_Floppies; }
            set { this.m_Floppies = value; }
        }
    }

    public class Paths
    {
        [XmlElement("QEmu")]
        public string QEmu;
        [XmlElement("VDK")]
        public string VDK;

        public Paths()
        {
        }
    }

    public class Misc
    {
        [XmlElement("StandardPC")]
        public bool StandardPC;
        [XmlElement("Memory")]
        public int Memory;
        [XmlElement("CPUs")]
        public int CPUs;

        public Misc()
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
    }

    public class Debug
    {
        public ParPort ParallelPort = new ParPort();
        public SerPort SerialPort = new SerPort();

        [XmlElement("VBE3")]
        public bool VBE3;

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
