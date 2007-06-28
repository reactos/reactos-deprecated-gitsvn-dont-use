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
            this.newhdNameLbl = new System.Windows.Forms.Label();
            this.newhdDriveLbl = new System.Windows.Forms.Label();
            this.newhdPathLbl = new System.Windows.Forms.Label();
            this.newhdSizeLbl = new System.Windows.Forms.Label();
            this.newhdOK = new System.Windows.Forms.Button();
            this.newhdCancel = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label5 = new System.Windows.Forms.Label();
            this.newhdNewImgRad = new System.Windows.Forms.RadioButton();
            this.newhdExistImgRad = new System.Windows.Forms.RadioButton();
            this.newhdSepDark = new System.Windows.Forms.Panel();
            this.newhdSepLight = new System.Windows.Forms.Panel();
            this.newhdHeaderpanel = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.newhdSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.newhdHeaderpanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // newhdName
            // 
            this.newhdName.Location = new System.Drawing.Point(47, 80);
            this.newhdName.Name = "newhdName";
            this.newhdName.Size = new System.Drawing.Size(184, 20);
            this.newhdName.TabIndex = 0;
            // 
            // newhdDrive
            // 
            this.newhdDrive.FormattingEnabled = true;
            this.newhdDrive.Location = new System.Drawing.Point(47, 159);
            this.newhdDrive.Name = "newhdDrive";
            this.newhdDrive.Size = new System.Drawing.Size(86, 21);
            this.newhdDrive.TabIndex = 3;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(156, 132);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "Browse";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // newhdPath
            // 
            this.newhdPath.Location = new System.Drawing.Point(47, 106);
            this.newhdPath.Name = "newhdPath";
            this.newhdPath.Size = new System.Drawing.Size(184, 20);
            this.newhdPath.TabIndex = 1;
            // 
            // newhdSize
            // 
            this.newhdSize.Location = new System.Drawing.Point(47, 186);
            this.newhdSize.Name = "newhdSize";
            this.newhdSize.Size = new System.Drawing.Size(84, 20);
            this.newhdSize.TabIndex = 4;
            // 
            // newhdBoot
            // 
            this.newhdBoot.AutoSize = true;
            this.newhdBoot.Location = new System.Drawing.Point(47, 220);
            this.newhdBoot.Name = "newhdBoot";
            this.newhdBoot.Size = new System.Drawing.Size(147, 17);
            this.newhdBoot.TabIndex = 5;
            this.newhdBoot.Text = "Set as primary boot image";
            this.newhdBoot.UseVisualStyleBackColor = true;
            // 
            // newhdNameLbl
            // 
            this.newhdNameLbl.AutoSize = true;
            this.newhdNameLbl.Location = new System.Drawing.Point(6, 83);
            this.newhdNameLbl.Name = "newhdNameLbl";
            this.newhdNameLbl.Size = new System.Drawing.Size(35, 13);
            this.newhdNameLbl.TabIndex = 6;
            this.newhdNameLbl.Text = "Name";
            // 
            // newhdDriveLbl
            // 
            this.newhdDriveLbl.AutoSize = true;
            this.newhdDriveLbl.Location = new System.Drawing.Point(6, 162);
            this.newhdDriveLbl.Name = "newhdDriveLbl";
            this.newhdDriveLbl.Size = new System.Drawing.Size(32, 13);
            this.newhdDriveLbl.TabIndex = 7;
            this.newhdDriveLbl.Text = "Drive";
            // 
            // newhdPathLbl
            // 
            this.newhdPathLbl.AutoSize = true;
            this.newhdPathLbl.Location = new System.Drawing.Point(6, 109);
            this.newhdPathLbl.Name = "newhdPathLbl";
            this.newhdPathLbl.Size = new System.Drawing.Size(29, 13);
            this.newhdPathLbl.TabIndex = 8;
            this.newhdPathLbl.Text = "Path";
            // 
            // newhdSizeLbl
            // 
            this.newhdSizeLbl.AutoSize = true;
            this.newhdSizeLbl.Location = new System.Drawing.Point(6, 188);
            this.newhdSizeLbl.Name = "newhdSizeLbl";
            this.newhdSizeLbl.Size = new System.Drawing.Size(27, 13);
            this.newhdSizeLbl.TabIndex = 9;
            this.newhdSizeLbl.Text = "Size";
            // 
            // newhdOK
            // 
            this.newhdOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.newhdOK.Location = new System.Drawing.Point(74, 249);
            this.newhdOK.Name = "newhdOK";
            this.newhdOK.Size = new System.Drawing.Size(75, 23);
            this.newhdOK.TabIndex = 6;
            this.newhdOK.Text = "Create";
            this.newhdOK.UseVisualStyleBackColor = true;
            this.newhdOK.Click += new System.EventHandler(this.newhdOK_Click);
            // 
            // newhdCancel
            // 
            this.newhdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.newhdCancel.Location = new System.Drawing.Point(156, 249);
            this.newhdCancel.Name = "newhdCancel";
            this.newhdCancel.Size = new System.Drawing.Size(75, 23);
            this.newhdCancel.TabIndex = 7;
            this.newhdCancel.Text = "Cancel";
            this.newhdCancel.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(3, 6);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(45, 31);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 12;
            this.pictureBox1.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(56, 8);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(139, 26);
            this.label5.TabIndex = 13;
            this.label5.Text = "Add a new hard disk for use\r\nwith your virtual machine";
            // 
            // newhdNewImgRad
            // 
            this.newhdNewImgRad.AutoSize = true;
            this.newhdNewImgRad.Checked = true;
            this.newhdNewImgRad.Location = new System.Drawing.Point(47, 53);
            this.newhdNewImgRad.Name = "newhdNewImgRad";
            this.newhdNewImgRad.Size = new System.Drawing.Size(78, 17);
            this.newhdNewImgRad.TabIndex = 14;
            this.newhdNewImgRad.TabStop = true;
            this.newhdNewImgRad.Text = "New image";
            this.newhdNewImgRad.UseVisualStyleBackColor = true;
            this.newhdNewImgRad.CheckedChanged += new System.EventHandler(this.newhdNewImgRad_CheckedChanged);
            // 
            // newhdExistImgRad
            // 
            this.newhdExistImgRad.AutoSize = true;
            this.newhdExistImgRad.Location = new System.Drawing.Point(139, 53);
            this.newhdExistImgRad.Name = "newhdExistImgRad";
            this.newhdExistImgRad.Size = new System.Drawing.Size(92, 17);
            this.newhdExistImgRad.TabIndex = 15;
            this.newhdExistImgRad.TabStop = true;
            this.newhdExistImgRad.Text = "Existing image";
            this.newhdExistImgRad.UseVisualStyleBackColor = true;
            this.newhdExistImgRad.CheckedChanged += new System.EventHandler(this.newhdExistImgRad_CheckedChanged);
            // 
            // newhdSepDark
            // 
            this.newhdSepDark.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.newhdSepDark.Location = new System.Drawing.Point(-2, 44);
            this.newhdSepDark.Name = "newhdSepDark";
            this.newhdSepDark.Size = new System.Drawing.Size(243, 1);
            this.newhdSepDark.TabIndex = 16;
            // 
            // newhdSepLight
            // 
            this.newhdSepLight.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.newhdSepLight.Location = new System.Drawing.Point(-2, 45);
            this.newhdSepLight.Name = "newhdSepLight";
            this.newhdSepLight.Size = new System.Drawing.Size(243, 1);
            this.newhdSepLight.TabIndex = 17;
            // 
            // newhdHeaderpanel
            // 
            this.newhdHeaderpanel.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.newhdHeaderpanel.Controls.Add(this.pictureBox1);
            this.newhdHeaderpanel.Controls.Add(this.label5);
            this.newhdHeaderpanel.Location = new System.Drawing.Point(0, 0);
            this.newhdHeaderpanel.Name = "newhdHeaderpanel";
            this.newhdHeaderpanel.Size = new System.Drawing.Size(241, 44);
            this.newhdHeaderpanel.TabIndex = 18;
            // 
            // NewHardDiskForm
            // 
            this.AcceptButton = this.newhdOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.newhdCancel;
            this.ClientSize = new System.Drawing.Size(241, 280);
            this.Controls.Add(this.newhdHeaderpanel);
            this.Controls.Add(this.newhdSepLight);
            this.Controls.Add(this.newhdSepDark);
            this.Controls.Add(this.newhdExistImgRad);
            this.Controls.Add(this.newhdNewImgRad);
            this.Controls.Add(this.newhdCancel);
            this.Controls.Add(this.newhdOK);
            this.Controls.Add(this.newhdSizeLbl);
            this.Controls.Add(this.newhdPathLbl);
            this.Controls.Add(this.newhdDriveLbl);
            this.Controls.Add(this.newhdNameLbl);
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
            this.newhdHeaderpanel.ResumeLayout(false);
            this.newhdHeaderpanel.PerformLayout();
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
        private System.Windows.Forms.Label newhdNameLbl;
        private System.Windows.Forms.Label newhdDriveLbl;
        private System.Windows.Forms.Label newhdPathLbl;
        private System.Windows.Forms.Label newhdSizeLbl;
        private System.Windows.Forms.Button newhdOK;
        private System.Windows.Forms.Button newhdCancel;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.RadioButton newhdNewImgRad;
        private System.Windows.Forms.RadioButton newhdExistImgRad;
        private System.Windows.Forms.Panel newhdSepDark;
        private System.Windows.Forms.Panel newhdSepLight;
        private System.Windows.Forms.Panel newhdHeaderpanel;
    }
}