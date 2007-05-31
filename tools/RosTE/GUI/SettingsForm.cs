using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

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
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            hardwareSelLstBox.ItemHeight = hardwareSelLstBox.Height / hardwareSelLstBox.Items.Count;
            hardwareSelLstBox.SelectedItem = 0;
            hardwareSelLstBox.SelectedValue = 0;

            optionsSelLstBox.ItemHeight = hardwareSelLstBox.ItemHeight;

            hardwarePanels = new Panel[] { memoryPanel, cdromPanel, harddiskPanel, networkPanel, ethernetPanel, audioPanel, displayPanel };
            optionsPanels = new Panel[] { generalPanel, powerPanel, snapshotsPanel, advancedPanel};

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

            listbox.Invalidate();
        }
    }
}