using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Xml.Serialization;
using Microsoft.Win32;
using System.Collections.Generic;

namespace RosTEGUI
{
	public partial class MainForm : Form
    {
        private MainConfig mainConf;
        private ArrayList virtualMachines;

        public MainForm()
        {
            InitializeComponent();
        }

        private void LoadSettingsDialog()
        {
            ListViewItem lvi = VirtMachListView.FocusedItem;

            SettingsForm setFrm = new SettingsForm(lvi.Tag);
            setFrm.StartPosition = FormStartPosition.CenterScreen;
            setFrm.Show();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void LoadVirtualMachines(MainConfig mainConf)
        {
            int num = mainConf.GetNumberOfVms();
            for (int i = 0; i < num; i++)
            {
                string path = mainConf.GetImagePath(i);
                VirtMachConfig vmConfig = new VirtMachConfig();
                if (vmConfig.LoadVMConfig(path))
                {
                    if (vmConfig.LoadVmSettings())
                    {
                        foreach (VirtMachInfo vmInfo in vmConfig.VMInfo)
                        {
                            VirtualMachine vm = new VirtualMachine(vmInfo);

                            ListViewItem lvi = VirtMachListView.Items.Add(vmConfig.ToString(), 0);
                            lvi.SubItems.Add(vm.MemSize.ToString() + " MB");
                            lvi.Tag = vm;
                        }
                    }
                }
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            mainConf = new MainConfig();
            if (mainConf.LoadMainConfig())
            {
                mainConf.LoadSettings();
                LoadVirtualMachines(mainConf);
            }
            else
            {
                // create settings for first run
                mainConf.CreateSettings();
            }
        }

        private void MainMenuHelpAbout_Click(object sender, EventArgs e)
        {
            AboutForm dlg = new AboutForm();
            dlg.StartPosition = FormStartPosition.CenterScreen;
            dlg.ShowDialog();
        }

        private void ImageListView_DoubleClick(object sender, EventArgs e)
        {
            LoadSettingsDialog();
        }

        private void toolbarExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void CreateNewVirtMach(object sender, EventArgs e)
        {
            NewVMWizard wizFrm = new NewVMWizard(mainConf);
            wizFrm.StartPosition = FormStartPosition.CenterScreen;

            if (wizFrm.ShowDialog() == DialogResult.OK)
            {
                VirtualMachine virtMach = new VirtualMachine();
                virtMach.Name = wizFrm.VMName;

                switch (wizFrm.Option)
                {
                    case 1:
                        if (!Directory.Exists(wizFrm.DefDir))
                            Directory.CreateDirectory(wizFrm.DefDir);

                        virtMach.DefDir = wizFrm.DefDir;
                        break;

                    case 2:

                        break;

                    case 3:
                        virtMach.DefDir = "Images\\" + wizFrm.VMName;
                        break;
                }

                ListViewItem lvi = VirtMachListView.Items.Add(virtMach.Name, 0);
                lvi.SubItems.Add(virtMach.MemSize.ToString() + " MB");
                lvi.Tag = virtMach;
            }
        }

        private void DeleteVirtMach(object sender, EventArgs e)
        {
            ListViewItem lvi = VirtMachListView.FocusedItem;
            if (lvi != null)
            {
                DeleteVM delFrm = new DeleteVM(lvi.Text/*lvi.Tag*/);
                delFrm.StartPosition = FormStartPosition.CenterScreen;

                if (delFrm.ShowDialog() == DialogResult.OK)
                {
                    if (delFrm.DeleteFullVM)
                    {
                        VirtualMachine vm = (VirtualMachine)lvi.Tag;

                        try
                        {
                            Directory.Delete(vm.DefDir, true);
                        }
                        catch (DirectoryNotFoundException)
                        {
                            MessageBox.Show(vm.DefDir + " has was not found!",
                                            "error",
                                            MessageBoxButtons.OK,
                                            MessageBoxIcon.Error);
                        }
                    }

                    //mainConf.DeleteVirtMach(lvi.Index/*lvi.Tag*/);
                    VirtMachListView.Items.Remove(lvi);
                }
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            VirtMachConfig vmc = new VirtMachConfig();

            foreach (ListViewItem lvi in VirtMachListView.Items)
            {
                VirtualMachine vm = (VirtualMachine)lvi.Tag;
                vmc.SaveVMConfig(vm);
            }

            mainConf.SaveSettings();
            mainConf.SaveMainConfig();
        }

        private void changeSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadSettingsDialog();
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConsoleSettings cs = new ConsoleSettings(mainConf);
            cs.StartPosition = FormStartPosition.CenterParent;
            if (cs.ShowDialog() == DialogResult.OK)
            {
                mainConf.UpdateSched = cs.UpdateSched;
                mainConf.QemuPath = cs.QemuPath;
                mainConf.VdkPath = cs.VdkPath;
                mainConf.DefVmPath = cs.DefVmPath;
                mainConf.AppDebug = cs.AppDebug;
            }
        }
    }
}
