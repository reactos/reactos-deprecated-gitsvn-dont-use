using System;
using System.Text;
using System.Windows.Forms;
using System.Collections;

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

        public NewHardDiskForm(ArrayList curDrives)
        {
            InitializeComponent();

            string[] allDrives = { "hda", "hdb", "hdd" };

            foreach (string str in allDrives)
            {
                bool found = false;

                foreach (string drive in curDrives)
                {
                    if (string.Compare(drive, str, true) == 0)
                    {
                        found = true;
                    }
                }
                if (!found)
                    newhdDrive.Items.Add(str);
            }

            if (newhdDrive.Items.Count > 0)
                newhdDrive.SelectedIndex = 0;
        }
    }
}