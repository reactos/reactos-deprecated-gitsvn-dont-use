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
    public partial class TCPTargetSelect : Form
    {
        public string Host
        {
            get { return HostChoice.Text; }
            set { HostChoice.Text = value; }
        }

        public int Port
        {
            get { return (int)PortNumber.Value; }
            set { PortNumber.Value = value; }
        }

        public TCPTargetSelect()
        {
            InitializeComponent();
        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }
    }
}
