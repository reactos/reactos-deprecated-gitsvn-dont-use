namespace RosTEGUI
{
    partial class DeleteVM
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DeleteVM));
            this.deleteOK = new System.Windows.Forms.Button();
            this.deleteCancel = new System.Windows.Forms.Button();
            this.deleteConsole = new System.Windows.Forms.RadioButton();
            this.deleteFullVM = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.deleteLab3 = new System.Windows.Forms.Label();
            this.deleteLab2 = new System.Windows.Forms.Label();
            this.deleteLab1 = new System.Windows.Forms.Label();
            this.deleteNameLab = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // deleteOK
            // 
            this.deleteOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.deleteOK.Location = new System.Drawing.Point(146, 212);
            this.deleteOK.Name = "deleteOK";
            this.deleteOK.Size = new System.Drawing.Size(75, 23);
            this.deleteOK.TabIndex = 0;
            this.deleteOK.Text = "OK";
            this.deleteOK.UseVisualStyleBackColor = true;
            // 
            // deleteCancel
            // 
            this.deleteCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.deleteCancel.Location = new System.Drawing.Point(227, 212);
            this.deleteCancel.Name = "deleteCancel";
            this.deleteCancel.Size = new System.Drawing.Size(75, 23);
            this.deleteCancel.TabIndex = 1;
            this.deleteCancel.Text = "Cancel";
            this.deleteCancel.UseVisualStyleBackColor = true;
            // 
            // deleteConsole
            // 
            this.deleteConsole.AutoSize = true;
            this.deleteConsole.Checked = true;
            this.deleteConsole.Location = new System.Drawing.Point(12, 24);
            this.deleteConsole.Name = "deleteConsole";
            this.deleteConsole.Size = new System.Drawing.Size(160, 17);
            this.deleteConsole.TabIndex = 3;
            this.deleteConsole.TabStop = true;
            this.deleteConsole.Text = "Delete only the console data";
            this.deleteConsole.UseVisualStyleBackColor = true;
            // 
            // deleteFullVM
            // 
            this.deleteFullVM.AutoSize = true;
            this.deleteFullVM.Location = new System.Drawing.Point(12, 87);
            this.deleteFullVM.Name = "deleteFullVM";
            this.deleteFullVM.Size = new System.Drawing.Size(221, 17);
            this.deleteFullVM.TabIndex = 4;
            this.deleteFullVM.TabStop = true;
            this.deleteFullVM.Text = "Delete everything including the VM image";
            this.deleteFullVM.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.deleteLab3);
            this.groupBox1.Controls.Add(this.deleteLab2);
            this.groupBox1.Controls.Add(this.deleteFullVM);
            this.groupBox1.Controls.Add(this.deleteConsole);
            this.groupBox1.Location = new System.Drawing.Point(12, 57);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(290, 149);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Please choose one of the following options";
            // 
            // deleteLab3
            // 
            this.deleteLab3.AutoSize = true;
            this.deleteLab3.Location = new System.Drawing.Point(32, 107);
            this.deleteLab3.Name = "deleteLab3";
            this.deleteLab3.Size = new System.Drawing.Size(242, 26);
            this.deleteLab3.TabIndex = 6;
            this.deleteLab3.Text = "This option will delete all data accociated with this\r\nvirtual machine, including" +
                " the image itself";
            // 
            // deleteLab2
            // 
            this.deleteLab2.AutoSize = true;
            this.deleteLab2.Location = new System.Drawing.Point(32, 44);
            this.deleteLab2.Name = "deleteLab2";
            this.deleteLab2.Size = new System.Drawing.Size(245, 26);
            this.deleteLab2.TabIndex = 5;
            this.deleteLab2.Text = "This option will delete all data accociated with this \r\nvirtual machine, however " +
                "it will leave the image";
            // 
            // deleteLab1
            // 
            this.deleteLab1.AutoSize = true;
            this.deleteLab1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.deleteLab1.Location = new System.Drawing.Point(62, 26);
            this.deleteLab1.Name = "deleteLab1";
            this.deleteLab1.Size = new System.Drawing.Size(54, 13);
            this.deleteLab1.TabIndex = 7;
            this.deleteLab1.Text = "Deleting";
            // 
            // deleteNameLab
            // 
            this.deleteNameLab.AutoSize = true;
            this.deleteNameLab.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.deleteNameLab.Location = new System.Drawing.Point(114, 26);
            this.deleteNameLab.Name = "deleteNameLab";
            this.deleteNameLab.Size = new System.Drawing.Size(51, 13);
            this.deleteNameLab.TabIndex = 8;
            this.deleteNameLab.Text = "<name>";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox1.ErrorImage = null;
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(12, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(44, 39);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 6;
            this.pictureBox1.TabStop = false;
            // 
            // DeleteVM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(314, 247);
            this.ControlBox = false;
            this.Controls.Add(this.deleteNameLab);
            this.Controls.Add(this.deleteLab1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.deleteCancel);
            this.Controls.Add(this.deleteOK);
            this.Name = "DeleteVM";
            this.Text = "Deleting ";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button deleteOK;
        private System.Windows.Forms.Button deleteCancel;
        private System.Windows.Forms.RadioButton deleteConsole;
        private System.Windows.Forms.RadioButton deleteFullVM;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label deleteLab3;
        private System.Windows.Forms.Label deleteLab2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label deleteLab1;
        private System.Windows.Forms.Label deleteNameLab;
    }
}