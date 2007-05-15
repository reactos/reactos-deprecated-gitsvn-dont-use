using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Xml.Serialization;
using Microsoft.Win32;


namespace Qemu_GUI
{
	public partial class MainForm : Form
    {
        private ToolStripSeparator toolStripSeparator2;
        private ToolStripMenuItem setAsDefaultToolStripMenuItem;
        private const string strRegKey = "Software\\RosTE";
        private const string strDefCon = "DefaultConfig";

        private Runner runner;
        private string currentDir = Directory.GetCurrentDirectory();
        private Data data = new Data();

        public MainForm()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            /* Fill a list with possible free driveletters */
            ArrayList DriveLetters = new ArrayList();
            for (int i = 100; i < 123; i++)
            {
                DriveLetters.Add(Convert.ToChar(i).ToString().ToUpper() + @":\");
            }

            DriveInfo[] drives = DriveInfo.GetDrives();
            foreach (DriveInfo drive in drives)
            {
                /* remove all driveletters which are in use */
                DriveLetters.Remove(drive.RootDirectory.ToString());
                if (drive.DriveType == DriveType.CDRom)
                    cboCDROM.Items.Add(drive.RootDirectory);
            }

            foreach (object o in DriveLetters)
                cboVDKDrive.Items.Add(o);

            cboVDKDrive.SelectedIndex = 0;
            cboCDROM.SelectedIndex = 0;
            cboBootFrom.SelectedIndex = 1;
            cboImageFormat.SelectedIndex = 4;
            cboMachine.SelectedIndex = 0;

            /* load default config file */
            LoadConfigFile();

            /* Find out which controls should be enabled or disabled */
            chkSerialToFile_CheckedChanged(null, null);
            chkSerialToPipe_CheckedChanged(null, null);
        }

        #region Harddisks
        private void btnBrowseHDA_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Select Harddisk Image";
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img|All Files | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDA.Text = openFile.FileName;
        }

