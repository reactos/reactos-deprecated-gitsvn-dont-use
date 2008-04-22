namespace MsgTranslator
{
    partial class MainForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.mainLookupButton = new System.Windows.Forms.Button();
            this.mainErrTxtBox = new System.Windows.Forms.TextBox();
            this.mainTabControl = new System.Windows.Forms.TabControl();
            this.errorMsgTab = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.errorHexTxtBox = new System.Windows.Forms.TextBox();
            this.errorBackButton = new System.Windows.Forms.Button();
            this.errorForwardButton = new System.Windows.Forms.Button();
            this.errorTypeValueLabel = new System.Windows.Forms.Label();
            this.errorCodeTxtBox = new System.Windows.Forms.TextBox();
            this.errorCodeLabel = new System.Windows.Forms.Label();
            this.errorMessageTxtBox = new System.Windows.Forms.TextBox();
            this.errorDecimalTxtBox = new System.Windows.Forms.TextBox();
            this.errorTypeLabel = new System.Windows.Forms.Label();
            this.errorMessageLabel = new System.Windows.Forms.Label();
            this.errorNumberLabel = new System.Windows.Forms.Label();
            this.wndmsgTab = new System.Windows.Forms.TabPage();
            this.label2 = new System.Windows.Forms.Label();
            this.wndmsgHexTxtBox = new System.Windows.Forms.TextBox();
            this.wndmsgDecimalTxtBox = new System.Windows.Forms.TextBox();
            this.wndmsgCodeTxtBox = new System.Windows.Forms.TextBox();
            this.wndmsgCodeLabel = new System.Windows.Forms.Label();
            this.wndmsgNumberLabel = new System.Windows.Forms.Label();
            this.bugurlTab = new System.Windows.Forms.TabPage();
            this.bugLinkLabel = new System.Windows.Forms.LinkLabel();
            this.optionsTab = new System.Windows.Forms.TabPage();
            this.optionsRunStartChkBox = new System.Windows.Forms.CheckBox();
            this.optionsMinimizeChkBox = new System.Windows.Forms.CheckBox();
            this.mainErrLabel = new System.Windows.Forms.Label();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.mainTabControl.SuspendLayout();
            this.errorMsgTab.SuspendLayout();
            this.wndmsgTab.SuspendLayout();
            this.bugurlTab.SuspendLayout();
            this.optionsTab.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainLookupButton
            // 
            this.mainLookupButton.Location = new System.Drawing.Point(237, 12);
            this.mainLookupButton.Name = "mainLookupButton";
            this.mainLookupButton.Size = new System.Drawing.Size(75, 23);
            this.mainLookupButton.TabIndex = 1;
            this.mainLookupButton.Text = "Lookup";
            this.mainLookupButton.UseVisualStyleBackColor = true;
            this.mainLookupButton.Click += new System.EventHandler(this.mainLookupButton_Click);
            // 
            // mainErrTxtBox
            // 
            this.mainErrTxtBox.Location = new System.Drawing.Point(79, 13);
            this.mainErrTxtBox.Name = "mainErrTxtBox";
            this.mainErrTxtBox.Size = new System.Drawing.Size(152, 20);
            this.mainErrTxtBox.TabIndex = 0;
            this.mainErrTxtBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.mainErrTxtBox_KeyPress);
            // 
            // mainTabControl
            // 
            this.mainTabControl.Controls.Add(this.errorMsgTab);
            this.mainTabControl.Controls.Add(this.wndmsgTab);
            this.mainTabControl.Controls.Add(this.bugurlTab);
            this.mainTabControl.Controls.Add(this.optionsTab);
            this.mainTabControl.Location = new System.Drawing.Point(12, 41);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(300, 211);
            this.mainTabControl.TabIndex = 2;
            this.mainTabControl.SelectedIndexChanged += new System.EventHandler(this.mainTabControl_SelectedIndexChanged);
            // 
            // errorMsgTab
            // 
            this.errorMsgTab.Controls.Add(this.label1);
            this.errorMsgTab.Controls.Add(this.errorHexTxtBox);
            this.errorMsgTab.Controls.Add(this.errorBackButton);
            this.errorMsgTab.Controls.Add(this.errorForwardButton);
            this.errorMsgTab.Controls.Add(this.errorTypeValueLabel);
            this.errorMsgTab.Controls.Add(this.errorCodeTxtBox);
            this.errorMsgTab.Controls.Add(this.errorCodeLabel);
            this.errorMsgTab.Controls.Add(this.errorMessageTxtBox);
            this.errorMsgTab.Controls.Add(this.errorDecimalTxtBox);
            this.errorMsgTab.Controls.Add(this.errorTypeLabel);
            this.errorMsgTab.Controls.Add(this.errorMessageLabel);
            this.errorMsgTab.Controls.Add(this.errorNumberLabel);
            this.errorMsgTab.Location = new System.Drawing.Point(4, 22);
            this.errorMsgTab.Name = "errorMsgTab";
            this.errorMsgTab.Padding = new System.Windows.Forms.Padding(3);
            this.errorMsgTab.Size = new System.Drawing.Size(292, 185);
            this.errorMsgTab.TabIndex = 1;
            this.errorMsgTab.Text = "Error Msg";
            this.errorMsgTab.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(162, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(12, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "/";
            // 
            // errorHexTxtBox
            // 
            this.errorHexTxtBox.Location = new System.Drawing.Point(176, 38);
            this.errorHexTxtBox.Name = "errorHexTxtBox";
            this.errorHexTxtBox.ReadOnly = true;
            this.errorHexTxtBox.Size = new System.Drawing.Size(104, 20);
            this.errorHexTxtBox.TabIndex = 6;
            // 
            // errorBackButton
            // 
            this.errorBackButton.Location = new System.Drawing.Point(236, 6);
            this.errorBackButton.Name = "errorBackButton";
            this.errorBackButton.Size = new System.Drawing.Size(22, 22);
            this.errorBackButton.TabIndex = 3;
            this.errorBackButton.Text = "<";
            this.errorBackButton.UseVisualStyleBackColor = true;
            this.errorBackButton.Click += new System.EventHandler(this.NavigateErrorPage);
            // 
            // errorForwardButton
            // 
            this.errorForwardButton.Location = new System.Drawing.Point(258, 6);
            this.errorForwardButton.Name = "errorForwardButton";
            this.errorForwardButton.Size = new System.Drawing.Size(22, 22);
            this.errorForwardButton.TabIndex = 4;
            this.errorForwardButton.Text = ">";
            this.errorForwardButton.UseVisualStyleBackColor = true;
            this.errorForwardButton.Click += new System.EventHandler(this.NavigateErrorPage);
            // 
            // errorTypeValueLabel
            // 
            this.errorTypeValueLabel.AutoSize = true;
            this.errorTypeValueLabel.Location = new System.Drawing.Point(64, 15);
            this.errorTypeValueLabel.Name = "errorTypeValueLabel";
            this.errorTypeValueLabel.Size = new System.Drawing.Size(0, 13);
            this.errorTypeValueLabel.TabIndex = 0;
            // 
            // errorCodeTxtBox
            // 
            this.errorCodeTxtBox.Location = new System.Drawing.Point(63, 64);
            this.errorCodeTxtBox.Name = "errorCodeTxtBox";
            this.errorCodeTxtBox.ReadOnly = true;
            this.errorCodeTxtBox.Size = new System.Drawing.Size(217, 20);
            this.errorCodeTxtBox.TabIndex = 7;
            // 
            // errorCodeLabel
            // 
            this.errorCodeLabel.AutoSize = true;
            this.errorCodeLabel.Location = new System.Drawing.Point(10, 67);
            this.errorCodeLabel.Name = "errorCodeLabel";
            this.errorCodeLabel.Size = new System.Drawing.Size(35, 13);
            this.errorCodeLabel.TabIndex = 0;
            this.errorCodeLabel.Text = "Code:";
            // 
            // errorMessageTxtBox
            // 
            this.errorMessageTxtBox.Location = new System.Drawing.Point(13, 109);
            this.errorMessageTxtBox.Multiline = true;
            this.errorMessageTxtBox.Name = "errorMessageTxtBox";
            this.errorMessageTxtBox.ReadOnly = true;
            this.errorMessageTxtBox.Size = new System.Drawing.Size(267, 59);
            this.errorMessageTxtBox.TabIndex = 8;
            // 
            // errorDecimalTxtBox
            // 
            this.errorDecimalTxtBox.Location = new System.Drawing.Point(63, 38);
            this.errorDecimalTxtBox.Name = "errorDecimalTxtBox";
            this.errorDecimalTxtBox.ReadOnly = true;
            this.errorDecimalTxtBox.Size = new System.Drawing.Size(98, 20);
            this.errorDecimalTxtBox.TabIndex = 5;
            // 
            // errorTypeLabel
            // 
            this.errorTypeLabel.AutoSize = true;
            this.errorTypeLabel.Location = new System.Drawing.Point(10, 15);
            this.errorTypeLabel.Name = "errorTypeLabel";
            this.errorTypeLabel.Size = new System.Drawing.Size(34, 13);
            this.errorTypeLabel.TabIndex = 0;
            this.errorTypeLabel.Text = "Type:";
            // 
            // errorMessageLabel
            // 
            this.errorMessageLabel.AutoSize = true;
            this.errorMessageLabel.Location = new System.Drawing.Point(10, 93);
            this.errorMessageLabel.Name = "errorMessageLabel";
            this.errorMessageLabel.Size = new System.Drawing.Size(53, 13);
            this.errorMessageLabel.TabIndex = 0;
            this.errorMessageLabel.Text = "Message:";
            // 
            // errorNumberLabel
            // 
            this.errorNumberLabel.AutoSize = true;
            this.errorNumberLabel.Location = new System.Drawing.Point(10, 41);
            this.errorNumberLabel.Name = "errorNumberLabel";
            this.errorNumberLabel.Size = new System.Drawing.Size(47, 13);
            this.errorNumberLabel.TabIndex = 0;
            this.errorNumberLabel.Text = "Number:";
            // 
            // wndmsgTab
            // 
            this.wndmsgTab.Controls.Add(this.label2);
            this.wndmsgTab.Controls.Add(this.wndmsgHexTxtBox);
            this.wndmsgTab.Controls.Add(this.wndmsgDecimalTxtBox);
            this.wndmsgTab.Controls.Add(this.wndmsgCodeTxtBox);
            this.wndmsgTab.Controls.Add(this.wndmsgCodeLabel);
            this.wndmsgTab.Controls.Add(this.wndmsgNumberLabel);
            this.wndmsgTab.Location = new System.Drawing.Point(4, 22);
            this.wndmsgTab.Name = "wndmsgTab";
            this.wndmsgTab.Size = new System.Drawing.Size(292, 185);
            this.wndmsgTab.TabIndex = 2;
            this.wndmsgTab.Text = "Window Msg";
            this.wndmsgTab.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(162, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(12, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "/";
            // 
            // wndmsgHexTxtBox
            // 
            this.wndmsgHexTxtBox.Location = new System.Drawing.Point(176, 12);
            this.wndmsgHexTxtBox.Name = "wndmsgHexTxtBox";
            this.wndmsgHexTxtBox.ReadOnly = true;
            this.wndmsgHexTxtBox.Size = new System.Drawing.Size(104, 20);
            this.wndmsgHexTxtBox.TabIndex = 4;
            // 
            // wndmsgDecimalTxtBox
            // 
            this.wndmsgDecimalTxtBox.Location = new System.Drawing.Point(63, 12);
            this.wndmsgDecimalTxtBox.Name = "wndmsgDecimalTxtBox";
            this.wndmsgDecimalTxtBox.ReadOnly = true;
            this.wndmsgDecimalTxtBox.Size = new System.Drawing.Size(98, 20);
            this.wndmsgDecimalTxtBox.TabIndex = 3;
            // 
            // wndmsgCodeTxtBox
            // 
            this.wndmsgCodeTxtBox.Location = new System.Drawing.Point(63, 38);
            this.wndmsgCodeTxtBox.Name = "wndmsgCodeTxtBox";
            this.wndmsgCodeTxtBox.ReadOnly = true;
            this.wndmsgCodeTxtBox.Size = new System.Drawing.Size(217, 20);
            this.wndmsgCodeTxtBox.TabIndex = 5;
            // 
            // wndmsgCodeLabel
            // 
            this.wndmsgCodeLabel.AutoSize = true;
            this.wndmsgCodeLabel.Location = new System.Drawing.Point(10, 41);
            this.wndmsgCodeLabel.Name = "wndmsgCodeLabel";
            this.wndmsgCodeLabel.Size = new System.Drawing.Size(35, 13);
            this.wndmsgCodeLabel.TabIndex = 0;
            this.wndmsgCodeLabel.Text = "Code:";
            // 
            // wndmsgNumberLabel
            // 
            this.wndmsgNumberLabel.AutoSize = true;
            this.wndmsgNumberLabel.Location = new System.Drawing.Point(10, 15);
            this.wndmsgNumberLabel.Name = "wndmsgNumberLabel";
            this.wndmsgNumberLabel.Size = new System.Drawing.Size(47, 13);
            this.wndmsgNumberLabel.TabIndex = 0;
            this.wndmsgNumberLabel.Text = "Number:";
            // 
            // bugurlTab
            // 
            this.bugurlTab.Controls.Add(this.bugLinkLabel);
            this.bugurlTab.Location = new System.Drawing.Point(4, 22);
            this.bugurlTab.Name = "bugurlTab";
            this.bugurlTab.Size = new System.Drawing.Size(292, 185);
            this.bugurlTab.TabIndex = 3;
            this.bugurlTab.Text = "Bug Url";
            this.bugurlTab.UseVisualStyleBackColor = true;
            // 
            // bugLinkLabel
            // 
            this.bugLinkLabel.AutoSize = true;
            this.bugLinkLabel.Location = new System.Drawing.Point(60, 87);
            this.bugLinkLabel.Name = "bugLinkLabel";
            this.bugLinkLabel.Size = new System.Drawing.Size(145, 13);
            this.bugLinkLabel.TabIndex = 3;
            this.bugLinkLabel.TabStop = true;
            this.bugLinkLabel.Text = "ReactOS Bugzilla Homepage";
            this.bugLinkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bugLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.bugLinkLabel_LinkClicked);
            // 
            // optionsTab
            // 
            this.optionsTab.Controls.Add(this.optionsRunStartChkBox);
            this.optionsTab.Controls.Add(this.optionsMinimizeChkBox);
            this.optionsTab.Location = new System.Drawing.Point(4, 22);
            this.optionsTab.Name = "optionsTab";
            this.optionsTab.Size = new System.Drawing.Size(292, 185);
            this.optionsTab.TabIndex = 4;
            this.optionsTab.Text = "Options";
            this.optionsTab.UseVisualStyleBackColor = true;
            // 
            // optionsRunStartChkBox
            // 
            this.optionsRunStartChkBox.AutoSize = true;
            this.optionsRunStartChkBox.Location = new System.Drawing.Point(5, 32);
            this.optionsRunStartChkBox.Name = "optionsRunStartChkBox";
            this.optionsRunStartChkBox.Size = new System.Drawing.Size(150, 17);
            this.optionsRunStartChkBox.TabIndex = 4;
            this.optionsRunStartChkBox.Text = "Run when Windows starts";
            this.optionsRunStartChkBox.UseVisualStyleBackColor = true;
            this.optionsRunStartChkBox.CheckedChanged += new System.EventHandler(this.OptionsHaveChanged);
            // 
            // optionsMinimizeChkBox
            // 
            this.optionsMinimizeChkBox.AutoSize = true;
            this.optionsMinimizeChkBox.Location = new System.Drawing.Point(5, 9);
            this.optionsMinimizeChkBox.Name = "optionsMinimizeChkBox";
            this.optionsMinimizeChkBox.Size = new System.Drawing.Size(125, 17);
            this.optionsMinimizeChkBox.TabIndex = 3;
            this.optionsMinimizeChkBox.Text = "Hide when minimized";
            this.optionsMinimizeChkBox.UseVisualStyleBackColor = true;
            this.optionsMinimizeChkBox.CheckedChanged += new System.EventHandler(this.OptionsHaveChanged);
            // 
            // mainErrLabel
            // 
            this.mainErrLabel.AutoSize = true;
            this.mainErrLabel.Location = new System.Drawing.Point(16, 17);
            this.mainErrLabel.Name = "mainErrLabel";
            this.mainErrLabel.Size = new System.Drawing.Size(53, 13);
            this.mainErrLabel.TabIndex = 0;
            this.mainErrLabel.Text = "Message:";
            // 
            // tabPage1
            // 
            this.tabPage1.Location = new System.Drawing.Point(0, 0);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Size = new System.Drawing.Size(200, 100);
            this.tabPage1.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(0, 0);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(200, 100);
            this.tabPage2.TabIndex = 0;
            // 
            // toolTip
            // 
            this.toolTip.AutoPopDelay = 5000;
            this.toolTip.InitialDelay = 500;
            this.toolTip.ReshowDelay = 100;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(324, 264);
            this.Controls.Add(this.mainTabControl);
            this.Controls.Add(this.mainErrLabel);
            this.Controls.Add(this.mainErrTxtBox);
            this.Controls.Add(this.mainLookupButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "Message Translator";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Resize += new System.EventHandler(this.MainForm_Resize);
            this.mainTabControl.ResumeLayout(false);
            this.errorMsgTab.ResumeLayout(false);
            this.errorMsgTab.PerformLayout();
            this.wndmsgTab.ResumeLayout(false);
            this.wndmsgTab.PerformLayout();
            this.bugurlTab.ResumeLayout(false);
            this.bugurlTab.PerformLayout();
            this.optionsTab.ResumeLayout(false);
            this.optionsTab.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button mainLookupButton;
        private System.Windows.Forms.TextBox mainErrTxtBox;
        private System.Windows.Forms.Label mainErrLabel;
        private System.Windows.Forms.CheckBox optionsMinimizeChkBox;
        private System.Windows.Forms.TextBox errorMessageTxtBox;
        private System.Windows.Forms.TextBox errorDecimalTxtBox;
        private System.Windows.Forms.Label errorMessageLabel;
        private System.Windows.Forms.Label errorNumberLabel;
        private System.Windows.Forms.Label errorTypeLabel;
        private System.Windows.Forms.TextBox wndmsgCodeTxtBox;
        private System.Windows.Forms.Label wndmsgCodeLabel;
        private System.Windows.Forms.Label wndmsgNumberLabel;
        private System.Windows.Forms.LinkLabel bugLinkLabel;
        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage errorMsgTab;
        private System.Windows.Forms.TabPage wndmsgTab;
        private System.Windows.Forms.TabPage bugurlTab;
        private System.Windows.Forms.TabPage optionsTab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.CheckBox optionsRunStartChkBox;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.TextBox errorCodeTxtBox;
        private System.Windows.Forms.Label errorCodeLabel;
        private System.Windows.Forms.Label errorTypeValueLabel;
        private System.Windows.Forms.Button errorBackButton;
        private System.Windows.Forms.Button errorForwardButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox errorHexTxtBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox wndmsgHexTxtBox;
        private System.Windows.Forms.TextBox wndmsgDecimalTxtBox;
    }
}

