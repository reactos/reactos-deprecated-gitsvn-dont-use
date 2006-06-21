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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TabPage tabMisc;
        private System.Windows.Forms.TabControl HardDisk2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.GroupBox grpFloppy;
        private System.Windows.Forms.CheckBox chkFloppyA;
        private System.Windows.Forms.CheckBox chkFloppyB;
        private System.Windows.Forms.Button btnBrowseFloppyA;
        private System.Windows.Forms.Button btnBrowseFloppyB;
        private System.Windows.Forms.TabPage tabFloppy;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnBrowseHDB;
        private System.Windows.Forms.Button btnBrowseHDA;
        private System.Windows.Forms.CheckBox chkUseHDA;
        private System.Windows.Forms.Button btnBrowseHDD;
        private System.Windows.Forms.Button btnBrowseHDC;
        private System.Windows.Forms.CheckBox chkUseHDD;
        private System.Windows.Forms.CheckBox chkUseHDC;
        private System.Windows.Forms.CheckBox chkUseHDB;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button btnBrowseCDROM;
        private System.Windows.Forms.CheckBox chkUseCDROM;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TabPage HardDisk;
        private System.Windows.Forms.TabPage tabCDROM;
        private System.Windows.Forms.TabPage tabAudio;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.CheckBox checkBox7;
        private System.Windows.Forms.CheckBox checkBox9;
        private System.Windows.Forms.CheckBox checkBox11;
        private System.Windows.Forms.CheckBox checkBox12;
        private System.Windows.Forms.TabPage tabTools;
        private System.Windows.Forms.ComboBox cboBootFrom;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button btnLaunch;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.Button button12;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Button btnCreateImage;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox txtImageSize;
        private System.Windows.Forms.TabPage tabNetwork;
        private System.Windows.Forms.TabPage tabDebug;
        private System.Windows.Forms.TabPage tabAbout;
        private System.Windows.Forms.GroupBox groupBox10;
        private System.Windows.Forms.GroupBox groupBox11;
        private System.Windows.Forms.GroupBox groupBox12;
        private System.Windows.Forms.CheckBox checkBox10;
        private System.Windows.Forms.Button button18;
        private System.Windows.Forms.GroupBox groupBox13;
        private System.Windows.Forms.Button button19;
        private System.Windows.Forms.CheckBox checkBox13;
        private System.Windows.Forms.GroupBox groupBox14;
        private System.Windows.Forms.CheckBox checkBox14;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.GroupBox groupBox15;
        private System.Windows.Forms.CheckBox checkBox15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.GroupBox groupBox16;
        private System.Windows.Forms.Button button20;
        private System.Windows.Forms.CheckBox checkBox16;
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
        private TextBox textBox5;
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
        private GroupBox groupBox2;
        private Button btnQEmuPath;
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.HardDisk2 = new System.Windows.Forms.TabControl();
            this.tabMisc = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnQEmuPath = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.chkKQEmu = new System.Windows.Forms.CheckBox();
            this.lblCPUs = new System.Windows.Forms.Label();
            this.numSMP = new System.Windows.Forms.NumericUpDown();
            this.numMemory = new System.Windows.Forms.NumericUpDown();
            this.lblMB = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cboBootFrom = new System.Windows.Forms.ComboBox();
            this.groupBox10 = new System.Windows.Forms.GroupBox();
            this.chkSetClock = new System.Windows.Forms.CheckBox();
            this.groupBox11 = new System.Windows.Forms.GroupBox();
            this.chkVGAoutput = new System.Windows.Forms.CheckBox();
            this.chkFullscreen = new System.Windows.Forms.CheckBox();
            this.tabCDROM = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.txtCDROM = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.btnBrowseCDROM = new System.Windows.Forms.Button();
            this.chkUseCDROM = new System.Windows.Forms.CheckBox();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.tabFloppy = new System.Windows.Forms.TabPage();
            this.grpFloppy = new System.Windows.Forms.GroupBox();
            this.txtFloppyB = new System.Windows.Forms.TextBox();
            this.txtFloppyA = new System.Windows.Forms.TextBox();
            this.btnBrowseFloppyB = new System.Windows.Forms.Button();
            this.btnBrowseFloppyA = new System.Windows.Forms.Button();
            this.chkFloppyB = new System.Windows.Forms.CheckBox();
            this.chkFloppyA = new System.Windows.Forms.CheckBox();
            this.HardDisk = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnBrowseHDD = new System.Windows.Forms.Button();
            this.btnBrowseHDC = new System.Windows.Forms.Button();
            this.chkUseHDD = new System.Windows.Forms.CheckBox();
            this.chkUseHDC = new System.Windows.Forms.CheckBox();
            this.btnBrowseHDB = new System.Windows.Forms.Button();
            this.btnBrowseHDA = new System.Windows.Forms.Button();
            this.chkUseHDB = new System.Windows.Forms.CheckBox();
            this.chkUseHDA = new System.Windows.Forms.CheckBox();
            this.tabTools = new System.Windows.Forms.TabPage();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.label11 = new System.Windows.Forms.Label();
            this.txtImageSize = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.btnCreateImage = new System.Windows.Forms.Button();
            this.cboImageFormat = new System.Windows.Forms.ComboBox();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.button12 = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.button10 = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.button11 = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.tabAudio = new System.Windows.Forms.TabPage();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.checkBox9 = new System.Windows.Forms.CheckBox();
            this.checkBox7 = new System.Windows.Forms.CheckBox();
            this.checkBox11 = new System.Windows.Forms.CheckBox();
            this.checkBox12 = new System.Windows.Forms.CheckBox();
            this.tabDebug = new System.Windows.Forms.TabPage();
            this.groupBox15 = new System.Windows.Forms.GroupBox();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.checkBox15 = new System.Windows.Forms.CheckBox();
            this.groupBox14 = new System.Windows.Forms.GroupBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.button18 = new System.Windows.Forms.Button();
            this.checkBox10 = new System.Windows.Forms.CheckBox();
            this.groupBox13 = new System.Windows.Forms.GroupBox();
            this.button19 = new System.Windows.Forms.Button();
            this.checkBox13 = new System.Windows.Forms.CheckBox();
            this.groupBox16 = new System.Windows.Forms.GroupBox();
            this.button20 = new System.Windows.Forms.Button();
            this.checkBox16 = new System.Windows.Forms.CheckBox();
            this.tabAbout = new System.Windows.Forms.TabPage();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.tabNetwork = new System.Windows.Forms.TabPage();
            this.btnLaunch = new System.Windows.Forms.Button();
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.HardDisk2.SuspendLayout();
            this.tabMisc.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).BeginInit();
            this.groupBox10.SuspendLayout();
            this.groupBox11.SuspendLayout();
            this.tabCDROM.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.tabFloppy.SuspendLayout();
            this.grpFloppy.SuspendLayout();
            this.HardDisk.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tabTools.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.tabAudio.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabDebug.SuspendLayout();
            this.groupBox15.SuspendLayout();
            this.groupBox14.SuspendLayout();
            this.groupBox12.SuspendLayout();
            this.groupBox13.SuspendLayout();
            this.groupBox16.SuspendLayout();
            this.tabAbout.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButton2);
            this.groupBox1.Controls.Add(this.radioButton1);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(274, 48);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Machine";
            // 
            // radioButton2
            // 
            this.radioButton2.CausesValidation = false;
            this.radioButton2.Location = new System.Drawing.Point(154, 16);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(98, 24);
            this.radioButton2.TabIndex = 5;
            this.radioButton2.Text = "ISA only PC";
            // 
            // radioButton1
            // 
            this.radioButton1.CausesValidation = false;
            this.radioButton1.Checked = true;
            this.radioButton1.Location = new System.Drawing.Point(19, 16);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(104, 24);
            this.radioButton1.TabIndex = 4;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "Standard PC";
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
            this.HardDisk2.Controls.Add(this.tabAbout);
            this.HardDisk2.Location = new System.Drawing.Point(12, 10);
            this.HardDisk2.Name = "HardDisk2";
            this.HardDisk2.SelectedIndex = 0;
            this.HardDisk2.Size = new System.Drawing.Size(468, 227);
            this.HardDisk2.TabIndex = 3;
            // 
            // tabMisc
            // 
            this.tabMisc.Controls.Add(this.groupBox2);
            this.tabMisc.Controls.Add(this.groupBox6);
            this.tabMisc.Controls.Add(this.groupBox1);
            this.tabMisc.Controls.Add(this.groupBox10);
            this.tabMisc.Controls.Add(this.groupBox11);
            this.tabMisc.Location = new System.Drawing.Point(4, 22);
            this.tabMisc.Name = "tabMisc";
            this.tabMisc.Size = new System.Drawing.Size(460, 201);
            this.tabMisc.TabIndex = 1;
            this.tabMisc.Text = "Misc";
            this.tabMisc.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnQEmuPath);
            this.groupBox2.Location = new System.Drawing.Point(283, 137);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(173, 61);
            this.groupBox2.TabIndex = 20;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "QEmu";
            // 
            // btnQEmuPath
            // 
            this.btnQEmuPath.Location = new System.Drawing.Point(29, 23);
            this.btnQEmuPath.Name = "btnQEmuPath";
            this.btnQEmuPath.Size = new System.Drawing.Size(118, 21);
            this.btnQEmuPath.TabIndex = 0;
            this.btnQEmuPath.Text = "Path to executable";
            this.btnQEmuPath.UseVisualStyleBackColor = true;
            this.btnQEmuPath.Click += new System.EventHandler(this.btnQEmuPath_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.chkKQEmu);
            this.groupBox6.Controls.Add(this.lblCPUs);
            this.groupBox6.Controls.Add(this.numSMP);
            this.groupBox6.Controls.Add(this.numMemory);
            this.groupBox6.Controls.Add(this.lblMB);
            this.groupBox6.Controls.Add(this.label3);
            this.groupBox6.Controls.Add(this.label1);
            this.groupBox6.Controls.Add(this.label2);
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
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(16, 51);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 23);
            this.label3.TabIndex = 6;
            this.label3.Text = "Memory";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(16, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 23);
            this.label1.TabIndex = 2;
            this.label1.Text = "Boot from";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(16, 76);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 23);
            this.label2.TabIndex = 3;
            this.label2.Text = "SMP (1 to 255)";
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
            // groupBox10
            // 
            this.groupBox10.Controls.Add(this.chkSetClock);
            this.groupBox10.Location = new System.Drawing.Point(283, 3);
            this.groupBox10.Name = "groupBox10";
            this.groupBox10.Size = new System.Drawing.Size(174, 48);
            this.groupBox10.TabIndex = 18;
            this.groupBox10.TabStop = false;
            this.groupBox10.Text = "Clock";
            // 
            // chkSetClock
            // 
            this.chkSetClock.AutoSize = true;
            this.chkSetClock.Location = new System.Drawing.Point(16, 21);
            this.chkSetClock.Name = "chkSetClock";
            this.chkSetClock.Size = new System.Drawing.Size(71, 17);
            this.chkSetClock.TabIndex = 12;
            this.chkSetClock.Text = "Set clock";
            this.chkSetClock.UseVisualStyleBackColor = true;
            // 
            // groupBox11
            // 
            this.groupBox11.Controls.Add(this.chkVGAoutput);
            this.groupBox11.Controls.Add(this.chkFullscreen);
            this.groupBox11.Location = new System.Drawing.Point(283, 57);
            this.groupBox11.Name = "groupBox11";
            this.groupBox11.Size = new System.Drawing.Size(174, 74);
            this.groupBox11.TabIndex = 19;
            this.groupBox11.TabStop = false;
            this.groupBox11.Text = "Display";
            // 
            // chkVGAoutput
            // 
            this.chkVGAoutput.AutoSize = true;
            this.chkVGAoutput.Location = new System.Drawing.Point(16, 51);
            this.chkVGAoutput.Name = "chkVGAoutput";
            this.chkVGAoutput.Size = new System.Drawing.Size(118, 17);
            this.chkVGAoutput.TabIndex = 20;
            this.chkVGAoutput.Text = "Display VGA output";
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
            this.tabCDROM.Controls.Add(this.groupBox4);
            this.tabCDROM.Location = new System.Drawing.Point(4, 22);
            this.tabCDROM.Name = "tabCDROM";
            this.tabCDROM.Size = new System.Drawing.Size(460, 201);
            this.tabCDROM.TabIndex = 4;
            this.tabCDROM.Text = "CD-ROM";
            this.tabCDROM.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.txtCDROM);
            this.groupBox4.Controls.Add(this.textBox1);
            this.groupBox4.Controls.Add(this.radioButton3);
            this.groupBox4.Controls.Add(this.btnBrowseCDROM);
            this.groupBox4.Controls.Add(this.chkUseCDROM);
            this.groupBox4.Controls.Add(this.radioButton4);
            this.groupBox4.Location = new System.Drawing.Point(3, 3);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(454, 195);
            this.groupBox4.TabIndex = 5;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "CD-ROM";
            // 
            // txtCDROM
            // 
            this.txtCDROM.Location = new System.Drawing.Point(61, 134);
            this.txtCDROM.Name = "txtCDROM";
            this.txtCDROM.Size = new System.Drawing.Size(338, 20);
            this.txtCDROM.TabIndex = 6;
            // 
            // textBox1
            // 
            this.textBox1.Enabled = false;
            this.textBox1.Location = new System.Drawing.Point(61, 78);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(152, 20);
            this.textBox1.TabIndex = 5;
            this.textBox1.Text = "HOST_CDROM_Letter_Name";
            // 
            // radioButton3
            // 
            this.radioButton3.Checked = true;
            this.radioButton3.Enabled = false;
            this.radioButton3.Location = new System.Drawing.Point(43, 54);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(104, 24);
            this.radioButton3.TabIndex = 3;
            this.radioButton3.TabStop = true;
            this.radioButton3.Text = "Host CD-ROM";
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
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
            // radioButton4
            // 
            this.radioButton4.Enabled = false;
            this.radioButton4.Location = new System.Drawing.Point(43, 104);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(144, 24);
            this.radioButton4.TabIndex = 4;
            this.radioButton4.Text = "Image File";
            this.radioButton4.CheckedChanged += new System.EventHandler(this.radioButton4_CheckedChanged);
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
            this.txtFloppyB.Location = new System.Drawing.Point(32, 113);
            this.txtFloppyB.Name = "txtFloppyB";
            this.txtFloppyB.Size = new System.Drawing.Size(369, 20);
            this.txtFloppyB.TabIndex = 5;
            // 
            // txtFloppyA
            // 
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
            this.HardDisk.Controls.Add(this.groupBox3);
            this.HardDisk.Location = new System.Drawing.Point(4, 22);
            this.HardDisk.Name = "HardDisk";
            this.HardDisk.Size = new System.Drawing.Size(460, 201);
            this.HardDisk.TabIndex = 3;
            this.HardDisk.Text = " Harddisk";
            this.HardDisk.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnBrowseHDD);
            this.groupBox3.Controls.Add(this.btnBrowseHDC);
            this.groupBox3.Controls.Add(this.chkUseHDD);
            this.groupBox3.Controls.Add(this.chkUseHDC);
            this.groupBox3.Controls.Add(this.btnBrowseHDB);
            this.groupBox3.Controls.Add(this.btnBrowseHDA);
            this.groupBox3.Controls.Add(this.chkUseHDB);
            this.groupBox3.Controls.Add(this.chkUseHDA);
            this.groupBox3.Location = new System.Drawing.Point(136, 40);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(232, 152);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = " Harddisk";
            // 
            // btnBrowseHDD
            // 
            this.btnBrowseHDD.Enabled = false;
            this.btnBrowseHDD.Location = new System.Drawing.Point(120, 112);
            this.btnBrowseHDD.Name = "btnBrowseHDD";
            this.btnBrowseHDD.Size = new System.Drawing.Size(104, 23);
            this.btnBrowseHDD.TabIndex = 11;
            this.btnBrowseHDD.Text = " Browse Disk D";
            this.btnBrowseHDD.Click += new System.EventHandler(this.btnBrowseHDD_Click);
            // 
            // btnBrowseHDC
            // 
            this.btnBrowseHDC.Enabled = false;
            this.btnBrowseHDC.Location = new System.Drawing.Point(120, 48);
            this.btnBrowseHDC.Name = "btnBrowseHDC";
            this.btnBrowseHDC.Size = new System.Drawing.Size(104, 23);
            this.btnBrowseHDC.TabIndex = 10;
            this.btnBrowseHDC.Text = " Browse Disk C";
            this.btnBrowseHDC.Click += new System.EventHandler(this.btnBrowseHDC_Click);
            // 
            // chkUseHDD
            // 
            this.chkUseHDD.Location = new System.Drawing.Point(120, 88);
            this.chkUseHDD.Name = "chkUseHDD";
            this.chkUseHDD.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDD.TabIndex = 9;
            this.chkUseHDD.Text = "HDD";
            this.chkUseHDD.CheckedChanged += new System.EventHandler(this.checkBox5_CheckedChanged);
            // 
            // chkUseHDC
            // 
            this.chkUseHDC.Location = new System.Drawing.Point(120, 24);
            this.chkUseHDC.Name = "chkUseHDC";
            this.chkUseHDC.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDC.TabIndex = 8;
            this.chkUseHDC.Text = "HDC";
            this.chkUseHDC.CheckedChanged += new System.EventHandler(this.checkBox6_CheckedChanged);
            // 
            // btnBrowseHDB
            // 
            this.btnBrowseHDB.Enabled = false;
            this.btnBrowseHDB.Location = new System.Drawing.Point(8, 112);
            this.btnBrowseHDB.Name = "btnBrowseHDB";
            this.btnBrowseHDB.Size = new System.Drawing.Size(104, 23);
            this.btnBrowseHDB.TabIndex = 7;
            this.btnBrowseHDB.Text = " Browse Disk B";
            this.btnBrowseHDB.Click += new System.EventHandler(this.btnBrowseHDB_Click);
            // 
            // btnBrowseHDA
            // 
            this.btnBrowseHDA.Enabled = false;
            this.btnBrowseHDA.Location = new System.Drawing.Point(8, 48);
            this.btnBrowseHDA.Name = "btnBrowseHDA";
            this.btnBrowseHDA.Size = new System.Drawing.Size(104, 23);
            this.btnBrowseHDA.TabIndex = 6;
            this.btnBrowseHDA.Text = " Browse Disk A";
            this.btnBrowseHDA.Click += new System.EventHandler(this.btnBrowseHDA_Click);
            // 
            // chkUseHDB
            // 
            this.chkUseHDB.Location = new System.Drawing.Point(8, 88);
            this.chkUseHDB.Name = "chkUseHDB";
            this.chkUseHDB.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDB.TabIndex = 5;
            this.chkUseHDB.Text = "HDB";
            this.chkUseHDB.CheckedChanged += new System.EventHandler(this.checkBox3_CheckedChanged);
            // 
            // chkUseHDA
            // 
            this.chkUseHDA.Location = new System.Drawing.Point(8, 24);
            this.chkUseHDA.Name = "chkUseHDA";
            this.chkUseHDA.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDA.TabIndex = 4;
            this.chkUseHDA.Text = "HDA";
            this.chkUseHDA.CheckedChanged += new System.EventHandler(this.checkBox4_CheckedChanged);
            // 
            // tabTools
            // 
            this.tabTools.Controls.Add(this.groupBox9);
            this.tabTools.Controls.Add(this.groupBox8);
            this.tabTools.Location = new System.Drawing.Point(4, 22);
            this.tabTools.Name = "tabTools";
            this.tabTools.Size = new System.Drawing.Size(460, 201);
            this.tabTools.TabIndex = 6;
            this.tabTools.Text = "Tools";
            this.tabTools.UseVisualStyleBackColor = true;
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.label11);
            this.groupBox9.Controls.Add(this.txtImageSize);
            this.groupBox9.Controls.Add(this.label12);
            this.groupBox9.Controls.Add(this.btnCreateImage);
            this.groupBox9.Controls.Add(this.cboImageFormat);
            this.groupBox9.Location = new System.Drawing.Point(264, 16);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(160, 184);
            this.groupBox9.TabIndex = 6;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Harddisk Tools";
            // 
            // label11
            // 
            this.label11.Location = new System.Drawing.Point(96, 32);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(48, 23);
            this.label11.TabIndex = 5;
            this.label11.Text = "Format";
            // 
            // txtImageSize
            // 
            this.txtImageSize.Location = new System.Drawing.Point(16, 64);
            this.txtImageSize.Name = "txtImageSize";
            this.txtImageSize.Size = new System.Drawing.Size(64, 20);
            this.txtImageSize.TabIndex = 3;
            this.txtImageSize.Text = "512";
            // 
            // label12
            // 
            this.label12.Location = new System.Drawing.Point(16, 32);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(64, 24);
            this.label12.TabIndex = 2;
            this.label12.Text = "Image Size";
            // 
            // btnCreateImage
            // 
            this.btnCreateImage.Location = new System.Drawing.Point(16, 112);
            this.btnCreateImage.Name = "btnCreateImage";
            this.btnCreateImage.Size = new System.Drawing.Size(128, 48);
            this.btnCreateImage.TabIndex = 1;
            this.btnCreateImage.Text = "Browse and Create";
            this.btnCreateImage.Click += new System.EventHandler(this.btnCreateImage_Click);
            // 
            // cboImageFormat
            // 
            this.cboImageFormat.Items.AddRange(new object[] {
            "cloop",
            "cow ",
            "qcow",
            "raw ",
            "vmdk ",
            "",
            " "});
            this.cboImageFormat.Location = new System.Drawing.Point(96, 64);
            this.cboImageFormat.Name = "cboImageFormat";
            this.cboImageFormat.Size = new System.Drawing.Size(56, 21);
            this.cboImageFormat.TabIndex = 7;
            this.cboImageFormat.Text = "vmdk ";
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.button12);
            this.groupBox8.Controls.Add(this.label8);
            this.groupBox8.Controls.Add(this.button10);
            this.groupBox8.Controls.Add(this.label9);
            this.groupBox8.Controls.Add(this.button11);
            this.groupBox8.Controls.Add(this.label10);
            this.groupBox8.Location = new System.Drawing.Point(56, 16);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(144, 184);
            this.groupBox8.TabIndex = 5;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "VDK Tool";
            // 
            // button12
            // 
            this.button12.Enabled = false;
            this.button12.Location = new System.Drawing.Point(16, 152);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(112, 23);
            this.button12.TabIndex = 5;
            this.button12.Text = "UnMount";
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(16, 24);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(100, 23);
            this.label8.TabIndex = 0;
            this.label8.Text = "Path to VDK";
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(16, 48);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(112, 23);
            this.button10.TabIndex = 1;
            this.button10.Text = " Browse";
            this.button10.Click += new System.EventHandler(this.button10_Click);
            // 
            // label9
            // 
            this.label9.Location = new System.Drawing.Point(16, 80);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(112, 23);
            this.label9.TabIndex = 2;
            this.label9.Text = "Mount a HdX Image";
            // 
            // button11
            // 
            this.button11.Enabled = false;
            this.button11.Location = new System.Drawing.Point(16, 104);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(112, 23);
            this.button11.TabIndex = 3;
            this.button11.Text = "Mount";
            this.button11.Click += new System.EventHandler(this.button11_Click);
            // 
            // label10
            // 
            this.label10.Location = new System.Drawing.Point(24, 136);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(112, 23);
            this.label10.TabIndex = 4;
            this.label10.Text = "UnMount HdX Image";
            // 
            // tabAudio
            // 
            this.tabAudio.Controls.Add(this.groupBox5);
            this.tabAudio.Location = new System.Drawing.Point(4, 22);
            this.tabAudio.Name = "tabAudio";
            this.tabAudio.Size = new System.Drawing.Size(460, 201);
            this.tabAudio.TabIndex = 5;
            this.tabAudio.Text = "Audio";
            this.tabAudio.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.pictureBox4);
            this.groupBox5.Controls.Add(this.pictureBox3);
            this.groupBox5.Controls.Add(this.pictureBox2);
            this.groupBox5.Controls.Add(this.pictureBox1);
            this.groupBox5.Controls.Add(this.checkBox9);
            this.groupBox5.Controls.Add(this.checkBox7);
            this.groupBox5.Controls.Add(this.checkBox11);
            this.groupBox5.Controls.Add(this.checkBox12);
            this.groupBox5.Location = new System.Drawing.Point(3, 3);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(454, 195);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Emulate Audio Cards";
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
            // checkBox9
            // 
            this.checkBox9.Checked = true;
            this.checkBox9.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox9.Location = new System.Drawing.Point(159, 70);
            this.checkBox9.Name = "checkBox9";
            this.checkBox9.Size = new System.Drawing.Size(176, 32);
            this.checkBox9.TabIndex = 1;
            this.checkBox9.Text = "Creative Sound Blaster 16";
            // 
            // checkBox7
            // 
            this.checkBox7.Checked = true;
            this.checkBox7.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox7.Location = new System.Drawing.Point(159, 40);
            this.checkBox7.Name = "checkBox7";
            this.checkBox7.Size = new System.Drawing.Size(168, 24);
            this.checkBox7.TabIndex = 0;
            this.checkBox7.Text = "PC Speaker";
            // 
            // checkBox11
            // 
            this.checkBox11.Location = new System.Drawing.Point(159, 144);
            this.checkBox11.Name = "checkBox11";
            this.checkBox11.Size = new System.Drawing.Size(176, 24);
            this.checkBox11.TabIndex = 2;
            this.checkBox11.Text = "ENSONIQ AudioPCI ES1370";
            // 
            // checkBox12
            // 
            this.checkBox12.Location = new System.Drawing.Point(157, 108);
            this.checkBox12.Name = "checkBox12";
            this.checkBox12.Size = new System.Drawing.Size(176, 24);
            this.checkBox12.TabIndex = 2;
            this.checkBox12.Text = " Yamaha YM3812 (OPL2)";
            // 
            // tabDebug
            // 
            this.tabDebug.Controls.Add(this.groupBox15);
            this.tabDebug.Controls.Add(this.groupBox14);
            this.tabDebug.Controls.Add(this.groupBox12);
            this.tabDebug.Controls.Add(this.groupBox13);
            this.tabDebug.Controls.Add(this.groupBox16);
            this.tabDebug.Controls.Add(this.checkBox16);
            this.tabDebug.Location = new System.Drawing.Point(4, 22);
            this.tabDebug.Name = "tabDebug";
            this.tabDebug.Size = new System.Drawing.Size(460, 201);
            this.tabDebug.TabIndex = 8;
            this.tabDebug.Text = "Debug";
            this.tabDebug.UseVisualStyleBackColor = true;
            // 
            // groupBox15
            // 
            this.groupBox15.Controls.Add(this.textBox7);
            this.groupBox15.Controls.Add(this.label14);
            this.groupBox15.Controls.Add(this.checkBox15);
            this.groupBox15.Location = new System.Drawing.Point(336, 88);
            this.groupBox15.Name = "groupBox15";
            this.groupBox15.Size = new System.Drawing.Size(112, 88);
            this.groupBox15.TabIndex = 3;
            this.groupBox15.TabStop = false;
            this.groupBox15.Text = "VNC Server";
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
            // checkBox15
            // 
            this.checkBox15.Location = new System.Drawing.Point(8, 16);
            this.checkBox15.Name = "checkBox15";
            this.checkBox15.Size = new System.Drawing.Size(96, 24);
            this.checkBox15.TabIndex = 0;
            this.checkBox15.Text = "Activate VNC";
            this.checkBox15.CheckedChanged += new System.EventHandler(this.checkBox15_CheckedChanged);
            // 
            // groupBox14
            // 
            this.groupBox14.Controls.Add(this.label13);
            this.groupBox14.Controls.Add(this.textBox6);
            this.groupBox14.Controls.Add(this.checkBox14);
            this.groupBox14.Location = new System.Drawing.Point(160, 8);
            this.groupBox14.Name = "groupBox14";
            this.groupBox14.Size = new System.Drawing.Size(160, 168);
            this.groupBox14.TabIndex = 2;
            this.groupBox14.TabStop = false;
            this.groupBox14.Text = "GDB";
            // 
            // label13
            // 
            this.label13.Location = new System.Drawing.Point(8, 96);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(144, 23);
            this.label13.TabIndex = 2;
            this.label13.Text = "Change connection port.";
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(8, 128);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(144, 20);
            this.textBox6.TabIndex = 1;
            this.textBox6.Text = "1234";
            // 
            // checkBox14
            // 
            this.checkBox14.Location = new System.Drawing.Point(8, 32);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(144, 24);
            this.checkBox14.TabIndex = 0;
            this.checkBox14.Text = "Wait connection to port";
            // 
            // groupBox12
            // 
            this.groupBox12.Controls.Add(this.button18);
            this.groupBox12.Controls.Add(this.checkBox10);
            this.groupBox12.Location = new System.Drawing.Point(32, 8);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(120, 80);
            this.groupBox12.TabIndex = 1;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Serial Port";
            // 
            // button18
            // 
            this.button18.Enabled = false;
            this.button18.Location = new System.Drawing.Point(8, 48);
            this.button18.Name = "button18";
            this.button18.Size = new System.Drawing.Size(96, 23);
            this.button18.TabIndex = 3;
            this.button18.Text = "Browse";
            this.button18.Click += new System.EventHandler(this.button18_Click);
            // 
            // checkBox10
            // 
            this.checkBox10.Location = new System.Drawing.Point(8, 16);
            this.checkBox10.Name = "checkBox10";
            this.checkBox10.Size = new System.Drawing.Size(104, 24);
            this.checkBox10.TabIndex = 2;
            this.checkBox10.Text = "Redirect to file";
            this.checkBox10.CheckedChanged += new System.EventHandler(this.checkBox10_CheckedChanged);
            // 
            // groupBox13
            // 
            this.groupBox13.Controls.Add(this.button19);
            this.groupBox13.Controls.Add(this.checkBox13);
            this.groupBox13.Location = new System.Drawing.Point(32, 96);
            this.groupBox13.Name = "groupBox13";
            this.groupBox13.Size = new System.Drawing.Size(120, 80);
            this.groupBox13.TabIndex = 1;
            this.groupBox13.TabStop = false;
            this.groupBox13.Text = "Parallel port";
            // 
            // button19
            // 
            this.button19.Enabled = false;
            this.button19.Location = new System.Drawing.Point(8, 48);
            this.button19.Name = "button19";
            this.button19.Size = new System.Drawing.Size(96, 23);
            this.button19.TabIndex = 3;
            this.button19.Text = "Browse";
            this.button19.Click += new System.EventHandler(this.button19_Click);
            // 
            // checkBox13
            // 
            this.checkBox13.Location = new System.Drawing.Point(8, 16);
            this.checkBox13.Name = "checkBox13";
            this.checkBox13.Size = new System.Drawing.Size(104, 24);
            this.checkBox13.TabIndex = 2;
            this.checkBox13.Text = "Redirect to File";
            this.checkBox13.CheckedChanged += new System.EventHandler(this.checkBox13_CheckedChanged);
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
            // checkBox16
            // 
            this.checkBox16.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.checkBox16.Location = new System.Drawing.Point(32, 169);
            this.checkBox16.Name = "checkBox16";
            this.checkBox16.Size = new System.Drawing.Size(416, 32);
            this.checkBox16.TabIndex = 2;
            this.checkBox16.Text = "Simulate a standard VGA card with Bochs VBE 3.0 extensions ";
            // 
            // tabAbout
            // 
            this.tabAbout.Controls.Add(this.textBox5);
            this.tabAbout.Location = new System.Drawing.Point(4, 22);
            this.tabAbout.Name = "tabAbout";
            this.tabAbout.Size = new System.Drawing.Size(460, 201);
            this.tabAbout.TabIndex = 9;
            this.tabAbout.Text = "About";
            this.tabAbout.UseVisualStyleBackColor = true;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(8, 8);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(464, 186);
            this.textBox5.TabIndex = 0;
            this.textBox5.Text = resources.GetString("textBox5.Text");
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
            this.Text = "Qemu GUI Lancher Version 1.0 written by Magnus Olsen for ReactOS";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.groupBox1.ResumeLayout(false);
            this.HardDisk2.ResumeLayout(false);
            this.tabMisc.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).EndInit();
            this.groupBox10.ResumeLayout(false);
            this.groupBox10.PerformLayout();
            this.groupBox11.ResumeLayout(false);
            this.groupBox11.PerformLayout();
            this.tabCDROM.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.tabFloppy.ResumeLayout(false);
            this.grpFloppy.ResumeLayout(false);
            this.grpFloppy.PerformLayout();
            this.HardDisk.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.tabTools.ResumeLayout(false);
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.tabAudio.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabDebug.ResumeLayout(false);
            this.groupBox15.ResumeLayout(false);
            this.groupBox15.PerformLayout();
            this.groupBox14.ResumeLayout(false);
            this.groupBox14.PerformLayout();
            this.groupBox12.ResumeLayout(false);
            this.groupBox13.ResumeLayout(false);
            this.groupBox16.ResumeLayout(false);
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

        static string vdk_path;
        private void button10_Click(object sender, System.EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
             vdk_path = folderBrowserDialog1.SelectedPath;
            if (vdk_path != "")
            {
                button11.Enabled = true;
                button12.Enabled = true;
            }
            else
            {
                button11.Enabled = false;
                button12.Enabled = false;
            }

        }

        private void btnCreateImage_Click(object sender, System.EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            if (saveFileDialog1.CheckPathExists)
            {
                qemu.CreateImage(saveFileDialog1.FileName, Convert.ToInt32(txtImageSize.Text), cboImageFormat.Text); 
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
            if (openFile.ShowDialog() == DialogResult.OK)
                qemu.Harddisks[0].Path = openFile.FileName;
        }

        private void btnBrowseHDB_Click(object sender, System.EventArgs e)
        {
            if (openFile.ShowDialog() == DialogResult.OK)
                qemu.Harddisks[1].Path = openFile.FileName;
        }

        private void btnBrowseHDC_Click(object sender, System.EventArgs e)
        {
            if (openFile.ShowDialog() == DialogResult.OK)
                qemu.Harddisks[2].Path = openFile.FileName;
        }

        private void btnBrowseHDD_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            qemu.Harddisks[3].Path = openFile.FileName;
        }
        #endregion

        #region CDROM
        private void btnBrowseCDROM_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            txtCDROM.Text = openFile.FileName;
        }
        #endregion

        static string serial_path;
        private void button18_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            serial_path = openFile.FileName;
        }

         static string par_path;
        private void button19_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            par_path = openFile.FileName;
        }

        static string qemu_state; 
        private void button20_Click(object sender, System.EventArgs e)
        {
            openFile.ShowDialog();
            qemu_state = openFile.FileName;
        }
           
        private void checkBox4_CheckedChanged(object sender, System.EventArgs e)
        {
            if (chkUseHDA.Checked == true)
                btnBrowseHDA.Enabled=true;
            else
                btnBrowseHDA.Enabled=false;

        }

        private void checkBox6_CheckedChanged(object sender, System.EventArgs e)
        {
            if (chkUseHDC.Checked == true)
            {
                radioButton3.Enabled = false;
                textBox1.Enabled = false;
                radioButton4.Enabled = false;
                btnBrowseCDROM.Enabled = false;
                chkUseCDROM.Enabled = false;
            }
            else
            {             
                chkUseCDROM.Enabled = true;
            }

            if (chkUseHDC.Checked == true)
                btnBrowseHDC.Enabled=true;
            else
                btnBrowseHDC.Enabled=false;
        }

        private void checkBox3_CheckedChanged(object sender, System.EventArgs e)
        {
            if (chkUseHDB.Checked == true)
                btnBrowseHDB.Enabled=true;
            else
                btnBrowseHDB.Enabled=false;
        }

        private void checkBox5_CheckedChanged(object sender, System.EventArgs e)
        {
            if (chkUseHDD.Checked == true)
                btnBrowseHDD.Enabled=true;
            else
                btnBrowseHDD.Enabled=false;
        }

        private void chkUseCDROM_CheckedChanged(object sender, System.EventArgs e)
        {
           
            if (chkUseHDC.Checked == true)
            {
                radioButton3.Enabled = false;
                textBox1.Enabled = false;
                radioButton4.Enabled = false;
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

                radioButton3.Enabled = true;                
                radioButton4.Enabled = true;
                if (radioButton4.Checked == true)
                {
                    textBox1.Enabled = false;
                    btnBrowseCDROM.Enabled = true;
                }
                else
                {
                    textBox1.Enabled = true;
                    btnBrowseCDROM.Enabled = false;
                }
            }
            else
            {
                radioButton3.Enabled = false;
                textBox1.Enabled = false;
                radioButton4.Enabled = false;
                btnBrowseCDROM.Enabled = false;
                chkUseHDC.Enabled = true;
            }
        }

        private void radioButton3_CheckedChanged(object sender, System.EventArgs e)
        {
            if (radioButton4.Checked == true)
            {
                textBox1.Enabled = false;
                btnBrowseCDROM.Enabled = true;
            }
            else
            {
                textBox1.Enabled = true;
                btnBrowseCDROM.Enabled = false;
            }
        }

        private void radioButton4_CheckedChanged(object sender, System.EventArgs e)
        {
            if (radioButton4.Checked == true)
            {
                textBox1.Enabled = false;
                btnBrowseCDROM.Enabled = true;
            }
            else
            {
                textBox1.Enabled = true;
                btnBrowseCDROM.Enabled = false;
            }
        }

        private void btnLaunch_Click(object sender, System.EventArgs e)
        {
            qemu.Start();  
        }

        private string GetArgv()
        {
            string arg = "-L " + qemu.Path + " ";
            bool audio_on = false;
            bool hdd_on = false;

            /// Machine settings
            if (radioButton1.Checked == false)
                arg = arg + "-M isapc ";
            else
                arg = arg + "-M pc ";

            /// Floppy settings
            if (chkFloppyA.Checked == true)
            {
                if (txtFloppyA.Text != "" && txtFloppyA.Text != null)
                {
                    arg = arg + "-fda " + txtFloppyA.Text + " ";
                }
            }
            if (chkFloppyB.Checked == true)
            {
                if (txtFloppyB.Text != "" && txtFloppyB.Text != null)
                {
                    arg = arg + "-fdb " + txtFloppyB.Text + " ";
                }
            }

            /// Harddisk settings
            if (chkUseHDA.Checked == true)
            {
                if (qemu.Harddisks[0].Path != "")
                {                    
                    arg = arg + "-hda "+qemu.Harddisks[0].Path+" ";
                }
            }
            if (chkUseHDB.Checked == true)
            {
                if (qemu.Harddisks[1].Path != "")
                {
                    arg = arg + "-hdb " + qemu.Harddisks[1].Path + " ";
                }
            }
            if (chkUseHDC.Checked == true)
            {
                if (qemu.Harddisks[2].Path != "")
                {
                    hdd_on = true;
                    arg = arg + "-hdc " + qemu.Harddisks[2].Path + " ";
                }
            }
            if (chkUseHDD.Checked == true)
            {
                if (qemu.Harddisks[3].Path != "")
                {
                    arg = arg + "-hdd " + qemu.Harddisks[3].Path + " ";
                }
            }
            /// cdrom
            if ((chkUseCDROM.Checked == true) && (hdd_on == false))
            {
                if (radioButton3.Checked == true)
                {
                    arg = arg + "-cdrom " + textBox1.Text +" "; 
                }
                else if (txtCDROM.Text != "" && txtCDROM.Text != null)
                {
                    arg = arg + "-cdrom " + txtCDROM.Text + " ";
                }
            }
            
            /// boot options
            if (cboBootFrom.Text == "Floppy")
            {
                arg = arg + "-boot a ";
            }            
            else if (cboBootFrom.Text == "HardDisk")
            {
                arg = arg + "-boot c ";
            }
           
            else if (cboBootFrom.Text == "CDRom")
            {
                arg = arg + "-boot d ";
            }

            /// memmory setting
            arg = arg + "-m " + numMemory.Value.ToString() + " ";

            // smp setting
            arg = arg + "-smp " + numSMP.Value.ToString() + " ";
            
            // no vga output
            if (chkVGAoutput.Checked == false)
            {
                arg = arg + "-nographic "; 
            }

            // set clock
            if (chkSetClock.Checked)
            {
                arg = arg + "-localtime "; 
            }

            // fullscreen
            if (chkFullscreen.Checked)
            {
                arg = arg + "-full-screen "; 
            }

            if (!chkKQEmu.Checked)
            {
                arg = arg + "-no-kqemu "; 
            }

            /// Audio setting
            if (checkBox7.Checked == true)
            {
               audio_on = true;
               arg = arg + "-soundhw pcspk"; 
            }

            if (checkBox9.Checked == true)
            {
                if (audio_on == false)
                {
                    arg = arg + "-soundhw sb16"; 
                    audio_on = true;
                }
                else
                {
                  arg = arg + ",sb16"; 
                }
            }

            if (checkBox12.Checked == true)
            {
                if (audio_on == false)
                {
                    arg = arg + "-soundhw adlib"; 
                    audio_on = true;
                }
                else
                {
                    arg = arg + ",adlib"; 
                }
            }

            if (checkBox11.Checked == true)
            {
                if (audio_on == false)
                {
                    arg = arg + "-soundhw es1370"; 
                    audio_on = true;
                }
                else
                {
                    arg = arg + ",es1370"; 
                }
            }

            if (audio_on == true)
            {
                   arg = arg + " "; 
            }

            /// serial 
            if (checkBox10.Checked == true)
            {
                if (serial_path != "" && serial_path != null)
                    arg = arg + "-serial file:" + serial_path +" "; 
            }
            
            // paraell port
            if (checkBox13.Checked == true)
            {
                if (par_path != "" && par_path != null)
                    arg = arg + "-parallel  file:" + par_path +" "; 
            }

            //  vga standard
            if (checkBox16.Checked == true)
            {                
                  arg = arg + "-std-vga ";
            }

            // gdb
            if (checkBox14.Checked == true)
            {                
                arg = arg + "-s ";
            }

            if (textBox6.Text != "1234")
            {
                arg = arg + "-p "+textBox6.Text+" ";
            }

            // qemu state
            openFile.FileName = qemu_state;
            if (openFile.CheckFileExists == true)
            {
              arg = arg + "-loadvm "+qemu_state+" "; 
            }
            

            

            return arg;
        }

        private void checkBox10_CheckedChanged(object sender, System.EventArgs e)
        {
            if (checkBox10.Checked == true)
            {
                button18.Enabled = true;
            }
            else
            {
                button18.Enabled = false;
            }
        }

        private void checkBox13_CheckedChanged(object sender, System.EventArgs e)
        {
            if (checkBox13.Checked == true)
            {
                button19.Enabled = true;
            }
            else
            {
                button19.Enabled = false;
            }
        
        }

        private void checkBox15_CheckedChanged(object sender, System.EventArgs e)
        {
            if (checkBox15.Checked == true)
            {
                textBox7.Enabled = true;
            }
            else
            {
                textBox7.Enabled = false;
            }
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            qemu = new QEmu(); 
            cboBootFrom.SelectedIndex = 1;
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

        private void btnQEmuPath_Click(object sender, EventArgs e)
        {
            openFile.Filter = "Executable files (*.exe)|*.exe"; 
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                qemu.Path = Path.GetDirectoryName(openFile.FileName); 
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
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
            }
        }

	}
}
