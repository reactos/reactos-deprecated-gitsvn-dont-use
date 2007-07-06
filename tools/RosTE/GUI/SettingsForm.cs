using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Management;
using System.Runtime.InteropServices;
using System.IO;


namespace RosTEGUI
{
    public partial class SettingsForm : Form
    {
        private VirtualMachine VirtMach;
        private Panel[] hardwarePanels;
        private Panel[] optionsPanels;
        private int hardwarePrevSel = 0;
        private int optionsPrevSel = 0;

        public SettingsForm(object sender)
        {
            InitializeComponent();

            VirtMach = (VirtualMachine)sender;
            Text = VirtMach.Name + " " + Text;
        }

        private bool LoadMemoryPage()
        {
            bool ret = false;

            ulong totMem = Native.Memory.GetTotalMemory();
            if (totMem != 0)
            {
                totMem /= (ulong)Math.Pow(1024, 2); // MB
                memoryPhyRam.Text = Convert.ToString(totMem) + " MB";

                memoryTrkBar.Maximum = Convert.ToInt32(totMem) * 2;
                memoryTrkBar.TickFrequency = memoryTrkBar.Maximum / 20;
                memoryUpDwn.Maximum = memoryTrkBar.Maximum;

                memoryMinLab.Text = Convert.ToString(0) + " MB";
                memoryMaxLab.Text = memoryTrkBar.Maximum.ToString() + " MB";

                memoryRecMin.Text = Convert.ToString(totMem / 8) + " MB";
                memoryRec.Text = Convert.ToString(totMem / 4) + " MB";
                memoryRecMax.Text = Convert.ToString(totMem / 1.4) + " MB";

                memoryTrkBar.Value = VirtMach.MemSize;
                memoryUpDwn.Value = VirtMach.MemSize;

                ret = true;
            }

            return ret;
        }

        private void LoadCdRomPage()
        {
            DriveInfo[] drives = DriveInfo.GetDrives();

            foreach(DriveInfo drive in drives)
            {
                if (drive.DriveType == DriveType.CDRom)
                    cdromPhyDrvCombo.Items.Add(drive);
            }

            cdromEnableChkBox.Checked = VirtMach.CdRomEnable;

            int id = cdromPhyDrvCombo.FindString(VirtMach.CdRomPhysDrv);
            if (id == -1) id = 0;
            cdromPhyDrvCombo.SelectedIndex = id;

            if (VirtMach.CdRomUsePhys)
            {
                cdromPhyDrvRadio.Checked = true;
            }
            else
            {
                cdromIsoRadio.Checked = true;
                cdromIsoTxtBox.Text = VirtMach.CdRomIsoImg;
            }
        }

        private void LoadHardDiskPage()
        {
            ArrayList hardDrives = VirtMach.GetHardDisks();
            foreach (VMHardDrive vmhd in hardDrives)
            {
                harddiskLstBox.Items.Add(vmhd);
            }

            if (harddiskLstBox.Items.Count > 0)
                harddiskLstBox.SelectedIndex = 0;
        }

        private void LoadFloppyPage()
        {
            DriveInfo[] drives = DriveInfo.GetDrives();

            foreach (DriveInfo drive in drives)
            {
                if (drive.DriveType == DriveType.Removable)
                    floppyPhyDrvCombo.Items.Add(drive);
            }

            floppyEnableChkBox.Checked = VirtMach.FloppyEnable;

            int id = floppyPhyDrvCombo.FindString(VirtMach.FloppyPhysDrv);
            if (id == -1) id = 0;
            floppyPhyDrvCombo.SelectedIndex = id;

            if (VirtMach.CdRomUsePhys)
            {
                floppyPhyDrvRadio.Checked = true;
            }
            else
            {
                floppyImgRadio.Checked = true;
                floppyImgTxtBox.Text = VirtMach.FloppyIsoImg;
            }
        }

