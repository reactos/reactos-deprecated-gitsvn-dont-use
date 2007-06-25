using System;
using System.IO;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class VirtualMachine
    {
        private Data data;
        private DataRow dataRow;
        private string machine;
        private StringCollection floppy;
        private StringCollection hardDisk;
        private string cdrom;
        private char boot;
        private string snapshot;
        private int memory;
        private int processors;
        private bool localTime;
        private bool fullScreen;

        public string Name
        {
            get
            {
                return (string)dataRow["Name"];
            }
            set
            {
                dataRow["Name"] = value;
            }
        }

        public string Machine
        {
            get { return machine; }
            set { machine = value; }
        }
        public StringCollection Floppy
        {
            get { return floppy; }
            // no set, use Floppy.Add()
        }
        public StringCollection HardDisk
        {
            get { return hardDisk; }
            // no set, use HardDisk.Add()
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

            DataTable dt = data.DataSet.Tables["VMConfig"];
            dataRow = dt.NewRow();
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

            Name = name;

            MessageBox.Show(name + " " + dir + " " + diskSize + " " + existImg + " " + memSize);

            return ret;
        }

        public bool LoadVirtMach(string path)
        {
            bool ret = false;


            return ret;
        }

    }
}



