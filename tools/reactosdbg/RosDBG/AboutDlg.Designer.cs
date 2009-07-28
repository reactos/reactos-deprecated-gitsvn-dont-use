namespace RosDBG
{
    partial class AboutDlg
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutDlg));
            this.btnOK = new System.Windows.Forms.Button();
            this.rosDbgLabel = new System.Windows.Forms.Label();
            this.creditsLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(180, 224);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(92, 28);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // rosDbgLabel
            // 
            this.rosDbgLabel.AutoSize = true;
            this.rosDbgLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rosDbgLabel.Location = new System.Drawing.Point(37, 30);
            this.rosDbgLabel.Name = "rosDbgLabel";
            this.rosDbgLabel.Size = new System.Drawing.Size(164, 13);
            this.rosDbgLabel.TabIndex = 1;
            this.rosDbgLabel.Text = "ReactOS Remote Debugger";
            // 
            // creditsLabel
            // 
            this.creditsLabel.AutoSize = true;
            this.creditsLabel.Location = new System.Drawing.Point(37, 70);
            this.creditsLabel.Name = "creditsLabel";
            this.creditsLabel.Size = new System.Drawing.Size(108, 78);
            this.creditsLabel.TabIndex = 3;
            this.creditsLabel.Text = "Brought to you by:\r\n\r\nArt Yerkes\r\nChristoph von Wittich\r\nGed Murphy\r\nGregor Schne" +
                "ider\r\n";
            // 
            // AboutDlg
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 264);
            this.Controls.Add(this.creditsLabel);
            this.Controls.Add(this.rosDbgLabel);
            this.Controls.Add(this.btnOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "About ReactOS Remote Debugger";
            this.Load += new System.EventHandler(this.AboutDlg_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Label rosDbgLabel;
        private System.Windows.Forms.Label creditsLabel;
    }
}