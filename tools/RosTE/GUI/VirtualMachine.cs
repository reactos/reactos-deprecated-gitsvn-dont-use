using System;
using System.IO;
using System.Collections.Specialized;
using System.Data;
using System.Xml;

namespace RosTEGUI
{
    public class VirtualMachine
    {
        private Data data;
        private DataRow dataRow;
        private string machine;
        private string cdrom;
        private char boot;
        private string snapshot;
        private int memory;
        private int processors;
        private bool localTime;
        private bool fullScreen;

        public string Name
        {
            get { return (string)dataRow["Name"]; }
            set { dataRow["Name"] = value; }
        }

        public string DefDir
        {
            get { return (string)dataRow["DefDir"]; }
        }

        public string Machine
        {
            get { return machine; }
            set { machine = value; }
        }


        public string Cdrom
        {
            get { return cdrom; }
            set { cdrom = value; }
        }
        public string Boot
        {
            get
            {
                switch (boot)
                {
                    case 'a': return "floppy";
                    case 'c': return "harddisk";
                    case 'd': return "cdrom";
                    case 'n': return "network";
                    default: throw new ArgumentException("Boot");
                }
            }
            set
            {
                switch (value)
                {
                    case "floppy": boot = 'a'; break;
                    case "harddisk": boot = 'c'; break;
                    case "cdrom": boot = 'd'; break;
                    case "network": boot = 'n'; break;
                    default: throw new ArgumentException("Boot");
                }
            }
        }
        public string SnapShot
        {
            get { return snapshot; }
            set { snapshot = value; }
        }
        public int Memory
        {
            get { return memory; }
            set { memory = value; }
        }
        public int Processors
        {
            get { return processors; }
            set { processors = value; }
        }
        public bool LocalTime
        {
            get { return localTime; }
            set { localTime = value; }
        }
        public bool FullScreen
        {
            get { return fullScreen; }
            set { fullScreen = value; }
        }

        public override string ToString()
        {
            return Name;
        }

        public VirtualMachine(Data dataIn)
        {
            data = dataIn;
        }

        // default
        public bool CreateVMConfig(string name)
        { 
            return CreateVMConfig(name, "Images\\" + name, 0.2f, null, 256);
        }

        // existing
        public bool CreateVMConfig(string name, string existImg, int memSize)
        {
            return CreateVMConfig(name, null, 0.0f, existImg, memSize);
        }

        // new
        public bool CreateVMConfig(string name,
                                   string dir,
                                   float diskSize,
                                   string existImg,
                                   int memSize)
        {
            bool ret = false;

            if (existImg != null)
            {
                DirectoryInfo di = Directory.GetParent(existImg);
                dir = di.FullName;
            }

            DataTable dt = data.DataSet.Tables["VMConfig"];
            int i = dt.Rows.Count + 1;
            dataRow = dt.NewRow();
            dataRow["VirtMachID"] = i;
            dataRow["Name"] = name;
            dataRow["MachType"] = "test";
            dataRow["DefDir"] = dir;
            dataRow["MemSize"] = memSize;
            dataRow["CdRomEnable"] = true;
            dataRow["CdRomUsePhys"] = false;
            dataRow["CdRomPhyDrv"] = "R:";
            dataRow["CdRomUseIso"] = true;
            dataRow["CdRomIsoImg"] = "err";
            dataRow["FloppyEnable"] = true;
            dataRow["FloppyUsePhys"] = true;
            dataRow["FloppyPhyDrive"] = "A:";
            dataRow["FloppyUseImg"] = false;
            dataRow["FloppyIsoImg"] = "err";

            dt.Rows.Add(dataRow);
            
            Name = name;

            return ret;
        }

        public bool LoadVMConfig(string path)
        {
            bool ret = false;
            string fileName = path + "\\Config.xml";

            if (File.Exists(fileName))
            {
                FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                XmlTextReader xtr = new XmlTextReader(fs);
                data.DataSet.ReadXml(xtr, System.Data.XmlReadMode.ReadSchema);
                xtr.Close();

                DataTable dt = data.DataSet.Tables["VMConfig"];
                dataRow = dt.Rows[0];

                ret = true;
            }

            return ret;
        }

        public void SaveVMConfig()
        {
            string fileName = DefDir + "\\Config.xml";
            Directory.CreateDirectory(DefDir);
            FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            XmlTextWriter xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
            data.DataSet.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
            xtw.Close();
        }
    }
}



