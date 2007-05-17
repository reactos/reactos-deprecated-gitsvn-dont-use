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
            this.selectionLstBox = new System.Windows.Forms.ListBox();
            this.baseContainerPanel = new System.Windows.Forms.Panel();
            this.tempDesignSheetTab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.generalPanel = new System.Windows.Forms.Panel();
            this.generalGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.pathsPanel = new System.Windows.Forms.Panel();
            this.pathsGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.drivesPanel = new System.Windows.Forms.Panel();
            this.drivesGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.networkPanel = new System.Windows.Forms.Panel();
            this.networkGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.ioPanel = new System.Windows.Forms.Panel();
            this.ioGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.mountPanel = new System.Windows.Forms.Panel();
            this.mountGrpBox = new System.Windows.Forms.GroupBox();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.debugPanel = new System.Windows.Forms.Panel();
            this.debugGrpBox = new System.Windows.Forms.GroupBox();
            this.settingsOKBtn = new System.Windows.Forms.Button();
            this.settingsCancelBtn = new System.Windows.Forms.Button();
            this.settingsApplyBtn = new System.Windows.Forms.Button();
            this.settingsHelpBtn = new System.Windows.Forms.Button();
            this.baseContainerPanel.SuspendLayout();
            this.tempDesignSheetTab.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.generalPanel.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.pathsPanel.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.drivesPanel.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.networkPanel.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.ioPanel.SuspendLayout();
            this.tabPage6.SuspendLayout();
            this.mountPanel.SuspendLayout();
            this.tabPage7.SuspendLayout();
            this.debugPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // selectionLstBox
            // 
            this.selectionLstBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.selectionLstBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.selectionLstBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.selectionLstBox.FormattingEnabled = true;
            this.selectionLstBox.ItemHeight = 45;
            this.selectionLstBox.Items.AddRange(new object[] {
            "General",
            "Paths",
            "Drives",
            "Network",
            "I/O",
            "Mount",
            "Debug"});
            this.selectionLstBox.Location = new System.Drawing.Point(12, 12);
            this.selectionLstBox.Name = "selectionLstBox";
            this.selectionLstBox.Size = new System.Drawing.Size(74, 362);
            this.selectionLstBox.TabIndex = 0;
            this.selectionLstBox.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.listboxSelection_DrawItem);
            this.selectionLstBox.SelectedIndexChanged += new System.EventHandler(this.listboxSelection_SelectedIndexChanged);
            // 
            // baseContainerPanel
            // 
            this.baseContainerPanel.Controls.Add(this.tempDesignSheetTab);
            this.baseContainerPanel.Location = new System.Drawing.Point(92, 6);
            this.baseContainerPanel.Name = "baseContainerPanel";
            this.baseContainerPanel.Size = new System.Drawing.Size(297, 396);
            this.baseContainerPanel.TabIndex = 2;
            // 
            // tempDesignSheetTab
            // 
            this.tempDesignSheetTab.Controls.Add(this.tabPage1);
            this.tempDesignSheetTab.Controls.Add(this.tabPage2);
            this.tempDesignSheetTab.Controls.Add(this.tabPage3);
            this.tempDesignSheetTab.Controls.Add(this.tabPage4);
            this.tempDesignSheetTab.Controls.Add(this.tabPage5);
            this.tempDesignSheetTab.Controls.Add(this.tabPage6);
            this.tempDesignSheetTab.Controls.Add(this.tabPage7);
            this.tempDesignSheetTab.Location = new System.Drawing.Point(0, 0);
            this.tempDesignSheetTab.Name = "tempDesignSheetTab";
            this.tempDesignSheetTab.SelectedIndex = 0;
            this.tempDesignSheetTab.Size = new System.Drawing.Size(294, 393);
            this.tempDesignSheetTab.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.generalPanel);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(286, 367);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "General";
            this.tabPage1.UseVisualStyleBackColor = true;
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
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.pathsPanel);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(286, 367);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Paths";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // pathsPanel
            // 
            this.pathsPanel.Controls.Add(this.pathsGrpBox);
            this.pathsPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pathsPanel.Location = new System.Drawing.Point(3, 3);
            this.pathsPanel.Name = "pathsPanel";
            this.pathsPanel.Size = new System.Drawing.Size(280, 361);
            this.pathsPanel.TabIndex = 0;
            // 
            // pathsGrpBox
            // 
            this.pathsGrpBox.Location = new System.Drawing.Point(3, 3);
            this.pathsGrpBox.Name = "pathsGrpBox";
            this.pathsGrpBox.Size = new System.Drawing.Size(283, 364);
            this.pathsGrpBox.TabIndex = 0;
            this.pathsGrpBox.TabStop = false;
            this.pathsGrpBox.Text = "Paths";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.drivesPanel);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(286, 367);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Drives";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // drivesPanel
            // 
            this.drivesPanel.Controls.Add(this.drivesGrpBox);
            this.drivesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.drivesPanel.Location = new System.Drawing.Point(3, 3);
            this.drivesPanel.Name = "drivesPanel";
            this.drivesPanel.Size = new System.Drawing.Size(280, 361);
            this.drivesPanel.TabIndex = 0;
            // 
            // drivesGrpBox
            // 
            this.drivesGrpBox.Location = new System.Drawing.Point(3, 3);
            this.drivesGrpBox.Name = "drivesGrpBox";
            this.drivesGrpBox.Size = new System.Drawing.Size(283, 364);
            this.drivesGrpBox.TabIndex = 0;
            this.drivesGrpBox.TabStop = false;
            this.drivesGrpBox.Text = "Drives";
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.networkPanel);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(286, 367);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Network";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // networkPanel
            // 
            this.networkPanel.Controls.Add(this.networkGrpBox);
            this.networkPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.networkPanel.Location = new System.Drawing.Point(3, 3);
            this.networkPanel.Name = "networkPanel";
            this.networkPanel.Size = new System.Drawing.Size(280, 361);
            this.networkPanel.TabIndex = 0;
            // 
            // networkGrpBox
            // 
            this.networkGrpBox.Location = new System.Drawing.Point(3, 3);
            this.networkGrpBox.Name = "networkGrpBox";
            this.networkGrpBox.Size = new System.Drawing.Size(283, 364);
            this.networkGrpBox.TabIndex = 0;
            this.networkGrpBox.TabStop = false;
            this.networkGrpBox.Text = "Network";
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.ioPanel);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(286, 367);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "I/O";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // ioPanel
            // 
            this.ioPanel.Controls.Add(this.ioGrpBox);
            this.ioPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ioPanel.Location = new System.Drawing.Point(3, 3);
            this.ioPanel.Name = "ioPanel";
            this.ioPanel.Size = new System.Drawing.Size(280, 361);
            this.ioPanel.TabIndex = 0;
            // 
            // ioGrpBox
            // 
            this.ioGrpBox.Location = new System.Drawing.Point(3, 3);
            this.ioGrpBox.Name = "ioGrpBox";
            this.ioGrpBox.Size = new System.Drawing.Size(283, 364);
            this.ioGrpBox.TabIndex = 0;
            this.ioGrpBox.TabStop = false;
            this.ioGrpBox.Text = "I/O";
            // 
            // tabPage6
            // 
            this.tabPage6.Controls.Add(this.mountPanel);
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage6.Size = new System.Drawing.Size(286, 367);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "Mount";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // mountPanel
            // 
            this.mountPanel.Controls.Add(this.mountGrpBox);
            this.mountPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mountPanel.Location = new System.Drawing.Point(3, 3);
            this.mountPanel.Name = "mountPanel";
            this.mountPanel.Size = new System.Drawing.Size(280, 361);
            this.mountPanel.TabIndex = 0;
            // 
            // mountGrpBox
            // 
            this.mountGrpBox.Location = new System.Drawing.Point(3, 3);
            this.mountGrpBox.Name = "mountGrpBox";
            this.mountGrpBox.Size = new System.Drawing.Size(283, 364);
            this.mountGrpBox.TabIndex = 0;
            this.mountGrpBox.TabStop = false;
            this.mountGrpBox.Text = "Mount";
            // 
            // tabPage7
            // 
            this.tabPage7.Controls.Add(this.debugPanel);
            this.tabPage7.Location = new System.Drawing.Point(4, 22);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage7.Size = new System.Drawing.Size(286, 367);
            this.tabPage7.TabIndex = 6;
            this.tabPage7.Text = "Debug";
            this.tabPage7.UseVisualStyleBackColor = true;
            // 
            // debugPanel
            // 
            this.debugPanel.Controls.Add(this.debugGrpBox);
            this.debugPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.debugPanel.Location = new System.Drawing.Point(3, 3);
            this.debugPanel.Name = "debugPanel";
            this.debugPanel.Size = new System.Drawing.Size(280, 361);
            this.debugPanel.TabIndex = 0;
            // 
            // debugGrpBox
            // 
            this.debugGrpBox.Location = new System.Drawing.Point(3, 3);
            this.debugGrpBox.Name = "debugGrpBox";
            this.debugGrpBox.Size = new System.Drawing.Size(283, 364);
            this.debugGrpBox.TabIndex = 0;
            this.debugGrpBox.TabStop = false;
            this.debugGrpBox.Text = "Debug";
            // 
            // settingsOKBtn
            // 
            this.settingsOKBtn.Location = new System.Drawing.Point(59, 400);
            this.settingsOKBtn.Name = "settingsOKBtn";
            this.settingsOKBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsOKBtn.TabIndex = 3;
            this.settingsOKBtn.Text = "OK";
            this.settingsOKBtn.UseVisualStyleBackColor = true;
            // 
            // settingsCancelBtn
            // 
            this.settingsCancelBtn.Location = new System.Drawing.Point(140, 400);
            this.settingsCancelBtn.Name = "settingsCancelBtn";
            this.settingsCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsCancelBtn.TabIndex = 4;
            this.settingsCancelBtn.Text = "Cancel";
            this.settingsCancelBtn.UseVisualStyleBackColor = true;
            // 
            // settingsApplyBtn
            // 
            this.settingsApplyBtn.Location = new System.Drawing.Point(221, 400);
            this.settingsApplyBtn.Name = "settingsApplyBtn";
            this.settingsApplyBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsApplyBtn.TabIndex = 5;
            this.settingsApplyBtn.Text = "Apply";
            this.settingsApplyBtn.UseVisualStyleBackColor = true;
            // 
            // settingsHelpBtn
            // 
            this.settingsHelpBtn.Location = new System.Drawing.Point(302, 400);
            this.settingsHelpBtn.Name = "settingsHelpBtn";
            this.settingsHelpBtn.Size = new System.Drawing.Size(75, 23);
            this.settingsHelpBtn.TabIndex = 6;
            this.settingsHelpBtn.Text = "Help";
            this.settingsHelpBtn.UseVisualStyleBackColor = true;
            // 
            // settingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(389, 435);
            this.Controls.Add(this.settingsHelpBtn);
            this.Controls.Add(this.settingsApplyBtn);
            this.Controls.Add(this.settingsCancelBtn);
            this.Controls.Add(this.settingsOKBtn);
            this.Controls.Add(this.baseContainerPanel);
            this.Controls.Add(this.selectionLstBox);
            this.Name = "settingsForm";
            this.Text = "SettingsForm";
            this.Load += new System.EventHandler(this.SettingsForm_Load);
            this.baseContainerPanel.ResumeLayout(false);
            this.tempDesignSheetTab.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.generalPanel.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.pathsPanel.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.drivesPanel.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.networkPanel.ResumeLayout(false);
            this.tabPage5.ResumeLayout(false);
            this.ioPanel.ResumeLayout(false);
            this.tabPage6.ResumeLayout(false);
            this.mountPanel.ResumeLayout(false);
            this.tabPage7.ResumeLayout(false);
            this.debugPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox selectionLstBox;
        private System.Windows.Forms.Panel baseContainerPanel;
        private System.Windows.Forms.Button settingsOKBtn;
        private System.Windows.Forms.Button settingsCancelBtn;
        private System.Windows.Forms.Button settingsApplyBtn;
        private System.Windows.Forms.Button settingsHelpBtn;
        private System.Windows.Forms.TabControl tempDesignSheetTab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel generalPanel;
        private System.Windows.Forms.GroupBox generalGrpBox;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Panel pathsPanel;
        private System.Windows.Forms.GroupBox pathsGrpBox;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Panel drivesPanel;
        private System.Windows.Forms.GroupBox drivesGrpBox;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Panel networkPanel;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.Panel ioPanel;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.Panel mountPanel;
        private System.Windows.Forms.TabPage tabPage7;
        private System.Windows.Forms.Panel debugPanel;
        private System.Windows.Forms.GroupBox networkGrpBox;
        private System.Windows.Forms.GroupBox ioGrpBox;
        private System.Windows.Forms.GroupBox mountGrpBox;
        private System.Windows.Forms.GroupBox debugGrpBox;
    }
}