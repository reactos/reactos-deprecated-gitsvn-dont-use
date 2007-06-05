namespace RosTEGUI
{
    partial class NewVMWizard
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewVMWizard));
            this.wizard1 = new Gui.Wizard.Wizard();
            this.wizardMemoryPage = new Gui.Wizard.WizardPage();
            this.label7 = new System.Windows.Forms.Label();
            this.memoryHorizRuleLight = new System.Windows.Forms.Panel();
            this.memoryHorizRuleDark = new System.Windows.Forms.Panel();
            this.memoryLabel6 = new System.Windows.Forms.Label();
            this.memoryPhyRam = new System.Windows.Forms.Label();
            this.memoryLabel2 = new System.Windows.Forms.Label();
            this.memoryRecMax = new System.Windows.Forms.Label();
            this.memoryRec = new System.Windows.Forms.Label();
            this.memoryRecMin = new System.Windows.Forms.Label();
            this.memoryLabel5 = new System.Windows.Forms.Label();
            this.memoryLabel4 = new System.Windows.Forms.Label();
            this.memoryLabel3 = new System.Windows.Forms.Label();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.header3 = new Gui.Wizard.Header();
            this.wizardHardDiskPage = new Gui.Wizard.WizardPage();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.numericUpDown2 = new System.Windows.Forms.NumericUpDown();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.header4 = new Gui.Wizard.Header();
            this.wizardDefaultDirPage = new Gui.Wizard.WizardPage();
            this.label11 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.wizardDefaultDirBut = new System.Windows.Forms.Button();
            this.wizardDefaultDirTextBox = new System.Windows.Forms.TextBox();
            this.header5 = new Gui.Wizard.Header();
            this.wizardFinishPage = new Gui.Wizard.WizardPage();
            this.infoPage2 = new Gui.Wizard.InfoPage();
            this.wizardNamePage = new Gui.Wizard.WizardPage();
            this.label13 = new System.Windows.Forms.Label();
            this.header2 = new Gui.Wizard.Header();
            this.label4 = new System.Windows.Forms.Label();
            this.wizardNameTxtBox = new System.Windows.Forms.TextBox();
            this.wizardSetupOptionPage = new Gui.Wizard.WizardPage();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.optionRadDefault = new System.Windows.Forms.RadioButton();
            this.optionRadExist = new System.Windows.Forms.RadioButton();
            this.optionRadNew = new System.Windows.Forms.RadioButton();
            this.header1 = new Gui.Wizard.Header();
            this.wizardStartPage = new Gui.Wizard.WizardPage();
            this.infoPage1 = new Gui.Wizard.InfoPage();
            this.wizardFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
            this.wizard1.SuspendLayout();
            this.wizardMemoryPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.wizardHardDiskPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            this.wizardDefaultDirPage.SuspendLayout();
            this.wizardFinishPage.SuspendLayout();
            this.wizardNamePage.SuspendLayout();
            this.wizardSetupOptionPage.SuspendLayout();
            this.wizardStartPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // wizard1
            // 
            this.wizard1.Controls.Add(this.wizardDefaultDirPage);
            this.wizard1.Controls.Add(this.wizardNamePage);
            this.wizard1.Controls.Add(this.wizardSetupOptionPage);
            this.wizard1.Controls.Add(this.wizardHardDiskPage);
            this.wizard1.Controls.Add(this.wizardMemoryPage);
            this.wizard1.Controls.Add(this.wizardFinishPage);
            this.wizard1.Controls.Add(this.wizardStartPage);
            this.wizard1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizard1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.wizard1.Location = new System.Drawing.Point(0, 0);
            this.wizard1.Name = "wizard1";
            this.wizard1.Pages.AddRange(new Gui.Wizard.WizardPage[] {
            this.wizardStartPage,
            this.wizardSetupOptionPage,
            this.wizardNamePage,
            this.wizardDefaultDirPage,
            this.wizardHardDiskPage,
            this.wizardMemoryPage,
            this.wizardFinishPage});
            this.wizard1.Size = new System.Drawing.Size(453, 320);
            this.wizard1.TabIndex = 0;
            this.wizard1.CloseFromCancel += new System.ComponentModel.CancelEventHandler(this.wizard1_CloseFromCancel);
            // 
            // wizardMemoryPage
            // 
            this.wizardMemoryPage.Controls.Add(this.label7);
            this.wizardMemoryPage.Controls.Add(this.memoryHorizRuleLight);
            this.wizardMemoryPage.Controls.Add(this.memoryHorizRuleDark);
            this.wizardMemoryPage.Controls.Add(this.memoryLabel6);
            this.wizardMemoryPage.Controls.Add(this.memoryPhyRam);
            this.wizardMemoryPage.Controls.Add(this.memoryLabel2);
            this.wizardMemoryPage.Controls.Add(this.memoryRecMax);
            this.wizardMemoryPage.Controls.Add(this.memoryRec);
            this.wizardMemoryPage.Controls.Add(this.memoryRecMin);
            this.wizardMemoryPage.Controls.Add(this.memoryLabel5);
            this.wizardMemoryPage.Controls.Add(this.memoryLabel4);
            this.wizardMemoryPage.Controls.Add(this.memoryLabel3);
            this.wizardMemoryPage.Controls.Add(this.numericUpDown1);
            this.wizardMemoryPage.Controls.Add(this.trackBar1);
            this.wizardMemoryPage.Controls.Add(this.header3);
            this.wizardMemoryPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardMemoryPage.IsFinishPage = false;
            this.wizardMemoryPage.Location = new System.Drawing.Point(0, 0);
            this.wizardMemoryPage.Name = "wizardMemoryPage";
            this.wizardMemoryPage.Size = new System.Drawing.Size(453, 272);
            this.wizardMemoryPage.TabIndex = 5;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 82);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(323, 13);
            this.label7.TabIndex = 26;
            this.label7.Text = "Select the size of memory to make available to the virtual machine";
            // 
            // memoryHorizRuleLight
            // 
            this.memoryHorizRuleLight.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.memoryHorizRuleLight.Location = new System.Drawing.Point(8, 196);
            this.memoryHorizRuleLight.Name = "memoryHorizRuleLight";
            this.memoryHorizRuleLight.Size = new System.Drawing.Size(250, 1);
            this.memoryHorizRuleLight.TabIndex = 25;
            // 
            // memoryHorizRuleDark
            // 
            this.memoryHorizRuleDark.BackColor = System.Drawing.SystemColors.ControlDark;
            this.memoryHorizRuleDark.Location = new System.Drawing.Point(8, 195);
            this.memoryHorizRuleDark.Name = "memoryHorizRuleDark";
            this.memoryHorizRuleDark.Size = new System.Drawing.Size(250, 1);
            this.memoryHorizRuleDark.TabIndex = 24;
            // 
            // memoryLabel6
            // 
            this.memoryLabel6.AutoSize = true;
            this.memoryLabel6.Location = new System.Drawing.Point(266, 219);
            this.memoryLabel6.Name = "memoryLabel6";
            this.memoryLabel6.Size = new System.Drawing.Size(181, 39);
            this.memoryLabel6.TabIndex = 11;
            this.memoryLabel6.Text = "Setting the virtual memory above\r\nthe recomended maximum may \r\nresult in signific" +
                "ant performance loss";
            // 
            // memoryPhyRam
            // 
            this.memoryPhyRam.AutoSize = true;
            this.memoryPhyRam.Location = new System.Drawing.Point(196, 173);
            this.memoryPhyRam.Name = "memoryPhyRam";
            this.memoryPhyRam.Size = new System.Drawing.Size(25, 13);
            this.memoryPhyRam.TabIndex = 23;
            this.memoryPhyRam.Text = "ram";
            // 
            // memoryLabel2
            // 
            this.memoryLabel2.AutoSize = true;
            this.memoryLabel2.Location = new System.Drawing.Point(38, 173);
            this.memoryLabel2.Name = "memoryLabel2";
            this.memoryLabel2.Size = new System.Drawing.Size(110, 13);
            this.memoryLabel2.TabIndex = 22;
            this.memoryLabel2.Text = "Current physical RAM";
            // 
            // memoryRecMax
            // 
            this.memoryRecMax.AutoSize = true;
            this.memoryRecMax.Location = new System.Drawing.Point(199, 245);
            this.memoryRecMax.Name = "memoryRecMax";
            this.memoryRecMax.Size = new System.Drawing.Size(27, 13);
            this.memoryRecMax.TabIndex = 20;
            this.memoryRecMax.Text = "max";
            // 
            // memoryRec
            // 
            this.memoryRec.AutoSize = true;
            this.memoryRec.Location = new System.Drawing.Point(199, 225);
            this.memoryRec.Name = "memoryRec";
            this.memoryRec.Size = new System.Drawing.Size(22, 13);
            this.memoryRec.TabIndex = 19;
            this.memoryRec.Text = "rec";
            // 
            // memoryRecMin
            // 
            this.memoryRecMin.AutoSize = true;
            this.memoryRecMin.Location = new System.Drawing.Point(198, 202);
            this.memoryRecMin.Name = "memoryRecMin";
            this.memoryRecMin.Size = new System.Drawing.Size(23, 13);
            this.memoryRecMin.TabIndex = 18;
            this.memoryRecMin.Text = "min";
            // 
            // memoryLabel5
            // 
            this.memoryLabel5.AutoSize = true;
            this.memoryLabel5.Location = new System.Drawing.Point(38, 245);
            this.memoryLabel5.Name = "memoryLabel5";
            this.memoryLabel5.Size = new System.Drawing.Size(116, 13);
            this.memoryLabel5.TabIndex = 17;
            this.memoryLabel5.Text = "Recomended maximum";
            // 
            // memoryLabel4
            // 
            this.memoryLabel4.AutoSize = true;
            this.memoryLabel4.Location = new System.Drawing.Point(38, 225);
            this.memoryLabel4.Name = "memoryLabel4";
            this.memoryLabel4.Size = new System.Drawing.Size(69, 13);
            this.memoryLabel4.TabIndex = 16;
            this.memoryLabel4.Text = "Recomended";
            // 
            // memoryLabel3
            // 
            this.memoryLabel3.AutoSize = true;
            this.memoryLabel3.Location = new System.Drawing.Point(38, 202);
            this.memoryLabel3.Name = "memoryLabel3";
            this.memoryLabel3.Size = new System.Drawing.Size(112, 13);
            this.memoryLabel3.TabIndex = 15;
            this.memoryLabel3.Text = "Recomended minimum";
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(380, 119);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(61, 21);
            this.numericUpDown1.TabIndex = 2;
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(12, 119);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(362, 45);
            this.trackBar1.TabIndex = 1;
            // 
            // header3
            // 
            this.header3.BackColor = System.Drawing.SystemColors.Control;
            this.header3.CausesValidation = false;
            this.header3.Description = "Choose the size of the virtual memory which will be available to your virtual mac" +
                "hine";
            this.header3.Dock = System.Windows.Forms.DockStyle.Top;
            this.header3.Image = ((System.Drawing.Image)(resources.GetObject("header3.Image")));
            this.header3.Location = new System.Drawing.Point(0, 0);
            this.header3.Name = "header3";
            this.header3.Size = new System.Drawing.Size(453, 64);
            this.header3.TabIndex = 0;
            this.header3.Title = "Virtual Memory";
            // 
            // wizardHardDiskPage
            // 
            this.wizardHardDiskPage.Controls.Add(this.label14);
            this.wizardHardDiskPage.Controls.Add(this.label15);
            this.wizardHardDiskPage.Controls.Add(this.label10);
            this.wizardHardDiskPage.Controls.Add(this.label5);
            this.wizardHardDiskPage.Controls.Add(this.label6);
            this.wizardHardDiskPage.Controls.Add(this.label9);
            this.wizardHardDiskPage.Controls.Add(this.label12);
            this.wizardHardDiskPage.Controls.Add(this.numericUpDown2);
            this.wizardHardDiskPage.Controls.Add(this.trackBar2);
            this.wizardHardDiskPage.Controls.Add(this.header4);
            this.wizardHardDiskPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardHardDiskPage.IsFinishPage = false;
            this.wizardHardDiskPage.Location = new System.Drawing.Point(0, 0);
            this.wizardHardDiskPage.Name = "wizardHardDiskPage";
            this.wizardHardDiskPage.Size = new System.Drawing.Size(453, 272);
            this.wizardHardDiskPage.TabIndex = 6;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(197, 190);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(18, 13);
            this.label14.TabIndex = 36;
            this.label14.Text = "C:";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(39, 190);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(66, 13);
            this.label15.TabIndex = 35;
            this.label15.Text = "Physical disk";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 83);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(352, 26);
            this.label10.TabIndex = 34;
            this.label10.Text = "Select a hard disk size for the virtual machine. The location of this image\r\ncorr" +
                "esponds to the default directory previously chosen";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(197, 215);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(25, 13);
            this.label5.TabIndex = 33;
            this.label5.Text = "size";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(39, 215);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(87, 13);
            this.label6.TabIndex = 32;
            this.label6.Text = "Physical disk size";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(199, 238);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(23, 13);
            this.label9.TabIndex = 29;
            this.label9.Text = "min";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(39, 238);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(112, 13);
            this.label12.TabIndex = 26;
            this.label12.Text = "Recomended minimum";
            // 
            // numericUpDown2
            // 
            this.numericUpDown2.Location = new System.Drawing.Point(380, 140);
            this.numericUpDown2.Name = "numericUpDown2";
            this.numericUpDown2.Size = new System.Drawing.Size(61, 21);
            this.numericUpDown2.TabIndex = 25;
            // 
            // trackBar2
            // 
            this.trackBar2.Location = new System.Drawing.Point(12, 140);
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(362, 45);
            this.trackBar2.TabIndex = 24;
            // 
            // header4
            // 
            this.header4.BackColor = System.Drawing.SystemColors.Control;
            this.header4.CausesValidation = false;
            this.header4.Description = "Choose the size of hard diskfor your virtual machine";
            this.header4.Dock = System.Windows.Forms.DockStyle.Top;
            this.header4.Image = ((System.Drawing.Image)(resources.GetObject("header4.Image")));
            this.header4.Location = new System.Drawing.Point(0, 0);
            this.header4.Name = "header4";
            this.header4.Size = new System.Drawing.Size(453, 64);
            this.header4.TabIndex = 0;
            this.header4.Title = "Hard Disk Size";
            // 
            // wizardDefaultDirPage
            // 
            this.wizardDefaultDirPage.Controls.Add(this.label11);
            this.wizardDefaultDirPage.Controls.Add(this.label8);
            this.wizardDefaultDirPage.Controls.Add(this.wizardDefaultDirBut);
            this.wizardDefaultDirPage.Controls.Add(this.wizardDefaultDirTextBox);
            this.wizardDefaultDirPage.Controls.Add(this.header5);
            this.wizardDefaultDirPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardDefaultDirPage.IsFinishPage = false;
            this.wizardDefaultDirPage.Location = new System.Drawing.Point(0, 0);
            this.wizardDefaultDirPage.Name = "wizardDefaultDirPage";
            this.wizardDefaultDirPage.Size = new System.Drawing.Size(453, 272);
            this.wizardDefaultDirPage.TabIndex = 7;
            this.wizardDefaultDirPage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.wizardDefaultDirPage_CloseFromNext);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 137);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(89, 13);
            this.label11.TabIndex = 4;
            this.label11.Text = "Default Directory";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 83);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(343, 39);
            this.label8.TabIndex = 3;
            this.label8.Text = "Type a location for you virtual machine. This is the location which your\r\nvirtual" +
                " machine image and it\'s corresponding configuration settings wil\r\n be stored.";
            // 
            // wizardDefaultDirBut
            // 
            this.wizardDefaultDirBut.Location = new System.Drawing.Point(366, 155);
            this.wizardDefaultDirBut.Name = "wizardDefaultDirBut";
            this.wizardDefaultDirBut.Size = new System.Drawing.Size(75, 23);
            this.wizardDefaultDirBut.TabIndex = 2;
            this.wizardDefaultDirBut.Text = "Browse...";
            this.wizardDefaultDirBut.UseVisualStyleBackColor = true;
            this.wizardDefaultDirBut.Click += new System.EventHandler(this.button1_Click);
            // 
            // wizardDefaultDirTextBox
            // 
            this.wizardDefaultDirTextBox.Location = new System.Drawing.Point(12, 157);
            this.wizardDefaultDirTextBox.Name = "wizardDefaultDirTextBox";
            this.wizardDefaultDirTextBox.Size = new System.Drawing.Size(348, 21);
            this.wizardDefaultDirTextBox.TabIndex = 1;
            // 
            // header5
            // 
            this.header5.BackColor = System.Drawing.SystemColors.Control;
            this.header5.CausesValidation = false;
            this.header5.Description = "Choose the location of the directory which wil be used to hold your virtual machi" +
                "ne and accociated settings";
            this.header5.Dock = System.Windows.Forms.DockStyle.Top;
            this.header5.Image = ((System.Drawing.Image)(resources.GetObject("header5.Image")));
            this.header5.Location = new System.Drawing.Point(0, 0);
            this.header5.Name = "header5";
            this.header5.Size = new System.Drawing.Size(453, 64);
            this.header5.TabIndex = 0;
            this.header5.Title = "Default Directory";
            // 
            // wizardFinishPage
            // 
            this.wizardFinishPage.Controls.Add(this.infoPage2);
            this.wizardFinishPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardFinishPage.IsFinishPage = false;
            this.wizardFinishPage.Location = new System.Drawing.Point(0, 0);
            this.wizardFinishPage.Name = "wizardFinishPage";
            this.wizardFinishPage.Size = new System.Drawing.Size(453, 272);
            this.wizardFinishPage.TabIndex = 4;
            // 
            // infoPage2
            // 
            this.infoPage2.BackColor = System.Drawing.Color.White;
            this.infoPage2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.infoPage2.Image = ((System.Drawing.Image)(resources.GetObject("infoPage2.Image")));
            this.infoPage2.Location = new System.Drawing.Point(0, 0);
            this.infoPage2.Name = "infoPage2";
            this.infoPage2.PageText = "You new virtual machine is now ready to use via the console. Use the settings dia" +
                "log for any additional fine tuning";
            this.infoPage2.PageTitle = "Completed the New Virtual Machine wizard";
            this.infoPage2.Size = new System.Drawing.Size(453, 272);
            this.infoPage2.TabIndex = 0;
            // 
            // wizardNamePage
            // 
            this.wizardNamePage.Controls.Add(this.label13);
            this.wizardNamePage.Controls.Add(this.header2);
            this.wizardNamePage.Controls.Add(this.label4);
            this.wizardNamePage.Controls.Add(this.wizardNameTxtBox);
            this.wizardNamePage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardNamePage.IsFinishPage = false;
            this.wizardNamePage.Location = new System.Drawing.Point(0, 0);
            this.wizardNamePage.Name = "wizardNamePage";
            this.wizardNamePage.Size = new System.Drawing.Size(453, 272);
            this.wizardNamePage.TabIndex = 3;
            this.wizardNamePage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.wizardNamePage_CloseFromNext);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(9, 83);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(343, 39);
            this.label13.TabIndex = 4;
            this.label13.Text = "Type a location for you virtual machine. This is the location which your\r\nvirtual" +
                " machine image and it\'s corresponding configuration settings wil\r\n be stored.";
            // 
            // header2
            // 
            this.header2.BackColor = System.Drawing.SystemColors.Control;
            this.header2.CausesValidation = false;
            this.header2.Description = "Choose a name for you virtual machine which will be used to identify it via the c" +
                "onsole";
            this.header2.Dock = System.Windows.Forms.DockStyle.Top;
            this.header2.Image = ((System.Drawing.Image)(resources.GetObject("header2.Image")));
            this.header2.Location = new System.Drawing.Point(0, 0);
            this.header2.Name = "header2";
            this.header2.Size = new System.Drawing.Size(453, 64);
            this.header2.TabIndex = 2;
            this.header2.Title = "Virtual Machine name";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 137);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(188, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "Enter a name for your virtual machine";
            // 
            // wizardNameTxtBox
            // 
            this.wizardNameTxtBox.Location = new System.Drawing.Point(12, 157);
            this.wizardNameTxtBox.Name = "wizardNameTxtBox";
            this.wizardNameTxtBox.Size = new System.Drawing.Size(429, 21);
            this.wizardNameTxtBox.TabIndex = 0;
            // 
            // wizardSetupOptionPage
            // 
            this.wizardSetupOptionPage.Controls.Add(this.label3);
            this.wizardSetupOptionPage.Controls.Add(this.label2);
            this.wizardSetupOptionPage.Controls.Add(this.label1);
            this.wizardSetupOptionPage.Controls.Add(this.optionRadDefault);
            this.wizardSetupOptionPage.Controls.Add(this.optionRadExist);
            this.wizardSetupOptionPage.Controls.Add(this.optionRadNew);
            this.wizardSetupOptionPage.Controls.Add(this.header1);
            this.wizardSetupOptionPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardSetupOptionPage.IsFinishPage = false;
            this.wizardSetupOptionPage.Location = new System.Drawing.Point(0, 0);
            this.wizardSetupOptionPage.Name = "wizardSetupOptionPage";
            this.wizardSetupOptionPage.Size = new System.Drawing.Size(453, 272);
            this.wizardSetupOptionPage.TabIndex = 2;
            this.wizardSetupOptionPage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.wizardSetupOptionPage_CloseFromNext);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(64, 220);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(277, 26);
            this.label3.TabIndex = 6;
            this.label3.Text = "This option will set up a virtual machine using the default\r\nrecommended settings" +
                " for your hardware";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(64, 164);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(298, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "This option will add an existing virtual machine to the console";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(64, 110);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(297, 26);
            this.label1.TabIndex = 4;
            this.label1.Text = "This option will take you through the process of setting up a \r\nnew virtual machi" +
                "ne";
            // 
            // optionRadDefault
            // 
            this.optionRadDefault.AutoSize = true;
            this.optionRadDefault.Location = new System.Drawing.Point(46, 200);
            this.optionRadDefault.Name = "optionRadDefault";
            this.optionRadDefault.Size = new System.Drawing.Size(210, 17);
            this.optionRadDefault.TabIndex = 3;
            this.optionRadDefault.Text = "Use the default recommended settings";
            this.optionRadDefault.UseVisualStyleBackColor = true;
            // 
            // optionRadExist
            // 
            this.optionRadExist.AutoSize = true;
            this.optionRadExist.Location = new System.Drawing.Point(46, 144);
            this.optionRadExist.Name = "optionRadExist";
            this.optionRadExist.Size = new System.Drawing.Size(173, 17);
            this.optionRadExist.TabIndex = 2;
            this.optionRadExist.Text = "Use an existing virtual machine";
            this.optionRadExist.UseVisualStyleBackColor = true;
            // 
            // optionRadNew
            // 
            this.optionRadNew.AutoSize = true;
            this.optionRadNew.Checked = true;
            this.optionRadNew.Location = new System.Drawing.Point(46, 90);
            this.optionRadNew.Name = "optionRadNew";
            this.optionRadNew.Size = new System.Drawing.Size(165, 17);
            this.optionRadNew.TabIndex = 1;
            this.optionRadNew.TabStop = true;
            this.optionRadNew.Text = "Create a new virtual machine";
            this.optionRadNew.UseVisualStyleBackColor = true;
            // 
            // header1
            // 
            this.header1.BackColor = System.Drawing.SystemColors.Control;
            this.header1.CausesValidation = false;
            this.header1.Description = "Create a new virtual machine or hook up to an existing one";
            this.header1.Dock = System.Windows.Forms.DockStyle.Top;
            this.header1.Image = ((System.Drawing.Image)(resources.GetObject("header1.Image")));
            this.header1.Location = new System.Drawing.Point(0, 0);
            this.header1.Name = "header1";
            this.header1.Size = new System.Drawing.Size(453, 64);
            this.header1.TabIndex = 0;
            this.header1.Title = "Choose a setup option";
            // 
            // wizardStartPage
            // 
            this.wizardStartPage.Controls.Add(this.infoPage1);
            this.wizardStartPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardStartPage.IsFinishPage = false;
            this.wizardStartPage.Location = new System.Drawing.Point(0, 0);
            this.wizardStartPage.Name = "wizardStartPage";
            this.wizardStartPage.Size = new System.Drawing.Size(453, 272);
            this.wizardStartPage.TabIndex = 1;
            // 
            // infoPage1
            // 
            this.infoPage1.BackColor = System.Drawing.Color.White;
            this.infoPage1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.infoPage1.Image = ((System.Drawing.Image)(resources.GetObject("infoPage1.Image")));
            this.infoPage1.Location = new System.Drawing.Point(0, 0);
            this.infoPage1.Name = "infoPage1";
            this.infoPage1.PageText = "This wizard enables you to quickly set up a new virtual machine";
            this.infoPage1.PageTitle = "Welcome to the New Virtual Machine wizard";
            this.infoPage1.Size = new System.Drawing.Size(453, 272);
            this.infoPage1.TabIndex = 0;
            // 
            // NewVMWizard
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(453, 320);
            this.ControlBox = false;
            this.Controls.Add(this.wizard1);
            this.Name = "NewVMWizard";
            this.Text = "New Virtual Machine Wizard";
            this.wizard1.ResumeLayout(false);
            this.wizardMemoryPage.ResumeLayout(false);
            this.wizardMemoryPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.wizardHardDiskPage.ResumeLayout(false);
            this.wizardHardDiskPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            this.wizardDefaultDirPage.ResumeLayout(false);
            this.wizardDefaultDirPage.PerformLayout();
            this.wizardFinishPage.ResumeLayout(false);
            this.wizardNamePage.ResumeLayout(false);
            this.wizardNamePage.PerformLayout();
            this.wizardSetupOptionPage.ResumeLayout(false);
            this.wizardSetupOptionPage.PerformLayout();
            this.wizardStartPage.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Gui.Wizard.Wizard wizard1;
        private Gui.Wizard.WizardPage wizardStartPage;
        private Gui.Wizard.InfoPage infoPage1;
        private Gui.Wizard.WizardPage wizardSetupOptionPage;
        private System.Windows.Forms.RadioButton optionRadDefault;
        private System.Windows.Forms.RadioButton optionRadExist;
        private System.Windows.Forms.RadioButton optionRadNew;
        private Gui.Wizard.Header header1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private Gui.Wizard.WizardPage wizardNamePage;
        private Gui.Wizard.WizardPage wizardFinishPage;
        private Gui.Wizard.InfoPage infoPage2;
        private Gui.Wizard.Header header2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox wizardNameTxtBox;
        private Gui.Wizard.WizardPage wizardDefaultDirPage;
        private Gui.Wizard.WizardPage wizardHardDiskPage;
        private Gui.Wizard.WizardPage wizardMemoryPage;
        private Gui.Wizard.Header header3;
        private Gui.Wizard.Header header4;
        private Gui.Wizard.Header header5;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Label memoryLabel6;
        private System.Windows.Forms.Label memoryPhyRam;
        private System.Windows.Forms.Label memoryLabel2;
        private System.Windows.Forms.Label memoryRecMax;
        private System.Windows.Forms.Label memoryRec;
        private System.Windows.Forms.Label memoryRecMin;
        private System.Windows.Forms.Label memoryLabel5;
        private System.Windows.Forms.Label memoryLabel4;
        private System.Windows.Forms.Label memoryLabel3;
        private System.Windows.Forms.Panel memoryHorizRuleDark;
        private System.Windows.Forms.Panel memoryHorizRuleLight;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.NumericUpDown numericUpDown2;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.Button wizardDefaultDirBut;
        private System.Windows.Forms.TextBox wizardDefaultDirTextBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.FolderBrowserDialog wizardFolderBrowser;
    }
}