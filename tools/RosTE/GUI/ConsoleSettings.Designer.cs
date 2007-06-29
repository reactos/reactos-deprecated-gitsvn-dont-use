namespace RosTEGUI
{
    partial class ConsoleSettings
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.conUpdateNow = new System.Windows.Forms.Button();
            this.conUpdateCombo = new System.Windows.Forms.ComboBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button5 = new System.Windows.Forms.Button();
            this.conDefVmLoc = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.conVdkLoc = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.conQemuLoc = new System.Windows.Forms.TextBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.conAppDebug = new System.Windows.Forms.CheckBox();
            this.conDialogOK = new System.Windows.Forms.Button();
            this.conDialogCancel = new System.Windows.Forms.Button();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(303, 287);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.groupBox4);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(295, 261);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "General";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.conUpdateNow);
            this.groupBox4.Controls.Add(this.conUpdateCombo);
            this.groupBox4.Location = new System.Drawing.Point(6, 153);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(283, 80);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Check for updates";
            // 
            // conUpdateNow
            // 
            this.conUpdateNow.Location = new System.Drawing.Point(202, 33);
            this.conUpdateNow.Name = "conUpdateNow";
            this.conUpdateNow.Size = new System.Drawing.Size(75, 23);
            this.conUpdateNow.TabIndex = 1;
            this.conUpdateNow.Text = "Check now";
            this.conUpdateNow.UseVisualStyleBackColor = true;
            // 
            // conUpdateCombo
            // 
            this.conUpdateCombo.FormattingEnabled = true;
            this.conUpdateCombo.Items.AddRange(new object[] {
            "On startup",
            "Daily",
            "Weekly",
            "Monthly"});
            this.conUpdateCombo.Location = new System.Drawing.Point(6, 33);
            this.conUpdateCombo.Name = "conUpdateCombo";
            this.conUpdateCombo.Size = new System.Drawing.Size(190, 21);
            this.conUpdateCombo.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.groupBox3);
            this.tabPage2.Controls.Add(this.groupBox2);
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(295, 261);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Paths";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.button5);
            this.groupBox3.Controls.Add(this.conDefVmLoc);
            this.groupBox3.Location = new System.Drawing.Point(6, 170);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(283, 65);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Default location for virtual machines";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(202, 28);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 23);
            this.button5.TabIndex = 1;
            this.button5.Text = "Browse...";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // conDefVmLoc
            // 
            this.conDefVmLoc.Location = new System.Drawing.Point(6, 30);
            this.conDefVmLoc.Name = "conDefVmLoc";
            this.conDefVmLoc.Size = new System.Drawing.Size(190, 20);
            this.conDefVmLoc.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button4);
            this.groupBox2.Controls.Add(this.conVdkLoc);
            this.groupBox2.Location = new System.Drawing.Point(6, 88);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(283, 65);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Location of VDK";
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(202, 27);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 1;
            this.button4.Text = "Browse...";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // conVdkLoc
            // 
            this.conVdkLoc.Location = new System.Drawing.Point(6, 29);
            this.conVdkLoc.Name = "conVdkLoc";
            this.conVdkLoc.Size = new System.Drawing.Size(190, 20);
            this.conVdkLoc.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button3);
            this.groupBox1.Controls.Add(this.conQemuLoc);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(283, 65);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Location of QEmu";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(202, 26);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 1;
            this.button3.Text = "Browse...";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // conQemuLoc
            // 
            this.conQemuLoc.Location = new System.Drawing.Point(6, 28);
            this.conQemuLoc.Name = "conQemuLoc";
            this.conQemuLoc.Size = new System.Drawing.Size(190, 20);
            this.conQemuLoc.TabIndex = 0;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.conAppDebug);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(295, 261);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Advanced";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // conAppDebug
            // 
            this.conAppDebug.AutoSize = true;
            this.conAppDebug.Location = new System.Drawing.Point(21, 33);
            this.conAppDebug.Name = "conAppDebug";
            this.conAppDebug.Size = new System.Drawing.Size(170, 17);
            this.conAppDebug.TabIndex = 0;
            this.conAppDebug.Text = "Turn on application debugging";
            this.conAppDebug.UseVisualStyleBackColor = true;
            // 
            // conDialogOK
            // 
            this.conDialogOK.Location = new System.Drawing.Point(159, 305);
            this.conDialogOK.Name = "conDialogOK";
            this.conDialogOK.Size = new System.Drawing.Size(75, 23);
            this.conDialogOK.TabIndex = 1;
            this.conDialogOK.Text = "OK";
            this.conDialogOK.UseVisualStyleBackColor = true;
            this.conDialogOK.Click += new System.EventHandler(this.conDialogOK_Click);
            // 
            // conDialogCancel
            // 
            this.conDialogCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.conDialogCancel.Location = new System.Drawing.Point(240, 305);
            this.conDialogCancel.Name = "conDialogCancel";
            this.conDialogCancel.Size = new System.Drawing.Size(75, 23);
            this.conDialogCancel.TabIndex = 2;
            this.conDialogCancel.Text = "Cancel";
            this.conDialogCancel.UseVisualStyleBackColor = true;
            // 
            // ConsoleSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(327, 340);
            this.Controls.Add(this.conDialogCancel);
            this.Controls.Add(this.conDialogOK);
            this.Controls.Add(this.tabControl1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ConsoleSettings";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "Settings";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button conDialogOK;
        private System.Windows.Forms.Button conDialogCancel;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.TextBox conDefVmLoc;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.TextBox conVdkLoc;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox conQemuLoc;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox conAppDebug;
        private System.Windows.Forms.Button conUpdateNow;
        private System.Windows.Forms.ComboBox conUpdateCombo;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
    }
}