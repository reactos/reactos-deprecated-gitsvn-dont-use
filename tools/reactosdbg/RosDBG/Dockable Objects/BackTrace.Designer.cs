namespace RosDBG
{
    partial class BackTrace
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BackTrace));
            this.StackFrames = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // StackFrames
            // 
            this.StackFrames.Dock = System.Windows.Forms.DockStyle.Fill;
            this.StackFrames.FormattingEnabled = true;
            this.StackFrames.Location = new System.Drawing.Point(0, 0);
            this.StackFrames.Name = "StackFrames";
            this.StackFrames.Size = new System.Drawing.Size(241, 225);
            this.StackFrames.TabIndex = 0;
            this.StackFrames.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.StackFrames_MouseDoubleClick);
            this.StackFrames.SelectedIndexChanged += new System.EventHandler(this.StackFrames_SelectedIndexChanged);
            // 
            // BackTrace
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(241, 229);
            this.Controls.Add(this.StackFrames);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HideOnClose = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "BackTrace";
            this.Text = "Backtrace";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox StackFrames;

    }
}
