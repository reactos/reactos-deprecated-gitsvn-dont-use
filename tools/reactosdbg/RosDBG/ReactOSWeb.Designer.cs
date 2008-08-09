namespace RosDBG
{
    partial class ReactOSWeb
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ReactOSWeb));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.navBack = new System.Windows.Forms.ToolStripButton();
            this.navForward = new System.Windows.Forms.ToolStripButton();
            this.navReload = new System.Windows.Forms.ToolStripButton();
            this.navHome = new System.Windows.Forms.ToolStripButton();
            this.AddressInput = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.navPaste = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.BugzillaInput = new System.Windows.Forms.ToolStripTextBox();
            this.BrowserView = new System.Windows.Forms.WebBrowser();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
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
            this.splitContainer1.Panel1.Controls.Add(this.toolStrip1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.BrowserView);
            this.splitContainer1.Size = new System.Drawing.Size(443, 317);
            this.splitContainer1.SplitterDistance = 27;
            this.splitContainer1.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.navBack,
            this.navForward,
            this.navReload,
            this.navHome,
            this.AddressInput,
            this.toolStripSeparator2,
            this.navPaste,
            this.toolStripSeparator1,
            this.toolStripLabel1,
            this.BugzillaInput});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(443, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // navBack
            // 
            this.navBack.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navBack.Enabled = false;
            this.navBack.Image = ((System.Drawing.Image)(resources.GetObject("navBack.Image")));
            this.navBack.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navBack.Name = "navBack";
            this.navBack.Size = new System.Drawing.Size(23, 22);
            this.navBack.ToolTipText = "Back";
            this.navBack.Click += new System.EventHandler(this.navBack_Click);
            // 
            // navForward
            // 
            this.navForward.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navForward.Enabled = false;
            this.navForward.Image = ((System.Drawing.Image)(resources.GetObject("navForward.Image")));
            this.navForward.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navForward.Name = "navForward";
            this.navForward.Size = new System.Drawing.Size(23, 22);
            this.navForward.Text = "toolStripButton2";
            this.navForward.ToolTipText = "Forward";
            this.navForward.Click += new System.EventHandler(this.navForward_Click);
            // 
            // navReload
            // 
            this.navReload.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navReload.Image = ((System.Drawing.Image)(resources.GetObject("navReload.Image")));
            this.navReload.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navReload.Name = "navReload";
            this.navReload.Size = new System.Drawing.Size(23, 22);
            this.navReload.Text = "toolStripButton1";
            this.navReload.ToolTipText = "Reload";
            this.navReload.Click += new System.EventHandler(this.navReload_Click);
            // 
            // navHome
            // 
            this.navHome.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navHome.Image = ((System.Drawing.Image)(resources.GetObject("navHome.Image")));
            this.navHome.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navHome.Name = "navHome";
            this.navHome.Size = new System.Drawing.Size(23, 22);
            this.navHome.Text = "toolStripButton6";
            this.navHome.ToolTipText = "Home";
            this.navHome.Click += new System.EventHandler(this.navHome_Click);
            // 
            // AddressInput
            // 
            this.AddressInput.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.AddressInput.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.AllUrl;
            this.AddressInput.Name = "AddressInput";
            this.AddressInput.Size = new System.Drawing.Size(200, 25);
            this.AddressInput.Sorted = true;
            this.AddressInput.KeyUp += new System.Windows.Forms.KeyEventHandler(this.AddressInput_KeyUp);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // navPaste
            // 
            this.navPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navPaste.Image = ((System.Drawing.Image)(resources.GetObject("navPaste.Image")));
            this.navPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navPaste.Name = "navPaste";
            this.navPaste.Size = new System.Drawing.Size(23, 22);
            this.navPaste.Text = "toolStripButton4";
            this.navPaste.ToolTipText = "Paste";
            this.navPaste.Click += new System.EventHandler(this.navPaste_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(48, 22);
            this.toolStripLabel1.Text = "Bugzilla";
            // 
            // BugzillaInput
            // 
            this.BugzillaInput.Name = "BugzillaInput";
            this.BugzillaInput.Size = new System.Drawing.Size(50, 23);
            this.BugzillaInput.KeyUp += new System.Windows.Forms.KeyEventHandler(this.BugzillaInput_KeyUp);
            // 
            // BrowserView
            // 
            this.BrowserView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.BrowserView.Location = new System.Drawing.Point(0, 0);
            this.BrowserView.MinimumSize = new System.Drawing.Size(20, 20);
            this.BrowserView.Name = "BrowserView";
            this.BrowserView.Size = new System.Drawing.Size(443, 286);
            this.BrowserView.TabIndex = 0;
            this.BrowserView.Navigating += new System.Windows.Forms.WebBrowserNavigatingEventHandler(this.BrowserView_Navigating);
            // 
            // ReactOSWeb
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "ReactOSWeb";
            this.Size = new System.Drawing.Size(443, 317);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.WebBrowser BrowserView;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton navBack;
        private System.Windows.Forms.ToolStripButton navForward;
        private System.Windows.Forms.ToolStripComboBox AddressInput;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton navPaste;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripTextBox BugzillaInput;
        private System.Windows.Forms.ToolStripButton navReload;
        private System.Windows.Forms.ToolStripButton navHome;
    }
}
