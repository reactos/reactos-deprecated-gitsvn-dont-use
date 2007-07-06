namespace RosTEGUI
{
    partial class NewNetCardForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewNetCardForm));
            this.newhdHeaderpanel = new System.Windows.Forms.Panel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label5 = new System.Windows.Forms.Label();
            this.newhdSepLight = new System.Windows.Forms.Panel();
            this.newhdSepDark = new System.Windows.Forms.Panel();
            this.newhdHeaderpanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // newhdHeaderpanel
            // 
            this.newhdHeaderpanel.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.newhdHeaderpanel.Controls.Add(this.pictureBox1);
            this.newhdHeaderpanel.Controls.Add(this.label5);
            this.newhdHeaderpanel.Location = new System.Drawing.Point(0, 0);
            this.newhdHeaderpanel.Name = "newhdHeaderpanel";
            this.newhdHeaderpanel.Size = new System.Drawing.Size(292, 44);
            this.newhdHeaderpanel.TabIndex = 21;
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
            this.label5.Size = new System.Drawing.Size(132, 26);
            this.label5.TabIndex = 13;
            this.label5.Text = "Add a new netcard for use\r\nwith your virtual machine";
            // 
            // newhdSepLight
            // 
            this.newhdSepLight.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.newhdSepLight.Location = new System.Drawing.Point(-2, 45);
            this.newhdSepLight.Name = "newhdSepLight";
            this.newhdSepLight.Size = new System.Drawing.Size(292, 1);
            this.newhdSepLight.TabIndex = 20;
            // 
            // newhdSepDark
            // 
            this.newhdSepDark.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.newhdSepDark.Location = new System.Drawing.Point(-2, 44);
            this.newhdSepDark.Name = "newhdSepDark";
            this.newhdSepDark.Size = new System.Drawing.Size(292, 1);
            this.newhdSepDark.TabIndex = 19;
            // 
            // NewNetCardForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.newhdHeaderpanel);
            this.Controls.Add(this.newhdSepLight);
            this.Controls.Add(this.newhdSepDark);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NewNetCardForm";
            this.ShowIcon = false;
            this.Text = "New Netcard";
            this.newhdHeaderpanel.ResumeLayout(false);
            this.newhdHeaderpanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel newhdHeaderpanel;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel newhdSepLight;
        private System.Windows.Forms.Panel newhdSepDark;
    }
}