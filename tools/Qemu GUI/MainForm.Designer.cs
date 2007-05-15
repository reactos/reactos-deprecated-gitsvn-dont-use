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
            this.btnLaunch = new System.Windows.Forms.Button();
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.btnLoad = new System.Windows.Forms.Button();
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
            this.tabOther = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label10 = new System.Windows.Forms.Label();
            this.txtAppendCmd = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.btnBrowseBios = new System.Windows.Forms.Button();
            this.txtABios = new System.Windows.Forms.TextBox();
            this.grpVNC = new System.Windows.Forms.GroupBox();
            this.txtVNC = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.chkVNC = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.btnBrowseLinuxK = new System.Windows.Forms.Button();
            this.txtLinuxK = new System.Windows.Forms.TextBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.chkVBE30 = new System.Windows.Forms.CheckBox();
            this.tabTools = new System.Windows.Forms.TabPage();
            this.grpVDK = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
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
            this.tabDebug = new System.Windows.Forms.TabPage();
            this.grpGDB = new System.Windows.Forms.GroupBox();
            this.lblGDBPort = new System.Windows.Forms.Label();
            this.txtGDBPort = new System.Windows.Forms.TextBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.groupBox16 = new System.Windows.Forms.GroupBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.button20 = new System.Windows.Forms.Button();
            this.grpSerial = new System.Windows.Forms.GroupBox();
            this.chkSerialToScrn = new System.Windows.Forms.CheckBox();
            this.txtSerialPipe = new System.Windows.Forms.TextBox();
            this.chkSerialToPipe = new System.Windows.Forms.CheckBox();
            this.txtSerialFile = new System.Windows.Forms.TextBox();
            this.btnBrowseSerial = new System.Windows.Forms.Button();
            this.chkSerialToFile = new System.Windows.Forms.CheckBox();
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
            this.tabFloppy = new System.Windows.Forms.TabPage();
            this.grpFloppy = new System.Windows.Forms.GroupBox();
            this.txtFloppyB = new System.Windows.Forms.TextBox();
            this.txtFloppyA = new System.Windows.Forms.TextBox();
            this.btnBrowseFloppyB = new System.Windows.Forms.Button();
            this.btnBrowseFloppyA = new System.Windows.Forms.Button();
            this.chkFloppyB = new System.Windows.Forms.CheckBox();
            this.chkFloppyA = new System.Windows.Forms.CheckBox();
            this.tabCDROM = new System.Windows.Forms.TabPage();
            this.grpCDROM = new System.Windows.Forms.GroupBox();
            this.cboCDROM = new System.Windows.Forms.ComboBox();
            this.txtCDROM = new System.Windows.Forms.TextBox();
            this.optHostCDROM = new System.Windows.Forms.RadioButton();
            this.btnBrowseCDROM = new System.Windows.Forms.Button();
            this.chkUseCDROM = new System.Windows.Forms.CheckBox();
            this.optCDImage = new System.Windows.Forms.RadioButton();
            this.tabHardDisk = new System.Windows.Forms.TabPage();
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
            this.tabPaths = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtQEmuPath = new System.Windows.Forms.TextBox();
            this.txtVDKPath = new System.Windows.Forms.TextBox();
            this.btnVDKBrowse = new System.Windows.Forms.Button();
            this.lblVDKPath = new System.Windows.Forms.Label();
            this.btnQEmuPath = new System.Windows.Forms.Button();
            this.lblQEmuPath = new System.Windows.Forms.Label();
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
            this.grpMachine = new System.Windows.Forms.GroupBox();
            this.cboMachine = new System.Windows.Forms.ComboBox();
            this.grpClock = new System.Windows.Forms.GroupBox();
            this.chkSetClock = new System.Windows.Forms.CheckBox();
            this.grpDisplay = new System.Windows.Forms.GroupBox();
            this.chkVGAoutput = new System.Windows.Forms.CheckBox();
            this.chkFullscreen = new System.Windows.Forms.CheckBox();
            this.Tabs = new System.Windows.Forms.TabControl();
            this.menuStrip1.SuspendLayout();
            this.tabOther.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.grpVNC.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabTools.SuspendLayout();
            this.grpVDK.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.tabDebug.SuspendLayout();
            this.grpGDB.SuspendLayout();
            this.groupBox16.SuspendLayout();
            this.grpSerial.SuspendLayout();
            this.tabAudio.SuspendLayout();
            this.grpAudio.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabNetwork.SuspendLayout();
            this.grpNetwork.SuspendLayout();
            this.panelUser.SuspendLayout();
            this.panelNic.SuspendLayout();
            this.tabFloppy.SuspendLayout();
            this.grpFloppy.SuspendLayout();
            this.tabCDROM.SuspendLayout();
            this.grpCDROM.SuspendLayout();
            this.tabHardDisk.SuspendLayout();
            this.grpHarddisk.SuspendLayout();
            this.tabPaths.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tabGen.SuspendLayout();
            this.grpACPI.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).BeginInit();
            this.grpMachine.SuspendLayout();
            this.grpClock.SuspendLayout();
            this.grpDisplay.SuspendLayout();
            this.Tabs.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnLaunch
            // 
            this.btnLaunch.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.btnLaunch.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnLaunch.Location = new System.Drawing.Point(435, 266);
            this.btnLaunch.Name = "btnLaunch";
            this.btnLaunch.Size = new System.Drawing.Size(93, 23);
            this.btnLaunch.TabIndex = 4;
            this.btnLaunch.Text = "Launch";
            this.btnLaunch.UseVisualStyleBackColor = false;
            this.btnLaunch.Click += new System.EventHandler(this.btnLaunch_Click);
            // 
            // openFile
            // 
            this.openFile.Title = "Path to VDK";
            this.openFile.ValidateNames = false;
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(12, 266);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(93, 23);
            this.btnLoad.TabIndex = 5;
            this.btnLoad.Text = "Close";
            this.btnLoad.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStrip1.Size = new System.Drawing.Size(540, 24);
            this.menuStrip1.TabIndex = 7;
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
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadConfigToolStripMenuItem
            // 
            this.loadConfigToolStripMenuItem.Name = "loadConfigToolStripMenuItem";
            this.loadConfigToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.loadConfigToolStripMenuItem.Text = "Load config";
            this.loadConfigToolStripMenuItem.Click += new System.EventHandler(this.loadConfigToolStripMenuItem_Click);
            // 
            // saveConfigToolStripMenuItem
            // 
            this.saveConfigToolStripMenuItem.Name = "saveConfigToolStripMenuItem";
            this.saveConfigToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveConfigToolStripMenuItem.Text = "Save Config";
            this.saveConfigToolStripMenuItem.Click += new System.EventHandler(this.saveConfigToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(149, 6);
            // 
            // setAsDefaultToolStripMenuItem
            // 
            this.setAsDefaultToolStripMenuItem.Name = "setAsDefaultToolStripMenuItem";
            this.setAsDefaultToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.setAsDefaultToolStripMenuItem.Text = "Set as default";
            this.setAsDefaultToolStripMenuItem.Click += new System.EventHandler(this.setAsDefaultToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.helpContentsToolStripMenuItem,
            this.toolStripSeparator3,
            this.aboutReactOSQEmuManagerToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // helpContentsToolStripMenuItem
            // 
            this.helpContentsToolStripMenuItem.Name = "helpContentsToolStripMenuItem";
            this.helpContentsToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.helpContentsToolStripMenuItem.Text = "Help Contents";
            this.helpContentsToolStripMenuItem.Click += new System.EventHandler(this.helpContentsToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(232, 6);
            // 
            // aboutReactOSQEmuManagerToolStripMenuItem
            // 
            this.aboutReactOSQEmuManagerToolStripMenuItem.Name = "aboutReactOSQEmuManagerToolStripMenuItem";
            this.aboutReactOSQEmuManagerToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.aboutReactOSQEmuManagerToolStripMenuItem.Text = "About ReactOS QEmu Manager";
            this.aboutReactOSQEmuManagerToolStripMenuItem.Click += new System.EventHandler(this.aboutReactOSQEmuManagerToolStripMenuItem_Click);
            // 
            // tabOther
            // 
            this.tabOther.Controls.Add(this.groupBox4);
            this.tabOther.Controls.Add(this.groupBox3);
            this.tabOther.Controls.Add(this.grpVNC);
            this.tabOther.Controls.Add(this.groupBox1);
            this.tabOther.Controls.Add(this.checkBox2);
            this.tabOther.Controls.Add(this.chkVBE30);
            this.tabOther.Location = new System.Drawing.Point(4, 22);
            this.tabOther.Name = "tabOther";
            this.tabOther.Padding = new System.Windows.Forms.Padding(3);
            this.tabOther.Size = new System.Drawing.Size(508, 204);
            this.tabOther.TabIndex = 11;
            this.tabOther.Text = "Other";
            this.tabOther.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.txtAppendCmd);
            this.groupBox4.Location = new System.Drawing.Point(3, 117);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(502, 52);
            this.groupBox4.TabIndex = 12;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Append";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 22);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(51, 13);
            this.label10.TabIndex = 6;
            this.label10.Text = "Cmd Line";
            // 
            // txtAppendCmd
            // 
            this.txtAppendCmd.Location = new System.Drawing.Point(101, 19);
            this.txtAppendCmd.Name = "txtAppendCmd";
            this.txtAppendCmd.Size = new System.Drawing.Size(388, 20);
            this.txtAppendCmd.TabIndex = 4;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.btnBrowseBios);
            this.groupBox3.Controls.Add(this.txtABios);
            this.groupBox3.Enabled = false;
            this.groupBox3.Location = new System.Drawing.Point(3, 65);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(351, 52);
            this.groupBox3.TabIndex = 11;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "BIOS";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 20);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(77, 13);
            this.label8.TabIndex = 6;
            this.label8.Text = "Alternate BIOS";
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
            // txtABios
            // 
            this.txtABios.Location = new System.Drawing.Point(101, 17);
            this.txtABios.Name = "txtABios";
            this.txtABios.Size = new System.Drawing.Size(211, 20);
            this.txtABios.TabIndex = 4;
            // 
            // grpVNC
            // 
            this.grpVNC.Controls.Add(this.txtVNC);
            this.grpVNC.Controls.Add(this.label14);
            this.grpVNC.Controls.Add(this.chkVNC);
            this.grpVNC.Location = new System.Drawing.Point(363, 3);
            this.grpVNC.Name = "grpVNC";
            this.grpVNC.Size = new System.Drawing.Size(142, 114);
            this.grpVNC.TabIndex = 10;
            this.grpVNC.TabStop = false;
            this.grpVNC.Text = "VNC Server";
            // 
            // txtVNC
            // 
            this.txtVNC.Enabled = false;
            this.txtVNC.Location = new System.Drawing.Point(20, 83);
            this.txtVNC.Name = "txtVNC";
            this.txtVNC.Size = new System.Drawing.Size(96, 20);
            this.txtVNC.TabIndex = 2;
            this.txtVNC.Text = "0";
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(20, 59);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(88, 21);
            this.label14.TabIndex = 1;
            this.label14.Text = "Display Number";
            // 
            // chkVNC
            // 
            this.chkVNC.Location = new System.Drawing.Point(23, 17);
            this.chkVNC.Name = "chkVNC";
            this.chkVNC.Size = new System.Drawing.Size(96, 24);
            this.chkVNC.TabIndex = 0;
            this.chkVNC.Text = "Activate VNC";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.btnBrowseLinuxK);
            this.groupBox1.Controls.Add(this.txtLinuxK);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(356, 56);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Linux";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 20);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(69, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "Kernel Image";
            // 
            // btnBrowseLinuxK
            // 
            this.btnBrowseLinuxK.Location = new System.Drawing.Point(318, 15);
            this.btnBrowseLinuxK.Name = "btnBrowseLinuxK";
            this.btnBrowseLinuxK.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseLinuxK.TabIndex = 2;
            this.btnBrowseLinuxK.Text = "...";
            this.btnBrowseLinuxK.UseVisualStyleBackColor = true;
            // 
            // txtLinuxK
            // 
            this.txtLinuxK.Location = new System.Drawing.Point(101, 17);
            this.txtLinuxK.Name = "txtLinuxK";
            this.txtLinuxK.Size = new System.Drawing.Size(211, 20);
            this.txtLinuxK.TabIndex = 0;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(244, 177);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(253, 17);
            this.checkBox2.TabIndex = 8;
            this.checkBox2.Text = "Freeze CPU at startup (use \'c\' to start execution)";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // chkVBE30
            // 
            this.chkVBE30.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkVBE30.Location = new System.Drawing.Point(13, 175);
            this.chkVBE30.Name = "chkVBE30";
            this.chkVBE30.Size = new System.Drawing.Size(225, 23);
            this.chkVBE30.TabIndex = 3;
            this.chkVBE30.Text = "Simulate VGA card with Bochs VBE 3.0";
            // 
            // tabTools
            // 
            this.tabTools.Controls.Add(this.grpVDK);
            this.tabTools.Controls.Add(this.groupBox9);
            this.tabTools.Location = new System.Drawing.Point(4, 22);
            this.tabTools.Name = "tabTools";
            this.tabTools.Size = new System.Drawing.Size(508, 204);
            this.tabTools.TabIndex = 6;
            this.tabTools.Text = "Tools";
            this.tabTools.UseVisualStyleBackColor = true;
            // 
            // grpVDK
            // 
            this.grpVDK.Controls.Add(this.label9);
            this.grpVDK.Controls.Add(this.txtVDKImage);
            this.grpVDK.Controls.Add(this.cboVDKDrive);
            this.grpVDK.Controls.Add(this.lblDrive);
            this.grpVDK.Controls.Add(this.btnVDKImage);
            this.grpVDK.Controls.Add(this.btnUnmount);
            this.grpVDK.Controls.Add(this.btnMount);
            this.grpVDK.Controls.Add(this.lblImage);
            this.grpVDK.Location = new System.Drawing.Point(3, 83);
            this.grpVDK.Name = "grpVDK";
            this.grpVDK.Size = new System.Drawing.Size(502, 118);
            this.grpVDK.TabIndex = 7;
            this.grpVDK.TabStop = false;
            this.grpVDK.Text = "VDK";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(116, 102);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(222, 13);
            this.label9.TabIndex = 11;
            this.label9.Text = "Application must be run in administrator mode.";
            // 
            // txtVDKImage
            // 
            this.txtVDKImage.Location = new System.Drawing.Point(19, 32);
            this.txtVDKImage.Name = "txtVDKImage";
            this.txtVDKImage.Size = new System.Drawing.Size(443, 20);
            this.txtVDKImage.TabIndex = 6;
            // 
            // cboVDKDrive
            // 
            this.cboVDKDrive.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboVDKDrive.FormattingEnabled = true;
            this.cboVDKDrive.Location = new System.Drawing.Point(57, 70);
            this.cboVDKDrive.Name = "cboVDKDrive";
            this.cboVDKDrive.Size = new System.Drawing.Size(69, 21);
            this.cboVDKDrive.TabIndex = 10;
            this.cboVDKDrive.SelectedIndexChanged += new System.EventHandler(this.cboVDKDrive_SelectedIndexChanged);
            // 
            // lblDrive
            // 
            this.lblDrive.AutoSize = true;
            this.lblDrive.Location = new System.Drawing.Point(16, 73);
            this.lblDrive.Name = "lblDrive";
            this.lblDrive.Size = new System.Drawing.Size(35, 13);
            this.lblDrive.TabIndex = 9;
            this.lblDrive.Text = "Drive:";
            // 
            // btnVDKImage
            // 
            this.btnVDKImage.Location = new System.Drawing.Point(468, 30);
            this.btnVDKImage.Name = "btnVDKImage";
            this.btnVDKImage.Size = new System.Drawing.Size(27, 22);
            this.btnVDKImage.TabIndex = 8;
            this.btnVDKImage.Text = "...";
            this.btnVDKImage.UseVisualStyleBackColor = true;
            this.btnVDKImage.Click += new System.EventHandler(this.btnVDKImage_Click);
            // 
            // btnUnmount
            // 
            this.btnUnmount.Location = new System.Drawing.Point(402, 68);
            this.btnUnmount.Name = "btnUnmount";
            this.btnUnmount.Size = new System.Drawing.Size(93, 23);
            this.btnUnmount.TabIndex = 5;
            this.btnUnmount.Text = "Unmount";
            this.btnUnmount.Click += new System.EventHandler(this.btnUnmount_Click);
            // 
            // btnMount
            // 
            this.btnMount.Location = new System.Drawing.Point(303, 68);
            this.btnMount.Name = "btnMount";
            this.btnMount.Size = new System.Drawing.Size(93, 23);
            this.btnMount.TabIndex = 3;
            this.btnMount.Text = "Mount";
            this.btnMount.Click += new System.EventHandler(this.btnMount_Click);
            // 
            // lblImage
            // 
            this.lblImage.AutoSize = true;
            this.lblImage.Location = new System.Drawing.Point(16, 16);
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
            this.groupBox9.Location = new System.Drawing.Point(3, 3);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(502, 74);
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
            this.btnCreateImage.Location = new System.Drawing.Point(256, 39);
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
            this.cboImageFormat.Size = new System.Drawing.Size(102, 21);
            this.cboImageFormat.TabIndex = 7;
            // 
            // tabDebug
            // 
            this.tabDebug.Controls.Add(this.grpGDB);
            this.tabDebug.Controls.Add(this.groupBox16);
            this.tabDebug.Controls.Add(this.grpSerial);
            this.tabDebug.Location = new System.Drawing.Point(4, 22);
            this.tabDebug.Name = "tabDebug";
            this.tabDebug.Size = new System.Drawing.Size(508, 204);
            this.tabDebug.TabIndex = 8;
            this.tabDebug.Text = "Debug";
            this.tabDebug.UseVisualStyleBackColor = true;
            // 
            // grpGDB
            // 
            this.grpGDB.Controls.Add(this.lblGDBPort);
            this.grpGDB.Controls.Add(this.txtGDBPort);
            this.grpGDB.Controls.Add(this.checkBox14);
            this.grpGDB.Location = new System.Drawing.Point(350, 102);
            this.grpGDB.Name = "grpGDB";
            this.grpGDB.Size = new System.Drawing.Size(155, 96);
            this.grpGDB.TabIndex = 2;
            this.grpGDB.TabStop = false;
            this.grpGDB.Text = "GDB";
            // 
            // lblGDBPort
            // 
            this.lblGDBPort.Location = new System.Drawing.Point(2, 49);
            this.lblGDBPort.Name = "lblGDBPort";
            this.lblGDBPort.Size = new System.Drawing.Size(55, 23);
            this.lblGDBPort.TabIndex = 2;
            this.lblGDBPort.Text = "Port:";
            // 
            // txtGDBPort
            // 
            this.txtGDBPort.Location = new System.Drawing.Point(63, 49);
            this.txtGDBPort.Name = "txtGDBPort";
            this.txtGDBPort.Size = new System.Drawing.Size(47, 20);
            this.txtGDBPort.TabIndex = 1;
            this.txtGDBPort.Text = "1234";
            this.txtGDBPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // checkBox14
            // 
            this.checkBox14.Location = new System.Drawing.Point(5, 19);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(144, 24);
            this.checkBox14.TabIndex = 0;
            this.checkBox14.Text = "Wait connection to port";
            // 
            // groupBox16
            // 
            this.groupBox16.Controls.Add(this.checkBox3);
            this.groupBox16.Controls.Add(this.button20);
            this.groupBox16.Location = new System.Drawing.Point(350, 3);
            this.groupBox16.Name = "groupBox16";
            this.groupBox16.Size = new System.Drawing.Size(155, 96);
            this.groupBox16.TabIndex = 1;
            this.groupBox16.TabStop = false;
            this.groupBox16.Text = "LoadVM state";
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(45, 30);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(65, 17);
            this.checkBox3.TabIndex = 4;
            this.checkBox3.Text = "Enabled";
            this.checkBox3.UseVisualStyleBackColor = true;
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
            // grpSerial
            // 
            this.grpSerial.Controls.Add(this.chkSerialToScrn);
            this.grpSerial.Controls.Add(this.txtSerialPipe);
            this.grpSerial.Controls.Add(this.chkSerialToPipe);
            this.grpSerial.Controls.Add(this.txtSerialFile);
            this.grpSerial.Controls.Add(this.btnBrowseSerial);
            this.grpSerial.Controls.Add(this.chkSerialToFile);
            this.grpSerial.Location = new System.Drawing.Point(3, 3);
            this.grpSerial.Name = "grpSerial";
            this.grpSerial.Size = new System.Drawing.Size(341, 195);
            this.grpSerial.TabIndex = 1;
            this.grpSerial.TabStop = false;
            this.grpSerial.Text = "Serial Port";
            // 
            // chkSerialToScrn
            // 
            this.chkSerialToScrn.AutoSize = true;
            this.chkSerialToScrn.Enabled = false;
            this.chkSerialToScrn.Location = new System.Drawing.Point(6, 30);
            this.chkSerialToScrn.Name = "chkSerialToScrn";
            this.chkSerialToScrn.Size = new System.Drawing.Size(113, 17);
            this.chkSerialToScrn.TabIndex = 7;
            this.chkSerialToScrn.Text = "Redirect to screen";
            this.chkSerialToScrn.UseVisualStyleBackColor = true;
            this.chkSerialToScrn.CheckedChanged += new System.EventHandler(this.chkSerialToScrn_CheckedChanged);
            // 
            // txtSerialPipe
            // 
            this.txtSerialPipe.Location = new System.Drawing.Point(105, 112);
            this.txtSerialPipe.Name = "txtSerialPipe";
            this.txtSerialPipe.Size = new System.Drawing.Size(226, 20);
            this.txtSerialPipe.TabIndex = 6;
            // 
            // chkSerialToPipe
            // 
            this.chkSerialToPipe.AutoSize = true;
            this.chkSerialToPipe.Location = new System.Drawing.Point(6, 112);
            this.chkSerialToPipe.Name = "chkSerialToPipe";
            this.chkSerialToPipe.Size = new System.Drawing.Size(101, 17);
            this.chkSerialToPipe.TabIndex = 5;
            this.chkSerialToPipe.Text = "Redirect to pipe";
            this.chkSerialToPipe.UseVisualStyleBackColor = true;
            this.chkSerialToPipe.CheckedChanged += new System.EventHandler(this.chkSerialToPipe_CheckedChanged);
            // 
            // txtSerialFile
            // 
            this.txtSerialFile.Location = new System.Drawing.Point(105, 69);
            this.txtSerialFile.Name = "txtSerialFile";
            this.txtSerialFile.Size = new System.Drawing.Size(193, 20);
            this.txtSerialFile.TabIndex = 4;
            // 
            // btnBrowseSerial
            // 
            this.btnBrowseSerial.Enabled = false;
            this.btnBrowseSerial.Location = new System.Drawing.Point(304, 69);
            this.btnBrowseSerial.Name = "btnBrowseSerial";
            this.btnBrowseSerial.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseSerial.TabIndex = 3;
            this.btnBrowseSerial.Text = "...";
            this.btnBrowseSerial.Click += new System.EventHandler(this.btnBrowseSerial_Click);
            // 
            // chkSerialToFile
            // 
            this.chkSerialToFile.Location = new System.Drawing.Point(6, 66);
            this.chkSerialToFile.Name = "chkSerialToFile";
            this.chkSerialToFile.Size = new System.Drawing.Size(104, 24);
            this.chkSerialToFile.TabIndex = 2;
            this.chkSerialToFile.Text = "Redirect to file";
            this.chkSerialToFile.CheckedChanged += new System.EventHandler(this.chkSerialToFile_CheckedChanged);
            // 
            // tabAudio
            // 
            this.tabAudio.Controls.Add(this.grpAudio);
            this.tabAudio.Location = new System.Drawing.Point(4, 22);
            this.tabAudio.Name = "tabAudio";
            this.tabAudio.Size = new System.Drawing.Size(508, 204);
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
            this.grpAudio.Size = new System.Drawing.Size(502, 198);
            this.grpAudio.TabIndex = 4;
            this.grpAudio.TabStop = false;
            this.grpAudio.Text = "Audio Card Emulation";
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox4.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox4.Location = new System.Drawing.Point(131, 144);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(22, 22);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox4.TabIndex = 6;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox3.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox3.Location = new System.Drawing.Point(131, 108);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(22, 22);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox3.TabIndex = 5;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox2.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox2.Location = new System.Drawing.Point(131, 74);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(22, 22);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox2.TabIndex = 4;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Image = global::Qemu_GUI.Properties.Resources.audio;
            this.pictureBox1.Location = new System.Drawing.Point(131, 40);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(22, 22);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
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
            // tabNetwork
            // 
            this.tabNetwork.Controls.Add(this.grpNetwork);
            this.tabNetwork.Location = new System.Drawing.Point(4, 22);
            this.tabNetwork.Name = "tabNetwork";
            this.tabNetwork.Size = new System.Drawing.Size(508, 204);
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
            this.grpNetwork.Location = new System.Drawing.Point(3, 3);
            this.grpNetwork.Name = "grpNetwork";
            this.grpNetwork.Size = new System.Drawing.Size(502, 198);
            this.grpNetwork.TabIndex = 1;
            this.grpNetwork.TabStop = false;
            this.grpNetwork.Text = "Network";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(340, 165);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(120, 13);
            this.label6.TabIndex = 55;
            this.label6.Text = "Leave Blank for default.";
            // 
            // panelUser
            // 
            this.panelUser.Controls.Add(this.label5);
            this.panelUser.Controls.Add(this.txtNetHost);
            this.panelUser.Location = new System.Drawing.Point(296, 19);
            this.panelUser.Name = "panelUser";
            this.panelUser.Size = new System.Drawing.Size(189, 118);
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
            this.label4.Location = new System.Drawing.Point(203, 32);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 54;
            this.label4.Text = "Mode:";
            // 
            // rbtnNetUser
            // 
            this.rbtnNetUser.AutoSize = true;
            this.rbtnNetUser.Checked = true;
            this.rbtnNetUser.Location = new System.Drawing.Point(206, 51);
            this.rbtnNetUser.Name = "rbtnNetUser";
            this.rbtnNetUser.Size = new System.Drawing.Size(47, 17);
            this.rbtnNetUser.TabIndex = 53;
            this.rbtnNetUser.TabStop = true;
            this.rbtnNetUser.Text = "User";
            this.rbtnNetUser.UseVisualStyleBackColor = true;
            this.rbtnNetUser.CheckedChanged += new System.EventHandler(this.rbtnNetUser_CheckedChanged);
            // 
            // panelNic
            // 
            this.panelNic.Controls.Add(this.label3);
            this.panelNic.Controls.Add(this.txtNicMACaddr);
            this.panelNic.Controls.Add(this.label2);
            this.panelNic.Controls.Add(this.cmbNICmodels);
            this.panelNic.Location = new System.Drawing.Point(293, 20);
            this.panelNic.Name = "panelNic";
            this.panelNic.Size = new System.Drawing.Size(195, 142);
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
            this.rbtnNetNic.Location = new System.Drawing.Point(206, 74);
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
            this.label1.Location = new System.Drawing.Point(7, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 13);
            this.label1.TabIndex = 50;
            this.label1.Text = "Virtual NICs";
            // 
            // btnNetRemove
            // 
            this.btnNetRemove.Location = new System.Drawing.Point(112, 167);
            this.btnNetRemove.Name = "btnNetRemove";
            this.btnNetRemove.Size = new System.Drawing.Size(60, 25);
            this.btnNetRemove.TabIndex = 49;
            this.btnNetRemove.Text = "Remove";
            this.btnNetRemove.UseVisualStyleBackColor = true;
            this.btnNetRemove.Click += new System.EventHandler(this.btnNetRemove_Click);
            // 
            // btnNetAdd
            // 
            this.btnNetAdd.Location = new System.Drawing.Point(26, 167);
            this.btnNetAdd.Name = "btnNetAdd";
            this.btnNetAdd.Size = new System.Drawing.Size(60, 25);
            this.btnNetAdd.TabIndex = 48;
            this.btnNetAdd.Text = "Add";
            this.btnNetAdd.UseVisualStyleBackColor = true;
            this.btnNetAdd.Click += new System.EventHandler(this.btnNetAdd_Click);
            // 
            // listVLANs
            // 
            this.listVLANs.AutoArrange = false;
            this.listVLANs.LabelWrap = false;
            this.listVLANs.Location = new System.Drawing.Point(6, 36);
            this.listVLANs.Name = "listVLANs";
            this.listVLANs.ShowGroups = false;
            this.listVLANs.Size = new System.Drawing.Size(191, 125);
            this.listVLANs.TabIndex = 47;
            this.listVLANs.UseCompatibleStateImageBehavior = false;
            this.listVLANs.View = System.Windows.Forms.View.List;
            // 
            // tabFloppy
            // 
            this.tabFloppy.Controls.Add(this.grpFloppy);
            this.tabFloppy.Location = new System.Drawing.Point(4, 22);
            this.tabFloppy.Name = "tabFloppy";
            this.tabFloppy.Size = new System.Drawing.Size(508, 204);
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
            this.grpFloppy.Size = new System.Drawing.Size(502, 195);
            this.grpFloppy.TabIndex = 4;
            this.grpFloppy.TabStop = false;
            this.grpFloppy.Text = "Floppy";
            // 
            // txtFloppyB
            // 
            this.txtFloppyB.Enabled = false;
            this.txtFloppyB.Location = new System.Drawing.Point(32, 135);
            this.txtFloppyB.Name = "txtFloppyB";
            this.txtFloppyB.Size = new System.Drawing.Size(431, 20);
            this.txtFloppyB.TabIndex = 5;
            // 
            // txtFloppyA
            // 
            this.txtFloppyA.Enabled = false;
            this.txtFloppyA.Location = new System.Drawing.Point(32, 49);
            this.txtFloppyA.Name = "txtFloppyA";
            this.txtFloppyA.Size = new System.Drawing.Size(431, 20);
            this.txtFloppyA.TabIndex = 4;
            // 
            // btnBrowseFloppyB
            // 
            this.btnBrowseFloppyB.Enabled = false;
            this.btnBrowseFloppyB.Location = new System.Drawing.Point(469, 135);
            this.btnBrowseFloppyB.Name = "btnBrowseFloppyB";
            this.btnBrowseFloppyB.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseFloppyB.TabIndex = 3;
            this.btnBrowseFloppyB.Text = "...";
            this.btnBrowseFloppyB.Click += new System.EventHandler(this.btnBrowseFloppyB_Click);
            // 
            // btnBrowseFloppyA
            // 
            this.btnBrowseFloppyA.Enabled = false;
            this.btnBrowseFloppyA.Location = new System.Drawing.Point(469, 49);
            this.btnBrowseFloppyA.Name = "btnBrowseFloppyA";
            this.btnBrowseFloppyA.Size = new System.Drawing.Size(27, 22);
            this.btnBrowseFloppyA.TabIndex = 2;
            this.btnBrowseFloppyA.Text = "...";
            this.btnBrowseFloppyA.Click += new System.EventHandler(this.btnBrowseFloppyA_Click);
            // 
            // chkFloppyB
            // 
            this.chkFloppyB.Location = new System.Drawing.Point(16, 110);
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
            // tabCDROM
            // 
            this.tabCDROM.Controls.Add(this.grpCDROM);
            this.tabCDROM.Location = new System.Drawing.Point(4, 22);
            this.tabCDROM.Name = "tabCDROM";
            this.tabCDROM.Size = new System.Drawing.Size(508, 204);
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
            this.grpCDROM.Size = new System.Drawing.Size(502, 198);
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
            // tabHardDisk
            // 
            this.tabHardDisk.Controls.Add(this.grpHarddisk);
            this.tabHardDisk.Location = new System.Drawing.Point(4, 22);
            this.tabHardDisk.Name = "tabHardDisk";
            this.tabHardDisk.Size = new System.Drawing.Size(508, 204);
            this.tabHardDisk.TabIndex = 3;
            this.tabHardDisk.Text = " Harddisk";
            this.tabHardDisk.UseVisualStyleBackColor = true;
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
            this.grpHarddisk.Size = new System.Drawing.Size(502, 195);
            this.grpHarddisk.TabIndex = 5;
            this.grpHarddisk.TabStop = false;
            this.grpHarddisk.Text = " Harddisks";
            // 
            // txtHDD
            // 
            this.txtHDD.Enabled = false;
            this.txtHDD.Location = new System.Drawing.Point(77, 155);
            this.txtHDD.Name = "txtHDD";
            this.txtHDD.Size = new System.Drawing.Size(384, 20);
            this.txtHDD.TabIndex = 15;
            // 
            // txtHDC
            // 
            this.txtHDC.Enabled = false;
            this.txtHDC.Location = new System.Drawing.Point(77, 110);
            this.txtHDC.Name = "txtHDC";
            this.txtHDC.Size = new System.Drawing.Size(384, 20);
            this.txtHDC.TabIndex = 14;
            // 
            // txtHDB
            // 
            this.txtHDB.Enabled = false;
            this.txtHDB.Location = new System.Drawing.Point(77, 65);
            this.txtHDB.Name = "txtHDB";
            this.txtHDB.Size = new System.Drawing.Size(384, 20);
            this.txtHDB.TabIndex = 13;
            // 
            // txtHDA
            // 
            this.txtHDA.Enabled = false;
            this.txtHDA.Location = new System.Drawing.Point(77, 22);
            this.txtHDA.Name = "txtHDA";
            this.txtHDA.Size = new System.Drawing.Size(384, 20);
            this.txtHDA.TabIndex = 12;
            // 
            // btnBrowseHDD
            // 
            this.btnBrowseHDD.Enabled = false;
            this.btnBrowseHDD.Location = new System.Drawing.Point(467, 155);
            this.btnBrowseHDD.Name = "btnBrowseHDD";
            this.btnBrowseHDD.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDD.TabIndex = 11;
            this.btnBrowseHDD.Text = "...";
            this.btnBrowseHDD.Click += new System.EventHandler(this.btnBrowseHDD_Click);
            // 
            // btnBrowseHDC
            // 
            this.btnBrowseHDC.Enabled = false;
            this.btnBrowseHDC.Location = new System.Drawing.Point(467, 110);
            this.btnBrowseHDC.Name = "btnBrowseHDC";
            this.btnBrowseHDC.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDC.TabIndex = 10;
            this.btnBrowseHDC.Text = "...";
            this.btnBrowseHDC.Click += new System.EventHandler(this.btnBrowseHDC_Click);
            // 
            // chkUseHDD
            // 
            this.chkUseHDD.Location = new System.Drawing.Point(6, 151);
            this.chkUseHDD.Name = "chkUseHDD";
            this.chkUseHDD.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDD.TabIndex = 9;
            this.chkUseHDD.Text = "HDD";
            this.chkUseHDD.CheckedChanged += new System.EventHandler(this.chkUseHDD_CheckedChanged);
            // 
            // chkUseHDC
            // 
            this.chkUseHDC.Location = new System.Drawing.Point(6, 106);
            this.chkUseHDC.Name = "chkUseHDC";
            this.chkUseHDC.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDC.TabIndex = 8;
            this.chkUseHDC.Text = "HDC";
            this.chkUseHDC.CheckedChanged += new System.EventHandler(this.chkUseHDC_CheckedChanged);
            // 
            // btnBrowseHDB
            // 
            this.btnBrowseHDB.Enabled = false;
            this.btnBrowseHDB.Location = new System.Drawing.Point(467, 65);
            this.btnBrowseHDB.Name = "btnBrowseHDB";
            this.btnBrowseHDB.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDB.TabIndex = 7;
            this.btnBrowseHDB.Text = "...";
            this.btnBrowseHDB.Click += new System.EventHandler(this.btnBrowseHDB_Click);
            // 
            // btnBrowseHDA
            // 
            this.btnBrowseHDA.Enabled = false;
            this.btnBrowseHDA.Location = new System.Drawing.Point(467, 20);
            this.btnBrowseHDA.Name = "btnBrowseHDA";
            this.btnBrowseHDA.Size = new System.Drawing.Size(27, 23);
            this.btnBrowseHDA.TabIndex = 6;
            this.btnBrowseHDA.Text = " ...";
            this.btnBrowseHDA.Click += new System.EventHandler(this.btnBrowseHDA_Click);
            // 
            // chkUseHDB
            // 
            this.chkUseHDB.Location = new System.Drawing.Point(6, 61);
            this.chkUseHDB.Name = "chkUseHDB";
            this.chkUseHDB.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDB.TabIndex = 5;
            this.chkUseHDB.Text = "HDB";
            this.chkUseHDB.CheckedChanged += new System.EventHandler(this.chkUseHDB_CheckedChanged);
            // 
            // chkUseHDA
            // 
            this.chkUseHDA.Location = new System.Drawing.Point(6, 16);
            this.chkUseHDA.Name = "chkUseHDA";
            this.chkUseHDA.Size = new System.Drawing.Size(72, 24);
            this.chkUseHDA.TabIndex = 4;
            this.chkUseHDA.Text = "HDA";
            this.chkUseHDA.CheckedChanged += new System.EventHandler(this.chkUseHDA_CheckedChanged);
            // 
            // tabPaths
            // 
            this.tabPaths.Controls.Add(this.groupBox2);
            this.tabPaths.Location = new System.Drawing.Point(4, 22);
            this.tabPaths.Name = "tabPaths";
            this.tabPaths.Size = new System.Drawing.Size(508, 204);
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
            this.groupBox2.Size = new System.Drawing.Size(502, 198);
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
            // tabGen
            // 
            this.tabGen.Controls.Add(this.grpACPI);
            this.tabGen.Controls.Add(this.groupBox6);
            this.tabGen.Controls.Add(this.grpMachine);
            this.tabGen.Controls.Add(this.grpClock);
            this.tabGen.Controls.Add(this.grpDisplay);
            this.tabGen.Location = new System.Drawing.Point(4, 22);
            this.tabGen.Name = "tabGen";
            this.tabGen.Size = new System.Drawing.Size(508, 204);
            this.tabGen.TabIndex = 1;
            this.tabGen.Text = "General";
            this.tabGen.UseVisualStyleBackColor = true;
            // 
            // grpACPI
            // 
            this.grpACPI.Controls.Add(this.chkACPI);
            this.grpACPI.Location = new System.Drawing.Point(315, 57);
            this.grpACPI.Name = "grpACPI";
            this.grpACPI.Size = new System.Drawing.Size(190, 51);
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
            this.groupBox6.Location = new System.Drawing.Point(3, 57);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(306, 144);
            this.groupBox6.TabIndex = 4;
            this.groupBox6.TabStop = false;
            // 
            // chkKQEmu
            // 
            this.chkKQEmu.AutoSize = true;
            this.chkKQEmu.Checked = true;
            this.chkKQEmu.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkKQEmu.Location = new System.Drawing.Point(19, 102);
            this.chkKQEmu.Name = "chkKQEmu";
            this.chkKQEmu.Size = new System.Drawing.Size(177, 17);
            this.chkKQEmu.TabIndex = 18;
            this.chkKQEmu.Text = "Use KQEmu accelerator module";
            this.chkKQEmu.UseVisualStyleBackColor = true;
            // 
            // lblCPUs
            // 
            this.lblCPUs.AutoSize = true;
            this.lblCPUs.Location = new System.Drawing.Point(257, 75);
            this.lblCPUs.Name = "lblCPUs";
            this.lblCPUs.Size = new System.Drawing.Size(40, 13);
            this.lblCPUs.TabIndex = 14;
            this.lblCPUs.Text = "CPU(s)";
            // 
            // numSMP
            // 
            this.numSMP.Location = new System.Drawing.Point(205, 73);
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
            this.numMemory.Location = new System.Drawing.Point(205, 47);
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
            128,
            0,
            0,
            0});
            // 
            // lblMB
            // 
            this.lblMB.AutoSize = true;
            this.lblMB.Location = new System.Drawing.Point(257, 49);
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
            this.cboBootFrom.Location = new System.Drawing.Point(174, 21);
            this.cboBootFrom.Name = "cboBootFrom";
            this.cboBootFrom.Size = new System.Drawing.Size(121, 21);
            this.cboBootFrom.TabIndex = 1;
            // 
            // grpMachine
            // 
            this.grpMachine.Controls.Add(this.cboMachine);
            this.grpMachine.Location = new System.Drawing.Point(3, 3);
            this.grpMachine.Name = "grpMachine";
            this.grpMachine.Size = new System.Drawing.Size(306, 48);
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
            // grpClock
            // 
            this.grpClock.Controls.Add(this.chkSetClock);
            this.grpClock.Location = new System.Drawing.Point(315, 3);
            this.grpClock.Name = "grpClock";
            this.grpClock.Size = new System.Drawing.Size(190, 48);
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
            this.chkSetClock.Size = new System.Drawing.Size(128, 17);
            this.chkSetClock.TabIndex = 12;
            this.chkSetClock.Text = "Set clock to host time";
            this.chkSetClock.UseVisualStyleBackColor = true;
            // 
            // grpDisplay
            // 
            this.grpDisplay.Controls.Add(this.chkVGAoutput);
            this.grpDisplay.Controls.Add(this.chkFullscreen);
            this.grpDisplay.Location = new System.Drawing.Point(315, 114);
            this.grpDisplay.Name = "grpDisplay";
            this.grpDisplay.Size = new System.Drawing.Size(190, 87);
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
            this.Tabs.Location = new System.Drawing.Point(12, 30);
            this.Tabs.Name = "Tabs";
            this.Tabs.SelectedIndex = 0;
            this.Tabs.Size = new System.Drawing.Size(516, 230);
            this.Tabs.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.ClientSize = new System.Drawing.Size(540, 301);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnLaunch);
            this.Controls.Add(this.Tabs);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ReactOS Qemu GUI";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabOther.ResumeLayout(false);
            this.tabOther.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.grpVNC.ResumeLayout(false);
            this.grpVNC.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabTools.ResumeLayout(false);
            this.grpVDK.ResumeLayout(false);
            this.grpVDK.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.tabDebug.ResumeLayout(false);
            this.grpGDB.ResumeLayout(false);
            this.grpGDB.PerformLayout();
            this.groupBox16.ResumeLayout(false);
            this.groupBox16.PerformLayout();
            this.grpSerial.ResumeLayout(false);
            this.grpSerial.PerformLayout();
            this.tabAudio.ResumeLayout(false);
            this.grpAudio.ResumeLayout(false);
            this.grpAudio.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabNetwork.ResumeLayout(false);
            this.grpNetwork.ResumeLayout(false);
            this.grpNetwork.PerformLayout();
            this.panelUser.ResumeLayout(false);
            this.panelUser.PerformLayout();
            this.panelNic.ResumeLayout(false);
            this.panelNic.PerformLayout();
            this.tabFloppy.ResumeLayout(false);
            this.grpFloppy.ResumeLayout(false);
            this.grpFloppy.PerformLayout();
            this.tabCDROM.ResumeLayout(false);
            this.grpCDROM.ResumeLayout(false);
            this.grpCDROM.PerformLayout();
            this.tabHardDisk.ResumeLayout(false);
            this.grpHarddisk.ResumeLayout(false);
            this.grpHarddisk.PerformLayout();
            this.tabPaths.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.tabGen.ResumeLayout(false);
            this.grpACPI.ResumeLayout(false);
            this.grpACPI.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numSMP)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMemory)).EndInit();
            this.grpMachine.ResumeLayout(false);
            this.grpClock.ResumeLayout(false);
            this.grpClock.PerformLayout();
            this.grpDisplay.ResumeLayout(false);
            this.grpDisplay.PerformLayout();
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
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpContentsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutReactOSQEmuManagerToolStripMenuItem;
        private System.Windows.Forms.TabPage tabOther;
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button btnBrowseLinuxK;
        private System.Windows.Forms.TextBox txtLinuxK;
        private System.Windows.Forms.TabPage tabTools;
        private System.Windows.Forms.GroupBox grpVDK;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtVDKImage;
        private System.Windows.Forms.ComboBox cboVDKDrive;
        private System.Windows.Forms.Label lblDrive;
        private System.Windows.Forms.Button btnVDKImage;
        private System.Windows.Forms.Button btnUnmount;
        private System.Windows.Forms.Button btnMount;
        private System.Windows.Forms.Label lblImage;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Label lblImageSizeMB;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox txtImageSize;
        private System.Windows.Forms.Label lblImageSize;
        private System.Windows.Forms.Button btnCreateImage;
        private System.Windows.Forms.ComboBox cboImageFormat;
        private System.Windows.Forms.TabPage tabDebug;
        private System.Windows.Forms.GroupBox grpGDB;
        private System.Windows.Forms.Label lblGDBPort;
        private System.Windows.Forms.TextBox txtGDBPort;
        private System.Windows.Forms.CheckBox checkBox14;
        private System.Windows.Forms.GroupBox groupBox16;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.Button button20;
        private System.Windows.Forms.GroupBox grpSerial;
        private System.Windows.Forms.CheckBox chkSerialToScrn;
        private System.Windows.Forms.TextBox txtSerialPipe;
        private System.Windows.Forms.CheckBox chkSerialToPipe;
        private System.Windows.Forms.TextBox txtSerialFile;
        private System.Windows.Forms.Button btnBrowseSerial;
        private System.Windows.Forms.CheckBox chkSerialToFile;
        private System.Windows.Forms.TabPage tabAudio;
        private System.Windows.Forms.GroupBox grpAudio;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.CheckBox chkSoundBlaster;
        private System.Windows.Forms.CheckBox chkPCSpeaker;
        private System.Windows.Forms.CheckBox chkES1370;
        private System.Windows.Forms.CheckBox chkOPL2;
        private System.Windows.Forms.TabPage tabNetwork;
        private System.Windows.Forms.GroupBox grpNetwork;
        private System.Windows.Forms.Label label6;
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
        private System.Windows.Forms.TabPage tabFloppy;
        private System.Windows.Forms.GroupBox grpFloppy;
        private System.Windows.Forms.TextBox txtFloppyB;
        private System.Windows.Forms.TextBox txtFloppyA;
        private System.Windows.Forms.Button btnBrowseFloppyB;
        private System.Windows.Forms.Button btnBrowseFloppyA;
        private System.Windows.Forms.CheckBox chkFloppyB;
        private System.Windows.Forms.CheckBox chkFloppyA;
        private System.Windows.Forms.TabPage tabCDROM;
        private System.Windows.Forms.GroupBox grpCDROM;
        private System.Windows.Forms.ComboBox cboCDROM;
        private System.Windows.Forms.TextBox txtCDROM;
        private System.Windows.Forms.RadioButton optHostCDROM;
        private System.Windows.Forms.Button btnBrowseCDROM;
        private System.Windows.Forms.CheckBox chkUseCDROM;
        private System.Windows.Forms.RadioButton optCDImage;
        private System.Windows.Forms.TabPage tabHardDisk;
        private System.Windows.Forms.GroupBox grpHarddisk;
        private System.Windows.Forms.TextBox txtHDD;
        private System.Windows.Forms.TextBox txtHDC;
        private System.Windows.Forms.TextBox txtHDB;
        private System.Windows.Forms.TextBox txtHDA;
        private System.Windows.Forms.Button btnBrowseHDD;
        private System.Windows.Forms.Button btnBrowseHDC;
        private System.Windows.Forms.CheckBox chkUseHDD;
        private System.Windows.Forms.CheckBox chkUseHDC;
        private System.Windows.Forms.Button btnBrowseHDB;
        private System.Windows.Forms.Button btnBrowseHDA;
        private System.Windows.Forms.CheckBox chkUseHDB;
        private System.Windows.Forms.CheckBox chkUseHDA;
        private System.Windows.Forms.TabPage tabPaths;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox txtQEmuPath;
        private System.Windows.Forms.TextBox txtVDKPath;
        private System.Windows.Forms.Button btnVDKBrowse;
        private System.Windows.Forms.Label lblVDKPath;
        private System.Windows.Forms.Button btnQEmuPath;
        private System.Windows.Forms.Label lblQEmuPath;
        private System.Windows.Forms.TabPage tabGen;
        private System.Windows.Forms.GroupBox grpACPI;
        private System.Windows.Forms.CheckBox chkACPI;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.CheckBox chkKQEmu;
        private System.Windows.Forms.Label lblCPUs;
        private System.Windows.Forms.NumericUpDown numSMP;
        private System.Windows.Forms.NumericUpDown numMemory;
        private System.Windows.Forms.Label lblMB;
        private System.Windows.Forms.Label lblMemory;
        private System.Windows.Forms.Label lblBootFrom;
        private System.Windows.Forms.Label lblSMP;
        private System.Windows.Forms.ComboBox cboBootFrom;
        private System.Windows.Forms.GroupBox grpMachine;
        private System.Windows.Forms.ComboBox cboMachine;
        private System.Windows.Forms.GroupBox grpClock;
        private System.Windows.Forms.CheckBox chkSetClock;
        private System.Windows.Forms.GroupBox grpDisplay;
        private System.Windows.Forms.CheckBox chkVGAoutput;
        private System.Windows.Forms.CheckBox chkFullscreen;
        private System.Windows.Forms.TabControl Tabs;
        private System.Windows.Forms.ToolStripMenuItem loadConfigToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveConfigToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        #endregion
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox txtAppendCmd;
    }
}