        private void btnBrowseHDB_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Select Harddisk Image";
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img|All Files | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDB.Text = openFile.FileName;
        }

        private void btnBrowseHDC_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Select Harddisk Image";
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img|All Files | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDC.Text = openFile.FileName;
        }

        private void btnBrowseHDD_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Select Harddisk Image";
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img|All Files | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDD.Text = openFile.FileName;
        }

        private void chkUseHDA_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseHDA.Enabled = chkUseHDA.Checked;
            txtHDA.Enabled = chkUseHDA.Checked;
        }

        private void chkUseHDB_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseHDB.Enabled = chkUseHDB.Checked;
            txtHDB.Enabled = chkUseHDB.Checked;
        }

        private void chkUseHDC_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseHDC.Enabled = chkUseHDC.Checked;
            txtHDC.Enabled = chkUseHDC.Checked;
            txtHDC.Text = "";
        }

        private void chkUseHDD_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseHDD.Enabled = chkUseHDD.Checked;
            txtHDD.Enabled = chkUseHDD.Checked;
        }



        #endregion

        #region CDROM
        private void btnBrowseCDROM_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Select CD-ROM image";
            openFile.Filter = "CD-Images (*.iso)|*.iso";  
            if (openFile.ShowDialog() == DialogResult.OK) 
                txtCDROM.Text = openFile.FileName;
        }

        private void chkUseCDROM_CheckedChanged(object sender, System.EventArgs e)
        {

            if (chkUseHDC.Checked == true)
            {
                optHostCDROM.Enabled = false;
                cboCDROM.Enabled = false;
                optCDImage.Enabled = false;
                btnBrowseCDROM.Enabled = false;
                txtCDROM.Enabled = false;
            }
            else
            {
                chkUseCDROM.Enabled = true;
            }

            if (chkUseCDROM.Checked == true)
            {
                chkUseHDC.Enabled = false;
                chkUseHDC.Checked = false;
                txtHDC.Text = "Used for CD-ROM!";
                txtHDC.Enabled = false;
                txtCDROM.Enabled = true;
                optCDImage.Enabled = true;
                if (optCDImage.Checked == true)
                {
                    cboCDROM.Enabled = false;
                    btnBrowseCDROM.Enabled = true;
                }
                else
                {
                    cboCDROM.Enabled = true;
                    btnBrowseCDROM.Enabled = false;
                }
            }
            else
            {
                optHostCDROM.Enabled = false;
                cboCDROM.Enabled = false;
                optCDImage.Enabled = false;
                btnBrowseCDROM.Enabled = false;
                txtCDROM.Enabled = false;
                chkUseHDC.Enabled = true;
                txtHDC.Text = "";
            }
        }

        private void optHostCDROM_CheckedChanged(object sender, System.EventArgs e)
        {
            cboCDROM.Enabled = !optCDImage.Checked;
            btnBrowseCDROM.Enabled = optCDImage.Checked;
            txtCDROM.Enabled = false;
        }

        private void optCDImage_CheckedChanged(object sender, System.EventArgs e)
        {
            cboCDROM.Enabled = !optCDImage.Checked;
            btnBrowseCDROM.Enabled = optCDImage.Checked;
            txtCDROM.Enabled = true;
        }

        #endregion

        #region Debug

        private void chkSerialToScrn_CheckedChanged(object sender, EventArgs e)
        {
            if (chkSerialToScrn.Checked == true)
            {
                btnBrowseSerial.Enabled = false;
                txtSerialFile.Enabled = false;
                txtSerialPipe.Enabled = false;
                data.Debug.SerialPort.SRedirect = true;
                data.Debug.SerialPort.FRedirect = false;
                data.Debug.SerialPort.PRedirect = false;
                chkSerialToPipe.Enabled = false;
                chkSerialToFile.Enabled = false;
                chkSerialToPipe.Checked = false;
                chkSerialToFile.Checked = false;
                
            }
            else
            {
                btnBrowseSerial.Enabled = true;
                data.Debug.SerialPort.SRedirect = false;
                data.Debug.SerialPort.FRedirect = false;
                data.Debug.SerialPort.PRedirect = false;
                chkSerialToPipe.Enabled = true;
                chkSerialToFile.Enabled = true;

            }
        }
        private void chkSerialToFile_CheckedChanged(object sender, System.EventArgs e)
        {
            if (chkSerialToFile.Checked == true)
            {
                btnBrowseSerial.Enabled = true;
                txtSerialFile.Enabled = true;
                txtSerialPipe.Enabled = false;
                data.Debug.SerialPort.FRedirect = true;
                data.Debug.SerialPort.PRedirect = false;
                chkSerialToPipe.Enabled = false;
                chkSerialToPipe.Checked = false;
                chkSerialToScrn.Enabled = false;
                chkSerialToScrn.Checked = false;
            }
            else
            {
                data.Debug.SerialPort.FRedirect = false;
                chkSerialToPipe.Enabled = true;
                chkSerialToScrn.Enabled = true;
                txtSerialFile.Enabled = false;
                btnBrowseSerial.Enabled = false;
            }
        }
        private void chkSerialToPipe_CheckedChanged(object sender, EventArgs e)
        {
            if (chkSerialToPipe.Checked == true)
            {
                txtSerialPipe.Enabled = true;
                data.Debug.SerialPort.FRedirect = false;
                data.Debug.SerialPort.PRedirect = true;
                chkSerialToFile.Enabled = false;
                chkSerialToFile.Checked = false;
                txtSerialFile.Enabled = false;
                btnBrowseSerial.Enabled = false;
            }
            else
            {
                chkSerialToFile.Enabled = true;
                data.Debug.SerialPort.PRedirect = false;
                txtSerialPipe.Enabled = false;
            }
        }



        private void btnBrowseSerial_Click(object sender, System.EventArgs e)
        {
            saveFileDialog.Title = "Serial output to file";
            saveFileDialog.FileName = "serial_ouput.txt";
            saveFileDialog.Filter = "Text File (*.txt)|*.txt";
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                txtSerialFile.Text = saveFileDialog.FileName;
            }
        }

        static string saved_state;
        private void btnLoadVM_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            saved_state = openFile.FileName;
            data.Debug.EnableSavedState = true;
        }
        
        private void chkVNC_CheckedChanged(object sender, System.EventArgs e)
        {
            txtVNC.Enabled = chkVNC.Checked;
        }
        #endregion

        #region Floppy
        private void chkFloppyA_CheckedChanged(object sender, EventArgs e)
        {
            btnBrowseFloppyA.Enabled = chkFloppyA.Checked;
            txtFloppyA.Enabled = chkFloppyA.Checked;
        }

        private void chkFloppyB_CheckedChanged(object sender, EventArgs e)
        {
            btnBrowseFloppyB.Enabled = chkFloppyB.Checked;
            txtFloppyB.Enabled = chkFloppyB.Checked;
        }

        private void btnBrowseFloppyA_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Browse for a floppy image";
            openFile.Filter = "All Files | *.*";

            if (openFile.ShowDialog() == DialogResult.OK)
                txtFloppyA.Text = openFile.FileName;
        }

        private void btnBrowseFloppyB_Click(object sender, System.EventArgs e)
        {
            openFile.Title = "Browse for a floppy image";
            openFile.Filter = "All Files | *.*";

            if (openFile.ShowDialog() == DialogResult.OK)
                txtFloppyB.Text = openFile.FileName;
        }

        #endregion

        #region Paths
        private void btnQEmuPath_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.Description = "Select Qemu path";
            folderBrowserDialog1.ShowNewFolderButton = false;
            folderBrowserDialog1.RootFolder = Environment.SpecialFolder.Programs;

            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                txtQEmuPath.Text = folderBrowserDialog1.SelectedPath;
                data.Paths.Qemu = txtQEmuPath.Text; 
            }
        }

        private void btnVDKBrowse_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.Description = "Select VDK path";
            folderBrowserDialog1.ShowNewFolderButton = false;
            folderBrowserDialog1.RootFolder = Environment.SpecialFolder.Programs;

            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                txtVDKPath.Text = folderBrowserDialog1.SelectedPath;
                data.Paths.VDK = txtVDKPath.Text;
            }
        }

        #endregion

        #region Settings
        private void btnSave_Click(object sender, EventArgs e)
        {
            XmlSerializer s;
            TextWriter w;
            SaveSettings();
            saveFileDialog.Title = "Save settings";
            saveFileDialog.InitialDirectory = currentDir;
            saveFileDialog.FileName = "config.xml";
            saveFileDialog.Filter = "QEMU GUI Config (*.xml)|*.xml"; 
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    s = new XmlSerializer(typeof(Data));
                    w = new StreamWriter(saveFileDialog.FileName);
                    s.Serialize(w, data);
                }
                catch (Exception ex)
                {
                    ErrorForm error = new ErrorForm();
                    error.txtError.Text = "Error trying to save settings, contact developers with error log!";
                    error.txtError.Text += "Exception Info:" + Environment.NewLine + ex.Message;
                    error.txtError.Text += Environment.NewLine + ex.StackTrace;
                }
                                
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void LoadSettings()
        {
            string[] temp;
            /* General */
            cboMachine.SelectedIndex = (int) data.General.Machine;
            try
            {
                numMemory.Value = data.General.Memory;
            }
            catch (Exception)
            {
                numMemory.Value = 64;
            }
            try
            {
                numSMP.Value = data.General.CPUs;
            }
            catch (Exception)
            {
                numSMP.Value = 1;
            }
            cboBootFrom.SelectedItem = data.General.BootFrom;
            chkSetClock.Checked = data.General.SetClock;
            chkVGAoutput.Checked = data.General.VGA;
            chkFullscreen.Checked = data.General.Fullscreen;
            chkKQEmu.Checked = data.General.KQEmu;
            chkACPI.Checked = data.General.ACPI;

            /* CD-ROM */
            chkUseCDROM.Checked = data.CDROM.Enabled;
            if (chkUseCDROM.Checked)
            {
                txtHDC.Text = "Used for CD-ROM!";
                txtHDC.Enabled = false;
            }
            optHostCDROM.Checked = data.CDROM.UseFromHost;
            optCDImage.Checked = !data.CDROM.UseFromHost;
            txtCDROM.Text = data.CDROM.Image;
            cboCDROM.Text = data.CDROM.HostDrive;

            /* Floppies */
            chkFloppyA.Checked = data.Floppies.FDD[0].Enabled;
            txtFloppyA.Text = data.Floppies.FDD[0].Path;
            chkFloppyB.Checked = data.Floppies.FDD[1].Enabled;
            txtFloppyB.Text = data.Floppies.FDD[1].Path;

            /* Paths */
            txtQEmuPath.Text = data.Paths.Qemu;
            txtVDKPath.Text = data.Paths.VDK;

            /* Harddisks */
            chkUseHDA.Checked = data.Harddisks.HDD[0].Enabled;
            txtHDA.Text = data.Harddisks.HDD[0].Path;
            chkUseHDB.Checked = data.Harddisks.HDD[1].Enabled;
            txtHDB.Text = data.Harddisks.HDD[1].Path;
            chkUseHDC.Checked = data.Harddisks.HDD[2].Enabled;
            txtHDC.Text = data.Harddisks.HDD[2].Path;
            chkUseHDD.Checked = data.Harddisks.HDD[3].Enabled;
            txtHDD.Text = data.Harddisks.HDD[3].Path;

            /* Audio */
            chkES1370.Checked = data.Audio.ES1370;
            chkSoundBlaster.Checked = data.Audio.Soundblaster;
            chkPCSpeaker.Checked = data.Audio.Speaker;
            chkOPL2.Checked = data.Audio.OPL2;

            /* Debug */
            chkSerialToFile.Checked = data.Debug.SerialPort.FRedirect;
            txtSerialFile.Text = data.Debug.SerialPort.FileName;
            chkSerialToPipe.Checked = data.Debug.SerialPort.PRedirect;
            txtSerialPipe.Text = data.Debug.SerialPort.PipeName;
            chkSerialToScrn.Checked = data.Debug.SerialPort.SRedirect;
            chkSerialToFile_CheckedChanged(null, null);

            saved_state = data.Debug.SavedStatePath;
            chkVBE30.Checked = data.Debug.VBE3;
            txtGDBPort.Text = data.Debug.GDBPort.ToString();

            /* Network */
            foreach (string a in data.Network.VNicStringReader())
            {
                listVLANs.Items.Add(a.ToString());
                temp = a.Split(' ');
                if (temp[1].Contains("user"))
                {
                    VUser vnic = new VUser();
                    vnic.vlan = Int32.Parse(temp[1].Substring(10, 1));
                    vnic.hostname = temp[1].Substring(21);
                    VLanlist.Add(vnic);
                }
                else
                {
                    MessageBox.Show("unimplemented setting load for network!");
                }
            }

            /* Tools */
            txtVDKImage.Text = data.Tools.vdk.Image;
            cboVDKDrive.Text = data.Tools.vdk.DriveLetter;

            /* Other */
            txtLinuxK.Text = data.Other.LKernel;
            txtABios.Text = data.Other.ABios;
            txtAppendCmd.Text = data.Other.AppendCmdLine;
        }

        private void SaveSettings()
        {
            /* General */
            data.General.Machine = (Platforms) cboMachine.SelectedIndex;
            data.General.Memory = (int) numMemory.Value;
            data.General.CPUs = (int) numSMP.Value;
            data.General.BootFrom = cboBootFrom.SelectedItem.ToString();
            data.General.SetClock = chkSetClock.Checked;
            data.General.VGA = chkVGAoutput.Checked;
            data.General.Fullscreen = chkFullscreen.Checked;
            data.General.KQEmu = chkKQEmu.Checked;
            data.General.ACPI = chkACPI.Checked;

            /* Paths */
            data.Paths.Qemu = txtQEmuPath.Text;
            data.Paths.VDK = txtVDKPath.Text;

            /* Floppies */
            data.Floppies.FDD[0].Enabled = chkFloppyA.Checked;
            data.Floppies.FDD[0].Path = txtFloppyA.Text;
            data.Floppies.FDD[1].Enabled = chkFloppyB.Checked;
            data.Floppies.FDD[1].Path = txtFloppyB.Text;

            /* CD-ROM */
            data.CDROM.UseFromHost = optHostCDROM.Checked;
            data.CDROM.Image = txtCDROM.Text;
            data.CDROM.HostDrive = cboCDROM.Text;
            data.CDROM.Enabled = chkUseCDROM.Checked;

            /* Harddisks */
            data.Harddisks.HDD[0].Enabled = chkUseHDA.Checked;
            data.Harddisks.HDD[0].Path = txtHDA.Text;
            data.Harddisks.HDD[1].Enabled = chkUseHDB.Checked;
            data.Harddisks.HDD[1].Path = txtHDB.Text;
            data.Harddisks.HDD[2].Enabled = chkUseHDC.Checked;
            data.Harddisks.HDD[2].Path = txtHDC.Text;
            data.Harddisks.HDD[3].Enabled = chkUseHDD.Checked;
            data.Harddisks.HDD[3].Path = txtHDD.Text;

            /* Audio */
            data.Audio.ES1370 = chkES1370.Checked;
            data.Audio.Soundblaster = chkSoundBlaster.Checked;
            data.Audio.Speaker = chkPCSpeaker.Checked;
            data.Audio.OPL2 = chkOPL2.Checked;

            /* Debug */
            data.Debug.SerialPort.SRedirect = chkSerialToScrn.Checked;
            data.Debug.SerialPort.FRedirect = chkSerialToFile.Checked;
            data.Debug.SerialPort.FileName = txtSerialFile.Text;
            data.Debug.SerialPort.PRedirect = chkSerialToPipe.Checked;
            data.Debug.SerialPort.PipeName = txtSerialPipe.Text;
            data.Debug.SavedStatePath = saved_state;
            data.Debug.VBE3 = chkVBE30.Checked;
            data.Debug.GDBPort = Int32.Parse(txtGDBPort.Text);

            /* Network */
            for (int i = 0; i < VLanlist.Count; i++)
            {
                string vlan = VLanlist[i].ToString();
                data.Network.VNicString += vlan;
            }
            /* Tools */
            data.Tools.vdk.Image = txtVDKImage.Text;
            data.Tools.vdk.DriveLetter = cboVDKDrive.Text;

            /* Other */
            data.Other.LKernel = txtLinuxK.Text;
            data.Other.ABios = txtABios.Text;
            data.Other.AppendCmdLine = txtAppendCmd.Text;
        }

        #endregion

        #region VDK

        private void btnVDKImage_Click(object sender, EventArgs e)
        {
            openFile.Title = "Virtual Hardisk Image";
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img|All Files | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                txtVDKImage.Text = openFile.FileName;
                data.Tools.vdk.Image = openFile.FileName;
            }
        }
        
        private void cboVDKDrive_SelectedIndexChanged(object sender, EventArgs e)
        {
            data.Tools.vdk.DriveLetter = cboVDKDrive.SelectedItem.ToString();
        }

        private void btnUnmount_Click(object sender, EventArgs e)
        {
            data.Paths.VDK = txtVDKPath.Text;
            
            if (data.Paths.VDK == "")
                return;

            runner = new Runner(data);
            runner.UnmountImage();
        }

        private void btnMount_Click(object sender, EventArgs e)
        {
            data.Tools.vdk.Image  = txtVDKImage.Text;
            data.Paths.VDK = txtVDKPath.Text;

            /* Check out if we have a vdk path and a image path */
            if (data.Tools.vdk.Image == "" || data.Paths.VDK == "")
            {
                /* we don't do nothing */
                return;
            }
            runner = new Runner(data);
            if (runner.MountImage() == true)
            {
                Thread.Sleep(500);
                Process p = new Process();

                p.StartInfo.FileName = "explorer";
                p.StartInfo.Arguments = data.Tools.vdk.DriveLetter;
                try
                {
                    p.Start();
                }
                catch
                {
                    MessageBox.Show("Error launching explorer!", "Error!");
                }
            }

        }

        #endregion VDK

        #region Network



        public static ArrayList VLanlist = new ArrayList();


        private void btnNetAdd_Click(object sender, EventArgs e)
        {
            if (rbtnNetUser.Checked == true)
            {
                VUser item = new VUser();
                if (txtNetHost.Text == "")
                    item.hostname = "host";
                else
                    item.hostname = txtNetHost.Text;

                item.vlan = VLanlist.Count;
                listVLANs.Items.Add(item.ToString());
                VLanlist.Add(item);

            }
            if (rbtnNetNic.Checked == true)
            {
                VNic item = new VNic();
                if (txtNicMACaddr.Text == "")
                    item.macAddress = "";//???
                else
                    item.macAddress = txtNicMACaddr.Text;

                item.vlan = VLanlist.Count;
                listVLANs.Items.Add(item.ToString());
                VLanlist.Add(item);
            }

        }

        private void rbtnNetUser_CheckedChanged(object sender, EventArgs e)
        {
            panelUser.Visible = rbtnNetUser.Checked;
        }

        private void rbtnNetNic_CheckedChanged(object sender, EventArgs e)
        {
             panelNic.Visible = rbtnNetNic.Checked;
        }

        private void btnNetRemove_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < listVLANs.SelectedIndices.Count; i++)
            {
                VLanlist.RemoveAt(listVLANs.SelectedIndices[i]);
                listVLANs.Items.RemoveAt(listVLANs.SelectedIndices[i]);
            }
        }

        #endregion

        private void btnCreateImage_Click(object sender, System.EventArgs e)
        {
            saveFileDialog.Title = "Create a virtual Hardisk";
            saveFileDialog.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img|All Files | *.*";
            saveFileDialog.FileName = "image." + cboImageFormat.Text;
            saveFileDialog.DefaultExt = cboImageFormat.Text;
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                SaveSettings();
                runner = new Runner(data);
                if (runner.CreateImage(saveFileDialog.FileName, Convert.ToInt32(txtImageSize.Text), cboImageFormat.Text))
                    MessageBox.Show("Image created");
            }
        }

        private void btnLaunch_Click(object sender, System.EventArgs e)
        {
            SaveSettings();

            bool HasHDisk = false;
            bool HasFDisk = false;

            /* check the QEmu path */
            if (!Directory.Exists(data.Paths.Qemu))
            {
                MessageBox.Show("\"" + data.Paths.Qemu + "\"" + " does not exist", "Error - QEmu path");
                return;
            }

            /* There must be atleast one source of OS */
            for (int i = 0; i < data.Harddisks.HDD.Length; i++)
            {
                if (data.Harddisks.HDD[i].Enabled == true)
                {
                    if (File.Exists(data.Harddisks.HDD[i].Path))
                    {
                        HasHDisk = true;
                    }
                    else
                    {
                        MessageBox.Show("\"" + data.Harddisks.HDD[i].Path + "\"" + " does not exist", "Error - Harddisk file");
                        return;
                    }
                }
            }

            for (int i = 0; i < data.Floppies.FDD.Length; i++)
            {
                if (data.Floppies.FDD[i].Enabled == true)
                {
                    if (File.Exists(data.Floppies.FDD[i].Path))
                    {
                        HasFDisk = true;
                    }
                    else
                    {
                        MessageBox.Show("\"" + data.Floppies.FDD[i].Path + "\"" + " does not exist", "Error - Floppy File");
                        return;
                    }
                }
            }

            /* ensure we have a correct CD image if we need to boot from it */
            if (optCDImage.Enabled && txtCDROM.Text.Length > 0)
            {
                if (!File.Exists(txtCDROM.Text))
                {
                    MessageBox.Show("\"" + txtCDROM.Text + "\"" + " does not exist", "Error - CD-ROM image");
                    return;
                }
            }

            if (HasHDisk == false && data.CDROM.Enabled == false && HasFDisk == false)
                MessageBox.Show("Must enable atleast 1 Hard disk, CD-Rom or Floppy disk!", "Error");//or specify linux kernel image???
            else
            {
                /* we must know where to look for qemu */
                if (data.Paths.Qemu.Length == 0)
                    MessageBox.Show("Please specify Qemu Path!", "Error");
                else
                {
                    runner = new Runner(data);
                    runner.StartQemu((Platforms)cboMachine.SelectedIndex);
                }
            }
        }

        private void loadConfigToolStripMenuItem_Click(object sender, EventArgs e)
        {
            XmlSerializer s;
            TextReader r;
            openFile.Title = "Load settings";
            openFile.InitialDirectory = currentDir;
            openFile.FileName = "config.xml";
            openFile.Filter = "Config (*.xml)|*.xml";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    s = new XmlSerializer(typeof(Data));
                    r = new StreamReader(openFile.FileName);
                    data = (Data)s.Deserialize(r);

                    LoadSettings();
                }
                catch (Exception ex)
                {
                    ErrorForm error = new ErrorForm();
                    error.txtError.Text = "Error Ocourred while loading settings! Exception Information:";
                    error.txtError.Text += ex.Message;
                    error.txtError.Text += Environment.NewLine + "Your settings file may be corrupt.";
                    error.Show();
                }
            }
        }

        private void saveConfigToolStripMenuItem_Click(object sender, EventArgs e)
        {
            XmlSerializer s;
            TextWriter w;
            SaveSettings();
            saveFileDialog.Title = "Save settings";
            saveFileDialog.InitialDirectory = currentDir;
            saveFileDialog.FileName = "config.xml";
            saveFileDialog.Filter = "Config (*.xml)|*.xml";
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    s = new XmlSerializer(typeof(Data));
                    w = new StreamWriter(saveFileDialog.FileName);
                    s.Serialize(w, data);
                }
                catch (Exception ex)
                {
                    ErrorForm error = new ErrorForm();
                    error.txtError.Text = "Error trying to save settings!";
                    error.txtError.Text += "Exception Info:" + Environment.NewLine + ex.Message;
                    error.txtError.Text += Environment.NewLine + ex.StackTrace;
                }
            }
        }

        private void helpContentsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // load up html help file
        }

        private void aboutReactOSQEmuManagerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutForm Dlg = new AboutForm();
            Dlg.Show();
        }

        private void setAsDefaultToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFile.Title = "Default config settings";
            openFile.InitialDirectory = currentDir;
            openFile.FileName = "config.xml";
            openFile.Filter = "Config (*.xml)|*.xml";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                RegistryKey RegKey = Registry.CurrentUser.CreateSubKey(strRegKey);

                if (RegKey != null)
                {
                    RegKey.SetValue(strDefCon, (string)openFile.FileName);
                    RegKey.Close();
                }
            }
        }

        private void LoadConfigFile()
        {
            /* Load the default config file */
            bool bLoaded = false;
            RegistryKey RegKey = Registry.CurrentUser.OpenSubKey(strRegKey);

            if (RegKey != null)
            {
                string DefaultConfig = (string)RegKey.GetValue(strDefCon);

                if (File.Exists(DefaultConfig))
                {
                    XmlSerializer s = new XmlSerializer(typeof(Data));
                    TextReader r = new StreamReader(DefaultConfig);
                    data = (Data)s.Deserialize(r);
                    r.Close();
                    LoadSettings();
                    bLoaded = true;
                }
                else
                {
                    MessageBox.Show("Unable to load the default config file");
                }

                RegKey.Close();
            }

            if (!bLoaded)
            {
                /* if no settings file found, fallback to hardcoded defaults */

                /* Network */
                VUser def = new VUser();
                def.vlan = 0;
                def.hostname = "host";
                listVLANs.Items.Add(def.ToString());
                VLanlist.Add(def);
            }
        }
    }
}
