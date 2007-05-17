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
        private Panel[] panels;
        private int PrevSel = 0;

        public SettingsForm()
        {
            InitializeComponent();
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            selectionLstBox.ItemHeight = selectionLstBox.Height / selectionLstBox.Items.Count;
            selectionLstBox.SelectedItem = 0;
            selectionLstBox.SelectedValue = 0;

            panels = new Panel[] { generalPanel, pathsPanel, drivesPanel, networkPanel, ioPanel, mountPanel, debugPanel };

            foreach (Panel pan in panels)
            {
                pan.Parent = tempDesignSheetTab.Parent;
                pan.Visible = false;
            }

            tempDesignSheetTab.Visible = false;
            panels[0].Visible = true;
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

            StringFormat strfmt = new StringFormat();

            strfmt.Alignment = StringAlignment.Center;
            strfmt.LineAlignment = StringAlignment.Center;

            e.Graphics.DrawString(selectionLstBox.Items[e.Index].ToString(),
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

            panels[PrevSel].Visible = false;
            PrevSel = listbox.SelectedIndex;
            panels[(int)listbox.SelectedIndex].Visible = true;
        }
    }
}