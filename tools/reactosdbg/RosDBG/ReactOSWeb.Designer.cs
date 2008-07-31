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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.Paste = new System.Windows.Forms.LinkLabel();
            this.label2 = new System.Windows.Forms.Label();
            this.BugzillaNumber = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.AddressInput = new System.Windows.Forms.TextBox();
            this.BrowserView = new System.Windows.Forms.WebBrowser();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
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
            this.splitContainer1.Panel1.Controls.Add(this.Paste);
            this.splitContainer1.Panel1.Controls.Add(this.label2);
            this.splitContainer1.Panel1.Controls.Add(this.BugzillaNumber);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.AddressInput);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.BrowserView);
            this.splitContainer1.Size = new System.Drawing.Size(443, 317);
            this.splitContainer1.SplitterDistance = 27;
            this.splitContainer1.TabIndex = 0;
            // 
            // Paste
            // 
            this.Paste.AutoSize = true;
            this.Paste.Location = new System.Drawing.Point(223, 6);
            this.Paste.Name = "Paste";
            this.Paste.Size = new System.Drawing.Size(34, 13);
            this.Paste.TabIndex = 4;
            this.Paste.TabStop = true;
            this.Paste.Text = "Paste";
            this.Paste.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.Paste_LinkClicked);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(291, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(43, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Bugzilla";
            // 
            // BugzillaNumber
            // 
            this.BugzillaNumber.Location = new System.Drawing.Point(340, 3);
            this.BugzillaNumber.Name = "BugzillaNumber";
            this.BugzillaNumber.Size = new System.Drawing.Size(100, 20);
            this.BugzillaNumber.TabIndex = 2;
            this.BugzillaNumber.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.BugzillaNumber_KeyPress);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Address";
            // 
            // AddressInput
            // 
            this.AddressInput.Location = new System.Drawing.Point(56, 3);
            this.AddressInput.Name = "AddressInput";
            this.AddressInput.Size = new System.Drawing.Size(161, 20);
            this.AddressInput.TabIndex = 0;
            this.AddressInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.AddressInput_KeyPress);
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
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox AddressInput;
        private System.Windows.Forms.LinkLabel Paste;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox BugzillaNumber;
        private System.Windows.Forms.WebBrowser BrowserView;
    }
}