        private void LoadFormData()
        {
            if (!LoadMemoryPage())
                MessageBox.Show("An error occured whilst loading memory page data");
            LoadCdRomPage();
            LoadHardDiskPage();
            LoadFloppyPage();
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            hardwareSelLstBox.Items.Add(new OptListBoxItem("Memory", 0));
            hardwareSelLstBox.Items.Add(new OptListBoxItem("CD-ROM", 1));
            hardwareSelLstBox.Items.Add(new OptListBoxItem("Hard Disk", 2));
            hardwareSelLstBox.Items.Add(new OptListBoxItem("Floppy", 3));
            hardwareSelLstBox.Items.Add(new OptListBoxItem("Ethernet", 4));
            hardwareSelLstBox.Items.Add(new OptListBoxItem("Sound", 5));
            hardwareSelLstBox.Items.Add(new OptListBoxItem("Display", 6));

            optionsSelLstBox.Items.Add(new OptListBoxItem("General", 7));
            optionsSelLstBox.Items.Add(new OptListBoxItem("Power", 8));
            optionsSelLstBox.Items.Add(new OptListBoxItem("Snapshots", 9));
            optionsSelLstBox.Items.Add(new OptListBoxItem("Advanced", 10));
            optionsSelLstBox.Items.Add(new OptListBoxItem("Debug", 11));

            hardwareSelLstBox.ItemHeight = hardwareSelLstBox.Height / hardwareSelLstBox.Items.Count;
            optionsSelLstBox.ItemHeight = hardwareSelLstBox.ItemHeight;

            hardwarePanels = new Panel[] { memoryPanel, cdromPanel, harddiskPanel, networkPanel, ethernetPanel, audioPanel, displayPanel };
            optionsPanels = new Panel[] { generalPanel, powerPanel, snapshotsPanel, advancedPanel, debugPanel};

            foreach (Panel pan in hardwarePanels)
            {
                pan.Parent = hardwareTempDesignSheetTab.Parent;
                pan.Visible = false;
            }

            foreach (Panel pan in optionsPanels)
            {
                pan.Parent = optionsTempDesignSheetTab.Parent;
                pan.Visible = false;
            }

            hardwareTempDesignSheetTab.Visible = false;
            optionsTempDesignSheetTab.Visible = false;
            hardwarePanels[0].Visible = true;
            optionsPanels[0].Visible = true;
            hardwareSelLstBox.SelectedItem = hardwareSelLstBox.Items[0];
            optionsSelLstBox.SelectedItem = optionsSelLstBox.Items[0];

            //we load all form data when the form opens, not as each individual
            //page is loaded. This is because we save all form data on close
            LoadFormData();
        }

        private void listboxSelection_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox listbox = (ListBox)sender;

            if (settingsTab.SelectedIndex == 0)
            {
                hardwarePanels[hardwarePrevSel].Visible = false;
                hardwarePrevSel = listbox.SelectedIndex;
                hardwarePanels[(int)listbox.SelectedIndex].Visible = true;
            }
            else if (settingsTab.SelectedIndex == 1)
            {
                optionsPanels[optionsPrevSel].Visible = false;
                optionsPrevSel = listbox.SelectedIndex;
                optionsPanels[(int)listbox.SelectedIndex].Visible = true;
            }
        }

        private void settingsCancelBtn_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ethEditBtn_Click(object sender, EventArgs e)
        {
            if (ethEditBtn.Text == "Edit")
            {
                ethVlanTxtBox.BorderStyle = BorderStyle.Fixed3D;
                ethVlanTxtBox.Enabled = true;
                ethNetTypeTxtBox.BorderStyle = BorderStyle.Fixed3D;
                ethNetTypeTxtBox.Enabled = true;
                ethMacAddrTxtBox.BorderStyle = BorderStyle.Fixed3D;
                ethMacAddrTxtBox.Enabled = true;
                ethEditBtn.Text = "OK";
            }
            else if (ethEditBtn.Text == "OK")
            {
                ethVlanTxtBox.BorderStyle = BorderStyle.None;
                ethVlanTxtBox.Enabled = false;
                ethNetTypeTxtBox.BorderStyle = BorderStyle.None;
                ethNetTypeTxtBox.Enabled = false;
                ethMacAddrTxtBox.BorderStyle = BorderStyle.None;
                ethMacAddrTxtBox.Enabled = false;
                ethEditBtn.Text = "Edit";

                // set values
            }
        }

        private void hardwareSelLstBox_MouseEnter(object sender, EventArgs e)
        {
            ListBox listbox = (ListBox)sender;
        }

        private void memoryTrkBar_ValueChanged(object sender, EventArgs e)
        {
            TrackBar tb = (TrackBar)sender;
            memoryUpDwn.Value = tb.Value;
            
            char[] chars = { ' ', 'M', 'B' };
            string max = memoryRecMax.Text.TrimEnd(chars);
            string min = memoryRecMin.Text.TrimEnd(chars);
            if (tb.Value > Convert.ToInt32(max))
            {
                memoryRecMin.ForeColor = SystemColors.WindowText;
                memoryRecMax.ForeColor = Color.Red;
            }
            else if (tb.Value < Convert.ToInt32(min))
            {
                memoryRecMin.ForeColor = Color.Red;
                memoryRecMax.ForeColor = SystemColors.WindowText;
            }
            else
            {
                memoryRecMin.ForeColor = SystemColors.WindowText;
                memoryRecMax.ForeColor = SystemColors.WindowText;
            }
        }

