using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public partial class ErrorForm : Form
    {
        public ErrorForm(string message)
        {
            InitializeComponent();

            errorText.Text = message;
        }

        private void errorCloseBtn_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void errorCopyBtn_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(errorText.Text);
        }
    }
}