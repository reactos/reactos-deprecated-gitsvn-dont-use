using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace RosTEGUI
{
    public partial class NewVMWizard : Form
    {
        public string VMName
        {
            get { return nameTxtBox.Text; }
        }
        public string DefDir
        {
            get { return defaultDirTxtBox.Text; }
        }
        public int DiskSizeGB
        {
            get { return (int)harddiskUpDwn.Value; }
        }
        public string ExistImg
        {
            get { return imageLocTxtBox.Text; }
        }
        public int MemSizeMB
        {
            get { return (int)memoryUpDwn.Value; }
        }

        public NewVMWizard()
        {
            InitializeComponent();
        }

        private void wizard1_CloseFromCancel(object sender, CancelEventArgs e)
        {
            if (MessageBox.Show(this,
                                "Are you sure you want to close the wizard?",
                                "Wizard Cancelled", 
                                MessageBoxButtons.YesNo
                                ) == DialogResult.No)
            {
                e.Cancel = true;
            }
        }

        private void wizardSetupOptionPage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadDefault.Checked == true)
                e.Page = wizardFinishPage;
            else if (optionRadExist.Checked == true)
                e.Page = imageInfoPage;
            else
                e.Page = nameInfoPage;
        }

        private void wizardNamePage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (nameTxtBox.Text == "")
            {
                MessageBox.Show("You must enter a name", "Error");
                e.Page = nameInfoPage;
            }
        }

        private void wizardDefaultDirPage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (defaultDirTxtBox.Text == "")
            {
                MessageBox.Show("You must enter a name", "Error");
                e.Page = defaultDirInfoPage;
            }
            else if (Directory.Exists(defaultDirTxtBox.Text) == false)
            {
                MessageBox.Show("Directory does not exist", "Error");
                e.Page = defaultDirInfoPage;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult result = wizardFolderBrowser.ShowDialog();

            if (result == DialogResult.OK)
            {
                defaultDirTxtBox.Text = wizardFolderBrowser.SelectedPath;
            }
        }

        private void wizardHardDiskPage_Enter(object sender, EventArgs e)
        {
            DriveInfo di = new DriveInfo(defaultDirTxtBox.Text);
            long AvailInGB = di.AvailableFreeSpace / (long)Math.Pow(1024, 3);

            harddiskTrkBar.Maximum = Convert.ToInt32(AvailInGB);
            harddiskTrkBar.TickFrequency = harddiskTrkBar.Maximum / 20;
            harddiskUpDwn.Maximum = harddiskTrkBar.Maximum;

            harddiskDiskLab.Text = di.Name;            
            harddiskDiskSizeLab.Text = Convert.ToString(AvailInGB) + " GB";
            harddiskRecMinLab.Text = "60 MB";
        }

        private void wizardMemoryPage_Enter(object sender, EventArgs e)
        {
            ulong totMem = Native.Memory.GetTotalMemory();
            if (totMem != 0)
            {
                totMem /= 1048576; //(1024^2)
                memoryPhyRam.Text = Convert.ToString(totMem) + " MB";

                memoryTrkBar.Maximum = Convert.ToInt32(totMem) * 2;
                memoryTrkBar.TickFrequency = memoryTrkBar.Maximum / 20;
                memoryUpDwn.Maximum = memoryTrkBar.Maximum;

                //memoryMinLab.Text = Convert.ToString(0) + " MB";
                //memoryMaxLab.Text = memoryTrkBar.Maximum.ToString() + " MB";

                memoryRecMin.Text = Convert.ToString(totMem / 8) + " MB";
                memoryRec.Text = Convert.ToString(totMem / 4) + " MB";
                memoryRecMax.Text = Convert.ToString(totMem / 1.4) + " MB";
            }
        }

        private void harddiskTrkBar_Scroll(object sender, EventArgs e)
        {
            harddiskUpDwn.Value = harddiskTrkBar.Value;
        }

        private void harddiskUpdown_ValueChanged(object sender, EventArgs e)
        {
            harddiskTrkBar.Value = (int)harddiskUpDwn.Value;
        }

        private void memoryTrkBar_Scroll(object sender, EventArgs e)
        {
            memoryUpDwn.Value = memoryTrkBar.Value;
        }

        private void memoryUpDwn_ValueChanged(object sender, EventArgs e)
        {
            memoryTrkBar.Value = (int)memoryUpDwn.Value;
        }

        private void wizardFinishPage_CloseFromBack(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadDefault.Checked == true)
                e.Page = optionInfoPage;
            else
                e.Page = memoryInfoPage;
        }

        private void imageInfoPage_CloseFromBack(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadExist.Checked == true)
                e.Page = optionInfoPage;
        }

        private void imageFileBrowse_Click(object sender, EventArgs e)
        {
            wizardOpenFile.FileName = "";
            wizardOpenFile.InitialDirectory = "C:\\";
            wizardOpenFile.Filter = "img file (*.raw)|*.raw|vdk files (*.vmdk)|*.vmdk|dsk files (*.dsk)|*.dsk";
            wizardOpenFile.FilterIndex = 1;
            wizardOpenFile.RestoreDirectory = true;

            DialogResult result = wizardOpenFile.ShowDialog();
            if (result == DialogResult.OK)
            {
                imageLocTxtBox.Text = wizardOpenFile.FileName;
            }
        }

        private void harddiskInfoPage_CloseFromBack(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadDefault.Checked == true)
                e.Page = harddiskInfoPage;
        }

        private void harddiskInfoPage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadNew.Checked == true)
                e.Page = memoryInfoPage;
        }

        private void memoryInfoPage_CloseFromBack(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadNew.Checked == true)
                e.Page = harddiskInfoPage;
        }
    }
}