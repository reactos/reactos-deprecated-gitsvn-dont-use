namespace RosTEGUI
{
    partial class SettingsForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
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
            this.hardwareSelLstBox = new System.Windows.Forms.ListBox();
            this.hardwareContainerPanel = new System.Windows.Forms.Panel();
            this.hardwareTempDesignSheetTab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.memoryPanel = new System.Windows.Forms.Panel();
            this.memoryGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.cdromPanel = new System.Windows.Forms.Panel();
            this.cdromGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.harddiskPanel = new System.Windows.Forms.Panel();
            this.harddiskGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.networkPanel = new System.Windows.Forms.Panel();
            this.floppyGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.ethernetPanel = new System.Windows.Forms.Panel();
            this.ethernetGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.soundPanel = new System.Windows.Forms.Panel();
            this.soundGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.displayPanel = new System.Windows.Forms.Panel();
            this.displayGrpBox = new System.Windows.Forms.GroupBox();
            this.settingsOKBtn = new System.Windows.Forms.Button();
            this.settingsCancelBtn = new System.Windows.Forms.Button();
            this.settingsApplyBtn = new System.Windows.Forms.Button();
            this.settingsHelpBtn = new System.Windows.Forms.Button();
            this.settingsTab = new System.Windows.Forms.TabControl();
            this.settingsHardwareTab = new System.Windows.Forms.TabPage();
            this.settingsOptionsTab = new System.Windows.Forms.TabPage();
            this.optionsTempDesignSheetTab = new System.Windows.Forms.TabControl();
            this.tabPage8 = new System.Windows.Forms.TabPage();
            this.generalPanel = new System.Windows.Forms.Panel();
            this.generalGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage9 = new System.Windows.Forms.TabPage();
            this.powerPanel = new System.Windows.Forms.Panel();
            this.powerGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage10 = new System.Windows.Forms.TabPage();
            this.snapshotsPanel = new System.Windows.Forms.Panel();
            this.snapshotsGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage11 = new System.Windows.Forms.TabPage();
            this.advancedPanel = new System.Windows.Forms.Panel();
            this.advancedGrpBox = new System.Windows.Forms.GroupBox();
            this.optionsSelLstBox = new System.Windows.Forms.ListBox();
            this.optionsContainerPanel = new System.Windows.Forms.Panel();
            this.hardwareContainerPanel.SuspendLayout();
            this.hardwareTempDesignSheetTab.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.memoryPanel.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.cdromPanel.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.harddiskPanel.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.networkPanel.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.ethernetPanel.SuspendLayout();
            this.tabPage6.SuspendLayout();
            this.soundPanel.SuspendLayout();
            this.tabPage7.SuspendLayout();
            this.displayPanel.SuspendLayout();
            this.settingsTab.SuspendLayout();
            this.settingsHardwareTab.SuspendLayout();
            this.settingsOptionsTab.SuspendLayout();
            this.optionsTempDesignSheetTab.SuspendLayout();
            this.tabPage8.SuspendLayout();
            this.generalPanel.SuspendLayout();
            this.tabPage9.SuspendLayout();
            this.powerPanel.SuspendLayout();
            this.tabPage10.SuspendLayout();
            this.snapshotsPanel.SuspendLayout();
            this.tabPage11.SuspendLayout();
            this.advancedPanel.SuspendLayout();
            this.optionsContainerPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // hardwareSelLstBox
            // 
            this.hardwareSelLstBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.hardwareSelLstBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.hardwareSelLstBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.hardwareSelLstBox.FormattingEnabled = true;
            this.hardwareSelLstBox.ItemHeight = 45;
            this.hardwareSelLstBox.Items.AddRange(new object[] {
            "Memory",
            "CD-ROM",
            "Hard Disk",
            "Floppy",
            "Ethernet",
            "Sound",
            "Display"});
            this.hardwareSelLstBox.Location = new System.Drawing.Point(3, 6);
            this.hardwareSelLstBox.Name = "hardwareSelLstBox";
            this.hardwareSelLstBox.Size = new System.Drawing.Size(74, 362);
            this.hardwareSelLstBox.TabIndex = 0;
            this.hardwareSelLstBox.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.listboxSelection_DrawItem);
            this.hardwareSelLstBox.SelectedIndexChanged += new System.EventHandler(this.listboxSelection_SelectedIndexChanged);
            // 
            // hardwareContainerPanel
            // 
            this.hardwareContainerPanel.Controls.Add(this.hardwareTempDesignSheetTab);
            this.hardwareContainerPanel.Location = new System.Drawing.Point(83, 0);
            this.hardwareContainerPanel.Name = "hardwareContainerPanel";
            this.hardwareContainerPanel.Size = new System.Drawing.Size(297, 396);
            this.hardwareContainerPanel.TabIndex = 2;
            // 
            // hardwareTempDesignSheetTab
            // 
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage1);
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage2);
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage3);
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage4);
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage5);
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage6);
            this.hardwareTempDesignSheetTab.Controls.Add(this.tabPage7);
            this.hardwareTempDesignSheetTab.Location = new System.Drawing.Point(0, 0);
            this.hardwareTempDesignSheetTab.Name = "hardwareTempDesignSheetTab";
            this.hardwareTempDesignSheetTab.SelectedIndex = 0;
            this.hardwareTempDesignSheetTab.Size = new System.Drawing.Size(294, 393);
            this.hardwareTempDesignSheetTab.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.memoryPanel);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(286, 367);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Memory";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // memoryPanel
            // 
            this.memoryPanel.Controls.Add(this.memoryGrpBox);
            this.memoryPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.memoryPanel.Location = new System.Drawing.Point(3, 3);
            this.memoryPanel.Name = "memoryPanel";
            this.memoryPanel.Size = new System.Drawing.Size(280, 361);
            this.memoryPanel.TabIndex = 0;
            // 
            // memoryGrpBox
            // 
            this.memoryGrpBox.Location = new System.Drawing.Point(3, 3);
            this.memoryGrpBox.Name = "memoryGrpBox";
            this.memoryGrpBox.Size = new System.Drawing.Size(283, 364);
            this.memoryGrpBox.TabIndex = 0;
            this.memoryGrpBox.TabStop = false;
            this.memoryGrpBox.Text = "Memory";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.cdromPanel);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(286, 367);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "CD-ROM";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // cdromPanel
            // 
            this.cdromPanel.Controls.Add(this.cdromGrpBox);
            this.cdromPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cdromPanel.Location = new System.Drawing.Point(3, 3);
            this.cdromPanel.Name = "cdromPanel";
            this.cdromPanel.Size = new System.Drawing.Size(280, 361);
            this.cdromPanel.TabIndex = 0;
            // 
            // cdromGrpBox
            // 
            this.cdromGrpBox.Location = new System.Drawing.Point(3, 3);
            this.cdromGrpBox.Name = "cdromGrpBox";
            this.cdromGrpBox.Size = new System.Drawing.Size(283, 364);
            this.cdromGrpBox.TabIndex = 0;
            this.cdromGrpBox.TabStop = false;
            this.cdromGrpBox.Text = "CD-ROM";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.harddiskPanel);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(286, 367);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Hard Disk";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // harddiskPanel
            // 
            this.harddiskPanel.Controls.Add(this.harddiskGrpBox);
            this.harddiskPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.harddiskPanel.Location = new System.Drawing.Point(3, 3);
            this.harddiskPanel.Name = "harddiskPanel";
            this.harddiskPanel.Size = new System.Drawing.Size(280, 361);
            this.harddiskPanel.TabIndex = 0;
            // 
            // harddiskGrpBox
            // 
            this.harddiskGrpBox.Location = new System.Drawing.Point(3, 3);
            this.harddiskGrpBox.Name = "harddiskGrpBox";
            this.harddiskGrpBox.Size = new System.Drawing.Size(283, 364);
            this.harddiskGrpBox.TabIndex = 0;
            this.harddiskGrpBox.TabStop = false;
            this.harddiskGrpBox.Text = "Hard Disk";
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.networkPanel);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(286, 367);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Floppy";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // networkPanel
            // 
            this.networkPanel.Controls.Add(this.floppyGrpBox);
            this.networkPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.networkPanel.Location = new System.Drawing.Point(3, 3);
            this.networkPanel.Name = "networkPanel";
            this.networkPanel.Size = new System.Drawing.Size(280, 361);
            this.networkPanel.TabIndex = 0;
            // 
            // floppyGrpBox
            // 
            this.floppyGrpBox.Location = new System.Drawing.Point(3, 3);
            this.floppyGrpBox.Name = "floppyGrpBox";
            this.floppyGrpBox.Size = new System.Drawing.Size(283, 364);
            this.floppyGrpBox.TabIndex = 0;
            this.floppyGrpBox.TabStop = false;
            this.floppyGrpBox.Text = "Floppy";
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.ethernetPanel);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(286, 367);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "Ethernet";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // ethernetPanel
            // 
            this.ethernetPanel.Controls.Add(this.ethernetGrpBox);
            this.ethernetPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ethernetPanel.Location = new System.Drawing.Point(3, 3);
            this.ethernetPanel.Name = "ethernetPanel";
            this.ethernetPanel.Size = new System.Drawing.Size(280, 361);
            this.ethernetPanel.TabIndex = 0;
            // 
            // ethernetGrpBox
            // 
            this.ethernetGrpBox.Location = new System.Drawing.Point(3, 3);
            this.ethernetGrpBox.Name = "ethernetGrpBox";
            this.ethernetGrpBox.Size = new System.Drawing.Size(283, 364);
            this.ethernetGrpBox.TabIndex = 0;
            this.ethernetGrpBox.TabStop = false;
            this.ethernetGrpBox.Text = "Ethernet";
            // 
            // tabPage6
            // 
            this.tabPage6.Controls.Add(this.soundPanel);
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage6.Size = new System.Drawing.Size(286, 367);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "Sound";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // soundPanel
            // 
            this.soundPanel.Controls.Add(this.soundGrpBox);
            this.soundPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.soundPanel.Location = new System.Drawing.Point(3, 3);
            this.soundPanel.Name = "soundPanel";
            this.soundPanel.Size = new System.Drawing.Size(280, 361);
            this.soundPanel.TabIndex = 0;
            // 
            // soundGrpBox
            // 
            this.soundGrpBox.Location = new System.Drawing.Point(3, 3);
            this.soundGrpBox.Name = "soundGrpBox";
            this.soundGrpBox.Size = new System.Drawing.Size(283, 364);
            this.soundGrpBox.TabIndex = 0;
            this.soundGrpBox.TabStop = false;
            this.soundGrpBox.Text = "Sound";
            // 
            // tabPage7
            // 
            this.tabPage7.Controls.Add(this.displayPanel);
            this.tabPage7.Location = new System.Drawing.Point(4, 22);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage7.Size = new System.Drawing.Size(286, 367);
            this.tabPage7.TabIndex = 6;
            this.tabPage7.Text = "Display";
            this.tabPage7.UseVisualStyleBackColor = true;
            // 
            // displayPanel
            // 
            this.displayPanel.Controls.Add(this.displayGrpBox);
            this.displayPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.displayPanel.Location = new System.Drawing.Point(3, 3);
            this.displayPanel.Name = "displayPanel";
            this.displayPanel.Size = new System.Drawing.Size(280, 361);
            this.displayPanel.TabIndex = 0;
            // 
            // displayGrpBox
            // 
            this.displayGrpBox.Location = new System.Drawing.Point(3, 3);
            this.displayGrpBox.Name = "displayGrpBox";
            this.displayGrpBox.Size = new System.Drawing.Size(283, 364);
            this.displayGrpBox.TabIndex = 0;
            this.displayGrpBox.TabStop = false;
            this.displayGrpBox.Text = "Display";
            // 
            // settingsOKBtn
            // 
            this.settingsOKBtn.Location = new System.Drawing.Point(87, 444);
            this.settingsOKBtn.Name = "settingsOKBtn";
            this.settingsOKBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsOKBtn.TabIndex = 3;
            this.settingsOKBtn.Text = "OK";
            this.settingsOKBtn.UseVisualStyleBackColor = true;
            // 
            // settingsCancelBtn
            // 
            this.settingsCancelBtn.Location = new System.Drawing.Point(168, 444);
            this.settingsCancelBtn.Name = "settingsCancelBtn";
            this.settingsCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsCancelBtn.TabIndex = 4;
            this.settingsCancelBtn.Text = "Cancel";
            this.settingsCancelBtn.UseVisualStyleBackColor = true;
            // 
            // settingsApplyBtn
            // 
            this.settingsApplyBtn.Location = new System.Drawing.Point(249, 444);
            this.settingsApplyBtn.Name = "settingsApplyBtn";
            this.settingsApplyBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsApplyBtn.TabIndex = 5;
            this.settingsApplyBtn.Text = "Apply";
            this.settingsApplyBtn.UseVisualStyleBackColor = true;
            // 
            // settingsHelpBtn
            // 
            this.settingsHelpBtn.Location = new System.Drawing.Point(330, 444);
            this.settingsHelpBtn.Name = "settingsHelpBtn";
            this.settingsHelpBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsHelpBtn.TabIndex = 6;
            this.settingsHelpBtn.Text = "Help";
            this.settingsHelpBtn.UseVisualStyleBackColor = true;
            // 
            // settingsTab
            // 
            this.settingsTab.Controls.Add(this.settingsHardwareTab);
            this.settingsTab.Controls.Add(this.settingsOptionsTab);
            this.settingsTab.Location = new System.Drawing.Point(12, 12);
            this.settingsTab.Name = "settingsTab";
            this.settingsTab.SelectedIndex = 0;
            this.settingsTab.Size = new System.Drawing.Size(393, 426);
            this.settingsTab.TabIndex = 7;
            // 
            // settingsHardwareTab
            // 
            this.settingsHardwareTab.Controls.Add(this.hardwareSelLstBox);
            this.settingsHardwareTab.Controls.Add(this.hardwareContainerPanel);
            this.settingsHardwareTab.Location = new System.Drawing.Point(4, 22);
            this.settingsHardwareTab.Name = "settingsHardwareTab";
            this.settingsHardwareTab.Padding = new System.Windows.Forms.Padding(3);
            this.settingsHardwareTab.Size = new System.Drawing.Size(385, 400);
            this.settingsHardwareTab.TabIndex = 0;
            this.settingsHardwareTab.Text = "Hardware";
            this.settingsHardwareTab.UseVisualStyleBackColor = true;
            // 
            // settingsOptionsTab
            // 
            this.settingsOptionsTab.Controls.Add(this.optionsContainerPanel);
            this.settingsOptionsTab.Controls.Add(this.optionsSelLstBox);
            this.settingsOptionsTab.Location = new System.Drawing.Point(4, 22);
            this.settingsOptionsTab.Name = "settingsOptionsTab";
            this.settingsOptionsTab.Padding = new System.Windows.Forms.Padding(3);
            this.settingsOptionsTab.Size = new System.Drawing.Size(385, 400);
            this.settingsOptionsTab.TabIndex = 1;
            this.settingsOptionsTab.Text = "Options";
            this.settingsOptionsTab.UseVisualStyleBackColor = true;
            // 
            // optionsTempDesignSheetTab
            // 
            this.optionsTempDesignSheetTab.Controls.Add(this.tabPage8);
            this.optionsTempDesignSheetTab.Controls.Add(this.tabPage9);
            this.optionsTempDesignSheetTab.Controls.Add(this.tabPage10);
            this.optionsTempDesignSheetTab.Controls.Add(this.tabPage11);
            this.optionsTempDesignSheetTab.Location = new System.Drawing.Point(0, 0);
            this.optionsTempDesignSheetTab.Name = "optionsTempDesignSheetTab";
            this.optionsTempDesignSheetTab.SelectedIndex = 0;
            this.optionsTempDesignSheetTab.Size = new System.Drawing.Size(294, 393);
            this.optionsTempDesignSheetTab.TabIndex = 2;
            // 
            // tabPage8
            // 
            this.tabPage8.Controls.Add(this.generalPanel);
            this.tabPage8.Location = new System.Drawing.Point(4, 22);
            this.tabPage8.Name = "tabPage8";
            this.tabPage8.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage8.Size = new System.Drawing.Size(286, 367);
            this.tabPage8.TabIndex = 0;
            this.tabPage8.Text = "General";
            this.tabPage8.UseVisualStyleBackColor = true;
            // 
            // generalPanel
            // 
            this.generalPanel.Controls.Add(this.generalGrpBox);
            this.generalPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.generalPanel.Location = new System.Drawing.Point(3, 3);
            this.generalPanel.Name = "generalPanel";
            this.generalPanel.Size = new System.Drawing.Size(280, 361);
            this.generalPanel.TabIndex = 0;
            // 
            // generalGrpBox
            // 
            this.generalGrpBox.Location = new System.Drawing.Point(3, 3);
            this.generalGrpBox.Name = "generalGrpBox";
            this.generalGrpBox.Size = new System.Drawing.Size(283, 364);
            this.generalGrpBox.TabIndex = 0;
            this.generalGrpBox.TabStop = false;
            this.generalGrpBox.Text = "General";
            // 
            // tabPage9
            // 
            this.tabPage9.Controls.Add(this.powerPanel);
            this.tabPage9.Location = new System.Drawing.Point(4, 22);
            this.tabPage9.Name = "tabPage9";
            this.tabPage9.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage9.Size = new System.Drawing.Size(286, 367);
            this.tabPage9.TabIndex = 1;
            this.tabPage9.Text = "Power";
            this.tabPage9.UseVisualStyleBackColor = true;
            // 
            // powerPanel
            // 
            this.powerPanel.Controls.Add(this.powerGrpBox);
            this.powerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.powerPanel.Location = new System.Drawing.Point(3, 3);
            this.powerPanel.Name = "powerPanel";
            this.powerPanel.Size = new System.Drawing.Size(280, 361);
            this.powerPanel.TabIndex = 0;
            // 
            // powerGrpBox
            // 
            this.powerGrpBox.Location = new System.Drawing.Point(3, 3);
            this.powerGrpBox.Name = "powerGrpBox";
            this.powerGrpBox.Size = new System.Drawing.Size(283, 364);
            this.powerGrpBox.TabIndex = 0;
            this.powerGrpBox.TabStop = false;
            this.powerGrpBox.Text = "Power";
            // 
            // tabPage10
            // 
            this.tabPage10.Controls.Add(this.snapshotsPanel);
            this.tabPage10.Location = new System.Drawing.Point(4, 22);
            this.tabPage10.Name = "tabPage10";
            this.tabPage10.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage10.Size = new System.Drawing.Size(286, 367);
            this.tabPage10.TabIndex = 2;
            this.tabPage10.Text = "Snapshots";
            this.tabPage10.UseVisualStyleBackColor = true;
            // 
            // snapshotsPanel
            // 
            this.snapshotsPanel.Controls.Add(this.snapshotsGrpBox);
            this.snapshotsPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.snapshotsPanel.Location = new System.Drawing.Point(3, 3);
            this.snapshotsPanel.Name = "snapshotsPanel";
            this.snapshotsPanel.Size = new System.Drawing.Size(280, 361);
            this.snapshotsPanel.TabIndex = 0;
            // 
            // snapshotsGrpBox
            // 
            this.snapshotsGrpBox.Location = new System.Drawing.Point(3, 3);
            this.snapshotsGrpBox.Name = "snapshotsGrpBox";
            this.snapshotsGrpBox.Size = new System.Drawing.Size(283, 364);
            this.snapshotsGrpBox.TabIndex = 0;
            this.snapshotsGrpBox.TabStop = false;
            this.snapshotsGrpBox.Text = "Snapshots";
            // 
            // tabPage11
            // 
            this.tabPage11.Controls.Add(this.advancedPanel);
            this.tabPage11.Location = new System.Drawing.Point(4, 22);
            this.tabPage11.Name = "tabPage11";
            this.tabPage11.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage11.Size = new System.Drawing.Size(286, 367);
            this.tabPage11.TabIndex = 3;
            this.tabPage11.Text = "Advanced";
            this.tabPage11.UseVisualStyleBackColor = true;
            // 
            // advancedPanel
            // 
            this.advancedPanel.Controls.Add(this.advancedGrpBox);
            this.advancedPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.advancedPanel.Location = new System.Drawing.Point(3, 3);
            this.advancedPanel.Name = "advancedPanel";
            this.advancedPanel.Size = new System.Drawing.Size(280, 361);
            this.advancedPanel.TabIndex = 0;
            // 
            // advancedGrpBox
            // 
            this.advancedGrpBox.Location = new System.Drawing.Point(3, 3);
            this.advancedGrpBox.Name = "advancedGrpBox";
            this.advancedGrpBox.Size = new System.Drawing.Size(283, 364);
            this.advancedGrpBox.TabIndex = 0;
            this.advancedGrpBox.TabStop = false;
            this.advancedGrpBox.Text = "Advanced";
            // 
            // optionsSelLstBox
            // 
            this.optionsSelLstBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.optionsSelLstBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.optionsSelLstBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.optionsSelLstBox.FormattingEnabled = true;
            this.optionsSelLstBox.ItemHeight = 45;
            this.optionsSelLstBox.Items.AddRange(new object[] {
            "General",
            "Power",
            "Snapshots",
            "Advanced"});
            this.optionsSelLstBox.Location = new System.Drawing.Point(3, 6);
            this.optionsSelLstBox.Name = "optionsSelLstBox";
            this.optionsSelLstBox.Size = new System.Drawing.Size(74, 362);
            this.optionsSelLstBox.TabIndex = 1;
            this.optionsSelLstBox.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.listboxSelection_DrawItem);
            this.optionsSelLstBox.SelectedIndexChanged += new System.EventHandler(this.listboxSelection_SelectedIndexChanged);
            // 
            // optionsContainerPanel
            // 
            this.optionsContainerPanel.Controls.Add(this.optionsTempDesignSheetTab);
            this.optionsContainerPanel.Location = new System.Drawing.Point(83, 0);
            this.optionsContainerPanel.Name = "optionsContainerPanel";
            this.optionsContainerPanel.Size = new System.Drawing.Size(297, 396);
            this.optionsContainerPanel.TabIndex = 8;
            // 
            // SettingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(417, 475);
            this.Controls.Add(this.settingsTab);
            this.Controls.Add(this.settingsHelpBtn);
            this.Controls.Add(this.settingsApplyBtn);
            this.Controls.Add(this.settingsCancelBtn);
            this.Controls.Add(this.settingsOKBtn);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "SettingsForm";
            this.Text = "Virtual Machine Settings";
            this.Load += new System.EventHandler(this.SettingsForm_Load);
            this.hardwareContainerPanel.ResumeLayout(false);
            this.hardwareTempDesignSheetTab.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.memoryPanel.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.cdromPanel.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.harddiskPanel.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.networkPanel.ResumeLayout(false);
            this.tabPage5.ResumeLayout(false);
            this.ethernetPanel.ResumeLayout(false);
            this.tabPage6.ResumeLayout(false);
            this.soundPanel.ResumeLayout(false);
            this.tabPage7.ResumeLayout(false);
            this.displayPanel.ResumeLayout(false);
            this.settingsTab.ResumeLayout(false);
            this.settingsHardwareTab.ResumeLayout(false);
            this.settingsOptionsTab.ResumeLayout(false);
            this.optionsTempDesignSheetTab.ResumeLayout(false);
            this.tabPage8.ResumeLayout(false);
            this.generalPanel.ResumeLayout(false);
            this.tabPage9.ResumeLayout(false);
            this.powerPanel.ResumeLayout(false);
            this.tabPage10.ResumeLayout(false);
            this.snapshotsPanel.ResumeLayout(false);
            this.tabPage11.ResumeLayout(false);
            this.advancedPanel.ResumeLayout(false);
            this.optionsContainerPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox hardwareSelLstBox;
        private System.Windows.Forms.Panel hardwareContainerPanel;
        private System.Windows.Forms.Button settingsOKBtn;
        private System.Windows.Forms.Button settingsCancelBtn;
        private System.Windows.Forms.Button settingsApplyBtn;
        private System.Windows.Forms.Button settingsHelpBtn;
        private System.Windows.Forms.TabControl hardwareTempDesignSheetTab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel memoryPanel;
        private System.Windows.Forms.GroupBox memoryGrpBox;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Panel cdromPanel;
        private System.Windows.Forms.GroupBox cdromGrpBox;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Panel harddiskPanel;
        private System.Windows.Forms.GroupBox harddiskGrpBox;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Panel networkPanel;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.Panel ethernetPanel;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.Panel soundPanel;
        private System.Windows.Forms.TabPage tabPage7;
        private System.Windows.Forms.Panel displayPanel;
        private System.Windows.Forms.GroupBox floppyGrpBox;
        private System.Windows.Forms.GroupBox ethernetGrpBox;
        private System.Windows.Forms.GroupBox soundGrpBox;
        private System.Windows.Forms.GroupBox displayGrpBox;
        private System.Windows.Forms.TabControl settingsTab;
        private System.Windows.Forms.TabPage settingsHardwareTab;
        private System.Windows.Forms.TabPage settingsOptionsTab;
        private System.Windows.Forms.ListBox optionsSelLstBox;
        private System.Windows.Forms.TabControl optionsTempDesignSheetTab;
        private System.Windows.Forms.TabPage tabPage8;
        private System.Windows.Forms.Panel generalPanel;
        private System.Windows.Forms.GroupBox generalGrpBox;
        private System.Windows.Forms.TabPage tabPage9;
        private System.Windows.Forms.Panel powerPanel;
        private System.Windows.Forms.GroupBox powerGrpBox;
        private System.Windows.Forms.TabPage tabPage10;
        private System.Windows.Forms.Panel snapshotsPanel;
        private System.Windows.Forms.GroupBox snapshotsGrpBox;
        private System.Windows.Forms.TabPage tabPage11;
        private System.Windows.Forms.Panel advancedPanel;
        private System.Windows.Forms.GroupBox advancedGrpBox;
        private System.Windows.Forms.Panel optionsContainerPanel;
    }
}