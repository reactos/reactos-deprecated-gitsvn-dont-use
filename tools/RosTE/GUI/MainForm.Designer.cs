namespace RosTEGUI
{
    partial class MainForm
    {
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("ReactOS", 0);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.ImageListView = new System.Windows.Forms.ListView();
            this.columnName = new System.Windows.Forms.ColumnHeader();
            this.columnMemory = new System.Windows.Forms.ColumnHeader();
            this.columnHardDisk = new System.Windows.Forms.ColumnHeader();
            this.columnCdrom = new System.Windows.Forms.ColumnHeader();
            this.columnNetwork = new System.Windows.Forms.ColumnHeader();
            this.columnAudio = new System.Windows.Forms.ColumnHeader();
            this.columnDebug = new System.Windows.Forms.ColumnHeader();
            this.mainImageList = new System.Windows.Forms.ImageList(this.components);
            this.toolbarLaunch = new System.Windows.Forms.ToolStripButton();
            this.toolbarStop = new System.Windows.Forms.ToolStripButton();
            this.toolbarSep1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolbarImageAdd = new System.Windows.Forms.ToolStripButton();
            this.toolbarImageDel = new System.Windows.Forms.ToolStripButton();
            this.toolbarSep2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolbarHelpContents = new System.Windows.Forms.ToolStripButton();
            this.toolbarSep4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolbarExit = new System.Windows.Forms.ToolStripButton();
            this.mainToolbar = new System.Windows.Forms.ToolStrip();
            this.toolbarSnapShot = new System.Windows.Forms.ToolStripButton();
            this.toolbarScreenShot = new System.Windows.Forms.ToolStripButton();
            this.toolbarSep3 = new System.Windows.Forms.ToolStripSeparator();
            this.mainMenu = new System.Windows.Forms.MenuStrip();
            this.mainmenuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mainmenuFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.mainmenuOptions = new System.Windows.Forms.ToolStripMenuItem();
            this.mainmenuHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.mainmenuHelpContent = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.MainMenuHelpAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.mainToolbar.SuspendLayout();
            this.mainMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // ImageListView
            // 
            this.ImageListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnName,
            this.columnMemory,
            this.columnHardDisk,
            this.columnCdrom,
            this.columnNetwork,
            this.columnAudio,
            this.columnDebug});
            this.ImageListView.FullRowSelect = true;
            this.ImageListView.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1});
            this.ImageListView.Location = new System.Drawing.Point(12, 52);
            this.ImageListView.Name = "ImageListView";
            this.ImageListView.Size = new System.Drawing.Size(516, 237);
            this.ImageListView.SmallImageList = this.mainImageList;
            this.ImageListView.TabIndex = 9;
            this.ImageListView.UseCompatibleStateImageBehavior = false;
            this.ImageListView.View = System.Windows.Forms.View.Details;
            this.ImageListView.DoubleClick += new System.EventHandler(this.ImageListView_DoubleClick);
            // 
            // columnName
            // 
            this.columnName.Tag = "";
            this.columnName.Text = "Name";
            this.columnName.Width = 130;
            // 
            // columnMemory
            // 
            this.columnMemory.Text = "Memory";
            this.columnMemory.Width = 55;
            // 
            // columnHardDisk
            // 
            this.columnHardDisk.Text = "Hard Disk";
            this.columnHardDisk.Width = 65;
            // 
            // columnCdrom
            // 
            this.columnCdrom.Text = "CD-ROM";
            // 
            // columnNetwork
            // 
            this.columnNetwork.Text = "Network";
            // 
            // columnAudio
            // 
            this.columnAudio.Text = "Audio";
            // 
            // columnDebug
            // 
            this.columnDebug.Text = "Debug";
            // 
            // mainImageList
            // 
            this.mainImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("mainImageList.ImageStream")));
            this.mainImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.mainImageList.Images.SetKeyName(0, "images.png");
            // 
            // toolbarLaunch
            // 
            this.toolbarLaunch.Image = ((System.Drawing.Image)(resources.GetObject("toolbarLaunch.Image")));
            this.toolbarLaunch.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarLaunch.Name = "toolbarLaunch";
            this.toolbarLaunch.Size = new System.Drawing.Size(61, 22);
            this.toolbarLaunch.Text = "Launch";
            this.toolbarLaunch.ToolTipText = "Starts the virtual machine";
            // 
            // toolbarStop
            // 
            this.toolbarStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarStop.Image = ((System.Drawing.Image)(resources.GetObject("toolbarStop.Image")));
            this.toolbarStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarStop.Name = "toolbarStop";
            this.toolbarStop.Size = new System.Drawing.Size(23, 22);
            this.toolbarStop.ToolTipText = "Stops the virtual machine";
            // 
            // toolbarSep1
            // 
            this.toolbarSep1.Name = "toolbarSep1";
            this.toolbarSep1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolbarImageAdd
            // 
            this.toolbarImageAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarImageAdd.Image = ((System.Drawing.Image)(resources.GetObject("toolbarImageAdd.Image")));
            this.toolbarImageAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarImageAdd.Name = "toolbarImageAdd";
            this.toolbarImageAdd.Size = new System.Drawing.Size(23, 22);
            this.toolbarImageAdd.ToolTipText = "Adds a new image";
            this.toolbarImageAdd.Click += new System.EventHandler(this.toolbarImageAdd_Click);
            // 
            // toolbarImageDel
            // 
            this.toolbarImageDel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarImageDel.Image = ((System.Drawing.Image)(resources.GetObject("toolbarImageDel.Image")));
            this.toolbarImageDel.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarImageDel.Name = "toolbarImageDel";
            this.toolbarImageDel.Size = new System.Drawing.Size(23, 22);
            this.toolbarImageDel.ToolTipText = "Deletes an existing image";
            // 
            // toolbarSep2
            // 
            this.toolbarSep2.Name = "toolbarSep2";
            this.toolbarSep2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolbarHelpContents
            // 
            this.toolbarHelpContents.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarHelpContents.Image = ((System.Drawing.Image)(resources.GetObject("toolbarHelpContents.Image")));
            this.toolbarHelpContents.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarHelpContents.Name = "toolbarHelpContents";
            this.toolbarHelpContents.Size = new System.Drawing.Size(23, 22);
            this.toolbarHelpContents.ToolTipText = "Opens help file";
            // 
            // toolbarSep4
            // 
            this.toolbarSep4.Name = "toolbarSep4";
            this.toolbarSep4.Size = new System.Drawing.Size(6, 25);
            // 
            // toolbarExit
            // 
            this.toolbarExit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarExit.Image = ((System.Drawing.Image)(resources.GetObject("toolbarExit.Image")));
            this.toolbarExit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarExit.Name = "toolbarExit";
            this.toolbarExit.Size = new System.Drawing.Size(23, 22);
            this.toolbarExit.ToolTipText = "Exits application";
            this.toolbarExit.Click += new System.EventHandler(this.toolbarExit_Click);
            // 
            // mainToolbar
            // 
            this.mainToolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolbarLaunch,
            this.toolbarStop,
            this.toolbarSep1,
            this.toolbarImageAdd,
            this.toolbarImageDel,
            this.toolbarSep2,
            this.toolbarSnapShot,
            this.toolbarScreenShot,
            this.toolbarSep3,
            this.toolbarHelpContents,
            this.toolbarSep4,
            this.toolbarExit});
            this.mainToolbar.Location = new System.Drawing.Point(0, 24);
            this.mainToolbar.Name = "mainToolbar";
            this.mainToolbar.Size = new System.Drawing.Size(540, 25);
            this.mainToolbar.TabIndex = 10;
            // 
            // toolbarSnapShot
            // 
            this.toolbarSnapShot.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarSnapShot.Image = ((System.Drawing.Image)(resources.GetObject("toolbarSnapShot.Image")));
            this.toolbarSnapShot.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarSnapShot.Name = "toolbarSnapShot";
            this.toolbarSnapShot.Size = new System.Drawing.Size(23, 22);
            this.toolbarSnapShot.Text = "toolStripButton1";
            this.toolbarSnapShot.ToolTipText = "Create a snap shot of the current status";
            // 
            // toolbarScreenShot
            // 
            this.toolbarScreenShot.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolbarScreenShot.Image = ((System.Drawing.Image)(resources.GetObject("toolbarScreenShot.Image")));
            this.toolbarScreenShot.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolbarScreenShot.Name = "toolbarScreenShot";
            this.toolbarScreenShot.Size = new System.Drawing.Size(23, 22);
            this.toolbarScreenShot.Text = "toolStripButton2";
            this.toolbarScreenShot.ToolTipText = "Grab a screenshot of the display";
            // 
            // toolbarSep3
            // 
            this.toolbarSep3.Name = "toolbarSep3";
            this.toolbarSep3.Size = new System.Drawing.Size(6, 25);
            // 
            // mainMenu
            // 
            this.mainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainmenuFile,
            this.mainmenuOptions,
            this.mainmenuHelp});
            this.mainMenu.Location = new System.Drawing.Point(0, 0);
            this.mainMenu.Name = "mainMenu";
            this.mainMenu.Size = new System.Drawing.Size(540, 24);
            this.mainMenu.TabIndex = 11;
            // 
            // mainmenuFile
            // 
            this.mainmenuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainmenuFileExit});
            this.mainmenuFile.Name = "mainmenuFile";
            this.mainmenuFile.Size = new System.Drawing.Size(35, 20);
            this.mainmenuFile.Text = "File";
            // 
            // mainmenuFileExit
            // 
            this.mainmenuFileExit.Name = "mainmenuFileExit";
            this.mainmenuFileExit.Size = new System.Drawing.Size(103, 22);
            this.mainmenuFileExit.Text = "Exit";
            this.mainmenuFileExit.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // mainmenuOptions
            // 
            this.mainmenuOptions.Name = "mainmenuOptions";
            this.mainmenuOptions.Size = new System.Drawing.Size(56, 20);
            this.mainmenuOptions.Text = "Options";
            // 
            // mainmenuHelp
            // 
            this.mainmenuHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainmenuHelpContent,
            this.toolStripSeparator1,
            this.MainMenuHelpAbout});
            this.mainmenuHelp.Name = "mainmenuHelp";
            this.mainmenuHelp.Size = new System.Drawing.Size(40, 20);
            this.mainmenuHelp.Text = "Help";
            // 
            // mainmenuHelpContent
            // 
            this.mainmenuHelpContent.Name = "mainmenuHelpContent";
            this.mainmenuHelpContent.Size = new System.Drawing.Size(124, 22);
            this.mainmenuHelpContent.Text = "Content";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(121, 6);
            // 
            // MainMenuHelpAbout
            // 
            this.MainMenuHelpAbout.Name = "MainMenuHelpAbout";
            this.MainMenuHelpAbout.Size = new System.Drawing.Size(124, 22);
            this.MainMenuHelpAbout.Text = "About";
            this.MainMenuHelpAbout.Click += new System.EventHandler(this.MainMenuHelpAbout_Click);
            // 
            // MainForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.ClientSize = new System.Drawing.Size(540, 301);
            this.Controls.Add(this.ImageListView);
            this.Controls.Add(this.mainToolbar);
            this.Controls.Add(this.mainMenu);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.MainMenuStrip = this.mainMenu;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ReactOS Test Environment";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.mainToolbar.ResumeLayout(false);
            this.mainToolbar.PerformLayout();
            this.mainMenu.ResumeLayout(false);
            this.mainMenu.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        #endregion

        private System.Windows.Forms.ListView ImageListView;
        private System.Windows.Forms.ColumnHeader columnName;
        private System.Windows.Forms.ColumnHeader columnMemory;
        private System.Windows.Forms.ColumnHeader columnHardDisk;
        private System.Windows.Forms.ColumnHeader columnCdrom;
        private System.Windows.Forms.ColumnHeader columnNetwork;
        private System.Windows.Forms.ColumnHeader columnAudio;
        private System.Windows.Forms.ColumnHeader columnDebug;
        private System.Windows.Forms.ImageList mainImageList;
        private System.ComponentModel.IContainer components;
        private System.Windows.Forms.ToolStripButton toolbarLaunch;
        private System.Windows.Forms.ToolStripButton toolbarStop;
        private System.Windows.Forms.ToolStripSeparator toolbarSep1;
        private System.Windows.Forms.ToolStripButton toolbarImageAdd;
        private System.Windows.Forms.ToolStripButton toolbarImageDel;
        private System.Windows.Forms.ToolStripSeparator toolbarSep2;
        private System.Windows.Forms.ToolStripButton toolbarHelpContents;
        private System.Windows.Forms.ToolStripSeparator toolbarSep4;
        private System.Windows.Forms.ToolStripButton toolbarExit;
        private System.Windows.Forms.ToolStrip mainToolbar;
        private System.Windows.Forms.MenuStrip mainMenu;
        private System.Windows.Forms.ToolStripMenuItem mainmenuFile;
        private System.Windows.Forms.ToolStripMenuItem mainmenuFileExit;
        private System.Windows.Forms.ToolStripMenuItem mainmenuHelp;
        private System.Windows.Forms.ToolStripMenuItem mainmenuHelpContent;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem MainMenuHelpAbout;
        private System.Windows.Forms.ToolStripButton toolbarSnapShot;
        private System.Windows.Forms.ToolStripButton toolbarScreenShot;
        private System.Windows.Forms.ToolStripSeparator toolbarSep3;
        private System.Windows.Forms.ToolStripMenuItem mainmenuOptions;

    }
}
