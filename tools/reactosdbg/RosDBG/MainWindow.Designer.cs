namespace RosDBG
{
    partial class MainWindow
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
            this.components = new System.ComponentModel.Container();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openSourceFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.connectSerialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectPipeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectTCPIPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.symbolDirectoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteToInteractionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.memoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bugcheckToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.continueToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stepToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.nextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.breakToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.breakHereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.breakpointClipboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.followListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.NewWindowItem = new System.Windows.Forms.ToolStripMenuItem();
            this.detachCurrentTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeCurrentTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.infoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.WorkTabs = new System.Windows.Forms.TabControl();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.RunStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.contextMenuTabStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.contextMenuTabStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.menuStrip1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(529, 416);
            this.splitContainer1.SplitterDistance = 26;
            this.splitContainer1.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.debugToolStripMenuItem,
            this.windowToolStripMenuItem,
            this.toolStripMenuItem2});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(529, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openSourceFileToolStripMenuItem,
            this.toolStripSeparator2,
            this.connectSerialToolStripMenuItem,
            this.connectPipeToolStripMenuItem,
            this.connectTCPIPToolStripMenuItem,
            this.toolStripSeparator3,
            this.symbolDirectoryToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // openSourceFileToolStripMenuItem
            // 
            this.openSourceFileToolStripMenuItem.Name = "openSourceFileToolStripMenuItem";
            this.openSourceFileToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openSourceFileToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.openSourceFileToolStripMenuItem.Text = "&Open Source File ...";
            this.openSourceFileToolStripMenuItem.Click += new System.EventHandler(this.openSourceFileToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(210, 6);
            // 
            // connectSerialToolStripMenuItem
            // 
            this.connectSerialToolStripMenuItem.Name = "connectSerialToolStripMenuItem";
            this.connectSerialToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.L)));
            this.connectSerialToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.connectSerialToolStripMenuItem.Text = "Connect Seria&l ...";
            this.connectSerialToolStripMenuItem.Click += new System.EventHandler(this.connectSerialToolStripMenuItem_Click);
            // 
            // connectPipeToolStripMenuItem
            // 
            this.connectPipeToolStripMenuItem.Name = "connectPipeToolStripMenuItem";
            this.connectPipeToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.connectPipeToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.connectPipeToolStripMenuItem.Text = "Connect &Pipe ...";
            this.connectPipeToolStripMenuItem.Click += new System.EventHandler(this.connectPipeToolStripMenuItem_Click);
            // 
            // connectTCPIPToolStripMenuItem
            // 
            this.connectTCPIPToolStripMenuItem.Name = "connectTCPIPToolStripMenuItem";
            this.connectTCPIPToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.T)));
            this.connectTCPIPToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.connectTCPIPToolStripMenuItem.Text = "Connect &TCP/IP ...";
            this.connectTCPIPToolStripMenuItem.Click += new System.EventHandler(this.connectTCPIPToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(210, 6);
            // 
            // symbolDirectoryToolStripMenuItem
            // 
            this.symbolDirectoryToolStripMenuItem.Name = "symbolDirectoryToolStripMenuItem";
            this.symbolDirectoryToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.symbolDirectoryToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.symbolDirectoryToolStripMenuItem.Text = "&Settings ...";
            this.symbolDirectoryToolStripMenuItem.Click += new System.EventHandler(this.symbolDirectoryToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(210, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.X)));
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem,
            this.pasteToInteractionToolStripMenuItem,
            this.memoryToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.copyToolStripMenuItem.Text = "&Copy";
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // pasteToInteractionToolStripMenuItem
            // 
            this.pasteToInteractionToolStripMenuItem.Enabled = false;
            this.pasteToInteractionToolStripMenuItem.Name = "pasteToInteractionToolStripMenuItem";
            this.pasteToInteractionToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.pasteToInteractionToolStripMenuItem.Text = "&Paste to Interaction";
            // 
            // memoryToolStripMenuItem
            // 
            this.memoryToolStripMenuItem.Enabled = false;
            this.memoryToolStripMenuItem.Name = "memoryToolStripMenuItem";
            this.memoryToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.memoryToolStripMenuItem.Text = "&Memory at Clipboard";
            // 
            // debugToolStripMenuItem
            // 
            this.debugToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.bugcheckToolStripMenuItem,
            this.continueToolStripMenuItem,
            this.stepToolStripMenuItem,
            this.nextToolStripMenuItem,
            this.toolStripSeparator4,
            this.breakToolStripMenuItem,
            this.breakHereToolStripMenuItem,
            this.breakpointClipboardToolStripMenuItem,
            this.followListToolStripMenuItem});
            this.debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            this.debugToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.debugToolStripMenuItem.Text = "&Debug";
            // 
            // bugcheckToolStripMenuItem
            // 
            this.bugcheckToolStripMenuItem.Enabled = false;
            this.bugcheckToolStripMenuItem.Name = "bugcheckToolStripMenuItem";
            this.bugcheckToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.bugcheckToolStripMenuItem.Text = "&Bugcheck";
            // 
            // continueToolStripMenuItem
            // 
            this.continueToolStripMenuItem.Name = "continueToolStripMenuItem";
            this.continueToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.continueToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.continueToolStripMenuItem.Text = "&Continue";
            this.continueToolStripMenuItem.Click += new System.EventHandler(this.continueToolStripMenuItem_Click);
            // 
            // stepToolStripMenuItem
            // 
            this.stepToolStripMenuItem.Name = "stepToolStripMenuItem";
            this.stepToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F11;
            this.stepToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.stepToolStripMenuItem.Text = "&Step";
            this.stepToolStripMenuItem.Click += new System.EventHandler(this.stepToolStripMenuItem_Click);
            // 
            // nextToolStripMenuItem
            // 
            this.nextToolStripMenuItem.Name = "nextToolStripMenuItem";
            this.nextToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F10;
            this.nextToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.nextToolStripMenuItem.Text = "&Next";
            this.nextToolStripMenuItem.Click += new System.EventHandler(this.nextToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(170, 6);
            // 
            // breakToolStripMenuItem
            // 
            this.breakToolStripMenuItem.Name = "breakToolStripMenuItem";
            this.breakToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.breakToolStripMenuItem.Text = "B&reak";
            this.breakToolStripMenuItem.Click += new System.EventHandler(this.breakToolStripMenuItem_Click);
            // 
            // breakHereToolStripMenuItem
            // 
            this.breakHereToolStripMenuItem.Enabled = false;
            this.breakHereToolStripMenuItem.Name = "breakHereToolStripMenuItem";
            this.breakHereToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F9;
            this.breakHereToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.breakHereToolStripMenuItem.Text = "Breakpoint &Here";
            // 
            // breakpointClipboardToolStripMenuItem
            // 
            this.breakpointClipboardToolStripMenuItem.Enabled = false;
            this.breakpointClipboardToolStripMenuItem.Name = "breakpointClipboardToolStripMenuItem";
            this.breakpointClipboardToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.breakpointClipboardToolStripMenuItem.Text = "Breakpoint Cli&pboard";
            // 
            // followListToolStripMenuItem
            // 
            this.followListToolStripMenuItem.Enabled = false;
            this.followListToolStripMenuItem.Name = "followListToolStripMenuItem";
            this.followListToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
            this.followListToolStripMenuItem.Text = "&Follow List";
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NewWindowItem,
            this.detachCurrentTabToolStripMenuItem,
            this.closeCurrentTabToolStripMenuItem});
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.windowToolStripMenuItem.Text = "&Window";
            // 
            // NewWindowItem
            // 
            this.NewWindowItem.Name = "NewWindowItem";
            this.NewWindowItem.Size = new System.Drawing.Size(169, 22);
            this.NewWindowItem.Text = "&New";
            // 
            // detachCurrentTabToolStripMenuItem
            // 
            this.detachCurrentTabToolStripMenuItem.Name = "detachCurrentTabToolStripMenuItem";
            this.detachCurrentTabToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.detachCurrentTabToolStripMenuItem.Text = "&Detach Current Tab";
            this.detachCurrentTabToolStripMenuItem.Click += new System.EventHandler(this.detachCurrentTabToolStripMenuItem_Click);
            // 
            // closeCurrentTabToolStripMenuItem
            // 
            this.closeCurrentTabToolStripMenuItem.Name = "closeCurrentTabToolStripMenuItem";
            this.closeCurrentTabToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.closeCurrentTabToolStripMenuItem.Text = "&Close Current Tab";
            this.closeCurrentTabToolStripMenuItem.Click += new System.EventHandler(this.closeCurrentTabToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.helpToolStripMenuItem,
            this.infoToolStripMenuItem});
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(24, 20);
            this.toolStripMenuItem2.Text = "&?";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F1;
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.helpToolStripMenuItem.Text = "&Help";
            this.helpToolStripMenuItem.Click += new System.EventHandler(this.helpToolStripMenuItem_Click);
            // 
            // infoToolStripMenuItem
            // 
            this.infoToolStripMenuItem.Name = "infoToolStripMenuItem";
            this.infoToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.infoToolStripMenuItem.Text = "&About";
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer2.IsSplitterFixed = true;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.WorkTabs);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.statusStrip1);
            this.splitContainer2.Size = new System.Drawing.Size(529, 386);
            this.splitContainer2.SplitterDistance = 357;
            this.splitContainer2.TabIndex = 0;
            // 
            // WorkTabs
            // 
            this.WorkTabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.WorkTabs.Location = new System.Drawing.Point(0, 0);
            this.WorkTabs.Name = "WorkTabs";
            this.WorkTabs.SelectedIndex = 0;
            this.WorkTabs.Size = new System.Drawing.Size(529, 357);
            this.WorkTabs.TabIndex = 1;
            this.WorkTabs.MouseClick += new System.Windows.Forms.MouseEventHandler(this.WorkTabs_MouseClick);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.RunStatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 3);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(529, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // RunStatus
            // 
            this.RunStatus.Name = "RunStatus";
            this.RunStatus.Size = new System.Drawing.Size(0, 17);
            // 
            // contextMenuTabStrip
            // 
            this.contextMenuTabStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.closeToolStripMenuItem});
            this.contextMenuTabStrip.Name = "contextMenuTabStrip";
            this.contextMenuTabStrip.Size = new System.Drawing.Size(101, 26);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.closeToolStripMenuItem.Text = "&Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(529, 416);
            this.Controls.Add(this.splitContainer1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Text = "ReactOS Debug Shell";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            this.splitContainer2.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.contextMenuTabStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem NewWindowItem;
        private System.Windows.Forms.ToolStripMenuItem detachCurrentTabToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeCurrentTabToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.TabControl WorkTabs;
        private System.Windows.Forms.ToolStripMenuItem continueToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bugcheckToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openSourceFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem symbolDirectoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectSerialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectPipeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectTCPIPToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem breakHereToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem breakpointClipboardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem followListToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pasteToInteractionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem memoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem breakToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel RunStatus;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ContextMenuStrip contextMenuTabStrip;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem infoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stepToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem nextToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;

    }
}



