using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace RosTEGUI
{
    public partial class ConsoleSettings : Form
    {
        public string QemuPath
        {
            get { return conQemuLoc.Text; }
        }

        public string VdkPath
        {
            get { return conVdkLoc.Text; }
        }

        public string DefVmPath
        {
            get { return conDefVmLoc.Text; }
        }

        public int UpdateSched
        {
            get { return conUpdateCombo.SelectedIndex; }
        }

        public bool AppDebug
        {
            get { return conAppDebug.Checked; }
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
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                conQemuLoc.Text = folderBrowserDialog.SelectedPath;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                conVdkLoc.Text = folderBrowserDialog.SelectedPath;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                conDefVmLoc.Text = folderBrowserDialog.SelectedPath;
        }

        private void conDialogOK_Click(object sender, EventArgs e)
        {
            if (!File.Exists(conQemuLoc.Text + "\\qemu.exe"))
            {
                MessageBox.Show("Cannot find qemu.exe in " + conQemuLoc.Text);
                return;
            }

            if (!File.Exists(conVdkLoc.Text + "\\vdk.exe"))
            {
                MessageBox.Show("Cannot find vdk.exe in " + conVdkLoc.Text);
                return;
            }

            if (!Directory.Exists(conDefVmLoc.Text))
            {
                MessageBox.Show(conDefVmLoc.Text + " does not exist");
                return;
            }

            this.DialogResult = DialogResult.OK;
        }

    }
}