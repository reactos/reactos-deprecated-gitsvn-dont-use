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
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            hardwareSelLstBox.ItemHeight = hardwareSelLstBox.Height / hardwareSelLstBox.Items.Count;
            hardwareSelLstBox.SelectedItem = 0;
            hardwareSelLstBox.SelectedValue = 0;

            optionsSelLstBox.ItemHeight = optionsSelLstBox.Height / optionsSelLstBox.Items.Count;

            hardwarePanels = new Panel[] { memoryPanel, cdromPanel, harddiskPanel, networkPanel, ethernetPanel, soundPanel, displayPanel };
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

        private void listboxSelection_DrawItem(object sender, DrawItemEventArgs e)
        {
            if ((e.State & DrawItemState.Selected) == DrawItemState.Selected)
            {
                e.Graphics.FillRectangle(Brushes.Silver, e.Bounds);
            }
            else
            {
                e.Graphics.FillRectangle(Brushes.White, e.Bounds);
            }

            string buttStr = null;
            if (settingsTab.SelectedIndex == 0)
            {
                buttStr = hardwareSelLstBox.Items[e.Index].ToString();
            }
            else if (settingsTab.SelectedIndex == 1)
            {
                buttStr = optionsSelLstBox.Items[e.Index].ToString();
            }

            StringFormat strfmt = new StringFormat();
            strfmt.Alignment = StringAlignment.Center;
            strfmt.LineAlignment = StringAlignment.Center;

            e.Graphics.DrawString(buttStr,
                                  this.Font,
                                  Brushes.Black,
                                  e.Bounds.X + e.Bounds.Width / 2,
                                  e.Bounds.Y + e.Bounds.Height / 2,
                                  strfmt);

            e.DrawFocusRectangle();
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
    }
}