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
        //private ArrayList VirtualMachines;
        private MainConfig mainConf;
        private ArrayList vmConfigs;
        private Data mainData;
        private Data vmData;

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
            mainData = new Data();
            if (!mainData.LoadMainData())
                MessageBox.Show("Failed to load Main Schema");
            vmData = new Data();
            if (!vmData.LoadVirtMachData())
                MessageBox.Show("Failed to load VM Schema");

            vmConfigs = new ArrayList();

            // read config and load any existing vm's
            mainConf = new MainConfig(mainData);

            if (mainConf.LoadMainConfig())
                mainConf.LoadExistingImages(VirtMachListView);
        }

        private void MainMenuHelpAbout_Click(object sender, EventArgs e)
        {
            AboutForm dlg = new AboutForm();
            dlg.StartPosition = FormStartPosition.CenterScreen;
            dlg.Show();
        }

        private void ImageListView_DoubleClick(object sender, EventArgs e)
        {
            ListViewItem lvi = VirtMachListView.FocusedItem;
            //lvi.Tag

            SettingsForm setFrm = new SettingsForm(lvi.Text);
            setFrm.StartPosition = FormStartPosition.CenterScreen;
            setFrm.Show();
        }

        private void toolbarExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void CreateNewVirtMach(object sender, EventArgs e)
        {
            NewVMWizard wizFrm = new NewVMWizard();
            wizFrm.StartPosition = FormStartPosition.CenterScreen;

            if (wizFrm.ShowDialog() == DialogResult.OK)
            {
                if (wizFrm.Option == 1)
                {
                    int i = mainConf.AddVirtMach(wizFrm.DefDir);
                    VirtMachListView.Items.Add(i.ToString(), wizFrm.VMName, 0);

                    vmConfigs.Add(new VMConfig(vmData,
                                               wizFrm.VMName,
                                               wizFrm.DefDir,
                                               wizFrm.DiskSizeGB,
                                               wizFrm.ExistImg,
                                               wizFrm.MemSizeMB));
                }
                else if (wizFrm.Option == 2)
                {

                    DirectoryInfo di = Directory.GetParent(wizFrm.ExistImg);
                    int i = mainConf.AddVirtMach(di.FullName);
                    VirtMachListView.Items.Add(i.ToString(), wizFrm.VMName, 0);

                    vmConfigs.Add(new VMConfig(vmData,
                                               wizFrm.VMName,
                                               wizFrm.ExistImg,
                                               wizFrm.MemSizeMB));
                }
                else
                {
                    int i = mainConf.AddVirtMach("Images\\" + wizFrm.VMName);
                    VirtMachListView.Items.Add(i.ToString(), wizFrm.VMName, 0);

                    vmConfigs.Add(new VMConfig(vmData,
                                               wizFrm.VMName));
                }
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
                    mainConf.DeleteVirtMach(lvi.Index/*lvi.Tag*/);
                    VirtMachListView.Items.Remove(lvi);
                    vmConfigs.Remove(lvi.Tag);
                }
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            mainConf.SaveMainConfig();
        }
    }
}
