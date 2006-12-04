using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;
using System.Data;
using System.Globalization;
using System.IO;
using System.Xml.Serialization;


namespace Qemu_GUI
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class frmMain : System.Windows.Forms.Form
    {
        #region declarations
        private GroupBox grpMachine;
        private TabPage tabGen;
        private TabControl Tabs;
        private GroupBox grpFloppy;
        private CheckBox chkFloppyA;
        private CheckBox chkFloppyB;
        private Button btnBrowseFloppyA;
        private Button btnBrowseFloppyB;
        private TabPage tabFloppy;
        private GroupBox grpHarddisk;
        private Button btnBrowseHDB;
        private Button btnBrowseHDA;
        private CheckBox chkUseHDA;
        private Button btnBrowseHDD;
        private Button btnBrowseHDC;
        private CheckBox chkUseHDD;
        private CheckBox chkUseHDC;
        private CheckBox chkUseHDB;
        private GroupBox grpCDROM;
        private Button btnBrowseCDROM;
        private CheckBox chkUseCDROM;
        private RadioButton optHostCDROM;
        private RadioButton optCDImage;
        private TabPage tabHardDisk;
        private TabPage tabCDROM;
        private TabPage tabAudio;
        private GroupBox grpAudio;
        private CheckBox chkPCSpeaker;
        private CheckBox chkSoundBlaster;
        private CheckBox chkES1370;
        private CheckBox chkOPL2;
        private TabPage tabTools;
        private ComboBox cboBootFrom;
        private Label lblBootFrom;
        private Label lblSMP;
        private Label lblMemory;
        private GroupBox groupBox6;
        private Button btnLaunch;
        private GroupBox groupBox9;
        private Button btnCreateImage;
        private Label lblImageSize;
        private TextBox txtImageSize;
        private TabPage tabNetwork;
        private TabPage tabDebug;
        private TabPage tabAbout;
        private GroupBox grpClock;
        private GroupBox grpDisplay;
        private GroupBox grpSerial;
        private CheckBox chkSerialToFile;
        private Button btnBrowseSerial;
        private GroupBox grpParallel;
        private Button btnBrowseParallel;
        private CheckBox chkParallelToFile;
        private GroupBox grpGDB;
        private CheckBox checkBox14;
        private TextBox txtGDBPort;
        private Label lblGDBPort;
        private GroupBox grpVNC;
        private CheckBox chkVNC;
        private Label label14;
        private TextBox txtVNC;
        private GroupBox groupBox16;
        private Button button20;
        private FolderBrowserDialog folderBrowserDialog1;
        private OpenFileDialog openFile;
        private SaveFileDialog saveFileDialog;
        private ComboBox cboImageFormat;
        private Label label11;
        private Label lblMB;
        private CheckBox chkSetClock;
        private NumericUpDown numMemory;
        private NumericUpDown numSMP;
        private Label lblCPUs;
        private CheckBox chkKQEmu;
        private CheckBox chkFullscreen;
        private CheckBox chkVGAoutput;
        private TextBox txtAbout;
        private TextBox txtFloppyA;
        private TextBox txtFloppyB;
        private TextBox txtCDROM;
        private PictureBox pictureBox1;
        private PictureBox pictureBox4;
        private PictureBox pictureBox3;
        private PictureBox pictureBox2;
        private Button btnLoad;
        private Button btnSave;
        private TextBox txtHDD;
        private TextBox txtHDC;
        private TextBox txtHDB;
        private TextBox txtHDA;
        private ComboBox cboCDROM;
        private GroupBox grpVDK;
        private Button btnUnmount;
        private Button btnMount;
        private TabPage tabPaths;
        private GroupBox groupBox2;
        private TextBox txtVDKPath;
        private Button btnVDKBrowse;
        private Label lblVDKPath;
        private Label lblQEmuPath;
        private TextBox txtQEmuPath;
        private Button btnQEmuPath;
        private Label lblDrive;
        private Button btnVDKImage;
        private Label lblImage;
        private TextBox txtVDKImage;
        private ComboBox cboVDKDrive;
        private ComboBox cboMachine;
        private Label lblImageSizeMB;
        private GroupBox grpNetwork;
        private Data data;
        private GroupBox grpACPI;
        private CheckBox chkACPI;
        private string currentDir = Directory.GetCurrentDirectory();
        private CheckBox chckNetEnable;
        private RadioButton rbtnNetUser;
        private Panel panelNic;
        private Label label3;
        private TextBox txtNicMACaddr;
        private Label label2;
        private ComboBox cmbNICmodels;
        private RadioButton rbtnNetNic;
        private Label label1;
        private Button btnNetRemove;
        private Button btnNetAdd;
        private ListView listVLANs;
        private Label label4;
        private Panel panelUser;
        private TextBox txtNetHost;
        private Label label5;
        private Label label6;
        private TabPage tabOther;
        private GroupBox groupBox1;
        private Button button1;
        private TextBox textBox2;
        private CheckBox chkFloopySig;
        private CheckBox chkHardDiskHack;
        private CheckBox chkVBE30;
        private CheckBox checkBox2;
        private CheckBox checkBox3;
        private TextBox txtSerialFile;
        private TextBox txtSerialPipe;
        private CheckBox chkSerialToPipe;
        private TextBox txtParallelFile;
        private TextBox txtParallelPipe;
        private CheckBox checkBox5;
        private Label label7;
        private Runner runner;
        #endregion 

        public frmMain()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
            //if( disposing )
            //{
            //    if (components != null) 
            //    {
            //        components.Dispose();
            //    }
            //}
            base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.grpMachine = new System.Windows.Forms.GroupBox();
            this.cboMachine = new System.Windows.Forms.ComboBox();
            this.Tabs = new System.Windows.Forms.TabControl();
            this.tabGen = new System.Windows.Forms.TabPage();
            this.grpACPI = new System.Windows.Forms.GroupBox();
            this.chkACPI = new System.Windows.Forms.CheckBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.chkKQEmu = new System.Windows.Forms.CheckBox();
            this.lblCPUs = new System.Windows.Forms.Label();
            this.numSMP = new System.Windows.Forms.NumericUpDown();
            this.numMemory = new System.Windows.Forms.NumericUpDown();
            this.lblMB = new System.Windows.Forms.Label();
            this.lblMemory = new System.Windows.Forms.Label();
            this.lblBootFrom = new System.Windows.Forms.Label();
            this.lblSMP = new System.Windows.Forms.Label();
            this.cboBootFrom = new System.Windows.Forms.ComboBox();
            this.grpClock = new System.Windows.Forms.GroupBox();
            this.chkSetClock = new System.Windows.Forms.CheckBox();
            this.grpDisplay = new System.Windows.Forms.GroupBox();
            this.chkVGAoutput = new System.Windows.Forms.CheckBox();
            this.chkFullscreen = new System.Windows.Forms.CheckBox();
            this.tabPaths = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtQEmuPath = new System.Windows.Forms.TextBox();
            this.txtVDKPath = new System.Windows.Forms.TextBox();
            this.btnVDKBrowse = new System.Windows.Forms.Button();
            this.lblVDKPath = new System.Windows.Forms.Label();
            this.btnQEmuPath = new System.Windows.Forms.Button();
            this.lblQEmuPath = new System.Windows.Forms.Label();
            this.tabHardDisk = new System.Windows.Forms.TabPage();
            this.chkHardDiskHack = new System.Windows.Forms.CheckBox();
            this.grpHarddisk = new System.Windows.Forms.GroupBox();
            this.txtHDD = new System.Windows.Forms.TextBox();
            this.txtHDC = new System.Windows.Forms.TextBox();
            this.txtHDB = new System.Windows.Forms.TextBox();
            this.txtHDA = new System.Windows.Forms.TextBox();
            this.btnBrowseHDD = new System.Windows.Forms.Button();
            this.btnBrowseHDC = new System.Windows.Forms.Button();
            this.chkUseHDD = new System.Windows.Forms.CheckBox();
            this.chkUseHDC = new System.Windows.Forms.CheckBox();
            this.btnBrowseHDB = new System.Windows.Forms.Button();
            this.btnBrowseHDA = new System.Windows.Forms.Button();
            this.chkUseHDB = new System.Windows.Forms.CheckBox();
            this.chkUseHDA = new System.Windows.Forms.CheckBox();
            this.tabCDROM = new System.Windows.Forms.TabPage();
            this.grpCDROM = new System.Windows.Forms.GroupBox();
            this.cboCDROM = new System.Windows.Forms.ComboBox();
            this.txtCDROM = new System.Windows.Forms.TextBox();
            this.optHostCDROM = new System.Windows.Forms.RadioButton();
            this.btnBrowseCDROM = new System.Windows.Forms.Button();
            this.chkUseCDROM = new System.Windows.Forms.CheckBox();
            this.optCDImage = new System.Windows.Forms.RadioButton();
            this.tabFloppy = new System.Windows.Forms.TabPage();
            this.chkFloopySig = new System.Windows.Forms.CheckBox();
            this.grpFloppy = new System.Windows.Forms.GroupBox();
            this.txtFloppyB = new System.Windows.Forms.TextBox();
            this.txtFloppyA = new System.Windows.Forms.TextBox();
            this.btnBrowseFloppyB = new System.Windows.Forms.Button();
            this.btnBrowseFloppyA = new System.Windows.Forms.Button();
            this.chkFloppyB = new System.Windows.Forms.CheckBox();
            this.chkFloppyA = new System.Windows.Forms.CheckBox();
            this.tabNetwork = new System.Windows.Forms.TabPage();
            this.grpNetwork = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.panelUser = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.txtNetHost = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.rbtnNetUser = new System.Windows.Forms.RadioButton();
            this.panelNic = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.txtNicMACaddr = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cmbNICmodels = new System.Windows.Forms.ComboBox();
            this.rbtnNetNic = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.btnNetRemove = new System.Windows.Forms.Button();
            this.btnNetAdd = new System.Windows.Forms.Button();
            this.listVLANs = new System.Windows.Forms.ListView();
            this.chckNetEnable = new System.Windows.Forms.CheckBox();
            this.tabAudio = new System.Windows.Forms.TabPage();
            this.grpAudio = new System.Windows.Forms.GroupBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.chkSoundBlaster = new System.Windows.Forms.CheckBox();
            this.chkPCSpeaker = new System.Windows.Forms.CheckBox();
            this.chkES1370 = new System.Windows.Forms.CheckBox();
            this.chkOPL2 = new System.Windows.Forms.CheckBox();
            this.tabDebug = new System.Windows.Forms.TabPage();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.grpVNC = new System.Windows.Forms.GroupBox();
            this.txtVNC = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.chkVNC = new System.Windows.Forms.CheckBox();
            this.grpGDB = new System.Windows.Forms.GroupBox();
            this.lblGDBPort = new System.Windows.Forms.Label();
            this.txtGDBPort = new System.Windows.Forms.TextBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.groupBox16 = new System.Windows.Forms.GroupBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.button20 = new System.Windows.Forms.Button();
            this.grpSerial = new System.Windows.Forms.GroupBox();
            this.txtSerialPipe = new System.Windows.Forms.TextBox();
            this.chkSerialToPipe = new System.Windows.Forms.CheckBox();
            this.txtSerialFile = new System.Windows.Forms.TextBox();
            this.btnBrowseSerial = new System.Windows.Forms.Button();
            this.chkSerialToFile = new System.Windows.Forms.CheckBox();
            this.grpParallel = new System.Windows.Forms.GroupBox();
            this.txtParallelFile = new System.Windows.Forms.TextBox();
            this.txtParallelPipe = new System.Windows.Forms.TextBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.btnBrowseParallel = new System.Windows.Forms.Button();
            this.chkParallelToFile = new System.Windows.Forms.CheckBox();
            this.tabTools = new System.Windows.Forms.TabPage();
            this.grpVDK = new System.Windows.Forms.GroupBox();
            this.txtVDKImage = new System.Windows.Forms.TextBox();
            this.cboVDKDrive = new System.Windows.Forms.ComboBox();
            this.lblDrive = new System.Windows.Forms.Label();
            this.btnVDKImage = new System.Windows.Forms.Button();
            this.btnUnmount = new System.Windows.Forms.Button();
            this.btnMount = new System.Windows.Forms.Button();
            this.lblImage = new System.Windows.Forms.Label();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.lblImageSizeMB = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.txtImageSize = new System.Windows.Forms.TextBox();
            this.lblImageSize = new System.Windows.Forms.Label();
            this.btnCreateImage = new System.Windows.Forms.Button();
            this.cboImageFormat = new System.Windows.Forms.ComboBox();
            this.tabOther = new System.Windows.Forms.TabPage();
            this.chkVBE30 = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.tabAbout = new System.Windows.Forms.TabPage();
            this.txtAbout = new System.Windows.Forms.TextBox();
            this.btnLaunch = new System.Windows.Forms.Button();
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.grpMachine.SuspendLayout();
            this.Tabs.SuspendLayout();
            this.tabGen.SuspendLayout();
            this.grpACPI.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).BeginInit();
            this.grpClock.SuspendLayout();
            this.grpDisplay.SuspendLayout();
            this.tabPaths.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tabHardDisk.SuspendLayout();
            this.grpHarddisk.SuspendLayout();
            this.tabCDROM.SuspendLayout();
            this.grpCDROM.SuspendLayout();
            this.tabFloppy.SuspendLayout();
            this.grpFloppy.SuspendLayout();
            this.tabNetwork.SuspendLayout();
            this.grpNetwork.SuspendLayout();
            this.panelUser.SuspendLayout();
            this.panelNic.SuspendLayout();
            this.tabAudio.SuspendLayout();
            this.grpAudio.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabDebug.SuspendLayout();
            this.grpVNC.SuspendLayout();
            this.grpGDB.SuspendLayout();
            this.groupBox16.SuspendLayout();
            this.grpSerial.SuspendLayout();
            this.grpParallel.SuspendLayout();
            this.tabTools.SuspendLayout();
            this.grpVDK.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.tabOther.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabAbout.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpMachine
            // 
            this.grpMachine.Controls.Add(this.cboMachine);
            this.grpMachine.Location = new System.Drawing.Point(20, 3);
            this.grpMachine.Name = "grpMachine";
            this.grpMachine.Size = new System.Drawing.Size(274, 48);
            this.grpMachine.TabIndex = 2;
            this.grpMachine.TabStop = false;
            this.grpMachine.Text = "Machine";
            // 
            // cboMachine
            // 
            this.cboMachine.DisplayMember = "1";
            this.cboMachine.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboMachine.FormattingEnabled = true;
            this.cboMachine.Items.AddRange(new object[] {
            "Standard PC 32Bit",
            "ISA only PC 32Bit",
            "Standard PC 64Bit",
            "ISA only PC 64Bit",
            "ARM Integrator/CP (ARM926EJ-S) ",
            "ARM Integrator/CP (ARM1026EJ-S)",
            "ARM Versatile/PB  (ARM926EJ-S)",
            "ARM Versatile/AB  (ARM926EJ-S)",
            "PPC Heathrow based PowerMAC",
            "PPC Mac99 based PowerMAC",
            "PPC PowerPC PREP platform",
            "Sparc Sun4m Platform"});
            this.cboMachine.Location = new System.Drawing.Point(19, 19);
            this.cboMachine.Name = "cboMachine";
            this.cboMachine.Size = new System.Drawing.Size(239, 21);
            this.cboMachine.TabIndex = 20;
            // 
            // Tabs
            // 
            this.Tabs.Controls.Add(this.tabGen);
            this.Tabs.Controls.Add(this.tabPaths);
            this.Tabs.Controls.Add(this.tabHardDisk);
            this.Tabs.Controls.Add(this.tabCDROM);
            this.Tabs.Controls.Add(this.tabFloppy);
            this.Tabs.Controls.Add(this.tabNetwork);
            this.Tabs.Controls.Add(this.tabAudio);
            this.Tabs.Controls.Add(this.tabDebug);
            this.Tabs.Controls.Add(this.tabTools);
            this.Tabs.Controls.Add(this.tabOther);
            this.Tabs.Controls.Add(this.tabAbout);
            this.Tabs.Location = new System.Drawing.Point(3, 10);
            this.Tabs.Name = "Tabs";
            this.Tabs.SelectedIndex = 0;
            this.Tabs.Size = new System.Drawing.Size(535, 227);
            this.Tabs.TabIndex = 3;
            // 
            // tabGen
            // 
            this.tabGen.Controls.Add(this.grpACPI);
            this.tabGen.Controls.Add(this.groupBox6);
            this.tabGen.Controls.Add(this.grpMachine);
            this.tabGen.Controls.Add(this.grpClock);
            this.tabGen.Controls.Add(this.grpDisplay);
            this.tabGen.Location = new System.Drawing.Point(4, 22);
            this.tabGen.Name = "tabGen";
            this.tabGen.Size = new System.Drawing.Size(527, 201);
            this.tabGen.TabIndex = 1;
            this.tabGen.Text = "General";
            this.tabGen.UseVisualStyleBackColor = true;
            // 
            // grpACPI
            // 
            this.grpACPI.Controls.Add(this.chkACPI);
            this.grpACPI.Location = new System.Drawing.Point(300, 57);
            this.grpACPI.Name = "grpACPI";
            this.grpACPI.Size = new System.Drawing.Size(174, 51);
            this.grpACPI.TabIndex = 20;
            this.grpACPI.TabStop = false;
            this.grpACPI.Text = "ACPI";
            // 
            // chkACPI
            // 
            this.chkACPI.AutoSize = true;
            this.chkACPI.Checked = true;
            this.chkACPI.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkACPI.Location = new System.Drawing.Point(16, 20);
            this.chkACPI.Name = "chkACPI";
            this.chkACPI.Size = new System.Drawing.Size(86, 17);
            this.chkACPI.TabIndex = 0;
            this.chkACPI.Text = "Enable ACPI";
            this.chkACPI.UseVisualStyleBackColor = true;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.chkKQEmu);
            this.groupBox6.Controls.Add(this.lblCPUs);
            this.groupBox6.Controls.Add(this.numSMP);
            this.groupBox6.Controls.Add(this.numMemory);
            this.groupBox6.Controls.Add(this.lblMB);
            this.groupBox6.Controls.Add(this.lblMemory);
            this.groupBox6.Controls.Add(this.lblBootFrom);
            this.groupBox6.Controls.Add(this.lblSMP);
            this.groupBox6.Controls.Add(this.cboBootFrom);
            this.groupBox6.Location = new System.Drawing.Point(20, 57);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(274, 131);
            this.groupBox6.TabIndex = 4;
            this.groupBox6.TabStop = false;
            // 
            // chkKQEmu
            // 
            this.chkKQEmu.AutoSize = true;
            this.chkKQEmu.Checked = true;
            this.chkKQEmu.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkKQEmu.Location = new System.Drawing.Point(19, 106);
            this.chkKQEmu.Name = "chkKQEmu";
            this.chkKQEmu.Size = new System.Drawing.Size(177, 17);
            this.chkKQEmu.TabIndex = 18;
            this.chkKQEmu.Text = "Use KQEmu accelerator module";
            this.chkKQEmu.UseVisualStyleBackColor = true;
            // 
            // lblCPUs
            // 
            this.lblCPUs.AutoSize = true;
            this.lblCPUs.Location = new System.Drawing.Point(218, 75);
            this.lblCPUs.Name = "lblCPUs";
            this.lblCPUs.Size = new System.Drawing.Size(40, 13);
            this.lblCPUs.TabIndex = 14;
            this.lblCPUs.Text = "CPU(s)";
            // 
            // numSMP
            // 
            this.numSMP.Location = new System.Drawing.Point(166, 73);
            this.numSMP.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.numSMP.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numSMP.Name = "numSMP";
            this.numSMP.Size = new System.Drawing.Size(46, 20);
            this.numSMP.TabIndex = 13;
            this.numSMP.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numMemory
            // 
            this.numMemory.Location = new System.Drawing.Point(166, 47);
            this.numMemory.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numMemory.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numMemory.Name = "numMemory";
            this.numMemory.Size = new System.Drawing.Size(46, 20);
            this.numMemory.TabIndex = 12;
            this.numMemory.Value = new decimal(new int[] {
            64,
            0,
            0,
            0});
            // 
            // lblMB
            // 
            this.lblMB.AutoSize = true;
            this.lblMB.Location = new System.Drawing.Point(218, 49);
            this.lblMB.Name = "lblMB";
            this.lblMB.Size = new System.Drawing.Size(23, 13);
            this.lblMB.TabIndex = 11;
            this.lblMB.Text = "MB";
            // 
            // lblMemory
            // 
            this.lblMemory.Location = new System.Drawing.Point(16, 51);
            this.lblMemory.Name = "lblMemory";
            this.lblMemory.Size = new System.Drawing.Size(60, 23);
            this.lblMemory.TabIndex = 6;
            this.lblMemory.Text = "Memory";
            // 
            // lblBootFrom
            // 
            this.lblBootFrom.Location = new System.Drawing.Point(16, 24);
            this.lblBootFrom.Name = "lblBootFrom";
            this.lblBootFrom.Size = new System.Drawing.Size(60, 23);
            this.lblBootFrom.TabIndex = 2;
            this.lblBootFrom.Text = "Boot from";
            // 
            // lblSMP
            // 
            this.lblSMP.Location = new System.Drawing.Point(16, 76);
            this.lblSMP.Name = "lblSMP";
            this.lblSMP.Size = new System.Drawing.Size(80, 23);
            this.lblSMP.TabIndex = 3;
            this.lblSMP.Text = "SMP (1 to 255)";
            // 
            // cboBootFrom
            // 
            this.cboBootFrom.DisplayMember = "1";
            this.cboBootFrom.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboBootFrom.Items.AddRange(new object[] {
            "Floppy",
            "Harddisk",
            "CD-ROM"});
            this.cboBootFrom.Location = new System.Drawing.Point(135, 21);
            this.cboBootFrom.Name = "cboBootFrom";
            this.cboBootFrom.Size = new System.Drawing.Size(121, 21);
            this.cboBootFrom.TabIndex = 1;
            // 
            // grpClock
            // 
            this.grpClock.Controls.Add(this.chkSetClock);
            this.grpClock.Location = new System.Drawing.Point(300, 3);
            this.grpClock.Name = "grpClock";
            this.grpClock.Size = new System.Drawing.Size(174, 48);
            this.grpClock.TabIndex = 18;
            this.grpClock.TabStop = false;
            this.grpClock.Text = "Clock";
            // 
            // chkSetClock
            // 
            this.chkSetClock.AutoSize = true;
            this.chkSetClock.Checked = true;
            this.chkSetClock.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSetClock.Location = new System.Drawing.Point(16, 21);
            this.chkSetClock.Name = "chkSetClock";
            this.chkSetClock.Size = new System.Drawing.Size(71, 17);
            this.chkSetClock.TabIndex = 12;
            this.chkSetClock.Text = "Set clock";
            this.chkSetClock.UseVisualStyleBackColor = true;
            // 
            // grpDisplay
            // 
            this.grpDisplay.Controls.Add(this.chkVGAoutput);
            this.grpDisplay.Controls.Add(this.chkFullscreen);
            this.grpDisplay.Location = new System.Drawing.Point(300, 114);
            this.grpDisplay.Name = "grpDisplay";
            this.grpDisplay.Size = new System.Drawing.Size(174, 74);
            this.grpDisplay.TabIndex = 19;
            this.grpDisplay.TabStop = false;
            this.grpDisplay.Text = "Display";
            // 
            // chkVGAoutput
            // 
            this.chkVGAoutput.AutoSize = true;
            this.chkVGAoutput.Location = new System.Drawing.Point(16, 51);
            this.chkVGAoutput.Name = "chkVGAoutput";
            this.chkVGAoutput.Size = new System.Drawing.Size(105, 17);
            this.chkVGAoutput.TabIndex = 20;
            this.chkVGAoutput.Text = " VGA Display Off";
            this.chkVGAoutput.UseVisualStyleBackColor = true;
            // 
            // chkFullscreen
            // 
            this.chkFullscreen.AutoSize = true;
            this.chkFullscreen.Location = new System.Drawing.Point(16, 23);
            this.chkFullscreen.Name = "chkFullscreen";
            this.chkFullscreen.Size = new System.Drawing.Size(138, 17);
            this.chkFullscreen.TabIndex = 19;
            this.chkFullscreen.Text = "Start Qemu in fullscreen";
            this.chkFullscreen.UseVisualStyleBackColor = true;
            // 
            // tabPaths
            // 
            this.tabPaths.Controls.Add(this.groupBox2);
            this.tabPaths.Location = new System.Drawing.Point(4, 22);
            this.tabPaths.Name = "tabPaths";
            this.tabPaths.Size = new System.Drawing.Size(527, 201);
            this.tabPaths.TabIndex = 10;
            this.tabPaths.Text = "Paths";
            this.tabPaths.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.txtQEmuPath);
            this.groupBox2.Controls.Add(this.txtVDKPath);
            this.groupBox2.Controls.Add(this.btnVDKBrowse);
            this.groupBox2.Controls.Add(this.lblVDKPath);
            this.groupBox2.Controls.Add(this.btnQEmuPath);
            this.groupBox2.Controls.Add(this.lblQEmuPath);
            this.groupBox2.Location = new System.Drawing.Point(18, 13);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(491, 172);
            this.groupBox2.TabIndex = 21;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Paths";
            // 
            // txtQEmuPath
            // 
            this.txtQEmuPath.Location = new System.Drawing.Point(17, 46);
            this.txtQEmuPath.Name = "txtQEmuPath";
            this.txtQEmuPath.Size = new System.Drawing.Size(398, 20);
            this.txtQEmuPath.TabIndex = 1;
            // 
            // txtVDKPath
            // 
            this.txtVDKPath.Location = new System.Drawing.Point(17, 98);
            this.txtVDKPath.Name = "txtVDKPath";
            this.txtVDKPath.Size = new System.Drawing.Size(398, 20);
            this.txtVDKPath.TabIndex = 5;
            // 
            // btnVDKBrowse
            // 
            this.btnVDKBrowse.Location = new System.Drawing.Point(421, 97);
            this.btnVDKBrowse.Name = "btnVDKBrowse";
            this.btnVDKBrowse.Size = new System.Drawing.Size(27, 22);
            this.btnVDKBrowse.TabIndex = 4;
            this.btnVDKBrowse.Text = "...";
            this.btnVDKBrowse.UseVisualStyleBackColor = true;
            this.btnVDKBrowse.Click += new System.EventHandler(this.btnVDKBrowse_Click);
            // 
            // lblVDKPath
            // 
            this.lblVDKPath.AutoSize = true;
            this.lblVDKPath.Location = new System.Drawing.Point(15, 82);
            this.lblVDKPath.Name = "lblVDKPath";
            this.lblVDKPath.Size = new System.Drawing.Size(29, 13);
            this.lblVDKPath.TabIndex = 3;
            this.lblVDKPath.Text = "VDK";
            // 
            // btnQEmuPath
            // 
            this.btnQEmuPath.Location = new System.Drawing.Point(421, 45);
            this.btnQEmuPath.Name = "btnQEmuPath";
            this.btnQEmuPath.Size = new System.Drawing.Size(27, 22);
            this.btnQEmuPath.TabIndex = 0;
            this.btnQEmuPath.Text = "...";
            this.btnQEmuPath.UseVisualStyleBackColor = true;
            this.btnQEmuPath.Click += new System.EventHandler(this.btnQEmuPath_Click);
            // 
            // lblQEmuPath
            // 
            this.lblQEmuPath.AutoSize = true;
            this.lblQEmuPath.Location = new System.Drawing.Point(14, 30);
            this.lblQEmuPath.Name = "lblQEmuPath";
            this.lblQEmuPath.Size = new System.Drawing.Size(35, 13);
            this.lblQEmuPath.TabIndex = 2;
            this.lblQEmuPath.Text = "Qemu";
            // 
            // tabHardDisk
            // 
            this.tabHardDisk.Controls.Add(this.chkHardDiskHack);
            this.tabHardDisk.Controls.Add(this.grpHarddisk);
            this.tabHardDisk.Location = new System.Drawing.Point(4, 22);
            this.tabHardDisk.Name = "tabHardDisk";
            this.tabHardDisk.Size = new System.Drawing.Size(527, 201);
            this.tabHardDisk.TabIndex = 3;
            this.tabHardDisk.Text = " Harddisk";
            this.tabHardDisk.UseVisualStyleBackColor = true;
            // 
            // chkHardDiskHack
            // 
            this.chkHardDiskHack.AutoSize = true;
            this.chkHardDiskHack.Location = new System.Drawing.Point(19, 175);
            this.chkHardDiskHack.Name = "chkHardDiskHack";
            this.chkHardDiskHack.Size = new System.Drawing.Size(212, 17);
            this.chkHardDiskHack.TabIndex = 6;
            this.chkHardDiskHack.Text = "Win2k Hack (disk full bug during install)";
            this.chkHardDiskHack.UseVisualStyleBackColor = true;
            this.chkHardDiskHack.CheckedChanged += new System.EventHandler(this.chkHardDiskHack_CheckedChanged);
            // 
            // grpHarddisk
            // 
            this.grpHarddisk.Controls.Add(this.txtHDD);
            this.grpHarddisk.Controls.Add(this.txtHDC);
            this.grpHarddisk.Controls.Add(this.txtHDB);
            this.grpHarddisk.Controls.Add(this.txtHDA);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDD);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDC);
            this.grpHarddisk.Controls.Add(this.chkUseHDD);
            this.grpHarddisk.Controls.Add(this.chkUseHDC);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDB);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDA);
            this.grpHarddisk.Controls.Add(this.chkUseHDB);
            this.grpHarddisk.Controls.Add(this.chkUseHDA);
            this.grpHarddisk.Location = new System.Drawing.Point(19, 3);
            this.grpHarddisk.Name = "grpHarddisk";
            this.grpHarddisk.Size = new System.Drawing.Size(473, 165);
            this.grpHarddisk.TabIndex = 5;
            this.grpHarddisk.TabStop = false;
            this.grpHarddisk.Text = " Harddisks";
            // 
            // txtHDD
            // 
            this.txtHDD.Enabled = false;
            this.txtHDD.Location = new System.Drawing.Point(77, 127);
            this.txtHDD.Name = "txtHDD";
            this.txtHDD.Size = new System.Drawing.Size(341, 20);
            this.txtHDD.TabIndex = 15;
            // 
            // txtHDC
            // 
            this.txtHDC.Enabled = false;
            this.txtHDC.Location = new System.Drawing.Point(77, 92);
            this.txtHDC.Name = "txtHDC";
            this.txtHDC.Size = new System.Drawing.Size(341, 20);
            this.txtHDC.TabIndex = 14;
            // 
            // txtHDB
            // 
            this.txtHDB.Enabled = false;
            this.txtHDB.Location = new System.Drawing.Point(77, 53);
            this.txtHDB.Name = "txtHDB";
            this.txtHDB.Size = new System.Drawing.Size(341, 20);
            this.txtHDB.TabIndex = 13;
            // 
            // txtHDA
            // 
            this.txtHDA.Enabled = false;
            this.txtHDA.Location = new System.Drawing.Point(77, 21);
            this.txtHDA.Name = "txtHDA";
            this.txtHDA.Size = new System.Drawing.Size(341, 20);
            this.txtHDA.TabIndex = 12;
            // 
            // btnBrowseHDD
            // 
            this.btnBrowseHDD.Enabled = false;
            this.btnBrowseHDD.Location = new System.Drawing.Point(424, 127);
            this.btnBrowseHDD.Name = "btnBrowseHDD";
            this.btnBrowseHDD.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDD.TabIndex = 11;
            this.btnBrowseHDD.Text = "...";
            this.btnBrowseHDD.Click += new System.EventHandler(this.btnBrowseHDD_Click);
            // 
            // btnBrowseHDC
            // 
            this.btnBrowseHDC.Enabled = false;
            this.btnBrowseHDC.Location = new System.Drawing.Point(424, 89);
            this.btnBrowseHDC.Name = "btnBrowseHDC";
            this.btnBrowseHDC.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDC.TabIndex = 10;
            this.btnBrowseHDC.Text = "...";
            this.btnBrowseHDC.Click += new System.EventHandler(this.btnBrowseHDC_Click);
            // 
            // chkUseHDD
            // 
            this.chkUseHDD.Location = new System.Drawing.Point(18, 127);
            this.chkUseHDD.Name = "chkUseHDD";
            this.chkUseHDD.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDD.TabIndex = 9;
            this.chkUseHDD.Text = "HDD";
            this.chkUseHDD.CheckedChanged += new System.EventHandler(this.chkUseHDD_CheckedChanged);
            // 
            // chkUseHDC
            // 
            this.chkUseHDC.Location = new System.Drawing.Point(18, 88);
            this.chkUseHDC.Name = "chkUseHDC";
            this.chkUseHDC.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDC.TabIndex = 8;
            this.chkUseHDC.Text = "HDC";
            this.chkUseHDC.CheckedChanged += new System.EventHandler(this.chkUseHDC_CheckedChanged);
            // 
            // btnBrowseHDB
            // 
            this.btnBrowseHDB.Enabled = false;
            this.btnBrowseHDB.Location = new System.Drawing.Point(424, 49);
            this.btnBrowseHDB.Name = "btnBrowseHDB";
            this.btnBrowseHDB.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDB.TabIndex = 7;
            this.btnBrowseHDB.Text = "...";
            this.btnBrowseHDB.Click += new System.EventHandler(this.btnBrowseHDB_Click);
            // 
            // btnBrowseHDA
            // 
            this.btnBrowseHDA.Enabled = false;
            this.btnBrowseHDA.Location = new System.Drawing.Point(424, 19);
            this.btnBrowseHDA.Name = "btnBrowseHDA";
            this.btnBrowseHDA.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDA.TabIndex = 6;
            this.btnBrowseHDA.Text = " ...";
            this.btnBrowseHDA.Click += new System.EventHandler(this.btnBrowseHDA_Click);
            // 
            // chkUseHDB
            // 
            this.chkUseHDB.Location = new System.Drawing.Point(18, 49);
            this.chkUseHDB.Name = "chkUseHDB";
            this.chkUseHDB.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDB.TabIndex = 5;
            this.chkUseHDB.Text = "HDB";
            this.chkUseHDB.CheckedChanged += new System.EventHandler(this.chkUseHDB_CheckedChanged);
            // 
            // chkUseHDA
            // 
            this.chkUseHDA.Location = new System.Drawing.Point(18, 19);
            this.chkUseHDA.Name = "chkUseHDA";
            this.chkUseHDA.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDA.TabIndex = 4;
            this.chkUseHDA.Text = "HDA";
            this.chkUseHDA.CheckedChanged += new System.EventHandler(this.chkUseHDA_CheckedChanged);
            // 
            // tabCDROM
            // 
            this.tabCDROM.Controls.Add(this.grpCDROM);
            this.tabCDROM.Location = new System.Drawing.Point(4, 22);
            this.tabCDROM.Name = "tabCDROM";
            this.tabCDROM.Size = new System.Drawing.Size(527, 201);
            this.tabCDROM.TabIndex = 4;
            this.tabCDROM.Text = "CD-ROM";
            this.tabCDROM.UseVisualStyleBackColor = true;
            // 
            // grpCDROM
            // 
            this.grpCDROM.Controls.Add(this.cboCDROM);
            this.grpCDROM.Controls.Add(this.txtCDROM);
            this.grpCDROM.Controls.Add(this.optHostCDROM);
            this.grpCDROM.Controls.Add(this.btnBrowseCDROM);
            this.grpCDROM.Controls.Add(this.chkUseCDROM);
            this.grpCDROM.Controls.Add(this.optCDImage);
            this.grpCDROM.Location = new System.Drawing.Point(3, 3);
            this.grpCDROM.Name = "grpCDROM";
            this.grpCDROM.Size = new System.Drawing.Size(494, 195);
            this.grpCDROM.TabIndex = 5;
            this.grpCDROM.TabStop = false;
            this.grpCDROM.Text = "CD-ROM";
            // 
            // cboCDROM
            // 
            this.cboCDROM.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCDROM.Enabled = false;
            this.cboCDROM.FormattingEnabled = true;
            this.cboCDROM.Location = new System.Drawing.Point(61, 77);
            this.cboCDROM.Name = "cboCDROM";
            this.cboCDROM.Size = new System.Drawing.Size(96, 21);
            this.cboCDROM.TabIndex = 7;
            // 
            // txtCDROM
            // 
            this.txtCDROM.Enabled = false;
            this.txtCDROM.Location = new System.Drawing.Point(61, 134);
            this.txtCDROM.Name = "txtCDROM";
            this.txtCDROM.Size = new System.Drawing.Size(338, 20);
            this.txtCDROM.TabIndex = 6;
            // 
            // optHostCDROM
            // 
            this.optHostCDROM.Checked = true;
            this.optHostCDROM.Enabled = false;
            this.optHostCDROM.Location = new System.Drawing.Point(43, 54);
            this.optHostCDROM.Name = "optHostCDROM";
            this.optHostCDROM.Size = new System.Drawing.Size(104, 24);
            this.optHostCDROM.TabIndex = 3;
            this.optHostCDROM.TabStop = true;
            this.optHostCDROM.Text = "Host CD-ROM";
            this.optHostCDROM.CheckedChanged += new System.EventHandler(this.optHostCDROM_CheckedChanged);
            // 
            // btnBrowseCDROM
            // 
            this.btnBrowseCDROM.Enabled = false;
            this.btnBrowseCDROM.Location = new System.Drawing.Point(405, 134);
            this.btnBrowseCDROM.Name = "btnBrowseCDROM";
            this.btnBrowseCDROM.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseCDROM.TabIndex = 2;
            this.btnBrowseCDROM.Text = "...";
            this.btnBrowseCDROM.Click += new System.EventHandler(this.btnBrowseCDROM_Click);
            // 
            // chkUseCDROM
            // 
            this.chkUseCDROM.Location = new System.Drawing.Point(15, 24);
            this.chkUseCDROM.Name = "chkUseCDROM";
            this.chkUseCDROM.Size = new System.Drawing.Size(72, 24);
            this.chkUseCDROM.TabIndex = 0;
            this.chkUseCDROM.Text = "CD-ROM";
            this.chkUseCDROM.CheckedChanged += new System.EventHandler(this.chkUseCDROM_CheckedChanged);
            // 
            // optCDImage
            // 
            this.optCDImage.Enabled = false;
            this.optCDImage.Location = new System.Drawing.Point(43, 104);
            this.optCDImage.Name = "optCDImage";
            this.optCDImage.Size = new System.Drawing.Size(144, 24);
            this.optCDImage.TabIndex = 4;
            this.optCDImage.Text = "Image File";
            this.optCDImage.CheckedChanged += new System.EventHandler(this.optCDImage_CheckedChanged);
            // 
            // tabFloppy
            // 
            this.tabFloppy.Controls.Add(this.chkFloopySig);
            this.tabFloppy.Controls.Add(this.grpFloppy);
            this.tabFloppy.Location = new System.Drawing.Point(4, 22);
            this.tabFloppy.Name = "tabFloppy";
            this.tabFloppy.Size = new System.Drawing.Size(527, 201);
            this.tabFloppy.TabIndex = 2;
            this.tabFloppy.Text = "Floppy";
            this.tabFloppy.UseVisualStyleBackColor = true;
            // 
            // chkFloopySig
            // 
            this.chkFloopySig.AutoSize = true;
            this.chkFloopySig.Location = new System.Drawing.Point(17, 173);
            this.chkFloopySig.Name = "chkFloopySig";
            this.chkFloopySig.Size = new System.Drawing.Size(251, 17);
            this.chkFloopySig.TabIndex = 6;
            this.chkFloopySig.Text = "Disable boot signature checking for floppy disks";
            this.chkFloopySig.UseVisualStyleBackColor = true;
            this.chkFloopySig.CheckedChanged += new System.EventHandler(this.chkFloopySig_CheckedChanged);
            // 
            // grpFloppy
            // 
            this.grpFloppy.Controls.Add(this.txtFloppyB);
            this.grpFloppy.Controls.Add(this.txtFloppyA);
            this.grpFloppy.Controls.Add(this.btnBrowseFloppyB);
            this.grpFloppy.Controls.Add(this.btnBrowseFloppyA);
            this.grpFloppy.Controls.Add(this.chkFloppyB);
            this.grpFloppy.Controls.Add(this.chkFloppyA);
            this.grpFloppy.Location = new System.Drawing.Point(17, 15);
            this.grpFloppy.Name = "grpFloppy";
            this.grpFloppy.Size = new System.Drawing.Size(465, 152);
            this.grpFloppy.TabIndex = 4;
            this.grpFloppy.TabStop = false;
            this.grpFloppy.Text = "Floppy";
            // 
            // txtFloppyB
            // 
            this.txtFloppyB.Enabled = false;
            this.txtFloppyB.Location = new System.Drawing.Point(32, 113);
            this.txtFloppyB.Name = "txtFloppyB";
            this.txtFloppyB.Size = new System.Drawing.Size(369, 20);
            this.txtFloppyB.TabIndex = 5;
            // 
            // txtFloppyA
            // 
            this.txtFloppyA.Enabled = false;
            this.txtFloppyA.Location = new System.Drawing.Point(32, 49);
            this.txtFloppyA.Name = "txtFloppyA";
            this.txtFloppyA.Size = new System.Drawing.Size(369, 20);
            this.txtFloppyA.TabIndex = 4;
            // 
            // btnBrowseFloppyB
            // 
            this.btnBrowseFloppyB.Enabled = false;
            this.btnBrowseFloppyB.Location = new System.Drawing.Point(407, 113);
            this.btnBrowseFloppyB.Name = "btnBrowseFloppyB";
            this.btnBrowseFloppyB.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseFloppyB.TabIndex = 3;
            this.btnBrowseFloppyB.Text = "...";
            this.btnBrowseFloppyB.Click += new System.EventHandler(this.btnBrowseFloppyB_Click);
            // 
            // btnBrowseFloppyA
            // 
            this.btnBrowseFloppyA.Enabled = false;
            this.btnBrowseFloppyA.Location = new System.Drawing.Point(407, 49);
            this.btnBrowseFloppyA.Name = "btnBrowseFloppyA";
            this.btnBrowseFloppyA.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseFloppyA.TabIndex = 2;
            this.btnBrowseFloppyA.Text = "...";
            this.btnBrowseFloppyA.Click += new System.EventHandler(this.btnBrowseFloppyA_Click);
            // 
            // chkFloppyB
            // 
            this.chkFloppyB.Location = new System.Drawing.Point(16, 88);
            this.chkFloppyB.Name = "chkFloppyB";
            this.chkFloppyB.Size = new System.Drawing.Size(72, 24);
            this.chkFloppyB.TabIndex = 1;
            this.chkFloppyB.Text = "Floppy B";
            this.chkFloppyB.CheckedChanged += new System.EventHandler(this.chkFloppyB_CheckedChanged);
            // 
            // chkFloppyA
            // 
            this.chkFloppyA.Location = new System.Drawing.Point(16, 24);
            this.chkFloppyA.Name = "chkFloppyA";
            this.chkFloppyA.Size = new System.Drawing.Size(72, 24);
            this.chkFloppyA.TabIndex = 0;
            this.chkFloppyA.Text = "Floppy A";
            this.chkFloppyA.CheckedChanged += new System.EventHandler(this.chkFloppyA_CheckedChanged);
            // 
            // tabNetwork
            // 
            this.tabNetwork.Controls.Add(this.grpNetwork);
            this.tabNetwork.Location = new System.Drawing.Point(4, 22);
            this.tabNetwork.Name = "tabNetwork";
            this.tabNetwork.Size = new System.Drawing.Size(527, 201);
            this.tabNetwork.TabIndex = 7;
            this.tabNetwork.Text = "Network";
            this.tabNetwork.UseVisualStyleBackColor = true;
            // 
            // grpNetwork
            // 
            this.grpNetwork.Controls.Add(this.label6);
            this.grpNetwork.Controls.Add(this.panelUser);
            this.grpNetwork.Controls.Add(this.label4);
            this.grpNetwork.Controls.Add(this.rbtnNetUser);
            this.grpNetwork.Controls.Add(this.panelNic);
            this.grpNetwork.Controls.Add(this.rbtnNetNic);
            this.grpNetwork.Controls.Add(this.label1);
            this.grpNetwork.Controls.Add(this.btnNetRemove);
            this.grpNetwork.Controls.Add(this.btnNetAdd);
            this.grpNetwork.Controls.Add(this.listVLANs);
            this.grpNetwork.Controls.Add(this.chckNetEnable);
            this.grpNetwork.Location = new System.Drawing.Point(3, 3);
            this.grpNetwork.Name = "grpNetwork";
            this.grpNetwork.Size = new System.Drawing.Size(494, 195);
            this.grpNetwork.TabIndex = 1;
            this.grpNetwork.TabStop = false;
            this.grpNetwork.Text = "Network";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(320, 165);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(120, 13);
            this.label6.TabIndex = 55;
            this.label6.Text = "Leave Blank for default.";
            this.label6.Visible = false;
            // 
            // panelUser
            // 
            this.panelUser.Controls.Add(this.label5);
            this.panelUser.Controls.Add(this.txtNetHost);
            this.panelUser.Location = new System.Drawing.Point(296, 57);
            this.panelUser.Name = "panelUser";
            this.panelUser.Size = new System.Drawing.Size(192, 100);
            this.panelUser.TabIndex = 38;
            this.panelUser.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(20, 13);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 1;
            this.label5.Text = "Hostname:";
            // 
            // txtNetHost
            // 
            this.txtNetHost.Location = new System.Drawing.Point(23, 35);
            this.txtNetHost.Name = "txtNetHost";
            this.txtNetHost.Size = new System.Drawing.Size(121, 20);
            this.txtNetHost.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(200, 59);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 54;
            this.label4.Text = "Mode:";
            // 
            // rbtnNetUser
            // 
            this.rbtnNetUser.AutoSize = true;
            this.rbtnNetUser.Checked = true;
            this.rbtnNetUser.Location = new System.Drawing.Point(203, 78);
            this.rbtnNetUser.Name = "rbtnNetUser";
            this.rbtnNetUser.Size = new System.Drawing.Size(85, 17);
            this.rbtnNetUser.TabIndex = 53;
            this.rbtnNetUser.TabStop = true;
            this.rbtnNetUser.Text = "User(default)";
            this.rbtnNetUser.UseVisualStyleBackColor = true;
            this.rbtnNetUser.CheckedChanged += new System.EventHandler(this.rbtnNetUser_CheckedChanged);
            // 
            // panelNic
            // 
            this.panelNic.Controls.Add(this.label3);
            this.panelNic.Controls.Add(this.txtNicMACaddr);
            this.panelNic.Controls.Add(this.label2);
            this.panelNic.Controls.Add(this.cmbNICmodels);
            this.panelNic.Location = new System.Drawing.Point(296, 38);
            this.panelNic.Name = "panelNic";
            this.panelNic.Size = new System.Drawing.Size(192, 121);
            this.panelNic.TabIndex = 52;
            this.panelNic.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 13);
            this.label3.TabIndex = 37;
            this.label3.Text = "MAC Address:";
            // 
            // txtNicMACaddr
            // 
            this.txtNicMACaddr.Location = new System.Drawing.Point(23, 88);
            this.txtNicMACaddr.Name = "txtNicMACaddr";
            this.txtNicMACaddr.Size = new System.Drawing.Size(121, 20);
            this.txtNicMACaddr.TabIndex = 36;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 35;
            this.label2.Text = "Card to Emulate:";
            // 
            // cmbNICmodels
            // 
            this.cmbNICmodels.FormattingEnabled = true;
            this.cmbNICmodels.Items.AddRange(new object[] {
            "ne2k_pci",
            "ne2k_isa",
            "rtl8139",
            "smc91c11",
            "lance"});
            this.cmbNICmodels.Location = new System.Drawing.Point(23, 40);
            this.cmbNICmodels.Name = "cmbNICmodels";
            this.cmbNICmodels.Size = new System.Drawing.Size(121, 21);
            this.cmbNICmodels.TabIndex = 34;
            // 
            // rbtnNetNic
            // 
            this.rbtnNetNic.AutoSize = true;
            this.rbtnNetNic.Location = new System.Drawing.Point(203, 101);
            this.rbtnNetNic.Name = "rbtnNetNic";
            this.rbtnNetNic.Size = new System.Drawing.Size(43, 17);
            this.rbtnNetNic.TabIndex = 51;
            this.rbtnNetNic.Text = "NIC";
            this.rbtnNetNic.UseVisualStyleBackColor = true;
            this.rbtnNetNic.CheckedChanged += new System.EventHandler(this.rbtnNetNic_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 13);
            this.label1.TabIndex = 50;
            this.label1.Text = "VLANs";
            // 
            // btnNetRemove
            // 
            this.btnNetRemove.Location = new System.Drawing.Point(134, 153);
            this.btnNetRemove.Name = "btnNetRemove";
            this.btnNetRemove.Size = new System.Drawing.Size(60, 25);
            this.btnNetRemove.TabIndex = 49;
            this.btnNetRemove.Text = "Remove";
            this.btnNetRemove.UseVisualStyleBackColor = true;
            this.btnNetRemove.Click += new System.EventHandler(this.btnNetRemove_Click);
            // 
            // btnNetAdd
            // 
            this.btnNetAdd.Location = new System.Drawing.Point(16, 153);
            this.btnNetAdd.Name = "btnNetAdd";
            this.btnNetAdd.Size = new System.Drawing.Size(60, 25);
            this.btnNetAdd.TabIndex = 48;
            this.btnNetAdd.Text = "Add";
            this.btnNetAdd.UseVisualStyleBackColor = true;
            this.btnNetAdd.Click += new System.EventHandler(this.btnNetAdd_Click);
            // 
            // listVLANs
            // 
            this.listVLANs.LabelWrap = false;
            this.listVLANs.Location = new System.Drawing.Point(16, 59);
            this.listVLANs.Name = "listVLANs";
            this.listVLANs.ShowGroups = false;
            this.listVLANs.Size = new System.Drawing.Size(178, 88);
            this.listVLANs.TabIndex = 47;
            this.listVLANs.UseCompatibleStateImageBehavior = false;
            this.listVLANs.View = System.Windows.Forms.View.List;
            // 
            // chckNetEnable
            // 
            this.chckNetEnable.AutoSize = true;
            this.chckNetEnable.Location = new System.Drawing.Point(7, 20);
            this.chckNetEnable.Name = "chckNetEnable";
            this.chckNetEnable.Size = new System.Drawing.Size(59, 17);
            this.chckNetEnable.TabIndex = 28;
            this.chckNetEnable.Text = "Enable";
            this.chckNetEnable.UseVisualStyleBackColor = true;
            this.chckNetEnable.CheckedChanged += new System.EventHandler(this.chckNetEnable_CheckedChanged);
            // 
            // tabAudio
            // 
            this.tabAudio.Controls.Add(this.grpAudio);
            this.tabAudio.Location = new System.Drawing.Point(4, 22);
            this.tabAudio.Name = "tabAudio";
            this.tabAudio.Size = new System.Drawing.Size(527, 201);
            this.tabAudio.TabIndex = 5;
            this.tabAudio.Text = "Audio";
            this.tabAudio.UseVisualStyleBackColor = true;
            // 
            // grpAudio
            // 
            this.grpAudio.Controls.Add(this.pictureBox4);
            this.grpAudio.Controls.Add(this.pictureBox3);
            this.grpAudio.Controls.Add(this.pictureBox2);
            this.grpAudio.Controls.Add(this.pictureBox1);
            this.grpAudio.Controls.Add(this.chkSoundBlaster);
            this.grpAudio.Controls.Add(this.chkPCSpeaker);
            this.grpAudio.Controls.Add(this.chkES1370);
            this.grpAudio.Controls.Add(this.chkOPL2);
            this.grpAudio.Location = new System.Drawing.Point(3, 3);
            this.grpAudio.Name = "grpAudio";
            this.grpAudio.Size = new System.Drawing.Size(494, 195);
            this.grpAudio.TabIndex = 4;
            this.grpAudio.TabStop = false;
            this.grpAudio.Text = "Emulate Audio Cards";
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = global::Qemu_GUI.Properties.Resources.audio_volume_high;
            this.pictureBox4.Location = new System.Drawing.Point(116, 143);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(25, 24);
            this.pictureBox4.TabIndex = 6;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::Qemu_GUI.Properties.Resources.audio_volume_high;
            this.pictureBox3.Location = new System.Drawing.Point(116, 108);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(25, 24);
            this.pictureBox3.TabIndex = 5;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::Qemu_GUI.Properties.Resources.audio_volume_high;
            this.pictureBox2.Location = new System.Drawing.Point(116, 74);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(25, 24);
            this.pictureBox2.TabIndex = 4;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::Qemu_GUI.Properties.Resources.audio_volume_high;
            this.pictureBox1.Location = new System.Drawing.Point(116, 40);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(25, 24);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // chkSoundBlaster
            // 
            this.chkSoundBlaster.Checked = true;
            this.chkSoundBlaster.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSoundBlaster.Location = new System.Drawing.Point(159, 70);
            this.chkSoundBlaster.Name = "chkSoundBlaster";
            this.chkSoundBlaster.Size = new System.Drawing.Size(176, 32);
            this.chkSoundBlaster.TabIndex = 1;
            this.chkSoundBlaster.Text = "Creative Sound Blaster 16";
            // 
            // chkPCSpeaker
            // 
            this.chkPCSpeaker.Checked = true;
            this.chkPCSpeaker.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPCSpeaker.Location = new System.Drawing.Point(159, 40);
            this.chkPCSpeaker.Name = "chkPCSpeaker";
            this.chkPCSpeaker.Size = new System.Drawing.Size(168, 24);
            this.chkPCSpeaker.TabIndex = 0;
            this.chkPCSpeaker.Text = "PC Speaker";
            // 
            // chkES1370
            // 
            this.chkES1370.Location = new System.Drawing.Point(159, 144);
            this.chkES1370.Name = "chkES1370";
            this.chkES1370.Size = new System.Drawing.Size(176, 24);
            this.chkES1370.TabIndex = 2;
            this.chkES1370.Text = "ENSONIQ AudioPCI ES1370";
            // 
            // chkOPL2
            // 
            this.chkOPL2.Location = new System.Drawing.Point(159, 108);
            this.chkOPL2.Name = "chkOPL2";
            this.chkOPL2.Size = new System.Drawing.Size(176, 24);
            this.chkOPL2.TabIndex = 2;
            this.chkOPL2.Text = " Yamaha YM3812 (OPL2)";
            // 
            // tabDebug
            // 
            this.tabDebug.Controls.Add(this.checkBox2);
            this.tabDebug.Controls.Add(this.grpVNC);
            this.tabDebug.Controls.Add(this.grpGDB);
            this.tabDebug.Controls.Add(this.groupBox16);
            this.tabDebug.Controls.Add(this.grpSerial);
            this.tabDebug.Controls.Add(this.grpParallel);
            this.tabDebug.Location = new System.Drawing.Point(4, 22);
            this.tabDebug.Name = "tabDebug";
            this.tabDebug.Size = new System.Drawing.Size(527, 201);
            this.tabDebug.TabIndex = 8;
            this.tabDebug.Text = "Debug";
            this.tabDebug.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(5, 175);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(253, 17);
            this.checkBox2.TabIndex = 4;
            this.checkBox2.Text = "Freeze CPU at startup (use \'c\' to start execution)";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // grpVNC
            // 
            this.grpVNC.Controls.Add(this.txtVNC);
            this.grpVNC.Controls.Add(this.label14);
            this.grpVNC.Controls.Add(this.chkVNC);
            this.grpVNC.Location = new System.Drawing.Point(278, 3);
            this.grpVNC.Name = "grpVNC";
            this.grpVNC.Size = new System.Drawing.Size(119, 96);
            this.grpVNC.TabIndex = 3;
            this.grpVNC.TabStop = false;
            this.grpVNC.Text = "VNC Server";
            // 
            // txtVNC
            // 
            this.txtVNC.Enabled = false;
            this.txtVNC.Location = new System.Drawing.Point(8, 64);
            this.txtVNC.Name = "txtVNC";
            this.txtVNC.Size = new System.Drawing.Size(96, 20);
            this.txtVNC.TabIndex = 2;
            this.txtVNC.Text = "0";
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(8, 40);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(88, 21);
            this.label14.TabIndex = 1;
            this.label14.Text = "Display Number";
            // 
            // chkVNC
            // 
            this.chkVNC.Location = new System.Drawing.Point(8, 16);
            this.chkVNC.Name = "chkVNC";
            this.chkVNC.Size = new System.Drawing.Size(96, 24);
            this.chkVNC.TabIndex = 0;
            this.chkVNC.Text = "Activate VNC";
            this.chkVNC.CheckedChanged += new System.EventHandler(this.chkVNC_CheckedChanged);
            // 
            // grpGDB
            // 
            this.grpGDB.Controls.Add(this.lblGDBPort);
            this.grpGDB.Controls.Add(this.txtGDBPort);
            this.grpGDB.Controls.Add(this.checkBox14);
            this.grpGDB.Location = new System.Drawing.Point(278, 105);
            this.grpGDB.Name = "grpGDB";
            this.grpGDB.Size = new System.Drawing.Size(152, 80);
            this.grpGDB.TabIndex = 2;
            this.grpGDB.TabStop = false;
            this.grpGDB.Text = "GDB";
            // 
            // lblGDBPort
            // 
            this.lblGDBPort.Location = new System.Drawing.Point(3, 46);
            this.lblGDBPort.Name = "lblGDBPort";
            this.lblGDBPort.Size = new System.Drawing.Size(55, 23);
            this.lblGDBPort.TabIndex = 2;
            this.lblGDBPort.Text = "Port:";
            // 
            // txtGDBPort
            // 
            this.txtGDBPort.Location = new System.Drawing.Point(64, 46);
            this.txtGDBPort.Name = "txtGDBPort";
            this.txtGDBPort.Size = new System.Drawing.Size(60, 20);
            this.txtGDBPort.TabIndex = 1;
            this.txtGDBPort.Text = "1234";
            this.txtGDBPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // checkBox14
            // 
            this.checkBox14.Location = new System.Drawing.Point(6, 19);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(144, 24);
            this.checkBox14.TabIndex = 0;
            this.checkBox14.Text = "Wait connection to port";
            // 
            // groupBox16
            // 
            this.groupBox16.Controls.Add(this.checkBox3);
            this.groupBox16.Controls.Add(this.button20);
            this.groupBox16.Location = new System.Drawing.Point(403, 3);
            this.groupBox16.Name = "groupBox16";
            this.groupBox16.Size = new System.Drawing.Size(118, 96);
            this.groupBox16.TabIndex = 1;
            this.groupBox16.TabStop = false;
            this.groupBox16.Text = "LoadVM state";
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(6, 18);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(65, 17);
            this.checkBox3.TabIndex = 4;
            this.checkBox3.Text = "Enabled";
            this.checkBox3.UseVisualStyleBackColor = true;
            // 
            // button20
            // 
            this.button20.Location = new System.Drawing.Point(10, 47);
            this.button20.Name = "button20";
            this.button20.Size = new System.Drawing.Size(96, 23);
            this.button20.TabIndex = 3;
            this.button20.Text = "Browse";
            this.button20.Click += new System.EventHandler(this.btnLoadVM_Click);
            // 
            // grpSerial
            // 
            this.grpSerial.Controls.Add(this.txtSerialPipe);
            this.grpSerial.Controls.Add(this.chkSerialToPipe);
            this.grpSerial.Controls.Add(this.txtSerialFile);
            this.grpSerial.Controls.Add(this.btnBrowseSerial);
            this.grpSerial.Controls.Add(this.chkSerialToFile);
            this.grpSerial.Location = new System.Drawing.Point(5, 3);
            this.grpSerial.Name = "grpSerial";
            this.grpSerial.Size = new System.Drawing.Size(267, 80);
            this.grpSerial.TabIndex = 1;
            this.grpSerial.TabStop = false;
            this.grpSerial.Text = "Serial Port";
            // 
            // txtSerialPipe
            // 
            this.txtSerialPipe.Location = new System.Drawing.Point(107, 47);
            this.txtSerialPipe.Name = "txtSerialPipe";
            this.txtSerialPipe.Size = new System.Drawing.Size(146, 20);
            this.txtSerialPipe.TabIndex = 6;
            // 
            // chkSerialToPipe
            // 
            this.chkSerialToPipe.AutoSize = true;
            this.chkSerialToPipe.Location = new System.Drawing.Point(8, 47);
            this.chkSerialToPipe.Name = "chkSerialToPipe";
            this.chkSerialToPipe.Size = new System.Drawing.Size(101, 17);
            this.chkSerialToPipe.TabIndex = 5;
            this.chkSerialToPipe.Text = "Redirect to pipe";
            this.chkSerialToPipe.UseVisualStyleBackColor = true;
            this.chkSerialToPipe.CheckedChanged += new System.EventHandler(this.chkSerialToPipe_CheckedChanged);
            // 
            // txtSerialFile
            // 
            this.txtSerialFile.Location = new System.Drawing.Point(99, 19);
            this.txtSerialFile.Name = "txtSerialFile";
            this.txtSerialFile.Size = new System.Drawing.Size(129, 20);
            this.txtSerialFile.TabIndex = 4;
            // 
            // btnBrowseSerial
            // 
            this.btnBrowseSerial.Enabled = false;
            this.btnBrowseSerial.Location = new System.Drawing.Point(234, 19);
            this.btnBrowseSerial.Name = "btnBrowseSerial";
            this.btnBrowseSerial.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseSerial.TabIndex = 3;
            this.btnBrowseSerial.Text = "...";
            this.btnBrowseSerial.Click += new System.EventHandler(this.btnBrowseSerial_Click);
            // 
            // chkSerialToFile
            // 
            this.chkSerialToFile.Location = new System.Drawing.Point(8, 16);
            this.chkSerialToFile.Name = "chkSerialToFile";
            this.chkSerialToFile.Size = new System.Drawing.Size(104, 24);
            this.chkSerialToFile.TabIndex = 2;
            this.chkSerialToFile.Text = "Redirect to file";
            this.chkSerialToFile.CheckedChanged += new System.EventHandler(this.chkSerialToFile_CheckedChanged);
            // 
            // grpParallel
            // 
            this.grpParallel.Controls.Add(this.txtParallelFile);
            this.grpParallel.Controls.Add(this.txtParallelPipe);
            this.grpParallel.Controls.Add(this.checkBox5);
            this.grpParallel.Controls.Add(this.btnBrowseParallel);
            this.grpParallel.Controls.Add(this.chkParallelToFile);
            this.grpParallel.Location = new System.Drawing.Point(5, 89);
            this.grpParallel.Name = "grpParallel";
            this.grpParallel.Size = new System.Drawing.Size(267, 80);
            this.grpParallel.TabIndex = 1;
            this.grpParallel.TabStop = false;
            this.grpParallel.Text = "Parallel port";
            // 
            // txtParallelFile
            // 
            this.txtParallelFile.Location = new System.Drawing.Point(99, 16);
            this.txtParallelFile.Name = "txtParallelFile";
            this.txtParallelFile.Size = new System.Drawing.Size(129, 20);
            this.txtParallelFile.TabIndex = 8;
            // 
            // txtParallelPipe
            // 
            this.txtParallelPipe.Location = new System.Drawing.Point(107, 46);
            this.txtParallelPipe.Name = "txtParallelPipe";
            this.txtParallelPipe.Size = new System.Drawing.Size(146, 20);
            this.txtParallelPipe.TabIndex = 7;
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Location = new System.Drawing.Point(8, 44);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(101, 17);
            this.checkBox5.TabIndex = 6;
            this.checkBox5.Text = "Redirect to pipe";
            this.checkBox5.UseVisualStyleBackColor = true;
            // 
            // btnBrowseParallel
            // 
            this.btnBrowseParallel.Enabled = false;
            this.btnBrowseParallel.Location = new System.Drawing.Point(234, 16);
            this.btnBrowseParallel.Name = "btnBrowseParallel";
            this.btnBrowseParallel.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseParallel.TabIndex = 3;
            this.btnBrowseParallel.Text = "...";
            this.btnBrowseParallel.Click += new System.EventHandler(this.btnBrowseParallel_Click);
            // 
            // chkParallelToFile
            // 
            this.chkParallelToFile.Location = new System.Drawing.Point(8, 16);
            this.chkParallelToFile.Name = "chkParallelToFile";
            this.chkParallelToFile.Size = new System.Drawing.Size(104, 24);
            this.chkParallelToFile.TabIndex = 2;
            this.chkParallelToFile.Text = "Redirect to file";
            this.chkParallelToFile.CheckedChanged += new System.EventHandler(this.chkParallelToFile_CheckedChanged);
            // 
            // tabTools
            // 
            this.tabTools.Controls.Add(this.grpVDK);
            this.tabTools.Controls.Add(this.groupBox9);
            this.tabTools.Location = new System.Drawing.Point(4, 22);
            this.tabTools.Name = "tabTools";
            this.tabTools.Size = new System.Drawing.Size(527, 201);
            this.tabTools.TabIndex = 6;
            this.tabTools.Text = "Tools";
            this.tabTools.UseVisualStyleBackColor = true;
            // 
            // grpVDK
            // 
            this.grpVDK.Controls.Add(this.txtVDKImage);
            this.grpVDK.Controls.Add(this.cboVDKDrive);
            this.grpVDK.Controls.Add(this.lblDrive);
            this.grpVDK.Controls.Add(this.btnVDKImage);
            this.grpVDK.Controls.Add(this.btnUnmount);
            this.grpVDK.Controls.Add(this.btnMount);
            this.grpVDK.Controls.Add(this.lblImage);
            this.grpVDK.Enabled = false;
            this.grpVDK.Location = new System.Drawing.Point(4, 83);
            this.grpVDK.Name = "grpVDK";
            this.grpVDK.Size = new System.Drawing.Size(493, 115);
            this.grpVDK.TabIndex = 7;
            this.grpVDK.TabStop = false;
            this.grpVDK.Text = "VDK";
            // 
            // txtVDKImage
            // 
            this.txtVDKImage.Location = new System.Drawing.Point(16, 41);
            this.txtVDKImage.Name = "txtVDKImage";
            this.txtVDKImage.Size = new System.Drawing.Size(398, 20);
            this.txtVDKImage.TabIndex = 6;
            // 
            // cboVDKDrive
            // 
            this.cboVDKDrive.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboVDKDrive.FormattingEnabled = true;
            this.cboVDKDrive.Location = new System.Drawing.Point(64, 74);
            this.cboVDKDrive.Name = "cboVDKDrive";
            this.cboVDKDrive.Size = new System.Drawing.Size(69, 21);
            this.cboVDKDrive.TabIndex = 10;
            this.cboVDKDrive.SelectedIndexChanged += new System.EventHandler(this.cboVDKDrive_SelectedIndexChanged);
            // 
            // lblDrive
            // 
            this.lblDrive.AutoSize = true;
            this.lblDrive.Location = new System.Drawing.Point(14, 77);
            this.lblDrive.Name = "lblDrive";
            this.lblDrive.Size = new System.Drawing.Size(35, 13);
            this.lblDrive.TabIndex = 9;
            this.lblDrive.Text = "Drive:";
            // 
            // btnVDKImage
            // 
            this.btnVDKImage.Location = new System.Drawing.Point(419, 39);
            this.btnVDKImage.Name = "btnVDKImage";
            this.btnVDKImage.Size = new System.Drawing.Size(27, 22);
            this.btnVDKImage.TabIndex = 8;
            this.btnVDKImage.Text = "...";
            this.btnVDKImage.UseVisualStyleBackColor = true;
            this.btnVDKImage.Click += new System.EventHandler(this.btnVDKImage_Click);
            // 
            // btnUnmount
            // 
            this.btnUnmount.Location = new System.Drawing.Point(353, 77);
            this.btnUnmount.Name = "btnUnmount";
            this.btnUnmount.Size = new System.Drawing.Size(93, 23);
            this.btnUnmount.TabIndex = 5;
            this.btnUnmount.Text = "Unmount";
            this.btnUnmount.Click += new System.EventHandler(this.btnUnmount_Click);
            // 
            // btnMount
            // 
            this.btnMount.Location = new System.Drawing.Point(255, 77);
            this.btnMount.Name = "btnMount";
            this.btnMount.Size = new System.Drawing.Size(93, 23);
            this.btnMount.TabIndex = 3;
            this.btnMount.Text = "Mount";
            this.btnMount.Click += new System.EventHandler(this.btnMount_Click);
            // 
            // lblImage
            // 
            this.lblImage.AutoSize = true;
            this.lblImage.Location = new System.Drawing.Point(13, 25);
            this.lblImage.Name = "lblImage";
            this.lblImage.Size = new System.Drawing.Size(39, 13);
            this.lblImage.TabIndex = 7;
            this.lblImage.Text = "Image:";
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.lblImageSizeMB);
            this.groupBox9.Controls.Add(this.label11);
            this.groupBox9.Controls.Add(this.txtImageSize);
            this.groupBox9.Controls.Add(this.lblImageSize);
            this.groupBox9.Controls.Add(this.btnCreateImage);
            this.groupBox9.Controls.Add(this.cboImageFormat);
            this.groupBox9.Location = new System.Drawing.Point(4, 3);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(493, 74);
            this.groupBox9.TabIndex = 6;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Harddisk Tools";
            // 
            // lblImageSizeMB
            // 
            this.lblImageSizeMB.AutoSize = true;
            this.lblImageSizeMB.Location = new System.Drawing.Point(83, 42);
            this.lblImageSizeMB.Name = "lblImageSizeMB";
            this.lblImageSizeMB.Size = new System.Drawing.Size(23, 13);
            this.lblImageSizeMB.TabIndex = 8;
            this.lblImageSizeMB.Text = "MB";
            // 
            // label11
            // 
            this.label11.Location = new System.Drawing.Point(125, 24);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(48, 12);
            this.label11.TabIndex = 5;
            this.label11.Text = "Format";
            // 
            // txtImageSize
            // 
            this.txtImageSize.Location = new System.Drawing.Point(16, 39);
            this.txtImageSize.Name = "txtImageSize";
            this.txtImageSize.Size = new System.Drawing.Size(64, 20);
            this.txtImageSize.TabIndex = 3;
            this.txtImageSize.Text = "512";
            // 
            // lblImageSize
            // 
            this.lblImageSize.Location = new System.Drawing.Point(13, 24);
            this.lblImageSize.Name = "lblImageSize";
            this.lblImageSize.Size = new System.Drawing.Size(64, 24);
            this.lblImageSize.TabIndex = 2;
            this.lblImageSize.Text = "Size";
            // 
            // btnCreateImage
            // 
            this.btnCreateImage.Location = new System.Drawing.Point(201, 36);
            this.btnCreateImage.Name = "btnCreateImage";
            this.btnCreateImage.Size = new System.Drawing.Size(93, 23);
            this.btnCreateImage.TabIndex = 1;
            this.btnCreateImage.Text = "Create Image";
            this.btnCreateImage.Click += new System.EventHandler(this.btnCreateImage_Click);
            // 
            // cboImageFormat
            // 
            this.cboImageFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboImageFormat.Items.AddRange(new object[] {
            "cloop",
            "cow",
            "qcow",
            "raw",
            "vmdk"});
            this.cboImageFormat.Location = new System.Drawing.Point(128, 39);
            this.cboImageFormat.Name = "cboImageFormat";
            this.cboImageFormat.Size = new System.Drawing.Size(56, 21);
            this.cboImageFormat.TabIndex = 7;
            // 
            // tabOther
            // 
            this.tabOther.Controls.Add(this.chkVBE30);
            this.tabOther.Controls.Add(this.groupBox1);
            this.tabOther.Location = new System.Drawing.Point(4, 22);
            this.tabOther.Name = "tabOther";
            this.tabOther.Padding = new System.Windows.Forms.Padding(3);
            this.tabOther.Size = new System.Drawing.Size(527, 201);
            this.tabOther.TabIndex = 11;
            this.tabOther.Text = "Other";
            this.tabOther.UseVisualStyleBackColor = true;
            // 
            // chkVBE30
            // 
            this.chkVBE30.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkVBE30.Location = new System.Drawing.Point(13, 172);
            this.chkVBE30.Name = "chkVBE30";
            this.chkVBE30.Size = new System.Drawing.Size(416, 23);
            this.chkVBE30.TabIndex = 3;
            this.chkVBE30.Text = "Simulate a standard VGA card with Bochs VBE 3.0 extensions ";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.textBox2);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(308, 98);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Linux";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(275, 19);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(27, 22);
            this.button1.TabIndex = 2;
            this.button1.Text = "...";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(101, 19);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(168, 20);
            this.textBox2.TabIndex = 0;
            // 
            // tabAbout
            // 
            this.tabAbout.Controls.Add(this.txtAbout);
            this.tabAbout.Location = new System.Drawing.Point(4, 22);
            this.tabAbout.Name = "tabAbout";
            this.tabAbout.Size = new System.Drawing.Size(527, 201);
            this.tabAbout.TabIndex = 9;
            this.tabAbout.Text = "About";
            this.tabAbout.UseVisualStyleBackColor = true;
            // 
            // txtAbout
            // 
            this.txtAbout.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtAbout.Location = new System.Drawing.Point(8, 8);
            this.txtAbout.Multiline = true;
            this.txtAbout.Name = "txtAbout";
            this.txtAbout.ReadOnly = true;
            this.txtAbout.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtAbout.Size = new System.Drawing.Size(439, 186);
            this.txtAbout.TabIndex = 0;
            this.txtAbout.Text = resources.GetString("txtAbout.Text");
            // 
            // btnLaunch
            // 
            this.btnLaunch.Location = new System.Drawing.Point(423, 243);
            this.btnLaunch.Name = "btnLaunch";
            this.btnLaunch.Size = new System.Drawing.Size(93, 23);
            this.btnLaunch.TabIndex = 4;
            this.btnLaunch.Text = "Launch";
            this.btnLaunch.Click += new System.EventHandler(this.btnLaunch_Click);
            // 
            // openFile
            // 
            this.openFile.Title = "Path to VDK";
            this.openFile.ValidateNames = false;
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(12, 243);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(93, 23);
            this.btnLoad.TabIndex = 5;
            this.btnLoad.Text = "Load";
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(111, 243);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(93, 23);
            this.btnSave.TabIndex = 6;
            this.btnSave.Text = "Save";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 22);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(69, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "Kernel Image";
            // 
            // frmMain
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(540, 272);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnLaunch);
            this.Controls.Add(this.Tabs);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Qemu GUI Launcher Version 1.1";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.grpMachine.ResumeLayout(false);
            this.Tabs.ResumeLayout(false);
            this.tabGen.ResumeLayout(false);
            this.grpACPI.ResumeLayout(false);
            this.grpACPI.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).EndInit();
            this.grpClock.ResumeLayout(false);
            this.grpClock.PerformLayout();
            this.grpDisplay.ResumeLayout(false);
            this.grpDisplay.PerformLayout();
            this.tabPaths.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.tabHardDisk.ResumeLayout(false);
            this.tabHardDisk.PerformLayout();
            this.grpHarddisk.ResumeLayout(false);
            this.grpHarddisk.PerformLayout();
            this.tabCDROM.ResumeLayout(false);
            this.grpCDROM.ResumeLayout(false);
            this.grpCDROM.PerformLayout();
            this.tabFloppy.ResumeLayout(false);
            this.tabFloppy.PerformLayout();
            this.grpFloppy.ResumeLayout(false);
            this.grpFloppy.PerformLayout();
            this.tabNetwork.ResumeLayout(false);
            this.grpNetwork.ResumeLayout(false);
            this.grpNetwork.PerformLayout();
            this.panelUser.ResumeLayout(false);
            this.panelUser.PerformLayout();
            this.panelNic.ResumeLayout(false);
            this.panelNic.PerformLayout();
            this.tabAudio.ResumeLayout(false);
            this.grpAudio.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabDebug.ResumeLayout(false);
            this.tabDebug.PerformLayout();
            this.grpVNC.ResumeLayout(false);
            this.grpVNC.PerformLayout();
            this.grpGDB.ResumeLayout(false);
            this.grpGDB.PerformLayout();
            this.groupBox16.ResumeLayout(false);
            this.groupBox16.PerformLayout();
            this.grpSerial.ResumeLayout(false);
            this.grpSerial.PerformLayout();
            this.grpParallel.ResumeLayout(false);
            this.grpParallel.PerformLayout();
            this.tabTools.ResumeLayout(false);
            this.grpVDK.ResumeLayout(false);
            this.grpVDK.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.tabOther.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabAbout.ResumeLayout(false);
            this.tabAbout.PerformLayout();
            this.ResumeLayout(false);

        }
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new frmMain());
		}

        private void btnCreateImage_Click(object sender, System.EventArgs e)
        {
            saveFileDialog.Filter = "All files (*.*)|*.*";
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

        #region Harddisks
        private void btnBrowseHDA_Click(object sender, System.EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img | All Files | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDA.Text = openFile.FileName;
        }

        private void btnBrowseHDB_Click(object sender, System.EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDB.Text = openFile.FileName;
        }

        private void btnBrowseHDC_Click(object sender, System.EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDC.Text = openFile.FileName;
        }

        private void btnBrowseHDD_Click(object sender, System.EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img";
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

        private void chkHardDiskHack_CheckedChanged(object sender, EventArgs e)
        {
            if (chkHardDiskHack.Checked == true)
                data.Harddisks.W2kHack = true;
            else
                data.Harddisks.W2kHack = false;
        }

        #endregion

        #region CDROM
        private void btnBrowseCDROM_Click(object sender, System.EventArgs e)
        {
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
                optHostCDROM.Enabled = true;
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

        private void btnLaunch_Click(object sender, System.EventArgs e)
        {
            SaveSettings();

            bool HasHDisk = false;
            bool HasFDisk = false;

            /* There must be atleast one source of OS */
            for (int i = 0; i < data.Harddisks.HDD.Length; i++)
            {
                if (data.Harddisks.HDD[i].Enabled == true)
                    HasHDisk = true;
            }

            for (int i = 0; i < data.Floppies.FDD.Length; i++)
            {
                if (data.Floppies.FDD[i].Enabled == true)
                    HasFDisk = true;
            }
            if (HasHDisk == false && data.CDROM.Enabled == false && HasFDisk == false)
                MessageBox.Show("Must enable atleast 1 Hard disk, CD-Rom or Floppy disk!", "Error");//or specify linux kernel image???
            else
            {
                /* we must know where to look for qemu */
                if (data.Paths.QEmu.Length == 0)
                    MessageBox.Show("Please specify Qemu Path!", "Error");
                else
                {
                    runner = new Runner(data);
                    runner.StartQemu((Platforms)cboMachine.SelectedIndex);
                }
            }
        }

        #region Debug
        private void chkSerialToFile_CheckedChanged(object sender, System.EventArgs e)
        {
            if (chkSerialToFile.Checked == true)
            {
                btnBrowseSerial.Enabled = true;
                txtSerialFile.Enabled = true;
                data.Debug.SerialPort.FRedirect = true;
                data.Debug.SerialPort.PRedirect = false;
                chkSerialToPipe.Enabled = false;
                chkSerialToPipe.Checked = false;
                txtSerialPipe.Enabled = false;
            }
            else
            {
                data.Debug.SerialPort.FRedirect = false;
                chkSerialToPipe.Enabled = true;
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

        private void chkParallelToFile_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseParallel.Enabled = chkParallelToFile.Checked;
            data.Debug.ParallelPort.FRedirect = chkParallelToFile.Checked;
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

        private void btnBrowseParallel_Click(object sender, System.EventArgs e)
        {
            saveFileDialog.Title = "Parallel output to file";
            saveFileDialog.FileName = "parallel_output.txt";
            saveFileDialog.Filter = "Text File (*.txt)|*.txt";
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
                txtParallelFile.Text = saveFileDialog.FileName;
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

        private void frmMain_Load(object sender, EventArgs e)
        {
            data = new Data();

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

            /* Default config load */
            string DefaultConfig = currentDir + "/config.xml";
            if (File.Exists(DefaultConfig))
            {
                XmlSerializer s = new XmlSerializer(typeof(Data));
                TextReader r = new StreamReader(DefaultConfig);
                data = (Data)s.Deserialize(r);
                r.Close();
                LoadSettings();
            }
            else
            {
                /* if no settings file found, fallback to hardcoded defaults */

                /* Network */
                chckNetEnable.Checked = true;
                VUser def = new VUser();
                def.vlan = 0;
                def.hostname = "host";
                listVLANs.Items.Add(def.ToString());
                VLanlist.Add(def);
            }
        }

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
            if (openFile.ShowDialog() == DialogResult.OK)
                txtFloppyA.Text = openFile.FileName;
        }

        private void btnBrowseFloppyB_Click(object sender, System.EventArgs e)
        {
            if (openFile.ShowDialog() == DialogResult.OK)
                txtFloppyB.Text = openFile.FileName;
        }

        private void chkFloopySig_CheckedChanged(object sender, EventArgs e)
        {
            if (chkFloopySig.Checked == true)
                data.Floppies.fd_bootchk = true;
            else
                data.Floppies.fd_bootchk = false;
        }

        #endregion

        #region Paths
        private void btnQEmuPath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                txtQEmuPath.Text = folderBrowserDialog1.SelectedPath;
                data.Paths.QEmu = txtQEmuPath.Text; 
            }
        }

        private void btnVDKBrowse_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                txtVDKPath.Text = folderBrowserDialog1.SelectedPath;
                grpVDK.Enabled = true;
            }
            else
                grpVDK.Enabled = false;
        }

        #endregion

        #region Settings
        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveSettings();
            saveFileDialog.Title = "Save settings";
            saveFileDialog.InitialDirectory = currentDir;
            saveFileDialog.FileName = "config.xml";
            saveFileDialog.Filter = "QEMU GUI Config (*.xml)|*.xml"; 
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                XmlSerializer s = new XmlSerializer(typeof(Data));
                TextWriter w = new StreamWriter(saveFileDialog.FileName);
                s.Serialize(w, data);
                w.Close();
            }
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            openFile.Title = "Load settings";
            openFile.InitialDirectory = currentDir;
            openFile.FileName = "config.xml";
            openFile.Filter = "QEMU GUI Config (*.xml)|*.xml"; 
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                XmlSerializer s = new XmlSerializer(typeof(Data));
                TextReader r = new StreamReader(openFile.FileName);
                data = (Data)s.Deserialize(r);
                r.Close();
                LoadSettings();
            }
        }

        private void LoadSettings()
        {
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
            txtQEmuPath.Text = data.Paths.QEmu;
            txtVDKPath.Text = data.Paths.VDK;
            grpVDK.Enabled = (txtVDKPath.Text.Length > 0);

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
            chkSerialToFile_CheckedChanged(null, null);

            chkParallelToFile.Checked = data.Debug.ParallelPort.FRedirect;
            txtParallelFile.Text = data.Debug.ParallelPort.FileName;
            chkParallelToFile.Checked = data.Debug.ParallelPort.PRedirect;
            txtParallelPipe.Text = data.Debug.ParallelPort.PipeName;
            chkSerialToPipe_CheckedChanged(null, null);

            saved_state = data.Debug.SavedStatePath;
            chkVBE30.Checked = data.Debug.VBE3;
            txtGDBPort.Text = data.Debug.GDBPort.ToString();

            /* Network */
            chckNetEnable.Checked = data.Network.Enabled;
            foreach (string a in data.Network.VNicStringReader())
                if(a != "ignore")
                    listVLANs.Items.Add(a.ToString());
            chckNetEnable_CheckedChanged(null, null);//Make visible the correct panel


            /* Tools */
            txtVDKImage.Text = data.Tools.vdk.Image;
            cboVDKDrive.Text = data.Tools.vdk.DriveLetter;
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
            data.Paths.QEmu = txtQEmuPath.Text;
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
            data.Debug.SerialPort.FRedirect = chkSerialToFile.Checked;
            data.Debug.SerialPort.FileName = txtSerialFile.Text;
            data.Debug.ParallelPort.FRedirect = chkParallelToFile.Checked;
            data.Debug.ParallelPort.FileName = txtParallelFile.Text;
            data.Debug.SavedStatePath = saved_state;
            data.Debug.VBE3 = chkVBE30.Checked;
            data.Debug.GDBPort = Int32.Parse(txtGDBPort.Text);

            /* Network */
            data.Network.Enabled = chckNetEnable.Checked;
            //no need to save the vlanstring, we already send it directly

            /* Tools */
            data.Tools.vdk.Image = txtVDKImage.Text;
            data.Tools.vdk.DriveLetter = cboVDKDrive.Text;
        }

        #endregion

        #region VDK

        private void btnVDKImage_Click(object sender, EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img | All Files | *.* ";
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
            SaveSettings();
            runner = new Runner(data);
            runner.UnmountImage();

        }

        private void btnMount_Click(object sender, EventArgs e)
        {
            SaveSettings();
            runner = new Runner(data);
            if(runner.MountImage())
            {
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
        // This area needs heavy work, some of the shortcommings are:
        // VLAN numbers are not assigned by users
        // a VLAN can be redirected into another VLAN, currently not posible
        // Design may be clumsy and complicated, specially for users that dont know alot about these qemu settings
        // Find a way to name the VLANs in the listbox for the benefit of these users, instead of using the raw command string
        // Find a way to implement the remove button, this will get complicated on redirected VLANs...
        // for now -net user works great, which is the most important option ;)


        public static ArrayList VLanlist = new ArrayList();
        private void chckNetEnable_CheckedChanged(object sender, EventArgs e)
        {
            
            if (chckNetEnable.Checked == false)
            {
                data.Network.Enabled = true;
                /* Disable everything */
                listVLANs.Enabled = false;
                btnNetAdd.Enabled = false;
                btnNetRemove.Enabled = false;
                rbtnNetNic.Enabled = false;
                rbtnNetUser.Enabled = false;
                /* Make all panels invisible */
                panelUser.Visible = false;
                panelNic.Visible = false;
                label6.Visible = false;
            }
            else
            {
                data.Network.Enabled = false;
                /* Enable everything */
                listVLANs.Enabled = true;
                btnNetAdd.Enabled = true;
                btnNetRemove.Enabled = true;
                rbtnNetNic.Enabled = true;
                rbtnNetUser.Enabled = true;
                /* Find out which pannels need visibility */
                rbtnNetUser_CheckedChanged(null, null);
                rbtnNetNic_CheckedChanged(null, null);
                label6.Visible = true;
            }

        }

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
            //VLanlist.RemoveAt(VLanlist.
            //listVLANs.Items.RemoveAt(listVLANs.
        }

        #endregion





    }
}
