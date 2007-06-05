using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Management;
using System.Runtime.InteropServices;


namespace RosTEGUI
{
    public partial class SettingsForm : Form
    {
        private Panel[] hardwarePanels;
        private Panel[] optionsPanels;
        private int hardwarePrevSel = 0;
        private int optionsPrevSel = 0;

        public SettingsForm()
        {
            InitializeComponent();
        }

        private void LoadDynamicControlInfo()
        {
            // set the memory dialog info
            ulong totMem = Native.Memory.GetTotalMemory();
            if (totMem != 0)
            {
                totMem /= 1048576; //(1024^2)
                memoryPhyRam.Text = Convert.ToString(totMem) + " MB";

                memoryTrkBar.Maximum = Convert.ToInt32(totMem) * 2;
                memoryTrkBar.TickFrequency = memoryTrkBar.Maximum / 20;
                memoryUpDwn.Maximum = memoryTrkBar.Maximum;

                memoryMinLab.Text = Convert.ToString(0) + " MB";
                memoryMaxLab.Text = memoryTrkBar.Maximum.ToString() + " MB";

                memoryRecMin.Text = Convert.ToString(totMem / 8) + " MB";
                memoryRec.Text = Convert.ToString(totMem / 4) + " MB";
                memoryRecMax.Text = Convert.ToString(totMem / 1.4) + " MB";
            }
        }

        private void GetHardwareInfo()
        {

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

            LoadDynamicControlInfo();
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

        private void memoryTrkBar_Scroll(object sender, EventArgs e)
        {
            memoryUpDwn.Value = memoryTrkBar.Value;
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
    }
}