namespace RosDBG
{
    partial class Connect
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
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabSerial = new System.Windows.Forms.TabPage();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cBaud = new System.Windows.Forms.ComboBox();
            this.cPort = new System.Windows.Forms.ComboBox();
            this.tabPipe = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.cType = new System.Windows.Forms.ComboBox();
            this.CustomRadioBtn = new System.Windows.Forms.RadioButton();
            this.DefaultRadioBtn = new System.Windows.Forms.RadioButton();
            this.PipeNameTextBox = new System.Windows.Forms.TextBox();
            this.tabSocket = new System.Windows.Forms.TabPage();
            this.PortNumber = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.HostChoice = new System.Windows.Forms.ComboBox();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.tabControl.SuspendLayout();
            this.tabSerial.SuspendLayout();
            this.tabPipe.SuspendLayout();
            this.tabSocket.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PortNumber)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabSerial);
            this.tabControl.Controls.Add(this.tabPipe);
            this.tabControl.Controls.Add(this.tabSocket);
            this.tabControl.Location = new System.Drawing.Point(12, 12);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(343, 207);
            this.tabControl.TabIndex = 0;
            this.tabControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
            // 
            // tabSerial
            // 
            this.tabSerial.Controls.Add(this.label2);
            this.tabSerial.Controls.Add(this.label3);
            this.tabSerial.Controls.Add(this.cBaud);
            this.tabSerial.Controls.Add(this.cPort);
            this.tabSerial.Location = new System.Drawing.Point(4, 22);
            this.tabSerial.Name = "tabSerial";
            this.tabSerial.Padding = new System.Windows.Forms.Padding(3);
            this.tabSerial.Size = new System.Drawing.Size(335, 181);
            this.tabSerial.TabIndex = 0;
            this.tabSerial.Text = "Serial";
            this.tabSerial.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(88, 102);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Baud";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(88, 63);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(26, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Port";
            // 
            // cBaud
            // 
            this.cBaud.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cBaud.FormattingEnabled = true;
            this.cBaud.Items.AddRange(new object[] {
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.cBaud.Location = new System.Drawing.Point(126, 99);
            this.cBaud.Name = "cBaud";
            this.cBaud.Size = new System.Drawing.Size(121, 21);
            this.cBaud.TabIndex = 8;
            // 
            // cPort
            // 
            this.cPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cPort.FormattingEnabled = true;
            this.cPort.Location = new System.Drawing.Point(126, 60);
            this.cPort.Name = "cPort";
            this.cPort.Size = new System.Drawing.Size(121, 21);
            this.cPort.TabIndex = 7;
            // 
            // tabPipe
            // 
            this.tabPipe.Controls.Add(this.label1);
            this.tabPipe.Controls.Add(this.cType);
            this.tabPipe.Controls.Add(this.CustomRadioBtn);
            this.tabPipe.Controls.Add(this.DefaultRadioBtn);
            this.tabPipe.Controls.Add(this.PipeNameTextBox);
            this.tabPipe.Location = new System.Drawing.Point(4, 22);
            this.tabPipe.Name = "tabPipe";
            this.tabPipe.Padding = new System.Windows.Forms.Padding(3);
            this.tabPipe.Size = new System.Drawing.Size(335, 181);
            this.tabPipe.TabIndex = 1;
            this.tabPipe.Text = "Named Pipe";
            this.tabPipe.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(61, 43);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Type";
            // 
            // cType
            // 
            this.cType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cType.FormattingEnabled = true;
            this.cType.Items.AddRange(new object[] {
            "Client",
            "Server"});
            this.cType.Location = new System.Drawing.Point(118, 40);
            this.cType.Name = "cType";
            this.cType.Size = new System.Drawing.Size(156, 21);
            this.cType.TabIndex = 7;
            // 
            // CustomRadioBtn
            // 
            this.CustomRadioBtn.AutoSize = true;
            this.CustomRadioBtn.Location = new System.Drawing.Point(61, 95);
            this.CustomRadioBtn.Name = "CustomRadioBtn";
            this.CustomRadioBtn.Size = new System.Drawing.Size(91, 17);
            this.CustomRadioBtn.TabIndex = 9;
            this.CustomRadioBtn.Text = "Custom Name";
            this.CustomRadioBtn.UseVisualStyleBackColor = true;
            this.CustomRadioBtn.CheckedChanged += new System.EventHandler(this.CustomRadioBtn_CheckedChanged);
            // 
            // DefaultRadioBtn
            // 
            this.DefaultRadioBtn.AutoSize = true;
            this.DefaultRadioBtn.Checked = true;
            this.DefaultRadioBtn.Location = new System.Drawing.Point(61, 72);
            this.DefaultRadioBtn.Name = "DefaultRadioBtn";
            this.DefaultRadioBtn.Size = new System.Drawing.Size(81, 17);
            this.DefaultRadioBtn.TabIndex = 8;
            this.DefaultRadioBtn.TabStop = true;
            this.DefaultRadioBtn.Text = "Use Default";
            this.DefaultRadioBtn.UseVisualStyleBackColor = true;
            // 
            // PipeNameTextBox
            // 
            this.PipeNameTextBox.Enabled = false;
            this.PipeNameTextBox.Location = new System.Drawing.Point(61, 120);
            this.PipeNameTextBox.Name = "PipeNameTextBox";
            this.PipeNameTextBox.Size = new System.Drawing.Size(213, 20);
            this.PipeNameTextBox.TabIndex = 10;
            // 
            // tabSocket
            // 
            this.tabSocket.Controls.Add(this.PortNumber);
            this.tabSocket.Controls.Add(this.label4);
            this.tabSocket.Controls.Add(this.label5);
            this.tabSocket.Controls.Add(this.HostChoice);
            this.tabSocket.Location = new System.Drawing.Point(4, 22);
            this.tabSocket.Name = "tabSocket";
            this.tabSocket.Size = new System.Drawing.Size(335, 181);
            this.tabSocket.TabIndex = 2;
            this.tabSocket.Text = "Socket";
            this.tabSocket.UseVisualStyleBackColor = true;
            // 
            // PortNumber
            // 
            this.PortNumber.Location = new System.Drawing.Point(103, 99);
            this.PortNumber.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.PortNumber.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PortNumber.Name = "PortNumber";
            this.PortNumber.Size = new System.Drawing.Size(80, 20);
            this.PortNumber.TabIndex = 7;
            this.PortNumber.Value = new decimal(new int[] {
            1235,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(68, 101);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(26, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Port";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(68, 64);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 13);
            this.label5.TabIndex = 5;
            this.label5.Text = "Host";
            // 
            // HostChoice
            // 
            this.HostChoice.FormattingEnabled = true;
            this.HostChoice.Location = new System.Drawing.Point(103, 61);
            this.HostChoice.Name = "HostChoice";
            this.HostChoice.Size = new System.Drawing.Size(164, 21);
            this.HostChoice.TabIndex = 4;
            this.HostChoice.Text = "localhost";
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(266, 225);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(89, 27);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(171, 225);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(89, 27);
            this.btnOK.TabIndex = 2;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // Connect
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(361, 261);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.tabControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Connect";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Connect";
            this.Load += new System.EventHandler(this.Connect_Load);
            this.tabControl.ResumeLayout(false);
            this.tabSerial.ResumeLayout(false);
            this.tabSerial.PerformLayout();
            this.tabPipe.ResumeLayout(false);
            this.tabPipe.PerformLayout();
            this.tabSocket.ResumeLayout(false);
            this.tabSocket.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PortNumber)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabSerial;
        private System.Windows.Forms.TabPage tabPipe;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cType;
        private System.Windows.Forms.RadioButton CustomRadioBtn;
        private System.Windows.Forms.RadioButton DefaultRadioBtn;
        private System.Windows.Forms.TextBox PipeNameTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cBaud;
        private System.Windows.Forms.ComboBox cPort;
        private System.Windows.Forms.TabPage tabSocket;
        private System.Windows.Forms.NumericUpDown PortNumber;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox HostChoice;
    }
}