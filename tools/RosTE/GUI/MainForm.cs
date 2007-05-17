using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Xml.Serialization;
using Microsoft.Win32;


namespace RosTEGUI
{
	public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
        }

        private void MainMenuHelpAbout_Click(object sender, EventArgs e)
        {
            AboutForm Dlg = new AboutForm();
            Dlg.StartPosition = FormStartPosition.CenterScreen;
            Dlg.Show();
        }

        private void ImageListView_DoubleClick(object sender, EventArgs e)
        {
            SettingsForm SetFrm = new SettingsForm();
            SetFrm.StartPosition = FormStartPosition.CenterScreen;
            SetFrm.Show();
        }

        private void toolbarExit_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
