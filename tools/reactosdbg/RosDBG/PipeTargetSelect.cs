using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RosDBG
{
    public partial class PipeTargetSelect : Form
    {
        public string PipeName
        {
            get { return PipeNameTextBox.Text; }
            set { PipeNameTextBox.Text = value; }
        }

        public PipeTargetSelect()
        {
            InitializeComponent();
        }

        private void bOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rb = (RadioButton)sender;

            PipeNameTextBox.Enabled = rb.Checked;
        }
    }
}
