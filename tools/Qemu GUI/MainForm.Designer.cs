namespace Qemu_GUI
{
    partial class MainForm
    {
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            //if (disposing)
            //{
            //    if (components != null)
            //    {
            //        components.Dispose();
            //    }
            //}
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadConfigToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveConfigToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.setAsDefaultToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpContentsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.aboutReactOSQEmuManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnLaunch = new System.Windows.Forms.Button();
            this.tabDebug = new System.Windows.Forms.TabPage();
            this.groupBox16 = new System.Windows.Forms.GroupBox();
            this.button20 = new System.Windows.Forms.Button();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.grpGDB = new System.Windows.Forms.GroupBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.txtGDBPort = new System.Windows.Forms.TextBox();
            this.lblGDBPort = new System.Windows.Forms.Label();
            this.chkVBE30 = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.grpVNC = new System.Windows.Forms.GroupBox();
            this.chkVNC = new System.Windows.Forms.CheckBox();
            this.label14 = new System.Windows.Forms.Label();
            this.txtVNC = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.txtABios = new System.Windows.Forms.TextBox();
            this.btnBrowseBios = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.txtAppendCmd = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txtLinuxK = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.tabNetwork = new System.Windows.Forms.TabPage();
            this.grpNetwork = new System.Windows.Forms.GroupBox();
            this.listVLANs = new System.Windows.Forms.ListView();
            this.btnNetAdd = new System.Windows.Forms.Button();
            this.btnNetRemove = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.rbtnNetNic = new System.Windows.Forms.RadioButton();
            this.panelNic = new System.Windows.Forms.Panel();
            this.cmbNICmodels = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtNicMACaddr = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.rbtnNetUser = new System.Windows.Forms.RadioButton();
            this.label4 = new System.Windows.Forms.Label();
            this.panelUser = new System.Windows.Forms.Panel();
            this.txtNetHost = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.pictureBox27 = new System.Windows.Forms.PictureBox();
            this.grpAudio = new System.Windows.Forms.GroupBox();
            this.chkOPL2 = new System.Windows.Forms.CheckBox();
            this.chkES1370 = new System.Windows.Forms.CheckBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.chkPCSpeaker = new System.Windows.Forms.CheckBox();
            this.chkSoundBlaster = new System.Windows.Forms.CheckBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.grpSerial = new System.Windows.Forms.GroupBox();
            this.chkSerialToFile = new System.Windows.Forms.CheckBox();
            this.btnBrowseSerial = new System.Windows.Forms.Button();
            this.txtSerialFile = new System.Windows.Forms.TextBox();
            this.chkSerialToPipe = new System.Windows.Forms.CheckBox();
            this.txtSerialPipe = new System.Windows.Forms.TextBox();
            this.chkSerialToScrn = new System.Windows.Forms.CheckBox();
            this.pictureBox24 = new System.Windows.Forms.PictureBox();
            this.pictureBox25 = new System.Windows.Forms.PictureBox();
            this.pictureBox26 = new System.Windows.Forms.PictureBox();
            this.tabHardDisk = new System.Windows.Forms.TabPage();
            this.grpHarddisk = new System.Windows.Forms.GroupBox();
            this.chkUseHDB = new System.Windows.Forms.CheckBox();
            this.btnBrowseHDB = new System.Windows.Forms.Button();
            this.chkUseHDC = new System.Windows.Forms.CheckBox();
            this.chkUseHDD = new System.Windows.Forms.CheckBox();
            this.btnBrowseHDC = new System.Windows.Forms.Button();
            this.btnBrowseHDD = new System.Windows.Forms.Button();
            this.txtHDB = new System.Windows.Forms.TextBox();
            this.txtHDC = new System.Windows.Forms.TextBox();
            this.txtHDD = new System.Windows.Forms.TextBox();
            this.pictureBox17 = new System.Windows.Forms.PictureBox();
            this.pictureBox18 = new System.Windows.Forms.PictureBox();
            this.pictureBox19 = new System.Windows.Forms.PictureBox();
            this.grpFloppy = new System.Windows.Forms.GroupBox();
            this.chkFloppyA = new System.Windows.Forms.CheckBox();
            this.chkFloppyB = new System.Windows.Forms.CheckBox();
            this.btnBrowseFloppyA = new System.Windows.Forms.Button();
            this.btnBrowseFloppyB = new System.Windows.Forms.Button();
            this.txtFloppyA = new System.Windows.Forms.TextBox();
            this.txtFloppyB = new System.Windows.Forms.TextBox();
            this.pictureBox20 = new System.Windows.Forms.PictureBox();
            this.pictureBox21 = new System.Windows.Forms.PictureBox();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.cboImageFormat = new System.Windows.Forms.ComboBox();
            this.btnCreateImage = new System.Windows.Forms.Button();
            this.lblImageSize = new System.Windows.Forms.Label();
            this.txtImageSize = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.lblImageSizeMB = new System.Windows.Forms.Label();
            this.pictureBox22 = new System.Windows.Forms.PictureBox();
            this.grpVDK = new System.Windows.Forms.GroupBox();
            this.lblImage = new System.Windows.Forms.Label();
            this.btnMount = new System.Windows.Forms.Button();
            this.btnUnmount = new System.Windows.Forms.Button();
            this.btnVDKImage = new System.Windows.Forms.Button();
            this.lblDrive = new System.Windows.Forms.Label();
            this.cboVDKDrive = new System.Windows.Forms.ComboBox();
            this.txtVDKImage = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.pictureBox23 = new System.Windows.Forms.PictureBox();
            this.tabGen = new System.Windows.Forms.TabPage();
            this.grpDisplay = new System.Windows.Forms.GroupBox();
            this.chkFullscreen = new System.Windows.Forms.CheckBox();
            this.chkVGAoutput = new System.Windows.Forms.CheckBox();
            this.pictureBox11 = new System.Windows.Forms.PictureBox();
            this.pictureBox12 = new System.Windows.Forms.PictureBox();
            this.grpClock = new System.Windows.Forms.GroupBox();
            this.chkSetClock = new System.Windows.Forms.CheckBox();
            this.pictureBox9 = new System.Windows.Forms.PictureBox();
            this.grpMachine = new System.Windows.Forms.GroupBox();
            this.cboMachine = new System.Windows.Forms.ComboBox();
            this.pictureBox5 = new System.Windows.Forms.PictureBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.cboBootFrom = new System.Windows.Forms.ComboBox();
            this.lblSMP = new System.Windows.Forms.Label();
            this.lblBootFrom = new System.Windows.Forms.Label();
            this.lblMemory = new System.Windows.Forms.Label();
            this.lblMB = new System.Windows.Forms.Label();
            this.numMemory = new System.Windows.Forms.NumericUpDown();
            this.numSMP = new System.Windows.Forms.NumericUpDown();
            this.chkKQEmu = new System.Windows.Forms.CheckBox();
            this.pictureBox6 = new System.Windows.Forms.PictureBox();
            this.lblCPUs = new System.Windows.Forms.Label();
            this.pictureBox7 = new System.Windows.Forms.PictureBox();
            this.pictureBox8 = new System.Windows.Forms.PictureBox();
            this.grpACPI = new System.Windows.Forms.GroupBox();
            this.chkACPI = new System.Windows.Forms.CheckBox();
            this.pictureBox10 = new System.Windows.Forms.PictureBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lblQEmuPath = new System.Windows.Forms.Label();
            this.btnQEmuPath = new System.Windows.Forms.Button();
            this.lblVDKPath = new System.Windows.Forms.Label();
            this.btnVDKBrowse = new System.Windows.Forms.Button();
            this.txtVDKPath = new System.Windows.Forms.TextBox();
            this.txtQEmuPath = new System.Windows.Forms.TextBox();
            this.pictureBox15 = new System.Windows.Forms.PictureBox();
            this.pictureBox16 = new System.Windows.Forms.PictureBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.btnBrowseCDROM = new System.Windows.Forms.Button();
            this.txtCDROM = new System.Windows.Forms.TextBox();
            this.chkUseCDROM = new System.Windows.Forms.CheckBox();
            this.txtHDA = new System.Windows.Forms.TextBox();
            this.btnBrowseHDA = new System.Windows.Forms.Button();
            this.chkUseHDA = new System.Windows.Forms.CheckBox();
            this.pictureBox13 = new System.Windows.Forms.PictureBox();
            this.pictureBox14 = new System.Windows.Forms.PictureBox();
            this.Tabs = new System.Windows.Forms.TabControl();
            this.menuStrip1.SuspendLayout();
            this.tabDebug.SuspendLayout();
            this.groupBox16.SuspendLayout();
            this.grpGDB.SuspendLayout();
            this.grpVNC.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabNetwork.SuspendLayout();
            this.grpNetwork.SuspendLayout();
            this.panelNic.SuspendLayout();
            this.panelUser.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox27)).BeginInit();
            this.grpAudio.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.grpSerial.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox24)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox25)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox26)).BeginInit();
            this.tabHardDisk.SuspendLayout();
            this.grpHarddisk.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox17)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox18)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox19)).BeginInit();
            this.grpFloppy.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox20)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox21)).BeginInit();
            this.groupBox9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox22)).BeginInit();
            this.grpVDK.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox23)).BeginInit();
            this.tabGen.SuspendLayout();
            this.grpDisplay.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox11)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox12)).BeginInit();
            this.grpClock.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox9)).BeginInit();
            this.grpMachine.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).BeginInit();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox7)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox8)).BeginInit();
            this.grpACPI.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox10)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox15)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox16)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox13)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox14)).BeginInit();
            this.Tabs.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFile
            // 
            this.openFile.Title = "Path to VDK";
            this.openFile.ValidateNames = false;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStrip1.Size = new System.Drawing.Size(557, 25);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadConfigToolStripMenuItem,
            this.saveConfigToolStripMenuItem,
            this.toolStripSeparator2,
            this.setAsDefaultToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadConfigToolStripMenuItem
            // 
            this.loadConfigToolStripMenuItem.Name = "loadConfigToolStripMenuItem";
            this.loadConfigToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.loadConfigToolStripMenuItem.Text = "Load config";
            this.loadConfigToolStripMenuItem.Click += new System.EventHandler(this.loadConfigToolStripMenuItem_Click);
            // 
            // saveConfigToolStripMenuItem
            // 
            this.saveConfigToolStripMenuItem.Name = "saveConfigToolStripMenuItem";
            this.saveConfigToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.saveConfigToolStripMenuItem.Text = "Save Config";
            this.saveConfigToolStripMenuItem.Click += new System.EventHandler(this.saveConfigToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(152, 6);
            // 
            // setAsDefaultToolStripMenuItem
            // 
            this.setAsDefaultToolStripMenuItem.Name = "setAsDefaultToolStripMenuItem";
            this.setAsDefaultToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.setAsDefaultToolStripMenuItem.Text = "Set as default";
            this.setAsDefaultToolStripMenuItem.Click += new System.EventHandler(this.setAsDefaultToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(152, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.helpContentsToolStripMenuItem,
            this.toolStripSeparator3,
            this.aboutReactOSQEmuManagerToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // helpContentsToolStripMenuItem
            // 
            this.helpContentsToolStripMenuItem.Name = "helpContentsToolStripMenuItem";
            this.helpContentsToolStripMenuItem.Size = new System.Drawing.Size(260, 22);
            this.helpContentsToolStripMenuItem.Text = "Help Contents";
            this.helpContentsToolStripMenuItem.Click += new System.EventHandler(this.helpContentsToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(257, 6);
            // 
            // aboutReactOSQEmuManagerToolStripMenuItem
            // 
            this.aboutReactOSQEmuManagerToolStripMenuItem.Name = "aboutReactOSQEmuManagerToolStripMenuItem";
            this.aboutReactOSQEmuManagerToolStripMenuItem.Size = new System.Drawing.Size(260, 22);
            this.aboutReactOSQEmuManagerToolStripMenuItem.Text = "About ReactOS QEmu Manager";
            this.aboutReactOSQEmuManagerToolStripMenuItem.Click += new System.EventHandler(this.aboutReactOSQEmuManagerToolStripMenuItem_Click);
            // 
            // btnStop
            // 
            this.btnStop.Image = global::Qemu_GUI.Properties.Resources.stop;
            this.btnStop.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnStop.Location = new System.Drawing.Point(79, 28);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(60, 23);
            this.btnStop.TabIndex = 3;
            this.btnStop.Text = "Stop";
            this.btnStop.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnStop.UseVisualStyleBackColor = false;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnLaunch
            // 
            this.btnLaunch.Image = global::Qemu_GUI.Properties.Resources.launch;
            this.btnLaunch.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnLaunch.Location = new System.Drawing.Point(12, 28);
            this.btnLaunch.Name = "btnLaunch";
            this.btnLaunch.Size = new System.Drawing.Size(60, 23);
            this.btnLaunch.TabIndex = 2;
            this.btnLaunch.Text = "Start";
            this.btnLaunch.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnLaunch.UseVisualStyleBackColor = false;
            this.btnLaunch.Click += new System.EventHandler(this.btnLaunch_Click);
            // 
            // tabDebug
            // 
            this.tabDebug.Controls.Add(this.groupBox1);
            this.tabDebug.Controls.Add(this.groupBox4);
            this.tabDebug.Controls.Add(this.groupBox3);
            this.tabDebug.Controls.Add(this.grpVNC);
            this.tabDebug.Controls.Add(this.checkBox2);
            this.tabDebug.Controls.Add(this.chkVBE30);
            this.tabDebug.Controls.Add(this.grpGDB);
            this.tabDebug.Controls.Add(this.groupBox16);
            this.tabDebug.Location = new System.Drawing.Point(4, 25);
            this.tabDebug.Name = "tabDebug";
            this.tabDebug.Size = new System.Drawing.Size(549, 387);
            this.tabDebug.TabIndex = 8;
            this.tabDebug.Text = "Advanced";
            this.tabDebug.UseVisualStyleBackColor = true;
            // 
            // groupBox16
            // 
            this.groupBox16.Controls.Add(this.checkBox3);
            this.groupBox16.Controls.Add(this.button20);
            this.groupBox16.Location = new System.Drawing.Point(8, 175);
            this.groupBox16.Name = "groupBox16";
            this.groupBox16.Size = new System.Drawing.Size(162, 106);
            this.groupBox16.TabIndex = 1;
            this.groupBox16.TabStop = false;
            this.groupBox16.Text = "LoadVM state";
            // 
            // button20
            // 
            this.button20.Location = new System.Drawing.Point(29, 53);
            this.button20.Name = "button20";
            this.button20.Size = new System.Drawing.Size(96, 23);
            this.button20.TabIndex = 3;
            this.button20.Text = "Browse";
            this.button20.Click += new System.EventHandler(this.btnLoadVM_Click);
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(45, 30);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(78, 20);
            this.checkBox3.TabIndex = 4;
            this.checkBox3.Text = "Enabled";
            this.checkBox3.UseVisualStyleBackColor = true;
            // 
            // grpGDB
            // 
            this.grpGDB.Controls.Add(this.lblGDBPort);
            this.grpGDB.Controls.Add(this.txtGDBPort);
            this.grpGDB.Controls.Add(this.checkBox14);
            this.grpGDB.Location = new System.Drawing.Point(333, 175);
            this.grpGDB.Name = "grpGDB";
            this.grpGDB.Size = new System.Drawing.Size(208, 106);
            this.grpGDB.TabIndex = 2;
            this.grpGDB.TabStop = false;
            this.grpGDB.Text = "GDB";
            // 
            // checkBox14
            // 
            this.checkBox14.Location = new System.Drawing.Point(22, 21);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(180, 24);
            this.checkBox14.TabIndex = 0;
            this.checkBox14.Text = "Wait connection to port";
            // 
            // txtGDBPort
            // 
            this.txtGDBPort.Location = new System.Drawing.Point(107, 56);
            this.txtGDBPort.Name = "txtGDBPort";
            this.txtGDBPort.Size = new System.Drawing.Size(47, 22);
            this.txtGDBPort.TabIndex = 1;
            this.txtGDBPort.Text = "1234";
            this.txtGDBPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // lblGDBPort
            // 
            this.lblGDBPort.Location = new System.Drawing.Point(46, 56);
            this.lblGDBPort.Name = "lblGDBPort";
            this.lblGDBPort.Size = new System.Drawing.Size(55, 23);
            this.lblGDBPort.TabIndex = 2;
            this.lblGDBPort.Text = "Port:";
            // 
            // chkVBE30
            // 
            this.chkVBE30.Location = new System.Drawing.Point(12, 319);
            this.chkVBE30.Name = "chkVBE30";
            this.chkVBE30.Size = new System.Drawing.Size(336, 23);
            this.chkVBE30.TabIndex = 14;
            this.chkVBE30.Text = "Simulate VGA card with Bochs VBE 3.0";
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(12, 348);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(308, 20);
            this.checkBox2.TabIndex = 15;
            this.checkBox2.Text = "Freeze CPU at startup (use \'c\' to start execution)";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // grpVNC
            // 
            this.grpVNC.Controls.Add(this.txtVNC);
            this.grpVNC.Controls.Add(this.label14);
            this.grpVNC.Controls.Add(this.chkVNC);
            this.grpVNC.Location = new System.Drawing.Point(365, 3);
            this.grpVNC.Name = "grpVNC";
            this.grpVNC.Size = new System.Drawing.Size(176, 114);
            this.grpVNC.TabIndex = 16;
            this.grpVNC.TabStop = false;
            this.grpVNC.Text = "VNC Server";
            // 
            // chkVNC
            // 
            this.chkVNC.Location = new System.Drawing.Point(34, 21);
            this.chkVNC.Name = "chkVNC";
            this.chkVNC.Size = new System.Drawing.Size(116, 24);
            this.chkVNC.TabIndex = 0;
            this.chkVNC.Text = "Activate VNC";
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(31, 48);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(116, 21);
            this.label14.TabIndex = 1;
            this.label14.Text = "Display Number";
            // 
            // txtVNC
            // 
            this.txtVNC.Enabled = false;
            this.txtVNC.Location = new System.Drawing.Point(34, 72);
            this.txtVNC.Name = "txtVNC";
            this.txtVNC.Size = new System.Drawing.Size(96, 22);
            this.txtVNC.TabIndex = 2;
            this.txtVNC.Text = "0";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.btnBrowseBios);
            this.groupBox3.Controls.Add(this.txtABios);
            this.groupBox3.Enabled = false;
            this.groupBox3.Location = new System.Drawing.Point(3, 65);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(356, 52);
            this.groupBox3.TabIndex = 17;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "BIOS";
            // 
            // txtABios
            // 
            this.txtABios.Location = new System.Drawing.Point(101, 17);
            this.txtABios.Name = "txtABios";
            this.txtABios.Size = new System.Drawing.Size(211, 22);
            this.txtABios.TabIndex = 4;
            // 
            // btnBrowseBios
            // 
            this.btnBrowseBios.Location = new System.Drawing.Point(318, 15);
            this.btnBrowseBios.Name = "btnBrowseBios";
            this.btnBrowseBios.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseBios.TabIndex = 5;
            this.btnBrowseBios.Text = "...";
            this.btnBrowseBios.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 20);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(95, 16);
            this.label8.TabIndex = 6;
            this.label8.Text = "Alternate BIOS";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.txtAppendCmd);
            this.groupBox4.Location = new System.Drawing.Point(3, 117);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(538, 52);
            this.groupBox4.TabIndex = 18;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Append";
            // 
            // txtAppendCmd
            // 
            this.txtAppendCmd.Location = new System.Drawing.Point(101, 19);
            this.txtAppendCmd.Name = "txtAppendCmd";
            this.txtAppendCmd.Size = new System.Drawing.Size(388, 22);
            this.txtAppendCmd.TabIndex = 4;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 22);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(64, 16);
            this.label10.TabIndex = 6;
            this.label10.Text = "Cmd Line";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.txtLinuxK);
            this.groupBox1.Enabled = false;
            this.groupBox1.Location = new System.Drawing.Point(3, 7);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(356, 52);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Alternate Kernel";
            // 
            // txtLinuxK
            // 
            this.txtLinuxK.Location = new System.Drawing.Point(101, 17);
            this.txtLinuxK.Name = "txtLinuxK";
            this.txtLinuxK.Size = new System.Drawing.Size(211, 22);
            this.txtLinuxK.TabIndex = 4;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(318, 15);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(27, 22);
            this.button1.TabIndex = 5;
            this.button1.Text = "...";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(87, 16);
            this.label6.TabIndex = 6;
            this.label6.Text = "Kernel Image";
            // 
            // tabNetwork
            // 
            this.tabNetwork.Controls.Add(this.grpSerial);
            this.tabNetwork.Controls.Add(this.grpAudio);
            this.tabNetwork.Controls.Add(this.grpNetwork);
            this.tabNetwork.Location = new System.Drawing.Point(4, 25);
            this.tabNetwork.Name = "tabNetwork";
            this.tabNetwork.Size = new System.Drawing.Size(549, 387);
            this.tabNetwork.TabIndex = 7;
            this.tabNetwork.Text = "Devices";
            this.tabNetwork.UseVisualStyleBackColor = true;
            // 
            // grpNetwork
            // 
            this.grpNetwork.Controls.Add(this.pictureBox27);
            this.grpNetwork.Controls.Add(this.panelUser);
            this.grpNetwork.Controls.Add(this.label4);
            this.grpNetwork.Controls.Add(this.rbtnNetUser);
            this.grpNetwork.Controls.Add(this.panelNic);
            this.grpNetwork.Controls.Add(this.rbtnNetNic);
            this.grpNetwork.Controls.Add(this.label1);
            this.grpNetwork.Controls.Add(this.btnNetRemove);
            this.grpNetwork.Controls.Add(this.btnNetAdd);
            this.grpNetwork.Controls.Add(this.listVLANs);
            this.grpNetwork.Location = new System.Drawing.Point(8, 3);
            this.grpNetwork.Name = "grpNetwork";
            this.grpNetwork.Size = new System.Drawing.Size(533, 169);
            this.grpNetwork.TabIndex = 1;
            this.grpNetwork.TabStop = false;
            this.grpNetwork.Text = "Network";
            // 
            // listVLANs
            // 
            this.listVLANs.AutoArrange = false;
            this.listVLANs.LabelWrap = false;
            this.listVLANs.Location = new System.Drawing.Point(27, 49);
            this.listVLANs.Name = "listVLANs";
            this.listVLANs.ShowGroups = false;
            this.listVLANs.Size = new System.Drawing.Size(191, 94);
            this.listVLANs.TabIndex = 47;
            this.listVLANs.UseCompatibleStateImageBehavior = false;
            this.listVLANs.View = System.Windows.Forms.View.List;
            // 
            // btnNetAdd
            // 
            this.btnNetAdd.Location = new System.Drawing.Point(226, 107);
            this.btnNetAdd.Name = "btnNetAdd";
            this.btnNetAdd.Size = new System.Drawing.Size(70, 25);
            this.btnNetAdd.TabIndex = 48;
            this.btnNetAdd.Text = "Add";
            this.btnNetAdd.UseVisualStyleBackColor = true;
            this.btnNetAdd.Click += new System.EventHandler(this.btnNetAdd_Click);
            // 
            // btnNetRemove
            // 
            this.btnNetRemove.Location = new System.Drawing.Point(226, 138);
            this.btnNetRemove.Name = "btnNetRemove";
            this.btnNetRemove.Size = new System.Drawing.Size(70, 25);
            this.btnNetRemove.TabIndex = 49;
            this.btnNetRemove.Text = "Remove";
            this.btnNetRemove.UseVisualStyleBackColor = true;
            this.btnNetRemove.Click += new System.EventHandler(this.btnNetRemove_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(36, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 16);
            this.label1.TabIndex = 50;
            this.label1.Text = "Virtual NICs";
            // 
            // rbtnNetNic
            // 
            this.rbtnNetNic.AutoSize = true;
            this.rbtnNetNic.Location = new System.Drawing.Point(239, 74);
            this.rbtnNetNic.Name = "rbtnNetNic";
            this.rbtnNetNic.Size = new System.Drawing.Size(48, 20);
            this.rbtnNetNic.TabIndex = 51;
            this.rbtnNetNic.Text = "NIC";
            this.rbtnNetNic.UseVisualStyleBackColor = true;
            this.rbtnNetNic.CheckedChanged += new System.EventHandler(this.rbtnNetNic_CheckedChanged);
            // 
            // panelNic
            // 
            this.panelNic.Controls.Add(this.label3);
            this.panelNic.Controls.Add(this.txtNicMACaddr);
            this.panelNic.Controls.Add(this.label2);
            this.panelNic.Controls.Add(this.cmbNICmodels);
            this.panelNic.Location = new System.Drawing.Point(311, 22);
            this.panelNic.Name = "panelNic";
            this.panelNic.Size = new System.Drawing.Size(195, 121);
            this.panelNic.TabIndex = 52;
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
            this.cmbNICmodels.Size = new System.Drawing.Size(121, 24);
            this.cmbNICmodels.TabIndex = 34;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(106, 16);
            this.label2.TabIndex = 35;
            this.label2.Text = "Card to Emulate:";
            // 
            // txtNicMACaddr
            // 
            this.txtNicMACaddr.Location = new System.Drawing.Point(23, 88);
            this.txtNicMACaddr.Name = "txtNicMACaddr";
            this.txtNicMACaddr.Size = new System.Drawing.Size(121, 22);
            this.txtNicMACaddr.TabIndex = 36;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(94, 16);
            this.label3.TabIndex = 37;
            this.label3.Text = "MAC Address:";
            // 
            // rbtnNetUser
            // 
            this.rbtnNetUser.AutoSize = true;
            this.rbtnNetUser.Checked = true;
            this.rbtnNetUser.Location = new System.Drawing.Point(239, 49);
            this.rbtnNetUser.Name = "rbtnNetUser";
            this.rbtnNetUser.Size = new System.Drawing.Size(55, 20);
            this.rbtnNetUser.TabIndex = 53;
            this.rbtnNetUser.TabStop = true;
            this.rbtnNetUser.Text = "User";
            this.rbtnNetUser.UseVisualStyleBackColor = true;
            this.rbtnNetUser.CheckedChanged += new System.EventHandler(this.rbtnNetUser_CheckedChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(223, 21);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(46, 16);
            this.label4.TabIndex = 54;
            this.label4.Text = "Mode:";
            // 
            // panelUser
            // 
            this.panelUser.Controls.Add(this.label5);
            this.panelUser.Controls.Add(this.txtNetHost);
            this.panelUser.Location = new System.Drawing.Point(314, 21);
            this.panelUser.Name = "panelUser";
            this.panelUser.Size = new System.Drawing.Size(189, 67);
            this.panelUser.TabIndex = 38;
            // 
            // txtNetHost
            // 
            this.txtNetHost.Location = new System.Drawing.Point(23, 35);
            this.txtNetHost.Name = "txtNetHost";
            this.txtNetHost.Size = new System.Drawing.Size(121, 22);
            this.txtNetHost.TabIndex = 0;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(20, 13);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(73, 16);
            this.label5.TabIndex = 1;
            this.label5.Text = "Hostname:";
            // 
            // pictureBox27
            // 
            this.pictureBox27.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox27.Image = global::Qemu_GUI.Properties.Resources.network;
            this.pictureBox27.Location = new System.Drawing.Point(6, 19);
            this.pictureBox27.Name = "pictureBox27";
            this.pictureBox27.Size = new System.Drawing.Size(24, 24);
            this.pictureBox27.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox27.TabIndex = 55;
            this.pictureBox27.TabStop = false;
            // 
            // grpAudio
            // 
            this.grpAudio.Controls.Add(this.pictureBox4);
            this.grpAudio.Controls.Add(this.pictureBox1);
            this.grpAudio.Controls.Add(this.pictureBox3);
            this.grpAudio.Controls.Add(this.chkSoundBlaster);
            this.grpAudio.Controls.Add(this.chkPCSpeaker);
            this.grpAudio.Controls.Add(this.pictureBox2);
            this.grpAudio.Controls.Add(this.chkES1370);
            this.grpAudio.Controls.Add(this.chkOPL2);
            this.grpAudio.Location = new System.Drawing.Point(8, 178);
            this.grpAudio.Name = "grpAudio";
            this.grpAudio.Size = new System.Drawing.Size(254, 166);
            this.grpAudio.TabIndex = 5;
            this.grpAudio.TabStop = false;
            this.grpAudio.Text = "Audio Card Emulation";
            // 
            // chkOPL2
            // 
            this.chkOPL2.Location = new System.Drawing.Point(28, 80);
            this.chkOPL2.Name = "chkOPL2";
            this.chkOPL2.Size = new System.Drawing.Size(191, 20);
            this.chkOPL2.TabIndex = 2;
            this.chkOPL2.Text = "Yamaha YM3812 (OPL2)";
            // 
            // chkES1370
            // 
            this.chkES1370.Location = new System.Drawing.Point(28, 106);
            this.chkES1370.Name = "chkES1370";
            this.chkES1370.Size = new System.Drawing.Size(206, 20);
            this.chkES1370.TabIndex = 2;
            this.chkES1370.Text = "ENSONIQ AudioPCI ES1370";
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox2.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox2.Location = new System.Drawing.Point(6, 57);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(16, 16);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox2.TabIndex = 4;
            this.pictureBox2.TabStop = false;
            // 
            // chkPCSpeaker
            // 
            this.chkPCSpeaker.Checked = true;
            this.chkPCSpeaker.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPCSpeaker.Location = new System.Drawing.Point(28, 28);
            this.chkPCSpeaker.Name = "chkPCSpeaker";
            this.chkPCSpeaker.Size = new System.Drawing.Size(122, 20);
            this.chkPCSpeaker.TabIndex = 0;
            this.chkPCSpeaker.Text = "PC Speaker";
            // 
            // chkSoundBlaster
            // 
            this.chkSoundBlaster.Checked = true;
            this.chkSoundBlaster.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSoundBlaster.Location = new System.Drawing.Point(28, 54);
            this.chkSoundBlaster.Name = "chkSoundBlaster";
            this.chkSoundBlaster.Size = new System.Drawing.Size(122, 20);
            this.chkSoundBlaster.TabIndex = 1;
            this.chkSoundBlaster.Text = "Creative Sound Blaster 16";
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox3.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox3.Location = new System.Drawing.Point(6, 80);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(16, 16);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox3.TabIndex = 5;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox1.Location = new System.Drawing.Point(6, 28);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(16, 16);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox4.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox4.Location = new System.Drawing.Point(6, 109);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(16, 16);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox4.TabIndex = 6;
            this.pictureBox4.TabStop = false;
            // 
            // grpSerial
            // 
            this.grpSerial.Controls.Add(this.pictureBox26);
            this.grpSerial.Controls.Add(this.pictureBox25);
            this.grpSerial.Controls.Add(this.pictureBox24);
            this.grpSerial.Controls.Add(this.chkSerialToScrn);
            this.grpSerial.Controls.Add(this.txtSerialPipe);
            this.grpSerial.Controls.Add(this.chkSerialToPipe);
            this.grpSerial.Controls.Add(this.txtSerialFile);
            this.grpSerial.Controls.Add(this.btnBrowseSerial);
            this.grpSerial.Controls.Add(this.chkSerialToFile);
            this.grpSerial.Location = new System.Drawing.Point(272, 178);
            this.grpSerial.Name = "grpSerial";
            this.grpSerial.Size = new System.Drawing.Size(269, 166);
            this.grpSerial.TabIndex = 6;
            this.grpSerial.TabStop = false;
            this.grpSerial.Text = "Serial Port";
            // 
            // chkSerialToFile
            // 
            this.chkSerialToFile.Location = new System.Drawing.Point(36, 47);
            this.chkSerialToFile.Name = "chkSerialToFile";
            this.chkSerialToFile.Size = new System.Drawing.Size(122, 24);
            this.chkSerialToFile.TabIndex = 2;
            this.chkSerialToFile.Text = "Redirect to file";
            this.chkSerialToFile.CheckedChanged += new System.EventHandler(this.chkSerialToFile_CheckedChanged);
            // 
            // btnBrowseSerial
            // 
            this.btnBrowseSerial.Enabled = false;
            this.btnBrowseSerial.Location = new System.Drawing.Point(236, 80);
            this.btnBrowseSerial.Name = "btnBrowseSerial";
            this.btnBrowseSerial.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseSerial.TabIndex = 3;
            this.btnBrowseSerial.Text = "...";
            this.btnBrowseSerial.Click += new System.EventHandler(this.btnBrowseSerial_Click);
            // 
            // txtSerialFile
            // 
            this.txtSerialFile.Location = new System.Drawing.Point(52, 80);
            this.txtSerialFile.Name = "txtSerialFile";
            this.txtSerialFile.Size = new System.Drawing.Size(169, 22);
            this.txtSerialFile.TabIndex = 4;
            // 
            // chkSerialToPipe
            // 
            this.chkSerialToPipe.AutoSize = true;
            this.chkSerialToPipe.Location = new System.Drawing.Point(36, 108);
            this.chkSerialToPipe.Name = "chkSerialToPipe";
            this.chkSerialToPipe.Size = new System.Drawing.Size(122, 20);
            this.chkSerialToPipe.TabIndex = 5;
            this.chkSerialToPipe.Text = "Redirect to pipe";
            this.chkSerialToPipe.UseVisualStyleBackColor = true;
            this.chkSerialToPipe.CheckedChanged += new System.EventHandler(this.chkSerialToPipe_CheckedChanged);
            // 
            // txtSerialPipe
            // 
            this.txtSerialPipe.Location = new System.Drawing.Point(52, 134);
            this.txtSerialPipe.Name = "txtSerialPipe";
            this.txtSerialPipe.Size = new System.Drawing.Size(203, 22);
            this.txtSerialPipe.TabIndex = 6;
            // 
            // chkSerialToScrn
            // 
            this.chkSerialToScrn.AutoSize = true;
            this.chkSerialToScrn.Enabled = false;
            this.chkSerialToScrn.Location = new System.Drawing.Point(36, 21);
            this.chkSerialToScrn.Name = "chkSerialToScrn";
            this.chkSerialToScrn.Size = new System.Drawing.Size(136, 20);
            this.chkSerialToScrn.TabIndex = 7;
            this.chkSerialToScrn.Text = "Redirect to screen";
            this.chkSerialToScrn.UseVisualStyleBackColor = true;
            this.chkSerialToScrn.CheckedChanged += new System.EventHandler(this.chkSerialToScrn_CheckedChanged);
            // 
            // pictureBox24
            // 
            this.pictureBox24.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox24.Image = global::Qemu_GUI.Properties.Resources.debug;
            this.pictureBox24.Location = new System.Drawing.Point(6, 17);
            this.pictureBox24.Name = "pictureBox24";
            this.pictureBox24.Size = new System.Drawing.Size(24, 24);
            this.pictureBox24.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox24.TabIndex = 8;
            this.pictureBox24.TabStop = false;
            // 
            // pictureBox25
            // 
            this.pictureBox25.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox25.Image = global::Qemu_GUI.Properties.Resources.debug;
            this.pictureBox25.Location = new System.Drawing.Point(6, 50);
            this.pictureBox25.Name = "pictureBox25";
            this.pictureBox25.Size = new System.Drawing.Size(24, 24);
            this.pictureBox25.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox25.TabIndex = 9;
            this.pictureBox25.TabStop = false;
            // 
            // pictureBox26
            // 
            this.pictureBox26.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox26.Image = global::Qemu_GUI.Properties.Resources.debug;
            this.pictureBox26.Location = new System.Drawing.Point(6, 108);
            this.pictureBox26.Name = "pictureBox26";
            this.pictureBox26.Size = new System.Drawing.Size(24, 24);
            this.pictureBox26.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox26.TabIndex = 10;
            this.pictureBox26.TabStop = false;
            // 
            // tabHardDisk
            // 
            this.tabHardDisk.Controls.Add(this.grpVDK);
            this.tabHardDisk.Controls.Add(this.groupBox9);
            this.tabHardDisk.Controls.Add(this.grpFloppy);
            this.tabHardDisk.Controls.Add(this.grpHarddisk);
            this.tabHardDisk.Location = new System.Drawing.Point(4, 25);
            this.tabHardDisk.Name = "tabHardDisk";
            this.tabHardDisk.Size = new System.Drawing.Size(549, 387);
            this.tabHardDisk.TabIndex = 3;
            this.tabHardDisk.Text = "Media";
            this.tabHardDisk.UseVisualStyleBackColor = true;
            // 
            // grpHarddisk
            // 
            this.grpHarddisk.Controls.Add(this.pictureBox19);
            this.grpHarddisk.Controls.Add(this.pictureBox18);
            this.grpHarddisk.Controls.Add(this.pictureBox17);
            this.grpHarddisk.Controls.Add(this.txtHDD);
            this.grpHarddisk.Controls.Add(this.txtHDC);
            this.grpHarddisk.Controls.Add(this.txtHDB);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDD);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDC);
            this.grpHarddisk.Controls.Add(this.chkUseHDD);
            this.grpHarddisk.Controls.Add(this.chkUseHDC);
            this.grpHarddisk.Controls.Add(this.btnBrowseHDB);
            this.grpHarddisk.Controls.Add(this.chkUseHDB);
            this.grpHarddisk.Location = new System.Drawing.Point(8, 145);
            this.grpHarddisk.Name = "grpHarddisk";
            this.grpHarddisk.Size = new System.Drawing.Size(533, 118);
            this.grpHarddisk.TabIndex = 5;
            this.grpHarddisk.TabStop = false;
            this.grpHarddisk.Text = " Harddisks";
            // 
            // chkUseHDB
            // 
            this.chkUseHDB.Location = new System.Drawing.Point(35, 21);
            this.chkUseHDB.Name = "chkUseHDB";
            this.chkUseHDB.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDB.TabIndex = 5;
            this.chkUseHDB.Text = "HDB";
            this.chkUseHDB.CheckedChanged += new System.EventHandler(this.chkUseHDB_CheckedChanged);
            // 
            // btnBrowseHDB
            // 
            this.btnBrowseHDB.Enabled = false;
            this.btnBrowseHDB.Location = new System.Drawing.Point(467, 23);
            this.btnBrowseHDB.Name = "btnBrowseHDB";
            this.btnBrowseHDB.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseHDB.TabIndex = 7;
            this.btnBrowseHDB.Text = "...";
            this.btnBrowseHDB.Click += new System.EventHandler(this.btnBrowseHDB_Click);
            // 
            // chkUseHDC
            // 
            this.chkUseHDC.Location = new System.Drawing.Point(35, 51);
            this.chkUseHDC.Name = "chkUseHDC";
            this.chkUseHDC.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDC.TabIndex = 8;
            this.chkUseHDC.Text = "HDC";
            this.chkUseHDC.CheckedChanged += new System.EventHandler(this.chkUseHDC_CheckedChanged);
            // 
            // chkUseHDD
            // 
            this.chkUseHDD.Location = new System.Drawing.Point(35, 81);
            this.chkUseHDD.Name = "chkUseHDD";
            this.chkUseHDD.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDD.TabIndex = 9;
            this.chkUseHDD.Text = "HDD";
            this.chkUseHDD.CheckedChanged += new System.EventHandler(this.chkUseHDD_CheckedChanged);
            // 
            // btnBrowseHDC
            // 
            this.btnBrowseHDC.Enabled = false;
            this.btnBrowseHDC.Location = new System.Drawing.Point(467, 53);
            this.btnBrowseHDC.Name = "btnBrowseHDC";
            this.btnBrowseHDC.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseHDC.TabIndex = 10;
            this.btnBrowseHDC.Text = "...";
            this.btnBrowseHDC.Click += new System.EventHandler(this.btnBrowseHDC_Click);
            // 
            // btnBrowseHDD
            // 
            this.btnBrowseHDD.Enabled = false;
            this.btnBrowseHDD.Location = new System.Drawing.Point(467, 83);
            this.btnBrowseHDD.Name = "btnBrowseHDD";
            this.btnBrowseHDD.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseHDD.TabIndex = 11;
            this.btnBrowseHDD.Text = "...";
            this.btnBrowseHDD.Click += new System.EventHandler(this.btnBrowseHDD_Click);
            // 
            // txtHDB
            // 
            this.txtHDB.Enabled = false;
            this.txtHDB.Location = new System.Drawing.Point(106, 25);
            this.txtHDB.Name = "txtHDB";
            this.txtHDB.Size = new System.Drawing.Size(355, 22);
            this.txtHDB.TabIndex = 13;
            // 
            // txtHDC
            // 
            this.txtHDC.Enabled = false;
            this.txtHDC.Location = new System.Drawing.Point(106, 55);
            this.txtHDC.Name = "txtHDC";
            this.txtHDC.Size = new System.Drawing.Size(355, 22);
            this.txtHDC.TabIndex = 14;
            // 
            // txtHDD
            // 
            this.txtHDD.Enabled = false;
            this.txtHDD.Location = new System.Drawing.Point(106, 85);
            this.txtHDD.Name = "txtHDD";
            this.txtHDD.Size = new System.Drawing.Size(355, 22);
            this.txtHDD.TabIndex = 15;
            // 
            // pictureBox17
            // 
            this.pictureBox17.Image = global::Qemu_GUI.Properties.Resources.harddisk;
            this.pictureBox17.Location = new System.Drawing.Point(5, 21);
            this.pictureBox17.Name = "pictureBox17";
            this.pictureBox17.Size = new System.Drawing.Size(24, 24);
            this.pictureBox17.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox17.TabIndex = 26;
            this.pictureBox17.TabStop = false;
            // 
            // pictureBox18
            // 
            this.pictureBox18.Image = global::Qemu_GUI.Properties.Resources.harddisk;
            this.pictureBox18.Location = new System.Drawing.Point(5, 51);
            this.pictureBox18.Name = "pictureBox18";
            this.pictureBox18.Size = new System.Drawing.Size(24, 24);
            this.pictureBox18.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox18.TabIndex = 27;
            this.pictureBox18.TabStop = false;
            // 
            // pictureBox19
            // 
            this.pictureBox19.Image = global::Qemu_GUI.Properties.Resources.harddisk;
            this.pictureBox19.Location = new System.Drawing.Point(5, 81);
            this.pictureBox19.Name = "pictureBox19";
            this.pictureBox19.Size = new System.Drawing.Size(24, 24);
            this.pictureBox19.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox19.TabIndex = 28;
            this.pictureBox19.TabStop = false;
            // 
            // grpFloppy
            // 
            this.grpFloppy.Controls.Add(this.pictureBox21);
            this.grpFloppy.Controls.Add(this.pictureBox20);
            this.grpFloppy.Controls.Add(this.txtFloppyB);
            this.grpFloppy.Controls.Add(this.txtFloppyA);
            this.grpFloppy.Controls.Add(this.btnBrowseFloppyB);
            this.grpFloppy.Controls.Add(this.btnBrowseFloppyA);
            this.grpFloppy.Controls.Add(this.chkFloppyB);
            this.grpFloppy.Controls.Add(this.chkFloppyA);
            this.grpFloppy.Location = new System.Drawing.Point(10, 269);
            this.grpFloppy.Name = "grpFloppy";
            this.grpFloppy.Size = new System.Drawing.Size(532, 83);
            this.grpFloppy.TabIndex = 6;
            this.grpFloppy.TabStop = false;
            this.grpFloppy.Text = "Floppy";
            // 
            // chkFloppyA
            // 
            this.chkFloppyA.Location = new System.Drawing.Point(31, 17);
            this.chkFloppyA.Name = "chkFloppyA";
            this.chkFloppyA.Size = new System.Drawing.Size(82, 24);
            this.chkFloppyA.TabIndex = 0;
            this.chkFloppyA.Text = "Floppy A";
            this.chkFloppyA.CheckedChanged += new System.EventHandler(this.chkFloppyA_CheckedChanged);
            // 
            // chkFloppyB
            // 
            this.chkFloppyB.Location = new System.Drawing.Point(31, 47);
            this.chkFloppyB.Name = "chkFloppyB";
            this.chkFloppyB.Size = new System.Drawing.Size(82, 24);
            this.chkFloppyB.TabIndex = 1;
            this.chkFloppyB.Text = "Floppy B";
            this.chkFloppyB.CheckedChanged += new System.EventHandler(this.chkFloppyB_CheckedChanged);
            // 
            // btnBrowseFloppyA
            // 
            this.btnBrowseFloppyA.Enabled = false;
            this.btnBrowseFloppyA.Location = new System.Drawing.Point(467, 19);
            this.btnBrowseFloppyA.Name = "btnBrowseFloppyA";
            this.btnBrowseFloppyA.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseFloppyA.TabIndex = 2;
            this.btnBrowseFloppyA.Text = "...";
            this.btnBrowseFloppyA.Click += new System.EventHandler(this.btnBrowseFloppyA_Click);
            // 
            // btnBrowseFloppyB
            // 
            this.btnBrowseFloppyB.Enabled = false;
            this.btnBrowseFloppyB.Location = new System.Drawing.Point(467, 51);
            this.btnBrowseFloppyB.Name = "btnBrowseFloppyB";
            this.btnBrowseFloppyB.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseFloppyB.TabIndex = 3;
            this.btnBrowseFloppyB.Text = "...";
            this.btnBrowseFloppyB.Click += new System.EventHandler(this.btnBrowseFloppyB_Click);
            // 
            // txtFloppyA
            // 
            this.txtFloppyA.Enabled = false;
            this.txtFloppyA.Location = new System.Drawing.Point(119, 19);
            this.txtFloppyA.Name = "txtFloppyA";
            this.txtFloppyA.Size = new System.Drawing.Size(340, 22);
            this.txtFloppyA.TabIndex = 4;
            // 
            // txtFloppyB
            // 
            this.txtFloppyB.Enabled = false;
            this.txtFloppyB.Location = new System.Drawing.Point(119, 49);
            this.txtFloppyB.Name = "txtFloppyB";
            this.txtFloppyB.Size = new System.Drawing.Size(340, 22);
            this.txtFloppyB.TabIndex = 5;
            // 
            // pictureBox20
            // 
            this.pictureBox20.Image = global::Qemu_GUI.Properties.Resources.save;
            this.pictureBox20.Location = new System.Drawing.Point(5, 21);
            this.pictureBox20.Name = "pictureBox20";
            this.pictureBox20.Size = new System.Drawing.Size(16, 16);
            this.pictureBox20.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox20.TabIndex = 25;
            this.pictureBox20.TabStop = false;
            // 
            // pictureBox21
            // 
            this.pictureBox21.Image = global::Qemu_GUI.Properties.Resources.save;
            this.pictureBox21.Location = new System.Drawing.Point(5, 51);
            this.pictureBox21.Name = "pictureBox21";
            this.pictureBox21.Size = new System.Drawing.Size(16, 16);
            this.pictureBox21.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox21.TabIndex = 26;
            this.pictureBox21.TabStop = false;
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.pictureBox22);
            this.groupBox9.Controls.Add(this.lblImageSizeMB);
            this.groupBox9.Controls.Add(this.label11);
            this.groupBox9.Controls.Add(this.txtImageSize);
            this.groupBox9.Controls.Add(this.lblImageSize);
            this.groupBox9.Controls.Add(this.btnCreateImage);
            this.groupBox9.Controls.Add(this.cboImageFormat);
            this.groupBox9.Location = new System.Drawing.Point(10, 76);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(532, 63);
            this.groupBox9.TabIndex = 7;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Create Hard Disk Images";
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
            this.cboImageFormat.Location = new System.Drawing.Point(264, 25);
            this.cboImageFormat.Name = "cboImageFormat";
            this.cboImageFormat.Size = new System.Drawing.Size(102, 24);
            this.cboImageFormat.TabIndex = 7;
            // 
            // btnCreateImage
            // 
            this.btnCreateImage.Location = new System.Drawing.Point(387, 26);
            this.btnCreateImage.Name = "btnCreateImage";
            this.btnCreateImage.Size = new System.Drawing.Size(127, 23);
            this.btnCreateImage.TabIndex = 1;
            this.btnCreateImage.Text = "Create Image";
            this.btnCreateImage.Click += new System.EventHandler(this.btnCreateImage_Click);
            // 
            // lblImageSize
            // 
            this.lblImageSize.Location = new System.Drawing.Point(32, 27);
            this.lblImageSize.Name = "lblImageSize";
            this.lblImageSize.Size = new System.Drawing.Size(45, 23);
            this.lblImageSize.TabIndex = 2;
            this.lblImageSize.Text = "Size";
            // 
            // txtImageSize
            // 
            this.txtImageSize.Location = new System.Drawing.Point(83, 25);
            this.txtImageSize.Name = "txtImageSize";
            this.txtImageSize.Size = new System.Drawing.Size(64, 22);
            this.txtImageSize.TabIndex = 3;
            this.txtImageSize.Text = "512";
            // 
            // label11
            // 
            this.label11.Location = new System.Drawing.Point(201, 27);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(57, 20);
            this.label11.TabIndex = 5;
            this.label11.Text = "Format:";
            // 
            // lblImageSizeMB
            // 
            this.lblImageSizeMB.AutoSize = true;
            this.lblImageSizeMB.Location = new System.Drawing.Point(153, 28);
            this.lblImageSizeMB.Name = "lblImageSizeMB";
            this.lblImageSizeMB.Size = new System.Drawing.Size(28, 16);
            this.lblImageSizeMB.TabIndex = 8;
            this.lblImageSizeMB.Text = "MB";
            // 
            // pictureBox22
            // 
            this.pictureBox22.Image = global::Qemu_GUI.Properties.Resources.preferences_system;
            this.pictureBox22.Location = new System.Drawing.Point(5, 27);
            this.pictureBox22.Name = "pictureBox22";
            this.pictureBox22.Size = new System.Drawing.Size(16, 16);
            this.pictureBox22.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox22.TabIndex = 28;
            this.pictureBox22.TabStop = false;
            // 
            // grpVDK
            // 
            this.grpVDK.Controls.Add(this.pictureBox23);
            this.grpVDK.Controls.Add(this.label9);
            this.grpVDK.Controls.Add(this.txtVDKImage);
            this.grpVDK.Controls.Add(this.cboVDKDrive);
            this.grpVDK.Controls.Add(this.lblDrive);
            this.grpVDK.Controls.Add(this.btnVDKImage);
            this.grpVDK.Controls.Add(this.btnUnmount);
            this.grpVDK.Controls.Add(this.btnMount);
            this.grpVDK.Controls.Add(this.lblImage);
            this.grpVDK.Location = new System.Drawing.Point(10, 3);
            this.grpVDK.Name = "grpVDK";
            this.grpVDK.Size = new System.Drawing.Size(532, 67);
            this.grpVDK.TabIndex = 8;
            this.grpVDK.TabStop = false;
            this.grpVDK.Text = "Disk Mount - (VDK) must run in admin mode.";
            // 
            // lblImage
            // 
            this.lblImage.AutoSize = true;
            this.lblImage.Location = new System.Drawing.Point(28, 27);
            this.lblImage.Name = "lblImage";
            this.lblImage.Size = new System.Drawing.Size(49, 16);
            this.lblImage.TabIndex = 7;
            this.lblImage.Text = "Image:";
            // 
            // btnMount
            // 
            this.btnMount.Location = new System.Drawing.Point(320, 27);
            this.btnMount.Name = "btnMount";
            this.btnMount.Size = new System.Drawing.Size(93, 23);
            this.btnMount.TabIndex = 3;
            this.btnMount.Text = "Mount";
            this.btnMount.Click += new System.EventHandler(this.btnMount_Click);
            // 
            // btnUnmount
            // 
            this.btnUnmount.Location = new System.Drawing.Point(419, 27);
            this.btnUnmount.Name = "btnUnmount";
            this.btnUnmount.Size = new System.Drawing.Size(93, 23);
            this.btnUnmount.TabIndex = 5;
            this.btnUnmount.Text = "Unmount";
            this.btnUnmount.Click += new System.EventHandler(this.btnUnmount_Click);
            // 
            // btnVDKImage
            // 
            this.btnVDKImage.Location = new System.Drawing.Point(182, 27);
            this.btnVDKImage.Name = "btnVDKImage";
            this.btnVDKImage.Size = new System.Drawing.Size(27, 22);
            this.btnVDKImage.TabIndex = 8;
            this.btnVDKImage.Text = "...";
            this.btnVDKImage.UseVisualStyleBackColor = true;
            this.btnVDKImage.Click += new System.EventHandler(this.btnVDKImage_Click);
            // 
            // lblDrive
            // 
            this.lblDrive.AutoSize = true;
            this.lblDrive.Location = new System.Drawing.Point(215, 29);
            this.lblDrive.Name = "lblDrive";
            this.lblDrive.Size = new System.Drawing.Size(43, 16);
            this.lblDrive.TabIndex = 9;
            this.lblDrive.Text = "Drive:";
            // 
            // cboVDKDrive
            // 
            this.cboVDKDrive.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboVDKDrive.FormattingEnabled = true;
            this.cboVDKDrive.Location = new System.Drawing.Point(264, 27);
            this.cboVDKDrive.Name = "cboVDKDrive";
            this.cboVDKDrive.Size = new System.Drawing.Size(50, 24);
            this.cboVDKDrive.TabIndex = 10;
            // 
            // txtVDKImage
            // 
            this.txtVDKImage.Location = new System.Drawing.Point(83, 27);
            this.txtVDKImage.Name = "txtVDKImage";
            this.txtVDKImage.Size = new System.Drawing.Size(93, 22);
            this.txtVDKImage.TabIndex = 6;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(116, 102);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(280, 16);
            this.label9.TabIndex = 11;
            this.label9.Text = "Application must be run in administrator mode.";
            // 
            // pictureBox23
            // 
            this.pictureBox23.Image = global::Qemu_GUI.Properties.Resources.mount;
            this.pictureBox23.Location = new System.Drawing.Point(5, 27);
            this.pictureBox23.Name = "pictureBox23";
            this.pictureBox23.Size = new System.Drawing.Size(16, 16);
            this.pictureBox23.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox23.TabIndex = 29;
            this.pictureBox23.TabStop = false;
            // 
            // tabGen
            // 
            this.tabGen.Controls.Add(this.groupBox5);
            this.tabGen.Controls.Add(this.groupBox2);
            this.tabGen.Controls.Add(this.grpACPI);
            this.tabGen.Controls.Add(this.groupBox6);
            this.tabGen.Controls.Add(this.grpMachine);
            this.tabGen.Controls.Add(this.grpClock);
            this.tabGen.Controls.Add(this.grpDisplay);
            this.tabGen.Location = new System.Drawing.Point(4, 25);
            this.tabGen.Name = "tabGen";
            this.tabGen.Size = new System.Drawing.Size(549, 387);
            this.tabGen.TabIndex = 1;
            this.tabGen.Text = "Main";
            this.tabGen.UseVisualStyleBackColor = true;
            // 
            // grpDisplay
            // 
            this.grpDisplay.Controls.Add(this.pictureBox12);
            this.grpDisplay.Controls.Add(this.pictureBox11);
            this.grpDisplay.Controls.Add(this.chkVGAoutput);
            this.grpDisplay.Controls.Add(this.chkFullscreen);
            this.grpDisplay.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.grpDisplay.Location = new System.Drawing.Point(281, 122);
            this.grpDisplay.Name = "grpDisplay";
            this.grpDisplay.Size = new System.Drawing.Size(260, 92);
            this.grpDisplay.TabIndex = 19;
            this.grpDisplay.TabStop = false;
            this.grpDisplay.Text = "Display";
            // 
            // chkFullscreen
            // 
            this.chkFullscreen.AutoSize = true;
            this.chkFullscreen.Location = new System.Drawing.Point(36, 27);
            this.chkFullscreen.Name = "chkFullscreen";
            this.chkFullscreen.Size = new System.Drawing.Size(166, 20);
            this.chkFullscreen.TabIndex = 14;
            this.chkFullscreen.Text = "Start Qemu in fullscreen";
            this.chkFullscreen.UseVisualStyleBackColor = true;
            // 
            // chkVGAoutput
            // 
            this.chkVGAoutput.AutoSize = true;
            this.chkVGAoutput.Location = new System.Drawing.Point(36, 57);
            this.chkVGAoutput.Name = "chkVGAoutput";
            this.chkVGAoutput.Size = new System.Drawing.Size(126, 20);
            this.chkVGAoutput.TabIndex = 15;
            this.chkVGAoutput.Text = " VGA Display Off";
            this.chkVGAoutput.UseVisualStyleBackColor = true;
            // 
            // pictureBox11
            // 
            this.pictureBox11.Image = global::Qemu_GUI.Properties.Resources.screenshot;
            this.pictureBox11.Location = new System.Drawing.Point(6, 27);
            this.pictureBox11.Name = "pictureBox11";
            this.pictureBox11.Size = new System.Drawing.Size(16, 16);
            this.pictureBox11.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox11.TabIndex = 25;
            this.pictureBox11.TabStop = false;
            // 
            // pictureBox12
            // 
            this.pictureBox12.Image = global::Qemu_GUI.Properties.Resources.screenshot;
            this.pictureBox12.Location = new System.Drawing.Point(6, 57);
            this.pictureBox12.Name = "pictureBox12";
            this.pictureBox12.Size = new System.Drawing.Size(16, 16);
            this.pictureBox12.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox12.TabIndex = 26;
            this.pictureBox12.TabStop = false;
            // 
            // grpClock
            // 
            this.grpClock.Controls.Add(this.pictureBox9);
            this.grpClock.Controls.Add(this.chkSetClock);
            this.grpClock.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.grpClock.Location = new System.Drawing.Point(281, 3);
            this.grpClock.Name = "grpClock";
            this.grpClock.Size = new System.Drawing.Size(260, 54);
            this.grpClock.TabIndex = 18;
            this.grpClock.TabStop = false;
            this.grpClock.Text = "Clock";
            // 
            // chkSetClock
            // 
            this.chkSetClock.AutoSize = true;
            this.chkSetClock.Checked = true;
            this.chkSetClock.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSetClock.Location = new System.Drawing.Point(36, 23);
            this.chkSetClock.Name = "chkSetClock";
            this.chkSetClock.Size = new System.Drawing.Size(152, 20);
            this.chkSetClock.TabIndex = 12;
            this.chkSetClock.Text = "Set clock to host time";
            this.chkSetClock.UseVisualStyleBackColor = true;
            // 
            // pictureBox9
            // 
            this.pictureBox9.Image = global::Qemu_GUI.Properties.Resources.snapshot;
            this.pictureBox9.Location = new System.Drawing.Point(6, 25);
            this.pictureBox9.Name = "pictureBox9";
            this.pictureBox9.Size = new System.Drawing.Size(16, 16);
            this.pictureBox9.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox9.TabIndex = 23;
            this.pictureBox9.TabStop = false;
            // 
            // grpMachine
            // 
            this.grpMachine.Controls.Add(this.pictureBox5);
            this.grpMachine.Controls.Add(this.cboMachine);
            this.grpMachine.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.grpMachine.Location = new System.Drawing.Point(3, 3);
            this.grpMachine.Name = "grpMachine";
            this.grpMachine.Size = new System.Drawing.Size(272, 54);
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
            this.cboMachine.Location = new System.Drawing.Point(28, 19);
            this.cboMachine.Name = "cboMachine";
            this.cboMachine.Size = new System.Drawing.Size(206, 24);
            this.cboMachine.TabIndex = 4;
            // 
            // pictureBox5
            // 
            this.pictureBox5.Image = global::Qemu_GUI.Properties.Resources.machine;
            this.pictureBox5.Location = new System.Drawing.Point(6, 25);
            this.pictureBox5.Name = "pictureBox5";
            this.pictureBox5.Size = new System.Drawing.Size(16, 16);
            this.pictureBox5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox5.TabIndex = 21;
            this.pictureBox5.TabStop = false;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.pictureBox8);
            this.groupBox6.Controls.Add(this.pictureBox7);
            this.groupBox6.Controls.Add(this.lblCPUs);
            this.groupBox6.Controls.Add(this.pictureBox6);
            this.groupBox6.Controls.Add(this.chkKQEmu);
            this.groupBox6.Controls.Add(this.numSMP);
            this.groupBox6.Controls.Add(this.numMemory);
            this.groupBox6.Controls.Add(this.lblMB);
            this.groupBox6.Controls.Add(this.lblMemory);
            this.groupBox6.Controls.Add(this.lblBootFrom);
            this.groupBox6.Controls.Add(this.lblSMP);
            this.groupBox6.Controls.Add(this.cboBootFrom);
            this.groupBox6.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.groupBox6.Location = new System.Drawing.Point(3, 63);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(272, 152);
            this.groupBox6.TabIndex = 4;
            this.groupBox6.TabStop = false;
            // 
            // cboBootFrom
            // 
            this.cboBootFrom.DisplayMember = "1";
            this.cboBootFrom.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboBootFrom.Items.AddRange(new object[] {
            "Floppy",
            "Harddisk",
            "CD-ROM"});
            this.cboBootFrom.Location = new System.Drawing.Point(113, 21);
            this.cboBootFrom.Name = "cboBootFrom";
            this.cboBootFrom.Size = new System.Drawing.Size(121, 24);
            this.cboBootFrom.TabIndex = 5;
            // 
            // lblSMP
            // 
            this.lblSMP.Location = new System.Drawing.Point(36, 95);
            this.lblSMP.Name = "lblSMP";
            this.lblSMP.Size = new System.Drawing.Size(117, 23);
            this.lblSMP.TabIndex = 7;
            this.lblSMP.Text = "SMP";
            // 
            // lblBootFrom
            // 
            this.lblBootFrom.Location = new System.Drawing.Point(36, 24);
            this.lblBootFrom.Name = "lblBootFrom";
            this.lblBootFrom.Size = new System.Drawing.Size(71, 23);
            this.lblBootFrom.TabIndex = 2;
            this.lblBootFrom.Text = "Boot from";
            // 
            // lblMemory
            // 
            this.lblMemory.Location = new System.Drawing.Point(36, 58);
            this.lblMemory.Name = "lblMemory";
            this.lblMemory.Size = new System.Drawing.Size(102, 23);
            this.lblMemory.TabIndex = 6;
            this.lblMemory.Text = "Memory";
            // 
            // lblMB
            // 
            this.lblMB.AutoSize = true;
            this.lblMB.Location = new System.Drawing.Point(186, 58);
            this.lblMB.Name = "lblMB";
            this.lblMB.Size = new System.Drawing.Size(28, 17);
            this.lblMB.TabIndex = 11;
            this.lblMB.Text = "MB";
            // 
            // numMemory
            // 
            this.numMemory.Location = new System.Drawing.Point(120, 58);
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
            this.numMemory.Size = new System.Drawing.Size(55, 23);
            this.numMemory.TabIndex = 6;
            this.numMemory.Value = new decimal(new int[] {
            128,
            0,
            0,
            0});
            // 
            // numSMP
            // 
            this.numSMP.Location = new System.Drawing.Point(120, 91);
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
            this.numSMP.Size = new System.Drawing.Size(56, 23);
            this.numSMP.TabIndex = 7;
            this.numSMP.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // chkKQEmu
            // 
            this.chkKQEmu.AutoSize = true;
            this.chkKQEmu.Checked = true;
            this.chkKQEmu.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkKQEmu.Location = new System.Drawing.Point(8, 121);
            this.chkKQEmu.Name = "chkKQEmu";
            this.chkKQEmu.Size = new System.Drawing.Size(229, 21);
            this.chkKQEmu.TabIndex = 18;
            this.chkKQEmu.Text = "Use KQEmu accelerator module";
            this.chkKQEmu.UseVisualStyleBackColor = true;
            // 
            // pictureBox6
            // 
            this.pictureBox6.Image = global::Qemu_GUI.Properties.Resources.advanced;
            this.pictureBox6.Location = new System.Drawing.Point(5, 21);
            this.pictureBox6.Name = "pictureBox6";
            this.pictureBox6.Size = new System.Drawing.Size(24, 24);
            this.pictureBox6.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox6.TabIndex = 22;
            this.pictureBox6.TabStop = false;
            // 
            // lblCPUs
            // 
            this.lblCPUs.AutoSize = true;
            this.lblCPUs.Location = new System.Drawing.Point(186, 94);
            this.lblCPUs.Name = "lblCPUs";
            this.lblCPUs.Size = new System.Drawing.Size(53, 17);
            this.lblCPUs.TabIndex = 14;
            this.lblCPUs.Text = "CPU(s)";
            // 
            // pictureBox7
            // 
            this.pictureBox7.Image = global::Qemu_GUI.Properties.Resources.memory;
            this.pictureBox7.Location = new System.Drawing.Point(5, 58);
            this.pictureBox7.Name = "pictureBox7";
            this.pictureBox7.Size = new System.Drawing.Size(24, 24);
            this.pictureBox7.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox7.TabIndex = 23;
            this.pictureBox7.TabStop = false;
            // 
            // pictureBox8
            // 
            this.pictureBox8.Image = global::Qemu_GUI.Properties.Resources.machine;
            this.pictureBox8.Location = new System.Drawing.Point(6, 94);
            this.pictureBox8.Name = "pictureBox8";
            this.pictureBox8.Size = new System.Drawing.Size(16, 16);
            this.pictureBox8.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox8.TabIndex = 24;
            this.pictureBox8.TabStop = false;
            // 
            // grpACPI
            // 
            this.grpACPI.Controls.Add(this.pictureBox10);
            this.grpACPI.Controls.Add(this.chkACPI);
            this.grpACPI.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.grpACPI.Location = new System.Drawing.Point(281, 63);
            this.grpACPI.Name = "grpACPI";
            this.grpACPI.Size = new System.Drawing.Size(260, 53);
            this.grpACPI.TabIndex = 20;
            this.grpACPI.TabStop = false;
            this.grpACPI.Text = "ACPI";
            // 
            // chkACPI
            // 
            this.chkACPI.AutoSize = true;
            this.chkACPI.Checked = true;
            this.chkACPI.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkACPI.Location = new System.Drawing.Point(36, 27);
            this.chkACPI.Name = "chkACPI";
            this.chkACPI.Size = new System.Drawing.Size(103, 20);
            this.chkACPI.TabIndex = 13;
            this.chkACPI.Text = "Enable ACPI";
            this.chkACPI.UseVisualStyleBackColor = true;
            // 
            // pictureBox10
            // 
            this.pictureBox10.Image = global::Qemu_GUI.Properties.Resources.power;
            this.pictureBox10.Location = new System.Drawing.Point(6, 24);
            this.pictureBox10.Name = "pictureBox10";
            this.pictureBox10.Size = new System.Drawing.Size(24, 24);
            this.pictureBox10.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox10.TabIndex = 24;
            this.pictureBox10.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.pictureBox16);
            this.groupBox2.Controls.Add(this.pictureBox15);
            this.groupBox2.Controls.Add(this.txtQEmuPath);
            this.groupBox2.Controls.Add(this.txtVDKPath);
            this.groupBox2.Controls.Add(this.btnVDKBrowse);
            this.groupBox2.Controls.Add(this.lblVDKPath);
            this.groupBox2.Controls.Add(this.btnQEmuPath);
            this.groupBox2.Controls.Add(this.lblQEmuPath);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox2.Location = new System.Drawing.Point(3, 220);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(272, 149);
            this.groupBox2.TabIndex = 22;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Paths";
            // 
            // lblQEmuPath
            // 
            this.lblQEmuPath.AutoSize = true;
            this.lblQEmuPath.Location = new System.Drawing.Point(25, 24);
            this.lblQEmuPath.Name = "lblQEmuPath";
            this.lblQEmuPath.Size = new System.Drawing.Size(44, 16);
            this.lblQEmuPath.TabIndex = 2;
            this.lblQEmuPath.Text = "Qemu";
            // 
            // btnQEmuPath
            // 
            this.btnQEmuPath.Location = new System.Drawing.Point(201, 43);
            this.btnQEmuPath.Name = "btnQEmuPath";
            this.btnQEmuPath.Size = new System.Drawing.Size(47, 22);
            this.btnQEmuPath.TabIndex = 9;
            this.btnQEmuPath.Text = "...";
            this.btnQEmuPath.UseVisualStyleBackColor = true;
            this.btnQEmuPath.Click += new System.EventHandler(this.btnQEmuPath_Click);
            // 
            // lblVDKPath
            // 
            this.lblVDKPath.AutoSize = true;
            this.lblVDKPath.Location = new System.Drawing.Point(21, 76);
            this.lblVDKPath.Name = "lblVDKPath";
            this.lblVDKPath.Size = new System.Drawing.Size(35, 16);
            this.lblVDKPath.TabIndex = 3;
            this.lblVDKPath.Text = "VDK";
            // 
            // btnVDKBrowse
            // 
            this.btnVDKBrowse.Location = new System.Drawing.Point(201, 95);
            this.btnVDKBrowse.Name = "btnVDKBrowse";
            this.btnVDKBrowse.Size = new System.Drawing.Size(47, 22);
            this.btnVDKBrowse.TabIndex = 11;
            this.btnVDKBrowse.Text = "...";
            this.btnVDKBrowse.UseVisualStyleBackColor = true;
            this.btnVDKBrowse.Click += new System.EventHandler(this.btnVDKBrowse_Click);
            // 
            // txtVDKPath
            // 
            this.txtVDKPath.Location = new System.Drawing.Point(32, 95);
            this.txtVDKPath.Name = "txtVDKPath";
            this.txtVDKPath.Size = new System.Drawing.Size(156, 22);
            this.txtVDKPath.TabIndex = 10;
            // 
            // txtQEmuPath
            // 
            this.txtQEmuPath.Location = new System.Drawing.Point(32, 43);
            this.txtQEmuPath.Name = "txtQEmuPath";
            this.txtQEmuPath.Size = new System.Drawing.Size(156, 22);
            this.txtQEmuPath.TabIndex = 8;
            // 
            // pictureBox15
            // 
            this.pictureBox15.Image = global::Qemu_GUI.Properties.Resources.save;
            this.pictureBox15.Location = new System.Drawing.Point(6, 41);
            this.pictureBox15.Name = "pictureBox15";
            this.pictureBox15.Size = new System.Drawing.Size(16, 16);
            this.pictureBox15.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox15.TabIndex = 24;
            this.pictureBox15.TabStop = false;
            // 
            // pictureBox16
            // 
            this.pictureBox16.Image = global::Qemu_GUI.Properties.Resources.save;
            this.pictureBox16.Location = new System.Drawing.Point(6, 95);
            this.pictureBox16.Name = "pictureBox16";
            this.pictureBox16.Size = new System.Drawing.Size(16, 16);
            this.pictureBox16.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox16.TabIndex = 25;
            this.pictureBox16.TabStop = false;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.pictureBox14);
            this.groupBox5.Controls.Add(this.pictureBox13);
            this.groupBox5.Controls.Add(this.chkUseHDA);
            this.groupBox5.Controls.Add(this.btnBrowseHDA);
            this.groupBox5.Controls.Add(this.txtHDA);
            this.groupBox5.Controls.Add(this.chkUseCDROM);
            this.groupBox5.Controls.Add(this.txtCDROM);
            this.groupBox5.Controls.Add(this.btnBrowseCDROM);
            this.groupBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox5.Location = new System.Drawing.Point(281, 220);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(260, 149);
            this.groupBox5.TabIndex = 24;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Media";
            // 
            // btnBrowseCDROM
            // 
            this.btnBrowseCDROM.Enabled = false;
            this.btnBrowseCDROM.Location = new System.Drawing.Point(194, 113);
            this.btnBrowseCDROM.Name = "btnBrowseCDROM";
            this.btnBrowseCDROM.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseCDROM.TabIndex = 21;
            this.btnBrowseCDROM.Text = "...";
            this.btnBrowseCDROM.Click += new System.EventHandler(this.btnBrowseCDROM_Click);
            // 
            // txtCDROM
            // 
            this.txtCDROM.Enabled = false;
            this.txtCDROM.Location = new System.Drawing.Point(36, 113);
            this.txtCDROM.Name = "txtCDROM";
            this.txtCDROM.Size = new System.Drawing.Size(152, 22);
            this.txtCDROM.TabIndex = 20;
            // 
            // chkUseCDROM
            // 
            this.chkUseCDROM.Location = new System.Drawing.Point(36, 83);
            this.chkUseCDROM.Name = "chkUseCDROM";
            this.chkUseCDROM.Size = new System.Drawing.Size(99, 24);
            this.chkUseCDROM.TabIndex = 19;
            this.chkUseCDROM.Text = "CD-ROM";
            this.chkUseCDROM.CheckedChanged += new System.EventHandler(this.chkUseCDROM_CheckedChanged);
            // 
            // txtHDA
            // 
            this.txtHDA.Enabled = false;
            this.txtHDA.Location = new System.Drawing.Point(36, 51);
            this.txtHDA.Name = "txtHDA";
            this.txtHDA.Size = new System.Drawing.Size(152, 22);
            this.txtHDA.TabIndex = 17;
            // 
            // btnBrowseHDA
            // 
            this.btnBrowseHDA.Enabled = false;
            this.btnBrowseHDA.Location = new System.Drawing.Point(194, 51);
            this.btnBrowseHDA.Name = "btnBrowseHDA";
            this.btnBrowseHDA.Size = new System.Drawing.Size(47, 22);
            this.btnBrowseHDA.TabIndex = 18;
            this.btnBrowseHDA.Text = " ...";
            this.btnBrowseHDA.Click += new System.EventHandler(this.btnBrowseHDA_Click);
            // 
            // chkUseHDA
            // 
            this.chkUseHDA.Location = new System.Drawing.Point(36, 21);
            this.chkUseHDA.Name = "chkUseHDA";
            this.chkUseHDA.Size = new System.Drawing.Size(156, 24);
            this.chkUseHDA.TabIndex = 16;
            this.chkUseHDA.Text = "Hard Disk 1";
            this.chkUseHDA.CheckedChanged += new System.EventHandler(this.chkUseHDA_CheckedChanged);
            // 
            // pictureBox13
            // 
            this.pictureBox13.Image = global::Qemu_GUI.Properties.Resources.harddisk;
            this.pictureBox13.Location = new System.Drawing.Point(6, 24);
            this.pictureBox13.Name = "pictureBox13";
            this.pictureBox13.Size = new System.Drawing.Size(24, 24);
            this.pictureBox13.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox13.TabIndex = 25;
            this.pictureBox13.TabStop = false;
            // 
            // pictureBox14
            // 
            this.pictureBox14.Image = global::Qemu_GUI.Properties.Resources.cdrom;
            this.pictureBox14.Location = new System.Drawing.Point(6, 83);
            this.pictureBox14.Name = "pictureBox14";
            this.pictureBox14.Size = new System.Drawing.Size(24, 24);
            this.pictureBox14.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox14.TabIndex = 26;
            this.pictureBox14.TabStop = false;
            // 
            // Tabs
            // 
            this.Tabs.Controls.Add(this.tabGen);
            this.Tabs.Controls.Add(this.tabHardDisk);
            this.Tabs.Controls.Add(this.tabNetwork);
            this.Tabs.Controls.Add(this.tabDebug);
            this.Tabs.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.Tabs.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Tabs.Location = new System.Drawing.Point(0, 57);
            this.Tabs.Name = "Tabs";
            this.Tabs.SelectedIndex = 0;
            this.Tabs.Size = new System.Drawing.Size(557, 416);
            this.Tabs.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 15);
            this.ClientSize = new System.Drawing.Size(557, 473);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnLaunch);
            this.Controls.Add(this.Tabs);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "ReactOS Qemu GUI";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabDebug.ResumeLayout(false);
            this.tabDebug.PerformLayout();
            this.groupBox16.ResumeLayout(false);
            this.groupBox16.PerformLayout();
            this.grpGDB.ResumeLayout(false);
            this.grpGDB.PerformLayout();
            this.grpVNC.ResumeLayout(false);
            this.grpVNC.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabNetwork.ResumeLayout(false);
            this.grpNetwork.ResumeLayout(false);
            this.grpNetwork.PerformLayout();
            this.panelNic.ResumeLayout(false);
            this.panelNic.PerformLayout();
            this.panelUser.ResumeLayout(false);
            this.panelUser.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox27)).EndInit();
            this.grpAudio.ResumeLayout(false);
            this.grpAudio.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.grpSerial.ResumeLayout(false);
            this.grpSerial.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox24)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox25)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox26)).EndInit();
            this.tabHardDisk.ResumeLayout(false);
            this.grpHarddisk.ResumeLayout(false);
            this.grpHarddisk.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox17)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox18)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox19)).EndInit();
            this.grpFloppy.ResumeLayout(false);
            this.grpFloppy.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox20)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox21)).EndInit();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox22)).EndInit();
            this.grpVDK.ResumeLayout(false);
            this.grpVDK.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox23)).EndInit();
            this.tabGen.ResumeLayout(false);
            this.grpDisplay.ResumeLayout(false);
            this.grpDisplay.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox11)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox12)).EndInit();
            this.grpClock.ResumeLayout(false);
            this.grpClock.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox9)).EndInit();
            this.grpMachine.ResumeLayout(false);
            this.grpMachine.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).EndInit();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox7)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox8)).EndInit();
            this.grpACPI.ResumeLayout(false);
            this.grpACPI.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox10)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox15)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox16)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox13)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox14)).EndInit();
            this.Tabs.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        #endregion

        #region declarations
        private System.Windows.Forms.Button btnLaunch;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.OpenFileDialog openFile;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpContentsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutReactOSQEmuManagerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadConfigToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveConfigToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        #endregion
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.TabPage tabDebug;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txtLinuxK;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox txtAppendCmd;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button btnBrowseBios;
        private System.Windows.Forms.TextBox txtABios;
        private System.Windows.Forms.GroupBox grpVNC;
        private System.Windows.Forms.TextBox txtVNC;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.CheckBox chkVNC;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox chkVBE30;
        private System.Windows.Forms.GroupBox grpGDB;
        private System.Windows.Forms.Label lblGDBPort;
        private System.Windows.Forms.TextBox txtGDBPort;
        private System.Windows.Forms.CheckBox checkBox14;
        private System.Windows.Forms.GroupBox groupBox16;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.Button button20;
        private System.Windows.Forms.TabPage tabNetwork;
        private System.Windows.Forms.GroupBox grpSerial;
        private System.Windows.Forms.PictureBox pictureBox26;
        private System.Windows.Forms.PictureBox pictureBox25;
        private System.Windows.Forms.PictureBox pictureBox24;
        private System.Windows.Forms.CheckBox chkSerialToScrn;
        private System.Windows.Forms.TextBox txtSerialPipe;
        private System.Windows.Forms.CheckBox chkSerialToPipe;
        private System.Windows.Forms.TextBox txtSerialFile;
        private System.Windows.Forms.Button btnBrowseSerial;
        private System.Windows.Forms.CheckBox chkSerialToFile;
        private System.Windows.Forms.GroupBox grpAudio;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.CheckBox chkSoundBlaster;
        private System.Windows.Forms.CheckBox chkPCSpeaker;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.CheckBox chkES1370;
        private System.Windows.Forms.CheckBox chkOPL2;
        private System.Windows.Forms.GroupBox grpNetwork;
        private System.Windows.Forms.PictureBox pictureBox27;
        private System.Windows.Forms.Panel panelUser;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtNetHost;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RadioButton rbtnNetUser;
        private System.Windows.Forms.Panel panelNic;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtNicMACaddr;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cmbNICmodels;
        private System.Windows.Forms.RadioButton rbtnNetNic;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnNetRemove;
        private System.Windows.Forms.Button btnNetAdd;
        private System.Windows.Forms.ListView listVLANs;
        private System.Windows.Forms.TabPage tabHardDisk;
        private System.Windows.Forms.GroupBox grpVDK;
        private System.Windows.Forms.PictureBox pictureBox23;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtVDKImage;
        private System.Windows.Forms.ComboBox cboVDKDrive;
        private System.Windows.Forms.Label lblDrive;
        private System.Windows.Forms.Button btnVDKImage;
        private System.Windows.Forms.Button btnUnmount;
        private System.Windows.Forms.Button btnMount;
        private System.Windows.Forms.Label lblImage;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.PictureBox pictureBox22;
        private System.Windows.Forms.Label lblImageSizeMB;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox txtImageSize;
        private System.Windows.Forms.Label lblImageSize;
        private System.Windows.Forms.Button btnCreateImage;
        private System.Windows.Forms.ComboBox cboImageFormat;
        private System.Windows.Forms.GroupBox grpFloppy;
        private System.Windows.Forms.PictureBox pictureBox21;
        private System.Windows.Forms.PictureBox pictureBox20;
        private System.Windows.Forms.TextBox txtFloppyB;
        private System.Windows.Forms.TextBox txtFloppyA;
        private System.Windows.Forms.Button btnBrowseFloppyB;
        private System.Windows.Forms.Button btnBrowseFloppyA;
        private System.Windows.Forms.CheckBox chkFloppyB;
        private System.Windows.Forms.CheckBox chkFloppyA;
        private System.Windows.Forms.GroupBox grpHarddisk;
        private System.Windows.Forms.PictureBox pictureBox19;
        private System.Windows.Forms.PictureBox pictureBox18;
        private System.Windows.Forms.PictureBox pictureBox17;
        private System.Windows.Forms.TextBox txtHDD;
        private System.Windows.Forms.TextBox txtHDC;
        private System.Windows.Forms.TextBox txtHDB;
        private System.Windows.Forms.Button btnBrowseHDD;
        private System.Windows.Forms.Button btnBrowseHDC;
        private System.Windows.Forms.CheckBox chkUseHDD;
        private System.Windows.Forms.CheckBox chkUseHDC;
        private System.Windows.Forms.Button btnBrowseHDB;
        private System.Windows.Forms.CheckBox chkUseHDB;
        private System.Windows.Forms.TabPage tabGen;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.PictureBox pictureBox14;
        private System.Windows.Forms.PictureBox pictureBox13;
        private System.Windows.Forms.CheckBox chkUseHDA;
        private System.Windows.Forms.Button btnBrowseHDA;
        private System.Windows.Forms.TextBox txtHDA;
        private System.Windows.Forms.CheckBox chkUseCDROM;
        private System.Windows.Forms.TextBox txtCDROM;
        private System.Windows.Forms.Button btnBrowseCDROM;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.PictureBox pictureBox16;
        private System.Windows.Forms.PictureBox pictureBox15;
        private System.Windows.Forms.TextBox txtQEmuPath;
        private System.Windows.Forms.TextBox txtVDKPath;
        private System.Windows.Forms.Button btnVDKBrowse;
        private System.Windows.Forms.Label lblVDKPath;
        private System.Windows.Forms.Button btnQEmuPath;
        private System.Windows.Forms.Label lblQEmuPath;
        private System.Windows.Forms.GroupBox grpACPI;
        private System.Windows.Forms.PictureBox pictureBox10;
        private System.Windows.Forms.CheckBox chkACPI;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.PictureBox pictureBox8;
        private System.Windows.Forms.PictureBox pictureBox7;
        private System.Windows.Forms.Label lblCPUs;
        private System.Windows.Forms.PictureBox pictureBox6;
        private System.Windows.Forms.CheckBox chkKQEmu;
        private System.Windows.Forms.NumericUpDown numSMP;
        private System.Windows.Forms.NumericUpDown numMemory;
        private System.Windows.Forms.Label lblMB;
        private System.Windows.Forms.Label lblMemory;
        private System.Windows.Forms.Label lblBootFrom;
        private System.Windows.Forms.Label lblSMP;
        private System.Windows.Forms.ComboBox cboBootFrom;
        private System.Windows.Forms.GroupBox grpMachine;
        private System.Windows.Forms.PictureBox pictureBox5;
        private System.Windows.Forms.ComboBox cboMachine;
        private System.Windows.Forms.GroupBox grpClock;
        private System.Windows.Forms.PictureBox pictureBox9;
        private System.Windows.Forms.CheckBox chkSetClock;
        private System.Windows.Forms.GroupBox grpDisplay;
        private System.Windows.Forms.PictureBox pictureBox12;
        private System.Windows.Forms.PictureBox pictureBox11;
        private System.Windows.Forms.CheckBox chkVGAoutput;
        private System.Windows.Forms.CheckBox chkFullscreen;
        private System.Windows.Forms.TabControl Tabs;
    }
}