        private void memoryUpDwn_ValueChanged(object sender, EventArgs e)
        {
            memoryTrkBar.Value = (int)memoryUpDwn.Value;
        }

        private void cdromEnableChkBox_CheckedChanged(object sender, EventArgs e)
        {
            bool bEnabled = cdromEnableChkBox.Checked;

            cdromConGrpBox.Enabled = bEnabled;
        }

        private void floppyEnableChkBox_CheckedChanged(object sender, EventArgs e)
        {
            bool bEnabled = floppyEnableChkBox.Checked;

            floppyConnGrpBox.Enabled = bEnabled;
        }

        private void generalEditbutton_Click(object sender, EventArgs e)
        {
            generalVMName.ReadOnly = false;
            generalWorkDir.ReadOnly = false;
            generalWorkDirBrows.Enabled = true;
            generalEditbutton.Enabled = false;
        }

        private void generalOnVisible(object sender, EventArgs e)
        {
            Panel panel = (Panel)sender;

            if (panel.Visible)
            {
                generalVMName.Text = VirtMach.Name;
                generalWorkDir.Text = VirtMach.DefDir;

                if (VirtMach.MachType == "pc")
                    generalMachine.SelectedIndex = 0;
                else
                    generalMachine.SelectedIndex = 1;

                generalSetClockHost.Checked = VirtMach.SetClockToHost;
            }
        }

        private void harddiskLstBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox lb = (ListBox)sender;
            if (lb.SelectedItem != null)
            {
                VMHardDrive vmhd = (VMHardDrive)lb.SelectedItem;

                harddiskDriveName.Text = vmhd.Drive;
                harddiskFileNameTxtBox.Text = vmhd.Path;
                harddiskSizeLbl.Text = vmhd.Size.ToString();
                harddiskBootImageChk.Checked = vmhd.BootImg;
            }
        }

        private void harddiskAddBtn_Click(object sender, EventArgs e)
        {
            if (harddiskLstBox.Items.Count < 3)
            {
                ArrayList curDrives = new ArrayList(3);
                ArrayList hardDisks = VirtMach.GetHardDisks();
                
                foreach (VMHardDrive vmhd in hardDisks)
                {
                    curDrives.Add(vmhd.Drive);
                }

                NewHardDiskForm hdf = new NewHardDiskForm(curDrives);
                hdf.StartPosition = FormStartPosition.CenterParent;
                if (hdf.ShowDialog() == DialogResult.OK)
                {
                    VMHardDrive vmhd = VirtMach.AddHardDisk(hdf.DiskName,
                                                            hdf.QEmuDrive,
                                                            hdf.Path,
                                                            hdf.DiskSize,
                                                            hdf.BootImage);

                    if (vmhd != null)
                        harddiskLstBox.Items.Add(vmhd);
                }
            }
            else
            {
                MessageBox.Show("A maximum of 3 disk images is permittted");
            }
        }

        private void harddiskRemoveBtn_Click(object sender, EventArgs e)
        {
            int oldSel = harddiskLstBox.SelectedIndex;
            VMHardDrive vmhd = (VMHardDrive)harddiskLstBox.SelectedItem;

            DialogResult ret;
            ret = MessageBox.Show("Do you want to delete the image file too?",
                                  "Deleting harddisk",
                                  MessageBoxButtons.YesNoCancel,
                                  MessageBoxIcon.Warning);
            if (ret == DialogResult.Cancel)
            {
                return;
            }
            else if (ret == DialogResult.Yes)
            {
                ;//FIXME: delete the image
            }

            VirtMach.DeleteHardDisk(vmhd);
            harddiskLstBox.Items.Remove(vmhd);
            harddiskLstBox.SelectedIndex = oldSel - 1;
        }

        private void ethAddBtn_Click(object sender, EventArgs e)
        {
            NewNetCardForm ncf = new NewNetCardForm();
            ncf.StartPosition = FormStartPosition.CenterParent;
            if (ncf.ShowDialog() == DialogResult.OK)
            {

            }
        }
    }
}
