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
        ArrayList VirtualMachines = new ArrayList();

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
            // read config and load any existing vm's
            MainConfig mainConf = new MainConfig(this);
        }

        private void MainMenuHelpAbout_Click(object sender, EventArgs e)
        {
            AboutForm dlg = new AboutForm();
            dlg.StartPosition = FormStartPosition.CenterScreen;
            dlg.Show();
        }

        private void ImageListView_DoubleClick(object sender, EventArgs e)
        {
            SettingsForm setFrm = new SettingsForm();
            setFrm.StartPosition = FormStartPosition.CenterScreen;
            setFrm.Show();
        }

        private void toolbarExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void toolbarImageAdd_Click(object sender, EventArgs e)
        {
            NewVMWizard wizFrm = new NewVMWizard();
            wizFrm.StartPosition = FormStartPosition.CenterScreen;
            wizFrm.Show();
        }
    }
}
