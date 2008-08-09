using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace RosDBG
{
    public partial class SerialTargetSelect : Form
    {
        public String Port
        {
            get { return cPort.Text; }
            set { Port = value; }
        }

        public int Baudrate
        {
            get { return Convert.ToInt32(cBaud.Text); }
            set { Baudrate = value; }
        }

        public SerialTargetSelect()
        {
            InitializeComponent();
            foreach (string s in SerialPort.GetPortNames())
            {
                cPort.Items.Add(s);
            }
            cPort.SelectedIndex = 0;
            cBaud.SelectedIndex = 0;
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
    }
}