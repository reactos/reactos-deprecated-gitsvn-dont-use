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
        public ErrorForm(string message, string exception, string trace)
        {
            InitializeComponent();

            errorText.Text = message + " : " + exception + "\n\t" + trace;
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