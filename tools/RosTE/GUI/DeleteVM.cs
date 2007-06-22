using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public partial class DeleteVM : Form
    {
        public bool DeleteFullVM
        {
            get { return deleteFullVM.Checked; }
        }

        public DeleteVM(string text)
        {
            InitializeComponent();

            Text = Text + text;
            deleteNameLab.Text = text;
        }
    }
}