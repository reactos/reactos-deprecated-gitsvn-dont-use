using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public partial class NewHardDiskForm : Form
    {
        public string DiskName
        {
            get { return newhdName.Text; }
        }

        public string Path
        {
            get { return newhdPath.Text; }
        }

        public string QEmuDrive
        {
            get { return newhdDrive.Text; }
        }

        public int DiskSize
        {
            get { return Convert.ToInt32(newhdSize.Value); }
        }

        public bool BootImage
        {
            get { return newhdBoot.Checked; }
        }

        public NewHardDiskForm()
        {
            InitializeComponent();
        }
    }
}