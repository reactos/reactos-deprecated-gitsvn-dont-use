namespace RosDBG
{
    partial class SourceView
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
            this.SourceCode = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // SourceCode
            // 
            this.SourceCode.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SourceCode.Location = new System.Drawing.Point(0, 0);
            this.SourceCode.Name = "SourceCode";
            this.SourceCode.ReadOnly = true;
            this.SourceCode.Size = new System.Drawing.Size(497, 308);
            this.SourceCode.TabIndex = 0;
            this.SourceCode.Text = "";
            // 
            // SourceView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.SourceCode);
            this.Name = "SourceView";
            this.Size = new System.Drawing.Size(497, 308);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox SourceCode;
    }
}
