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
        public SettingsForm()
        {
            InitializeComponent();
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            listboxSelection.ItemHeight = listboxSelection.Height / listboxSelection.Items.Count;
        }

        private void listboxSelection_DrawItem(object sender, DrawItemEventArgs e)
        {
            if ((e.State & DrawItemState.Selected) == DrawItemState.Selected)
            {
                e.Graphics.FillRectangle(Brushes.LightBlue, e.Bounds);
            }
            else
            {
                e.Graphics.FillRectangle(Brushes.LightYellow, e.Bounds);
            }

            StringFormat strfmt = new StringFormat();

            strfmt.Alignment = StringAlignment.Center;
            strfmt.LineAlignment = StringAlignment.Center;

            e.Graphics.DrawString(listboxSelection.Items[e.Index].ToString(),
                                  this.Font, 
                                  Brushes.Black, 
                                  e.Bounds.X + e.Bounds.Width / 2,
                                  e.Bounds.Y + e.Bounds.Height / 2,
                                  strfmt);

            e.DrawFocusRectangle();
        }
    }
}