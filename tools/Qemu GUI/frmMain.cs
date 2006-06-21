using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
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
        private System.Windows.Forms.GroupBox grpMachine;
        private System.Windows.Forms.TabPage tabMisc;
        private System.Windows.Forms.TabControl HardDisk2;
        private System.Windows.Forms.GroupBox grpFloppy;
        private System.Windows.Forms.CheckBox chkFloppyA;
        private System.Windows.Forms.CheckBox chkFloppyB;
        private System.Windows.Forms.Button btnBrowseFloppyA;
        private System.Windows.Forms.Button btnBrowseFloppyB;
        private System.Windows.Forms.TabPage tabFloppy;
        private System.Windows.Forms.GroupBox grpHarddisk;
        private System.Windows.Forms.Button btnBrowseHDB;
        private System.Windows.Forms.Button btnBrowseHDA;
        private System.Windows.Forms.CheckBox chkUseHDA;
        private System.Windows.Forms.Button btnBrowseHDD;
        private System.Windows.Forms.Button btnBrowseHDC;
        private System.Windows.Forms.CheckBox chkUseHDD;
        private System.Windows.Forms.CheckBox chkUseHDC;
        private System.Windows.Forms.CheckBox chkUseHDB;
        private System.Windows.Forms.GroupBox grpCDROM;
        private System.Windows.Forms.Button btnBrowseCDROM;
        private System.Windows.Forms.CheckBox chkUseCDROM;
        private System.Windows.Forms.RadioButton optHostCDROM;
        private System.Windows.Forms.RadioButton optCDImage;
        private System.Windows.Forms.TabPage HardDisk;
        private System.Windows.Forms.TabPage tabCDROM;
        private System.Windows.Forms.TabPage tabAudio;
        private System.Windows.Forms.GroupBox grpAudio;
        private System.Windows.Forms.CheckBox chkPCSpeaker;
        private System.Windows.Forms.CheckBox chkSoundBlaster;
        private System.Windows.Forms.CheckBox chkES1370;
        private System.Windows.Forms.CheckBox chkOPL2;
        private System.Windows.Forms.TabPage tabTools;
        private System.Windows.Forms.ComboBox cboBootFrom;
        private System.Windows.Forms.Label lblBootFrom;
        private System.Windows.Forms.Label lblSMP;
        private System.Windows.Forms.Label lblMemory;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button btnLaunch;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Button btnCreateImage;
        private System.Windows.Forms.Label lblImageSize;
        private System.Windows.Forms.TextBox txtImageSize;
        private System.Windows.Forms.TabPage tabNetwork;
        private System.Windows.Forms.TabPage tabDebug;
        private System.Windows.Forms.TabPage tabAbout;
        private System.Windows.Forms.GroupBox grpClock;
        private System.Windows.Forms.GroupBox grpDisplay;
        private System.Windows.Forms.GroupBox grpSerial;
        private System.Windows.Forms.CheckBox chkSerialToFile;
        private System.Windows.Forms.Button btnBrowseSerial;
        private System.Windows.Forms.GroupBox grpParallel;
        private System.Windows.Forms.Button btnBrowseParallel;
        private System.Windows.Forms.CheckBox chkParallelToFile;
        private System.Windows.Forms.GroupBox grpGDB;
        private System.Windows.Forms.CheckBox checkBox14;
        private System.Windows.Forms.TextBox txtGDBPort;
        private System.Windows.Forms.Label lblGDBPort;
        private System.Windows.Forms.GroupBox grpVNC;
        private System.Windows.Forms.CheckBox chkVNC;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.GroupBox groupBox16;
        private System.Windows.Forms.Button button20;
        private System.Windows.Forms.CheckBox chkVBE30;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.OpenFileDialog openFile;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ComboBox cboImageFormat;
        private System.Windows.Forms.Label label11;
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
        private IContainer components;
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
        private QEmu qemu;

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
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
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
            this.HardDisk2 = new System.Windows.Forms.TabControl();
            this.tabMisc = new System.Windows.Forms.TabPage();
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
            this.tabCDROM = new System.Windows.Forms.TabPage();
            this.grpCDROM = new System.Windows.Forms.GroupBox();
            this.cboCDROM = new System.Windows.Forms.ComboBox();
            this.txtCDROM = new System.Windows.Forms.TextBox();
            this.optHostCDROM = new System.Windows.Forms.RadioButton();
            this.btnBrowseCDROM = new System.Windows.Forms.Button();
            this.chkUseCDROM = new System.Windows.Forms.CheckBox();
            this.optCDImage = new System.Windows.Forms.RadioButton();
            this.tabFloppy = new System.Windows.Forms.TabPage();
            this.grpFloppy = new System.Windows.Forms.GroupBox();
            this.txtFloppyB = new System.Windows.Forms.TextBox();
            this.txtFloppyA = new System.Windows.Forms.TextBox();
            this.btnBrowseFloppyB = new System.Windows.Forms.Button();
            this.btnBrowseFloppyA = new System.Windows.Forms.Button();
            this.chkFloppyB = new System.Windows.Forms.CheckBox();
            this.chkFloppyA = new System.Windows.Forms.CheckBox();
            this.HardDisk = new System.Windows.Forms.TabPage();
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
            this.grpVNC = new System.Windows.Forms.GroupBox();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.chkVNC = new System.Windows.Forms.CheckBox();
            this.grpGDB = new System.Windows.Forms.GroupBox();
            this.lblGDBPort = new System.Windows.Forms.Label();
            this.txtGDBPort = new System.Windows.Forms.TextBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.grpSerial = new System.Windows.Forms.GroupBox();
            this.btnBrowseSerial = new System.Windows.Forms.Button();
            this.chkSerialToFile = new System.Windows.Forms.CheckBox();
            this.grpParallel = new System.Windows.Forms.GroupBox();
            this.btnBrowseParallel = new System.Windows.Forms.Button();
            this.chkParallelToFile = new System.Windows.Forms.CheckBox();
            this.groupBox16 = new System.Windows.Forms.GroupBox();
            this.button20 = new System.Windows.Forms.Button();
            this.chkVBE30 = new System.Windows.Forms.CheckBox();
            this.tabNetwork = new System.Windows.Forms.TabPage();
            this.tabPaths = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtQEmuPath = new System.Windows.Forms.TextBox();
            this.txtVDKPath = new System.Windows.Forms.TextBox();
            this.btnVDKBrowse = new System.Windows.Forms.Button();
            this.lblVDKPath = new System.Windows.Forms.Label();
            this.btnQEmuPath = new System.Windows.Forms.Button();
            this.lblQEmuPath = new System.Windows.Forms.Label();
            this.tabAbout = new System.Windows.Forms.TabPage();
            this.txtAbout = new System.Windows.Forms.TextBox();
            this.btnLaunch = new System.Windows.Forms.Button();
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.grpMachine.SuspendLayout();
            this.HardDisk2.SuspendLayout();
            this.tabMisc.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).BeginInit();
            this.grpClock.SuspendLayout();
            this.grpDisplay.SuspendLayout();
            this.tabCDROM.SuspendLayout();
            this.grpCDROM.SuspendLayout();
            this.tabFloppy.SuspendLayout();
            this.grpFloppy.SuspendLayout();
            this.HardDisk.SuspendLayout();
            this.grpHarddisk.SuspendLayout();
            this.tabTools.SuspendLayout();
            this.grpVDK.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.tabAudio.SuspendLayout();
            this.grpAudio.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabDebug.SuspendLayout();
            this.grpVNC.SuspendLayout();
            this.grpGDB.SuspendLayout();
            this.grpSerial.SuspendLayout();
            this.grpParallel.SuspendLayout();
            this.groupBox16.SuspendLayout();
            this.tabPaths.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tabAbout.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpMachine
            // 
            this.grpMachine.Controls.Add(this.cboMachine);
            this.grpMachine.Location = new System.Drawing.Point(3, 3);
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
            "ARM Versatile/AB  (ARM926EJ-S)"});
            this.cboMachine.Location = new System.Drawing.Point(19, 19);
            this.cboMachine.Name = "cboMachine";
            this.cboMachine.Size = new System.Drawing.Size(239, 21);
            this.cboMachine.TabIndex = 20;
            // 
            // HardDisk2
            // 
            this.HardDisk2.Controls.Add(this.tabMisc);
            this.HardDisk2.Controls.Add(this.tabCDROM);
            this.HardDisk2.Controls.Add(this.tabFloppy);
            this.HardDisk2.Controls.Add(this.HardDisk);
            this.HardDisk2.Controls.Add(this.tabTools);
            this.HardDisk2.Controls.Add(this.tabAudio);
            this.HardDisk2.Controls.Add(this.tabDebug);
            this.HardDisk2.Controls.Add(this.tabNetwork);
            this.HardDisk2.Controls.Add(this.tabPaths);
            this.HardDisk2.Controls.Add(this.tabAbout);
            this.HardDisk2.Location = new System.Drawing.Point(12, 10);
            this.HardDisk2.Name = "HardDisk2";
            this.HardDisk2.SelectedIndex = 0;
            this.HardDisk2.Size = new System.Drawing.Size(468, 227);
            this.HardDisk2.TabIndex = 3;
            // 
            // tabMisc
            // 
            this.tabMisc.Controls.Add(this.groupBox6);
            this.tabMisc.Controls.Add(this.grpMachine);
            this.tabMisc.Controls.Add(this.grpClock);
            this.tabMisc.Controls.Add(this.grpDisplay);
            this.tabMisc.Location = new System.Drawing.Point(4, 22);
            this.tabMisc.Name = "tabMisc";
            this.tabMisc.Size = new System.Drawing.Size(460, 201);
            this.tabMisc.TabIndex = 1;
            this.tabMisc.Text = "Misc";
            this.tabMisc.UseVisualStyleBackColor = true;
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
            this.groupBox6.Location = new System.Drawing.Point(3, 57);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(274, 142);
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
            32,
            0,
            0,
            0});
            // 
            // lblMB
            // 
            this.lblMB.AutoSize = true;
            this.lblMB.Location = new System.Drawing.Point(229, 49);
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
            this.grpClock.Location = new System.Drawing.Point(283, 3);
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
            this.grpDisplay.Location = new System.Drawing.Point(283, 57);
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
            this.chkFullscreen.Size = new System.Drawing.Size(107, 17);
            this.chkFullscreen.TabIndex = 19;
            this.chkFullscreen.Text = "Start in fullscreen";
            this.chkFullscreen.UseVisualStyleBackColor = true;
            // 
            // tabCDROM
            // 
            this.tabCDROM.Controls.Add(this.grpCDROM);
            this.tabCDROM.Location = new System.Drawing.Point(4, 22);
            this.tabCDROM.Name = "tabCDROM";
            this.tabCDROM.Size = new System.Drawing.Size(460, 201);
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
            this.grpCDROM.Size = new System.Drawing.Size(454, 195);
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
            this.btnBrowseCDROM.Size = new System.Drawing.Size(27, 23);
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
            this.tabFloppy.Controls.Add(this.grpFloppy);
            this.tabFloppy.Location = new System.Drawing.Point(4, 22);
            this.tabFloppy.Name = "tabFloppy";
            this.tabFloppy.Size = new System.Drawing.Size(460, 201);
            this.tabFloppy.TabIndex = 2;
            this.tabFloppy.Text = "Floppy";
            this.tabFloppy.UseVisualStyleBackColor = true;
            // 
            // grpFloppy
            // 
            this.grpFloppy.Controls.Add(this.txtFloppyB);
            this.grpFloppy.Controls.Add(this.txtFloppyA);
            this.grpFloppy.Controls.Add(this.btnBrowseFloppyB);
            this.grpFloppy.Controls.Add(this.btnBrowseFloppyA);
            this.grpFloppy.Controls.Add(this.chkFloppyB);
            this.grpFloppy.Controls.Add(this.chkFloppyA);
            this.grpFloppy.Location = new System.Drawing.Point(3, 3);
            this.grpFloppy.Name = "grpFloppy";
            this.grpFloppy.Size = new System.Drawing.Size(454, 195);
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
            this.btnBrowseFloppyB.Location = new System.Drawing.Point(407, 110);
            this.btnBrowseFloppyB.Name = "btnBrowseFloppyB";
            this.btnBrowseFloppyB.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseFloppyB.TabIndex = 3;
            this.btnBrowseFloppyB.Text = "...";
            this.btnBrowseFloppyB.Click += new System.EventHandler(this.btnBrowseFloppyB_Click);
            // 
            // btnBrowseFloppyA
            // 
            this.btnBrowseFloppyA.Enabled = false;
            this.btnBrowseFloppyA.Location = new System.Drawing.Point(407, 46);
            this.btnBrowseFloppyA.Name = "btnBrowseFloppyA";
            this.btnBrowseFloppyA.Size = new System.Drawing.Size(27, 23);
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
            // HardDisk
            // 
            this.HardDisk.Controls.Add(this.grpHarddisk);
            this.HardDisk.Location = new System.Drawing.Point(4, 22);
            this.HardDisk.Name = "HardDisk";
            this.HardDisk.Size = new System.Drawing.Size(460, 201);
            this.HardDisk.TabIndex = 3;
            this.HardDisk.Text = " Harddisk";
            this.HardDisk.UseVisualStyleBackColor = true;
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
            this.grpHarddisk.Location = new System.Drawing.Point(3, 3);
            this.grpHarddisk.Name = "grpHarddisk";
            this.grpHarddisk.Size = new System.Drawing.Size(457, 195);
            this.grpHarddisk.TabIndex = 5;
            this.grpHarddisk.TabStop = false;
            this.grpHarddisk.Text = " Harddisk";
            // 
            // txtHDD
            // 
            this.txtHDD.Enabled = false;
            this.txtHDD.Location = new System.Drawing.Point(73, 163);
            this.txtHDD.Name = "txtHDD";
            this.txtHDD.Size = new System.Drawing.Size(341, 20);
            this.txtHDD.TabIndex = 15;
            // 
            // txtHDC
            // 
            this.txtHDC.Enabled = false;
            this.txtHDC.Location = new System.Drawing.Point(73, 122);
            this.txtHDC.Name = "txtHDC";
            this.txtHDC.Size = new System.Drawing.Size(341, 20);
            this.txtHDC.TabIndex = 14;
            // 
            // txtHDB
            // 
            this.txtHDB.Enabled = false;
            this.txtHDB.Location = new System.Drawing.Point(73, 77);
            this.txtHDB.Name = "txtHDB";
            this.txtHDB.Size = new System.Drawing.Size(341, 20);
            this.txtHDB.TabIndex = 13;
            // 
            // txtHDA
            // 
            this.txtHDA.Enabled = false;
            this.txtHDA.Location = new System.Drawing.Point(73, 30);
            this.txtHDA.Name = "txtHDA";
            this.txtHDA.Size = new System.Drawing.Size(341, 20);
            this.txtHDA.TabIndex = 12;
            // 
            // btnBrowseHDD
            // 
            this.btnBrowseHDD.Enabled = false;
            this.btnBrowseHDD.Location = new System.Drawing.Point(420, 163);
            this.btnBrowseHDD.Name = "btnBrowseHDD";
            this.btnBrowseHDD.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDD.TabIndex = 11;
            this.btnBrowseHDD.Text = "...";
            this.btnBrowseHDD.Click += new System.EventHandler(this.btnBrowseHDD_Click);
            // 
            // btnBrowseHDC
            // 
            this.btnBrowseHDC.Enabled = false;
            this.btnBrowseHDC.Location = new System.Drawing.Point(420, 119);
            this.btnBrowseHDC.Name = "btnBrowseHDC";
            this.btnBrowseHDC.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDC.TabIndex = 10;
            this.btnBrowseHDC.Text = "...";
            this.btnBrowseHDC.Click += new System.EventHandler(this.btnBrowseHDC_Click);
            // 
            // chkUseHDD
            // 
            this.chkUseHDD.Location = new System.Drawing.Point(14, 163);
            this.chkUseHDD.Name = "chkUseHDD";
            this.chkUseHDD.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDD.TabIndex = 9;
            this.chkUseHDD.Text = "HDD";
            this.chkUseHDD.CheckedChanged += new System.EventHandler(this.chkUseHDD_CheckedChanged);
            // 
            // chkUseHDC
            // 
            this.chkUseHDC.Location = new System.Drawing.Point(14, 118);
            this.chkUseHDC.Name = "chkUseHDC";
            this.chkUseHDC.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDC.TabIndex = 8;
            this.chkUseHDC.Text = "HDC";
            this.chkUseHDC.CheckedChanged += new System.EventHandler(this.chkUseHDC_CheckedChanged);
            // 
            // btnBrowseHDB
            // 
            this.btnBrowseHDB.Enabled = false;
            this.btnBrowseHDB.Location = new System.Drawing.Point(420, 73);
            this.btnBrowseHDB.Name = "btnBrowseHDB";
            this.btnBrowseHDB.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDB.TabIndex = 7;
            this.btnBrowseHDB.Text = "...";
            this.btnBrowseHDB.Click += new System.EventHandler(this.btnBrowseHDB_Click);
            // 
            // btnBrowseHDA
            // 
            this.btnBrowseHDA.Enabled = false;
            this.btnBrowseHDA.Location = new System.Drawing.Point(420, 28);
            this.btnBrowseHDA.Name = "btnBrowseHDA";
            this.btnBrowseHDA.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDA.TabIndex = 6;
            this.btnBrowseHDA.Text = " ...";
            this.btnBrowseHDA.Click += new System.EventHandler(this.btnBrowseHDA_Click);
            // 
            // chkUseHDB
            // 
            this.chkUseHDB.Location = new System.Drawing.Point(14, 73);
            this.chkUseHDB.Name = "chkUseHDB";
            this.chkUseHDB.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDB.TabIndex = 5;
            this.chkUseHDB.Text = "HDB";
            this.chkUseHDB.CheckedChanged += new System.EventHandler(this.chkUseHDB_CheckedChanged);
            // 
            // chkUseHDA
            // 
            this.chkUseHDA.Location = new System.Drawing.Point(14, 28);
            this.chkUseHDA.Name = "chkUseHDA";
            this.chkUseHDA.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDA.TabIndex = 4;
            this.chkUseHDA.Text = "HDA";
            this.chkUseHDA.CheckedChanged += new System.EventHandler(this.chkUseHDA_CheckedChanged);
            // 
            // tabTools
            // 
            this.tabTools.Controls.Add(this.grpVDK);
            this.tabTools.Controls.Add(this.groupBox9);
            this.tabTools.Location = new System.Drawing.Point(4, 22);
            this.tabTools.Name = "tabTools";
            this.tabTools.Size = new System.Drawing.Size(460, 201);
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
            this.grpVDK.Size = new System.Drawing.Size(453, 115);
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
            this.groupBox9.Size = new System.Drawing.Size(453, 74);
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
            // tabAudio
            // 
            this.tabAudio.Controls.Add(this.grpAudio);
            this.tabAudio.Location = new System.Drawing.Point(4, 22);
            this.tabAudio.Name = "tabAudio";
            this.tabAudio.Size = new System.Drawing.Size(460, 201);
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
            this.grpAudio.Size = new System.Drawing.Size(454, 195);
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
            this.chkOPL2.Location = new System.Drawing.Point(157, 108);
            this.chkOPL2.Name = "chkOPL2";
            this.chkOPL2.Size = new System.Drawing.Size(176, 24);
            this.chkOPL2.TabIndex = 2;
            this.chkOPL2.Text = " Yamaha YM3812 (OPL2)";
            // 
            // tabDebug
            // 
            this.tabDebug.Controls.Add(this.grpVNC);
            this.tabDebug.Controls.Add(this.grpGDB);
            this.tabDebug.Controls.Add(this.grpSerial);
            this.tabDebug.Controls.Add(this.grpParallel);
            this.tabDebug.Controls.Add(this.groupBox16);
            this.tabDebug.Controls.Add(this.chkVBE30);
            this.tabDebug.Location = new System.Drawing.Point(4, 22);
            this.tabDebug.Name = "tabDebug";
            this.tabDebug.Size = new System.Drawing.Size(460, 201);
            this.tabDebug.TabIndex = 8;
            this.tabDebug.Text = "Debug";
            this.tabDebug.UseVisualStyleBackColor = true;
            // 
            // grpVNC
            // 
            this.grpVNC.Controls.Add(this.textBox7);
            this.grpVNC.Controls.Add(this.label14);
            this.grpVNC.Controls.Add(this.chkVNC);
            this.grpVNC.Location = new System.Drawing.Point(336, 88);
            this.grpVNC.Name = "grpVNC";
            this.grpVNC.Size = new System.Drawing.Size(112, 88);
            this.grpVNC.TabIndex = 3;
            this.grpVNC.TabStop = false;
            this.grpVNC.Text = "VNC Server";
            // 
            // textBox7
            // 
            this.textBox7.Enabled = false;
            this.textBox7.Location = new System.Drawing.Point(8, 64);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(96, 20);
            this.textBox7.TabIndex = 2;
            this.textBox7.Text = "0";
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(8, 40);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(88, 23);
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
            this.grpGDB.Location = new System.Drawing.Point(160, 8);
            this.grpGDB.Name = "grpGDB";
            this.grpGDB.Size = new System.Drawing.Size(160, 168);
            this.grpGDB.TabIndex = 2;
            this.grpGDB.TabStop = false;
            this.grpGDB.Text = "GDB";
            // 
            // lblGDBPort
            // 
            this.lblGDBPort.Location = new System.Drawing.Point(6, 128);
            this.lblGDBPort.Name = "lblGDBPort";
            this.lblGDBPort.Size = new System.Drawing.Size(55, 23);
            this.lblGDBPort.TabIndex = 2;
            this.lblGDBPort.Text = "Port:";
            // 
            // txtGDBPort
            // 
            this.txtGDBPort.Location = new System.Drawing.Point(92, 128);
            this.txtGDBPort.Name = "txtGDBPort";
            this.txtGDBPort.Size = new System.Drawing.Size(60, 20);
            this.txtGDBPort.TabIndex = 1;
            this.txtGDBPort.Text = "1234";
            this.txtGDBPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // checkBox14
            // 
            this.checkBox14.Location = new System.Drawing.Point(8, 32);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(144, 24);
            this.checkBox14.TabIndex = 0;
            this.checkBox14.Text = "Wait connection to port";
            // 
            // grpSerial
            // 
            this.grpSerial.Controls.Add(this.btnBrowseSerial);
            this.grpSerial.Controls.Add(this.chkSerialToFile);
            this.grpSerial.Location = new System.Drawing.Point(32, 8);
            this.grpSerial.Name = "grpSerial";
            this.grpSerial.Size = new System.Drawing.Size(120, 80);
            this.grpSerial.TabIndex = 1;
            this.grpSerial.TabStop = false;
            this.grpSerial.Text = "Serial Port";
            // 
            // btnBrowseSerial
            // 
            this.btnBrowseSerial.Enabled = false;
            this.btnBrowseSerial.Location = new System.Drawing.Point(8, 48);
            this.btnBrowseSerial.Name = "btnBrowseSerial";
            this.btnBrowseSerial.Size = new System.Drawing.Size(96, 23);
            this.btnBrowseSerial.TabIndex = 3;
            this.btnBrowseSerial.Text = "Browse";
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
            this.grpParallel.Controls.Add(this.btnBrowseParallel);
            this.grpParallel.Controls.Add(this.chkParallelToFile);
            this.grpParallel.Location = new System.Drawing.Point(32, 96);
            this.grpParallel.Name = "grpParallel";
            this.grpParallel.Size = new System.Drawing.Size(120, 80);
            this.grpParallel.TabIndex = 1;
            this.grpParallel.TabStop = false;
            this.grpParallel.Text = "Parallel port";
            // 
            // btnBrowseParallel
            // 
            this.btnBrowseParallel.Enabled = false;
            this.btnBrowseParallel.Location = new System.Drawing.Point(8, 48);
            this.btnBrowseParallel.Name = "btnBrowseParallel";
            this.btnBrowseParallel.Size = new System.Drawing.Size(96, 23);
            this.btnBrowseParallel.TabIndex = 3;
            this.btnBrowseParallel.Text = "Browse";
            this.btnBrowseParallel.Click += new System.EventHandler(this.btnBrowseParallel_Click);
            // 
            // chkParallelToFile
            // 
            this.chkParallelToFile.Location = new System.Drawing.Point(8, 16);
            this.chkParallelToFile.Name = "chkParallelToFile";
            this.chkParallelToFile.Size = new System.Drawing.Size(104, 24);
            this.chkParallelToFile.TabIndex = 2;
            this.chkParallelToFile.Text = "Redirect to File";
            this.chkParallelToFile.CheckedChanged += new System.EventHandler(this.chkParallelToFile_CheckedChanged);
            // 
            // groupBox16
            // 
            this.groupBox16.Controls.Add(this.button20);
            this.groupBox16.Location = new System.Drawing.Point(336, 8);
            this.groupBox16.Name = "groupBox16";
            this.groupBox16.Size = new System.Drawing.Size(112, 72);
            this.groupBox16.TabIndex = 1;
            this.groupBox16.TabStop = false;
            this.groupBox16.Text = "LoadVM state";
            // 
            // button20
            // 
            this.button20.Location = new System.Drawing.Point(8, 24);
            this.button20.Name = "button20";
            this.button20.Size = new System.Drawing.Size(96, 23);
            this.button20.TabIndex = 3;
            this.button20.Text = "Browse";
            this.button20.Click += new System.EventHandler(this.button20_Click);
            // 
            // chkVBE30
            // 
            this.chkVBE30.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkVBE30.Location = new System.Drawing.Point(32, 169);
            this.chkVBE30.Name = "chkVBE30";
            this.chkVBE30.Size = new System.Drawing.Size(416, 32);
            this.chkVBE30.TabIndex = 2;
            this.chkVBE30.Text = "Simulate a standard VGA card with Bochs VBE 3.0 extensions ";
            // 
            // tabNetwork
            // 
            this.tabNetwork.Location = new System.Drawing.Point(4, 22);
            this.tabNetwork.Name = "tabNetwork";
            this.tabNetwork.Size = new System.Drawing.Size(460, 201);
            this.tabNetwork.TabIndex = 7;
            this.tabNetwork.Text = "Network";
            this.tabNetwork.UseVisualStyleBackColor = true;
            // 
            // tabPaths
            // 
            this.tabPaths.Controls.Add(this.groupBox2);
            this.tabPaths.Location = new System.Drawing.Point(4, 22);
            this.tabPaths.Name = "tabPaths";
            this.tabPaths.Size = new System.Drawing.Size(460, 201);
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
            this.groupBox2.Location = new System.Drawing.Point(3, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(454, 195);
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
            this.btnVDKBrowse.Size = new System.Drawing.Size(27, 21);
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
            this.btnQEmuPath.Size = new System.Drawing.Size(27, 21);
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
            this.lblQEmuPath.Size = new System.Drawing.Size(36, 13);
            this.lblQEmuPath.TabIndex = 2;
            this.lblQEmuPath.Text = "QEmu";
            // 
            // tabAbout
            // 
            this.tabAbout.Controls.Add(this.txtAbout);
            this.tabAbout.Location = new System.Drawing.Point(4, 22);
            this.tabAbout.Name = "tabAbout";
            this.tabAbout.Size = new System.Drawing.Size(460, 201);
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
            this.txtAbout.Size = new System.Drawing.Size(439, 186);
            this.txtAbout.TabIndex = 0;
            this.txtAbout.Text = resources.GetString("txtAbout.Text");
            // 
            // btnLaunch
            // 
            this.btnLaunch.Location = new System.Drawing.Point(387, 243);
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
            // frmMain
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(485, 270);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnLaunch);
            this.Controls.Add(this.HardDisk2);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Qemu GUI Launcher Version 1.0 written by Magnus Olsen for ReactOS";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.grpMachine.ResumeLayout(false);
            this.HardDisk2.ResumeLayout(false);
            this.tabMisc.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).EndInit();
            this.grpClock.ResumeLayout(false);
            this.grpClock.PerformLayout();
            this.grpDisplay.ResumeLayout(false);
            this.grpDisplay.PerformLayout();
            this.tabCDROM.ResumeLayout(false);
            this.grpCDROM.ResumeLayout(false);
            this.grpCDROM.PerformLayout();
            this.tabFloppy.ResumeLayout(false);
            this.grpFloppy.ResumeLayout(false);
            this.grpFloppy.PerformLayout();
            this.HardDisk.ResumeLayout(false);
            this.grpHarddisk.ResumeLayout(false);
            this.grpHarddisk.PerformLayout();
            this.tabTools.ResumeLayout(false);
            this.grpVDK.ResumeLayout(false);
            this.grpVDK.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.tabAudio.ResumeLayout(false);
            this.grpAudio.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabDebug.ResumeLayout(false);
            this.grpVNC.ResumeLayout(false);
            this.grpVNC.PerformLayout();
            this.grpGDB.ResumeLayout(false);
            this.grpGDB.PerformLayout();
            this.grpSerial.ResumeLayout(false);
            this.grpParallel.ResumeLayout(false);
            this.groupBox16.ResumeLayout(false);
            this.tabPaths.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
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
            saveFileDialog1.Filter = "All files (*.*)|*.*";
            saveFileDialog1.FileName = "image." + cboImageFormat.Text;
            saveFileDialog1.DefaultExt = cboImageFormat.Text;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if (!qemu.CreateImage(saveFileDialog1.FileName, Convert.ToInt32(txtImageSize.Text), cboImageFormat.Text))
                {
                    frmError fError = new frmError();
                    fError.txtError.Text = qemu.GetLastError();
                    fError.ShowDialog(this);
                }
                else
                    MessageBox.Show("Image created"); 
            }
        }

        private void button11_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
        }

        private void button14_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
        }


        #region Harddisks
        private void btnBrowseHDA_Click(object sender, System.EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img";
            if (openFile.ShowDialog() == DialogResult.OK)
                txtHDA.Text = openFile.FileName;
        }

        private void btnBrowseHDB_Click(object sender, System.EventArgs e)
        {
            openFile.Filter = "Harddisk images |*.vmdk;*.cloop;*.cow;*.qcow;*.raw;*.img";
            if (openFile.ShowDialog() == DialogResult.OK)
                qemu.Harddisks.HDD[1].Path = openFile.FileName;
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
                chkUseCDROM.Enabled = false;
            }
            else
            {
                chkUseCDROM.Enabled = true;
            }

            if (chkUseCDROM.Checked == true)
            {
                chkUseHDC.Enabled = false;
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
                chkUseHDC.Enabled = true;
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

        private void btnBrowseSerial_Click(object sender, System.EventArgs e)
        {
            if (openFile.ShowDialog() == DialogResult.OK) 
                qemu.Debug.SerialPort.FileName = openFile.FileName; 
        }

         static string par_path;
        private void btnBrowseParallel_Click(object sender, System.EventArgs e)
        {
            if (openFile.ShowDialog() == DialogResult.OK) 
                par_path = openFile.FileName;
        }

        static string qemu_state; 
        private void button20_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            qemu_state = openFile.FileName;
        }
 
        private void btnLaunch_Click(object sender, System.EventArgs e)
        {
            SaveSettings();
            if (!qemu.Start((Platforms)cboMachine.SelectedIndex))
            {
                frmError fError = new frmError();
                fError.txtError.Text = qemu.GetLastError();
                fError.ShowDialog(this); 
            }

        }
        /*
        private string GetArgv()
        {

            // gdb
            if (checkBox14.Checked == true)
            {                
                arg = arg + "-s ";
            }

            // qemu state
            openFile.FileName = qemu_state;
            if (openFile.CheckFileExists == true)
            {
              arg = arg + "-loadvm "+qemu_state+" "; 
            }

        }

          */
        #region Debug
        private void chkSerialToFile_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseSerial.Enabled = chkSerialToFile.Checked;
        }

        private void chkParallelToFile_CheckedChanged(object sender, System.EventArgs e)
        {
            btnBrowseParallel.Enabled = chkParallelToFile.Checked;
        }

        private void chkVNC_CheckedChanged(object sender, System.EventArgs e)
        {
            textBox7.Enabled = chkVNC.Checked;
        }
        #endregion

        private void frmMain_Load(object sender, EventArgs e)
        {
            qemu = new QEmu();

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

            /* try to load config.xml from current directory */
            try
            {
                XmlSerializer s = new XmlSerializer(typeof(QEmu));
                TextReader r = new StreamReader(".//config.xml");
                qemu = (QEmu)s.Deserialize(r);
                r.Close();
                LoadSettings();
            }
            catch
            {
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

        #endregion

        #region Paths
        private void btnQEmuPath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                txtQEmuPath.Text = folderBrowserDialog1.SelectedPath;
                qemu.Paths.QEmu = txtQEmuPath.Text; 
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
            saveFileDialog1.FileName = "config.xml";
            saveFileDialog1.Filter = "QEmu GUI Config (*.xml)|*.xml"; 
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                XmlSerializer s = new XmlSerializer(typeof(QEmu));
                TextWriter w = new StreamWriter(saveFileDialog1.FileName);
                s.Serialize(w, qemu);
                w.Close();
            }
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            openFile.Filter = "QEmu GUI Config (*.xml)|*.xml"; 
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                XmlSerializer s = new XmlSerializer(typeof(QEmu));
                TextReader r = new StreamReader(openFile.FileName);
                qemu = (QEmu)s.Deserialize(r);
                r.Close();
                LoadSettings(); 
            }
        }

        private void LoadSettings()
        {
            /* Misc */
            cboMachine.SelectedIndex = (int) qemu.Misc.Machine;  
            numMemory.Value = qemu.Misc.Memory;
            numSMP.Value = qemu.Misc.CPUs;
            cboBootFrom.Text = qemu.Misc.BootFrom;
            chkSetClock.Checked = qemu.Misc.SetClock;
            chkVGAoutput.Checked = qemu.Misc.VGA;
            chkFullscreen.Checked = qemu.Misc.Fullscreen;
            chkKQEmu.Checked = qemu.Misc.KQEmu;

            /* CD-ROM */
            optHostCDROM.Checked = qemu.CDROM.UseFromHost;
            optCDImage.Checked = !qemu.CDROM.UseFromHost;
            txtCDROM.Text = qemu.CDROM.Image;
            cboCDROM.Text = qemu.CDROM.HostDrive;
            chkUseCDROM.Checked = qemu.CDROM.Enabled;

            /* Floppies */
            chkFloppyA.Checked = qemu.Floppies.FDD[0].Enabled;
            txtFloppyA.Text = qemu.Floppies.FDD[0].Path;
            chkFloppyB.Checked = qemu.Floppies.FDD[1].Enabled;
            txtFloppyB.Text = qemu.Floppies.FDD[1].Path;

            /* Paths */
            txtQEmuPath.Text = qemu.Paths.QEmu;
            txtVDKPath.Text = qemu.Paths.VDK;
            grpVDK.Enabled = (txtVDKPath.Text.Length > 0);

            /* Harddisks */
            chkUseHDA.Checked = qemu.Harddisks.HDD[0].Enabled;
            txtHDA.Text = qemu.Harddisks.HDD[0].Path;
            chkUseHDB.Checked = qemu.Harddisks.HDD[1].Enabled;
            txtHDB.Text = qemu.Harddisks.HDD[1].Path;
            chkUseHDC.Checked = qemu.Harddisks.HDD[2].Enabled;
            txtHDC.Text = qemu.Harddisks.HDD[2].Path;
            chkUseHDD.Checked = qemu.Harddisks.HDD[3].Enabled;
            txtHDD.Text = qemu.Harddisks.HDD[3].Path;

            /* Audio */
            chkES1370.Checked = qemu.Audio.ES1370;
            chkSoundBlaster.Checked = qemu.Audio.Soundblaster;
            chkPCSpeaker.Checked = qemu.Audio.Speaker;
            chkOPL2.Checked = qemu.Audio.OPL2;

            /* Debug */
            chkSerialToFile.Checked = qemu.Debug.SerialPort.Redirect;
            chkParallelToFile.Checked = qemu.Debug.ParallelPort.Redirect;
            chkVBE30.Checked = qemu.Debug.VBE3;
            txtGDBPort.Text = qemu.Debug.GDBPort.ToString();

            /* Tools */
            txtVDKImage.Text = qemu.Tools.vdk.Image;
            cboVDKDrive.Text = qemu.Tools.vdk.DriveLetter;
        }

        private void SaveSettings()
        {
            /* Misc */            
            qemu.Misc.Machine = (Platforms) cboMachine.SelectedIndex;
            qemu.Misc.Memory = (int) numMemory.Value;
            qemu.Misc.CPUs = (int) numSMP.Value;
            qemu.Misc.BootFrom = cboBootFrom.Text;
            qemu.Misc.SetClock = chkSetClock.Checked;
            qemu.Misc.VGA = chkVGAoutput.Checked;
            qemu.Misc.Fullscreen = chkFullscreen.Checked;
            qemu.Misc.KQEmu = chkKQEmu.Checked;    

            /* Paths */
            qemu.Paths.QEmu = txtQEmuPath.Text;
            qemu.Paths.VDK = txtVDKPath.Text;

            /* Floppies */
            qemu.Floppies.FDD[0].Enabled = chkFloppyA.Checked;
            qemu.Floppies.FDD[0].Path = txtFloppyA.Text;
            qemu.Floppies.FDD[1].Enabled = chkFloppyB.Checked;
            qemu.Floppies.FDD[1].Path = txtFloppyB.Text;

            /* CD-ROM */
            qemu.CDROM.UseFromHost = optHostCDROM.Checked;
            qemu.CDROM.Image = txtCDROM.Text;
            qemu.CDROM.HostDrive = cboCDROM.Text;
            qemu.CDROM.Enabled = chkUseCDROM.Checked;

            /* Harddisks */
            qemu.Harddisks.HDD[0].Enabled = chkUseHDA.Checked;
            qemu.Harddisks.HDD[0].Path = txtHDA.Text;
            qemu.Harddisks.HDD[1].Enabled = chkUseHDB.Checked;
            qemu.Harddisks.HDD[1].Path = txtHDB.Text;
            qemu.Harddisks.HDD[2].Enabled = chkUseHDC.Checked;
            qemu.Harddisks.HDD[2].Path = txtHDC.Text;
            qemu.Harddisks.HDD[3].Enabled = chkUseHDD.Checked;
            qemu.Harddisks.HDD[3].Path = txtHDD.Text;

            /* Audio */
            qemu.Audio.ES1370 = chkES1370.Checked;
            qemu.Audio.Soundblaster = chkSoundBlaster.Checked;
            qemu.Audio.Speaker = chkPCSpeaker.Checked;
            qemu.Audio.OPL2 = chkOPL2.Checked;

            /* Debug */
            qemu.Debug.SerialPort.Redirect = chkSerialToFile.Checked;
            qemu.Debug.ParallelPort.Redirect = chkParallelToFile.Checked;
            qemu.Debug.VBE3 = chkVBE30.Checked;
            qemu.Debug.GDBPort = Int32.Parse(txtGDBPort.Text);

            /* Tools */
            qemu.Tools.vdk.Image = txtVDKImage.Text;
            qemu.Tools.vdk.DriveLetter = cboVDKDrive.Text;
        }

        #endregion

        private void btnVDKImage_Click(object sender, EventArgs e)
        {
            openFile.Filter = "VMWare Images (*.vmdk)|*.vmdk";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                txtVDKImage.Text = openFile.FileName;  
            }
        }

        private void btnUnmount_Click(object sender, EventArgs e)
        {
            if (!qemu.UnmountImage())
            {
                frmError fError = new frmError();
                fError.txtError.Text = qemu.GetLastError();
                fError.ShowDialog(this);
            }
        }

        private void btnMount_Click(object sender, EventArgs e)
        {
            SaveSettings(); 
            if (!qemu.MountImage())
            {
                frmError fError = new frmError();
                fError.txtError.Text = qemu.GetLastError();
                fError.ShowDialog(this);
            }
        }


 

    }
}
