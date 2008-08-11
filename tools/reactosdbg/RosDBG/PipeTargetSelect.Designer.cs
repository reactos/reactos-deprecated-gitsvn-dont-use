namespace RosDBG
{
    partial class PipeTargetSelect
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
            this.bOK = new System.Windows.Forms.Button();
            this.bCancel = new System.Windows.Forms.Button();
            this.PipeNameTextBox = new System.Windows.Forms.TextBox();
            this.DefaultRadioBtn = new System.Windows.Forms.RadioButton();
            this.CustomRadioBtn = new System.Windows.Forms.RadioButton();
            this.SuspendLayout();
            // 
            // bOK
            // 
            this.bOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bOK.Location = new System.Drawing.Point(69, 86);
            this.bOK.Name = "bOK";
            this.bOK.Size = new System.Drawing.Size(75, 23);
            this.bOK.TabIndex = 5;
            this.bOK.Text = "OK";
            this.bOK.UseVisualStyleBackColor = true;
            this.bOK.Click += new System.EventHandler(this.bOK_Click);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.Location = new System.Drawing.Point(150, 86);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(75, 23);
            this.bCancel.TabIndex = 6;
            this.bCancel.Text = "Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            this.bCancel.Click += new System.EventHandler(this.bCancel_Click);
            // 
            // PipeNameTextBox
            // 
            this.PipeNameTextBox.Enabled = false;
            this.PipeNameTextBox.Location = new System.Drawing.Point(15, 60);
            this.PipeNameTextBox.Name = "PipeNameTextBox";
            this.PipeNameTextBox.Size = new System.Drawing.Size(213, 20);
            this.PipeNameTextBox.TabIndex = 8;
            // 
            // DefaultRadioBtn
            // 
            this.DefaultRadioBtn.AutoSize = true;
            this.DefaultRadioBtn.Checked = true;
            this.DefaultRadioBtn.Location = new System.Drawing.Point(15, 12);
            this.DefaultRadioBtn.Name = "DefaultRadioBtn";
            this.DefaultRadioBtn.Size = new System.Drawing.Size(81, 17);
            this.DefaultRadioBtn.TabIndex = 9;
            this.DefaultRadioBtn.TabStop = true;
            this.DefaultRadioBtn.Text = "Use Default";
            this.DefaultRadioBtn.UseVisualStyleBackColor = true;
            // 
            // CustomRadioBtn
            // 
            this.CustomRadioBtn.AutoSize = true;
            this.CustomRadioBtn.Location = new System.Drawing.Point(15, 37);
            this.CustomRadioBtn.Name = "CustomRadioBtn";
            this.CustomRadioBtn.Size = new System.Drawing.Size(91, 17);
            this.CustomRadioBtn.TabIndex = 10;
            this.CustomRadioBtn.Text = "Custom Name";
            this.CustomRadioBtn.UseVisualStyleBackColor = true;
            this.CustomRadioBtn.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // PipeTargetSelect
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(237, 121);
            this.Controls.Add(this.CustomRadioBtn);
            this.Controls.Add(this.DefaultRadioBtn);
            this.Controls.Add(this.PipeNameTextBox);
            this.Controls.Add(this.bOK);
            this.Controls.Add(this.bCancel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "PipeTargetSelect";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "PipeTargetSelect";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bOK;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.TextBox PipeNameTextBox;
        private System.Windows.Forms.RadioButton DefaultRadioBtn;
        private System.Windows.Forms.RadioButton CustomRadioBtn;
    }
}