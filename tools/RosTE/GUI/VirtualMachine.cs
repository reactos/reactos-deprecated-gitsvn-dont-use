using System;
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


        public VirtualMachine()
        {
            floppy = new StringCollection();
            hardDisk = new StringCollection();

            Floppy.Add("test");
            MessageBox.Show(Floppy[0]);
        }

    }
}



