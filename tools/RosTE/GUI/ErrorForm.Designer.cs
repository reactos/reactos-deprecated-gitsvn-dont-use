namespace RosTEGUI
{
    partial class ErrorForm
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
            this.errorText = new System.Windows.Forms.TextBox();
            this.errorCloseBtn = new System.Windows.Forms.Button();
            this.errorCopyBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // errorText
            // 
            this.errorText.Location = new System.Drawing.Point(12, 12);
            this.errorText.Multiline = true;
            this.errorText.Name = "errorText";
            this.errorText.ReadOnly = true;
            this.errorText.Size = new System.Drawing.Size(436, 244);
            this.errorText.TabIndex = 0;
            // 
            // errorCloseBtn
            // 
            this.errorCloseBtn.Location = new System.Drawing.Point(373, 262);
            this.errorCloseBtn.Name = "errorCloseBtn";
            this.errorCloseBtn.Size = new System.Drawing.Size(75, 23);
            this.errorCloseBtn.TabIndex = 1;
            this.errorCloseBtn.Text = "Close";
            this.errorCloseBtn.UseVisualStyleBackColor = true;
            this.errorCloseBtn.Click += new System.EventHandler(this.errorCloseBtn_Click);
            // 
            // errorCopyBtn
            // 
            this.errorCopyBtn.Location = new System.Drawing.Point(12, 262);
            this.errorCopyBtn.Name = "errorCopyBtn";
            this.errorCopyBtn.Size = new System.Drawing.Size(75, 23);
            this.errorCopyBtn.TabIndex = 2;
            this.errorCopyBtn.Text = "Copy";
            this.errorCopyBtn.UseVisualStyleBackColor = true;
            this.errorCopyBtn.Click += new System.EventHandler(this.errorCopyBtn_Click);
            // 
            // ErrorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(460, 290);
            this.Controls.Add(this.errorCopyBtn);
            this.Controls.Add(this.errorCloseBtn);
            this.Controls.Add(this.errorText);
            this.MaximizeBox = false;
            this.Name = "ErrorForm";
            this.ShowIcon = false;
            this.Text = "Error";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox errorText;
        private System.Windows.Forms.Button errorCloseBtn;
        private System.Windows.Forms.Button errorCopyBtn;
    }
}