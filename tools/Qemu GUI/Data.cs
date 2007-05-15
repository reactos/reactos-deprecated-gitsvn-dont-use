using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Windows.Forms;


namespace Qemu_GUI
{

    public enum Platforms
    {
        x86 = 0,
        x86_ISA = 1,
        x64 = 2,
        x64_ISA = 3,
        ARM_integratorcp926 = 4,
        ARM_integratorcp1026 = 5,
        ARM_versatilepb = 6,
        ARM_versatileab = 7,
        PPC_g3bw = 8,
        PPC_mac99 = 9,
        PPC_prep = 10,
        Sparc_sun4m = 11
    }

    [XmlRoot("Settings")]
    public class Data : IDisposable 
    {
        private General m_Gen = new General();
        private Floppies m_Floppies = new Floppies();
        private Harddisks m_Harddisks = new Harddisks();
        private Audio m_Audio = new Audio();
        private Debug m_Debug = new Debug();
        private Paths m_Paths = new Paths();
        private CDROM m_CDROM = new CDROM();
        private Tools m_Tools = new Tools();
        private Network m_Net = new Network();
        private Other m_Other = new Other();

        public Data()
        {
        }

        public void Dispose()
        {
            Dispose();
        }

        [XmlElement("General")]
        public General General
        {
            get { return m_Gen; }
            set { this.m_Gen = value; }
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

        [XmlElement("Network")]
        public Network Network
        {
            get { return m_Net; }
            set { this.m_Net = value; }
        }

        [XmlElement("Other")]
        public Other Other
        {
            get { return m_Other; }
            set { this.m_Other = value; }
        }


        public string GetArgv()
        {
            /* qemu BIOS path */
            string arg;
            if(this.Other.ABios.Length > 0)
                arg = "-L \"" + this.Other.ABios + "\" ";
            else
                arg = "-L \"" + this.Paths.Qemu + "\" ";

            arg += General.ToString();
            arg += Floppies.ToString();  
            arg += Harddisks.ToString();
            arg += CDROM.ToString();
            arg += Audio.ToString();
            arg += Debug.ToString();
            arg += Network.ToString();
            arg += Other.ToString();

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

        [XmlElement("Qemu")]
        public string Qemu = "";

        [XmlElement("VDK")]
        public string VDK = "";

        public Paths()
        {

        }
    }

    public class General
    {
        [XmlElement("Machine")]
        public Platforms Machine;
        [XmlElement("Memory")]
        public int Memory;
        [XmlElement("CPUs")]
        public int CPUs;
        [XmlElement("BootFrom")]
        public string BootFrom;
        [XmlElement("SetClock")]
        public bool SetClock;
        [XmlElement("Fullscreen")]
        public bool Fullscreen;
        [XmlElement("VGA")]
        public bool VGA;
        [XmlElement("KQEmu")]
        public bool KQEmu;
        [XmlElement("ACPI")]
        public bool ACPI;

        private bool KqemuAllowed = false;

        public override string ToString()
        {
            /* Memory settings */
            string buffer = "-m " + this.Memory.ToString() + " ";

            /* SMP settings */
            if(this.CPUs > 1)
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

            if (!this.ACPI)
                buffer += "-no-acpi ";

            /* Machine settings */
            switch (this.Machine)
            {
                case Platforms.x86:
                    buffer += "-M pc ";
                    KqemuAllowed = true;
                    break;
                case Platforms.x86_ISA:
                    buffer += "-M isapc ";
                    KqemuAllowed = true;
                    break;
                case Platforms.x64:
                    buffer += "-M pc ";
                    KqemuAllowed = false;
                    break;
                case Platforms.x64_ISA:
                    buffer += "-M isapc ";
                    KqemuAllowed = false;
                    break;
                case Platforms.ARM_integratorcp926:
                    buffer += "-M integratorcp926 ";
                    KqemuAllowed = false;
                    break;
                case Platforms.ARM_integratorcp1026:
                    buffer += "-M integratorcp1026 ";
                    KqemuAllowed = false;
                    break;
                case Platforms.ARM_versatilepb:
                    buffer += "-M versatilepb ";
                    KqemuAllowed = false;
                    break;
                case Platforms.ARM_versatileab:
                    buffer += "-M versatileab ";
                    KqemuAllowed = false;
                    break;
                case Platforms.PPC_g3bw:
                    buffer += "-M g3bw ";
                    KqemuAllowed = false;
                    break;
                case Platforms.PPC_mac99:
                    buffer += "-M mac99 ";
                    KqemuAllowed = false;
                    break;
                case Platforms.PPC_prep:
                    buffer += "-M prep ";
                    KqemuAllowed = false;
                    break;
            }

            /* KQEmu */
            if(KqemuAllowed == true)
                if (this.KQEmu)
                    buffer += "-kernel-kqemu ";
                else
                    buffer += "-no-kqemu ";

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

        public General()
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
        [XmlElement("ParPort")]
        public ParPort ParallelPort = new ParPort();
        [XmlElement("SerPort")]
        public SerPort SerialPort = new SerPort();

        [XmlElement("VBE3")]
        public bool VBE3;
        [XmlElement("GDBPort")]
        public int GDBPort;

        public bool EnableSavedState;
        public string SavedStatePath;

        public override string ToString()
        {
            string buffer = "";
            
            /* Serial port */
            if (this.SerialPort.FRedirect)
            {
                if (this.SerialPort.FileName.Length > 0)
                    buffer = "-serial file:\"" + this.SerialPort.FileName + "\" ";
            }
            else if (this.SerialPort.PRedirect)
            {
                if (this.SerialPort.PipeName.Length > 0)
                    buffer = "-serial pipe:" + this.SerialPort.PipeName + " ";
            }
            else if (this.SerialPort.SRedirect)
            {
                /* fix me: use pipes!!!!!! */
                buffer = "-serial file:\"" + this.SerialPort.FileName + "\" ";
            }


            /* Parallel port */
            if (this.ParallelPort.FRedirect)
            {
                if (this.ParallelPort.FileName.Length > 0)
                    buffer += "-parallel file:\"" + this.ParallelPort.FileName + "\" ";
            }
            else if (this.ParallelPort.PRedirect)
            {
                if (this.ParallelPort.PipeName.Length > 0)
                    buffer += "-parallel pipe:\"" + this.ParallelPort.PipeName + "\" ";
            }

            /* Saved VM state */
            if (this.EnableSavedState)
            {
                if (this.SavedStatePath.Length > 0)
                {
                    buffer += "-loadvm \"" + this.SavedStatePath + "\" ";
                }
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
        [XmlAttribute("SRedirect")]
        public bool SRedirect;
        [XmlAttribute("FRedirect")]
        public bool FRedirect;
        [XmlAttribute("FileName")]
        public string FileName;
        [XmlAttribute("PRedirect")]
        public bool PRedirect;
        [XmlAttribute("PipeName")]
        public string PipeName;

        public SerPort()
        {

        }
    }

    public class ParPort
    {
        [XmlAttribute("FRedirect")]
        public bool FRedirect;
        [XmlAttribute("FileName")]
        public string FileName;
        [XmlAttribute("PRedirect")]
        public bool PRedirect;
        [XmlAttribute("PipeName")]
        public string PipeName;

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
                    buffer = "-cdrom //./PhysicalDrive1 ";// + this.HostDrive + " ";//-cdrom //./d:

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

    public class Network
    {
        [XmlAttribute("Enabled")]
        public bool Enabled;

        [XmlAttribute("VNicString")]
        public string VNicString;

        public string[] VNicStringReader()
        {


            string[] buffer = new string[1];
            int j = 0;
            char[] chararray;
            bool start = false;

            /* we have no adaptors */
            if (this.VNicString == null)
                return null;
            else
                chararray = this.VNicString.ToCharArray();

            /* This is not a real adaptor, skip it */
            if (this.VNicString == "-net none ")
            {
                buffer[j] = "ignore";
                j++; //if we are here there should NOT be any other -net options, but just in case...
            }
            else

            for (int i = 0; i < chararray.Length; i++)
            {

                if (chararray[i] == '-') //we found the start of a -net string
                    start = true;
                if (start == true)
                {
                    buffer[j] += chararray[i];
                }
                else
                {
                    start = false;
                    j++;
                }
            }
            return buffer;
        }


        public override string ToString()
        {
            string buffer = "";
            foreach(VLan lan in MainForm.VLanlist)
                buffer += lan.ToString();

            return buffer;
        }
    }

    public class Other
    {
        [XmlAttribute("LKernel")]
        public string LKernel;
        [XmlAttribute("ABios")]
        public string ABios;
        [XmlAttribute("AppendCmdLine")]
        public string AppendCmdLine;

        public override string ToString()
        {
            string buffer = "";

            if (LKernel.Length > 0)
                buffer += "-kernel " + LKernel + " ";

            if (AppendCmdLine.Length > 0)
                buffer += "-append " + AppendCmdLine + " ";

            return buffer;

            /* ABios is a special value that is handled in GetArgv()*/
        }

        public Other()
        {
        }
    }
}
