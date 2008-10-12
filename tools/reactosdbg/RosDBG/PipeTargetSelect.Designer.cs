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
            this.cType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // bOK
            // 
            this.bOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.bOK.Location = new System.Drawing.Point(69, 121);
            this.bOK.Name = "bOK";
            this.bOK.Size = new System.Drawing.Size(75, 23);
            this.bOK.TabIndex = 6;
            this.bOK.Text = "OK";
            this.bOK.UseVisualStyleBackColor = true;
            this.bOK.Click += new System.EventHandler(this.bOK_Click);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bCancel.Location = new System.Drawing.Point(150, 121);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(75, 23);
            this.bCancel.TabIndex = 7;
            this.bCancel.Text = "Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            this.bCancel.Click += new System.EventHandler(this.bCancel_Click);
            // 
            // PipeNameTextBox
            // 
            this.PipeNameTextBox.Enabled = false;
            this.PipeNameTextBox.Location = new System.Drawing.Point(12, 86);
            this.PipeNameTextBox.Name = "PipeNameTextBox";
            this.PipeNameTextBox.Size = new System.Drawing.Size(213, 20);
            this.PipeNameTextBox.TabIndex = 5;
            // 
            // DefaultRadioBtn
            // 
            this.DefaultRadioBtn.AutoSize = true;
            this.DefaultRadioBtn.Checked = true;
            this.DefaultRadioBtn.Location = new System.Drawing.Point(12, 38);
            this.DefaultRadioBtn.Name = "DefaultRadioBtn";
            this.DefaultRadioBtn.Size = new System.Drawing.Size(81, 17);
            this.DefaultRadioBtn.TabIndex = 3;
            this.DefaultRadioBtn.TabStop = true;
            this.DefaultRadioBtn.Text = "Use Default";
            this.DefaultRadioBtn.UseVisualStyleBackColor = true;
            // 
            // CustomRadioBtn
            // 
            this.CustomRadioBtn.AutoSize = true;
            this.CustomRadioBtn.Location = new System.Drawing.Point(12, 61);
            this.CustomRadioBtn.Name = "CustomRadioBtn";
            this.CustomRadioBtn.Size = new System.Drawing.Size(91, 17);
            this.CustomRadioBtn.TabIndex = 4;
            this.CustomRadioBtn.Text = "Custom Name";
            this.CustomRadioBtn.UseVisualStyleBackColor = true;
            this.CustomRadioBtn.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // cType
            // 
            this.cType.FormattingEnabled = true;
            this.cType.Items.AddRange(new object[] {
            "Client",
            "Server"});
            this.cType.Location = new System.Drawing.Point(69, 6);
            this.cType.Name = "cType";
            this.cType.Size = new System.Drawing.Size(156, 21);
            this.cType.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Type";
            // 
            // PipeTargetSelect
            // 
            this.AcceptButton = this.bOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(237, 156);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cType);
            this.Controls.Add(this.CustomRadioBtn);
            this.Controls.Add(this.DefaultRadioBtn);
            this.Controls.Add(this.PipeNameTextBox);
            this.Controls.Add(this.bOK);
            this.Controls.Add(this.bCancel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "PipeTargetSelect";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Named Pipe";
            this.Load += new System.EventHandler(this.PipeTargetSelect_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bOK;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.TextBox PipeNameTextBox;
        private System.Windows.Forms.RadioButton DefaultRadioBtn;
        private System.Windows.Forms.RadioButton CustomRadioBtn;
        private System.Windows.Forms.ComboBox cType;
        private System.Windows.Forms.Label label1;
    }
}