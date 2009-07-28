using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection; 

namespace RosDBG
{
    public partial class AboutDlg : Form
    {
        public AboutDlg()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AboutDlg_Load(object sender, EventArgs e)
        {
            rosDbgLabel.Text += " " + Assembly.GetExecutingAssembly().GetName().Version.ToString();
        }
    }
}
