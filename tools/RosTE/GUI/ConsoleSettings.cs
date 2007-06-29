using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public partial class ConsoleSettings : Form
    {
        private string GetFolderLocation()
        {
            folderBrowserDialog.ShowDialog();
            return folderBrowserDialog.SelectedPath;
        }

        public ConsoleSettings(MainConfig mainConf)
        {
            InitializeComponent();

            // set general tab
            conUpdateCombo.SelectedIndex = mainConf.UpdateSched;

            // set paths tab
            conQemuLoc.Text = mainConf.QemuPath;
            conVdkLoc.Text = mainConf.VdkPath;
            conDefVmLoc.Text = mainConf.DefVmPath;

            // set advanced tab
            conAppDebug.Checked = mainConf.AppDebug;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            conQemuLoc.Text = GetFolderLocation();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            conVdkLoc.Text = GetFolderLocation();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            conDefVmLoc.Text = GetFolderLocation();
        }
    }
}