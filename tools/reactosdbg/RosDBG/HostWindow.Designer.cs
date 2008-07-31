namespace RosDBG
{
    partial class HostWindow
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
            this.ContentSplitter = new System.Windows.Forms.SplitContainer();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redockToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ContentSplitter.Panel1.SuspendLayout();
            this.ContentSplitter.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // ContentSplitter
            // 
            this.ContentSplitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ContentSplitter.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.ContentSplitter.IsSplitterFixed = true;
            this.ContentSplitter.Location = new System.Drawing.Point(0, 0);
            this.ContentSplitter.Name = "ContentSplitter";
            this.ContentSplitter.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // ContentSplitter.Panel1
            // 
            this.ContentSplitter.Panel1.Controls.Add(this.menuStrip1);
            this.ContentSplitter.Size = new System.Drawing.Size(292, 273);
            this.ContentSplitter.SplitterDistance = 25;
            this.ContentSplitter.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.windowToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(292, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.redockToolStripMenuItem,
            this.closeToolStripMenuItem});
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.windowToolStripMenuItem.Text = "Window";
            // 
            // redockToolStripMenuItem
            // 
            this.redockToolStripMenuItem.Name = "redockToolStripMenuItem";
            this.redockToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.redockToolStripMenuItem.Text = "Redock";
            this.redockToolStripMenuItem.Click += new System.EventHandler(this.redockToolStripMenuItem_Click);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // HostWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 273);
            this.Controls.Add(this.ContentSplitter);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "HostWindow";
            this.Text = "HostWindow";
            this.ContentSplitter.Panel1.ResumeLayout(false);
            this.ContentSplitter.Panel1.PerformLayout();
            this.ContentSplitter.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer ContentSplitter;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redockToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
    }
}