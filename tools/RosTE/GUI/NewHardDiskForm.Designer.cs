namespace RosTEGUI
{
    partial class NewHardDiskForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewHardDiskForm));
            this.newhdName = new System.Windows.Forms.TextBox();
            this.newhdDrive = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.newhdPath = new System.Windows.Forms.TextBox();
            this.newhdSize = new System.Windows.Forms.NumericUpDown();
            this.newhdBoot = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.newhdOK = new System.Windows.Forms.Button();
            this.newhdCancel = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label5 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.newhdSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // newhdName
            // 
            this.newhdName.Location = new System.Drawing.Point(45, 49);
            this.newhdName.Name = "newhdName";
            this.newhdName.Size = new System.Drawing.Size(184, 20);
            this.newhdName.TabIndex = 0;
            // 
            // newhdDrive
            // 
            this.newhdDrive.FormattingEnabled = true;
            this.newhdDrive.Location = new System.Drawing.Point(45, 128);
            this.newhdDrive.Name = "newhdDrive";
            this.newhdDrive.Size = new System.Drawing.Size(86, 21);
            this.newhdDrive.TabIndex = 3;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(154, 101);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "Browse";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // newhdPath
            // 
            this.newhdPath.Location = new System.Drawing.Point(45, 75);
            this.newhdPath.Name = "newhdPath";
            this.newhdPath.Size = new System.Drawing.Size(184, 20);
            this.newhdPath.TabIndex = 1;
            // 
            // newhdSize
            // 
            this.newhdSize.Location = new System.Drawing.Point(45, 155);
            this.newhdSize.Name = "newhdSize";
            this.newhdSize.Size = new System.Drawing.Size(84, 20);
            this.newhdSize.TabIndex = 4;
            // 
            // newhdBoot
            // 
            this.newhdBoot.AutoSize = true;
            this.newhdBoot.Location = new System.Drawing.Point(45, 192);
            this.newhdBoot.Name = "newhdBoot";
            this.newhdBoot.Size = new System.Drawing.Size(147, 17);
            this.newhdBoot.TabIndex = 5;
            this.newhdBoot.Text = "Set as primary boot image";
            this.newhdBoot.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 52);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Name";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 131);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Drive";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Path";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 157);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Size";
            // 
            // newhdOK
            // 
            this.newhdOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.newhdOK.Location = new System.Drawing.Point(72, 218);
            this.newhdOK.Name = "newhdOK";
            this.newhdOK.Size = new System.Drawing.Size(75, 23);
            this.newhdOK.TabIndex = 6;
            this.newhdOK.Text = "OK";
            this.newhdOK.UseVisualStyleBackColor = true;
            // 
            // newhdCancel
            // 
            this.newhdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.newhdCancel.Location = new System.Drawing.Point(154, 218);
            this.newhdCancel.Name = "newhdCancel";
            this.newhdCancel.Size = new System.Drawing.Size(75, 23);
            this.newhdCancel.TabIndex = 7;
            this.newhdCancel.Text = "Cancel";
            this.newhdCancel.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(7, 9);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(45, 31);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 12;
            this.pictureBox1.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(58, 9);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(151, 26);
            this.label5.TabIndex = 13;
            this.label5.Text = "Create a new hard disk for use\r\nwith your virtual machine";
            // 
            // NewHardDiskForm
            // 
            this.AcceptButton = this.newhdOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.newhdCancel;
            this.ClientSize = new System.Drawing.Size(241, 248);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.newhdCancel);
            this.Controls.Add(this.newhdOK);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.newhdBoot);
            this.Controls.Add(this.newhdSize);
            this.Controls.Add(this.newhdPath);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.newhdDrive);
            this.Controls.Add(this.newhdName);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NewHardDiskForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "New harddisk";
            ((System.ComponentModel.ISupportInitialize)(this.newhdSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox newhdName;
        private System.Windows.Forms.ComboBox newhdDrive;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox newhdPath;
        private System.Windows.Forms.NumericUpDown newhdSize;
        private System.Windows.Forms.CheckBox newhdBoot;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button newhdOK;
        private System.Windows.Forms.Button newhdCancel;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label5;
    }
}