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
            this.wizardFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
            this.wizardOpenFile = new System.Windows.Forms.OpenFileDialog();
            this.wizardNewVM = new Gui.Wizard.Wizard();
            this.defaultDirInfoPage = new Gui.Wizard.WizardPage();
            this.defaultDirLab2 = new System.Windows.Forms.Label();
            this.defaultDirLab1 = new System.Windows.Forms.Label();
            this.defaultDirBrowse = new System.Windows.Forms.Button();
            this.defaultDirTxtBox = new System.Windows.Forms.TextBox();
            this.defaultDirHeader = new Gui.Wizard.Header();
            this.nameInfoPage = new Gui.Wizard.WizardPage();
            this.nameLab1 = new System.Windows.Forms.Label();
            this.nameHeader = new Gui.Wizard.Header();
            this.nameLab2 = new System.Windows.Forms.Label();
            this.nameTxtBox = new System.Windows.Forms.TextBox();
            this.optionInfoPage = new Gui.Wizard.WizardPage();
            this.optionLab3 = new System.Windows.Forms.Label();
            this.optionLab2 = new System.Windows.Forms.Label();
            this.optionLab1 = new System.Windows.Forms.Label();
            this.optionRadDefault = new System.Windows.Forms.RadioButton();
            this.optionRadExist = new System.Windows.Forms.RadioButton();
            this.optionRadNew = new System.Windows.Forms.RadioButton();
            this.optionHeader = new Gui.Wizard.Header();
            this.wizardStartPage = new Gui.Wizard.WizardPage();
            this.startInfoPage = new Gui.Wizard.InfoPage();
            this.wizardFinishPage = new Gui.Wizard.WizardPage();
            this.finishInfoPage = new Gui.Wizard.InfoPage();
            this.memoryInfoPage = new Gui.Wizard.WizardPage();
            this.memoryLab7 = new System.Windows.Forms.Label();
            this.memoryLab1 = new System.Windows.Forms.Label();
            this.memoryHorizRuleLight = new System.Windows.Forms.Panel();
            this.memoryHorizRuleDark = new System.Windows.Forms.Panel();
            this.memoryLab6 = new System.Windows.Forms.Label();
            this.memoryPhyRam = new System.Windows.Forms.Label();
            this.memoryLab2 = new System.Windows.Forms.Label();
            this.memoryRecMax = new System.Windows.Forms.Label();
            this.memoryRec = new System.Windows.Forms.Label();
            this.memoryRecMin = new System.Windows.Forms.Label();
            this.memoryLab5 = new System.Windows.Forms.Label();
            this.memoryLab4 = new System.Windows.Forms.Label();
            this.memoryLab3 = new System.Windows.Forms.Label();
            this.memoryUpDwn = new System.Windows.Forms.NumericUpDown();
            this.memoryTrkBar = new System.Windows.Forms.TrackBar();
            this.memoryheader = new Gui.Wizard.Header();
            this.imageInfoPage = new Gui.Wizard.WizardPage();
            this.imageHeader = new Gui.Wizard.Header();
            this.imageLab2 = new System.Windows.Forms.Label();
            this.imageLab1 = new System.Windows.Forms.Label();
            this.imageFileBrowse = new System.Windows.Forms.Button();
            this.imageLocTxtBox = new System.Windows.Forms.TextBox();
            this.harddiskInfoPage = new Gui.Wizard.WizardPage();
            this.label5 = new System.Windows.Forms.Label();
            this.harddiskDiskLab = new System.Windows.Forms.Label();
            this.harddiskLab2 = new System.Windows.Forms.Label();
            this.harddiskLab1 = new System.Windows.Forms.Label();
            this.harddiskDiskSizeLab = new System.Windows.Forms.Label();
            this.harddiskLab3 = new System.Windows.Forms.Label();
            this.harddiskRecMinLab = new System.Windows.Forms.Label();
            this.harddiskLab4 = new System.Windows.Forms.Label();
            this.harddiskUpDwn = new System.Windows.Forms.NumericUpDown();
            this.harddiskTrkBar = new System.Windows.Forms.TrackBar();
            this.harddiskHeader = new Gui.Wizard.Header();
            this.wizardNewVM.SuspendLayout();
            this.defaultDirInfoPage.SuspendLayout();
            this.nameInfoPage.SuspendLayout();
            this.optionInfoPage.SuspendLayout();
            this.wizardStartPage.SuspendLayout();
            this.wizardFinishPage.SuspendLayout();
            this.memoryInfoPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.memoryUpDwn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.memoryTrkBar)).BeginInit();
            this.imageInfoPage.SuspendLayout();
            this.harddiskInfoPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.harddiskUpDwn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.harddiskTrkBar)).BeginInit();
            this.SuspendLayout();
            // 
            // wizardOpenFile
            // 
            this.wizardOpenFile.FileName = "openFileDialog1";
            // 
            // wizardNewVM
            // 
            this.wizardNewVM.Controls.Add(this.defaultDirInfoPage);
            this.wizardNewVM.Controls.Add(this.nameInfoPage);
            this.wizardNewVM.Controls.Add(this.optionInfoPage);
            this.wizardNewVM.Controls.Add(this.wizardStartPage);
            this.wizardNewVM.Controls.Add(this.wizardFinishPage);
            this.wizardNewVM.Controls.Add(this.memoryInfoPage);
            this.wizardNewVM.Controls.Add(this.imageInfoPage);
            this.wizardNewVM.Controls.Add(this.harddiskInfoPage);
            this.wizardNewVM.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardNewVM.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.wizardNewVM.Location = new System.Drawing.Point(0, 0);
            this.wizardNewVM.Name = "wizardNewVM";
            this.wizardNewVM.Pages.AddRange(new Gui.Wizard.WizardPage[] {
            this.wizardStartPage,
            this.optionInfoPage,
            this.nameInfoPage,
            this.defaultDirInfoPage,
            this.harddiskInfoPage,
            this.imageInfoPage,
            this.memoryInfoPage,
            this.wizardFinishPage});
            this.wizardNewVM.Size = new System.Drawing.Size(453, 320);
            this.wizardNewVM.TabIndex = 0;
            this.wizardNewVM.CloseFromCancel += new System.ComponentModel.CancelEventHandler(this.wizard1_CloseFromCancel);
            // 
            // defaultDirInfoPage
            // 
            this.defaultDirInfoPage.Controls.Add(this.defaultDirLab2);
            this.defaultDirInfoPage.Controls.Add(this.defaultDirLab1);
            this.defaultDirInfoPage.Controls.Add(this.defaultDirBrowse);
            this.defaultDirInfoPage.Controls.Add(this.defaultDirTxtBox);
            this.defaultDirInfoPage.Controls.Add(this.defaultDirHeader);
            this.defaultDirInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.defaultDirInfoPage.IsFinishPage = false;
            this.defaultDirInfoPage.Location = new System.Drawing.Point(0, 0);
            this.defaultDirInfoPage.Name = "defaultDirInfoPage";
            this.defaultDirInfoPage.Size = new System.Drawing.Size(453, 272);
            this.defaultDirInfoPage.TabIndex = 7;
            this.defaultDirInfoPage.Enter += new System.EventHandler(this.defaultDirInfoPage_Enter);
            this.defaultDirInfoPage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.wizardDefaultDirPage_CloseFromNext);
            // 
            // defaultDirLab2
            // 
            this.defaultDirLab2.AutoSize = true;
            this.defaultDirLab2.Location = new System.Drawing.Point(12, 137);
            this.defaultDirLab2.Name = "defaultDirLab2";
            this.defaultDirLab2.Size = new System.Drawing.Size(89, 13);
            this.defaultDirLab2.TabIndex = 4;
            this.defaultDirLab2.Text = "Default Directory";
            // 
            // defaultDirLab1
            // 
            this.defaultDirLab1.AutoSize = true;
            this.defaultDirLab1.Location = new System.Drawing.Point(9, 83);
            this.defaultDirLab1.Name = "defaultDirLab1";
            this.defaultDirLab1.Size = new System.Drawing.Size(347, 39);
            this.defaultDirLab1.TabIndex = 3;
            this.defaultDirLab1.Text = "Type a location for your virtual machine. This is the location which your\r\nvirtua" +
                "l machine image and it\'s corresponding configuration settings wil\r\n be stored.";
            // 
            // defaultDirBrowse
            // 
            this.defaultDirBrowse.Location = new System.Drawing.Point(366, 155);
            this.defaultDirBrowse.Name = "defaultDirBrowse";
            this.defaultDirBrowse.Size = new System.Drawing.Size(75, 23);
            this.defaultDirBrowse.TabIndex = 2;
            this.defaultDirBrowse.Text = "Browse...";
            this.defaultDirBrowse.UseVisualStyleBackColor = true;
            this.defaultDirBrowse.Click += new System.EventHandler(this.defaultDirBrowse_Click);
            // 
            // defaultDirTxtBox
            // 
            this.defaultDirTxtBox.Location = new System.Drawing.Point(12, 157);
            this.defaultDirTxtBox.Name = "defaultDirTxtBox";
            this.defaultDirTxtBox.Size = new System.Drawing.Size(348, 21);
            this.defaultDirTxtBox.TabIndex = 1;
            // 
            // defaultDirHeader
            // 
            this.defaultDirHeader.BackColor = System.Drawing.SystemColors.Control;
            this.defaultDirHeader.CausesValidation = false;
            this.defaultDirHeader.Description = "Choose the location of the directory which wil be used to hold your virtual machi" +
                "ne and accociated settings";
            this.defaultDirHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.defaultDirHeader.Image = ((System.Drawing.Image)(resources.GetObject("defaultDirHeader.Image")));
            this.defaultDirHeader.Location = new System.Drawing.Point(0, 0);
            this.defaultDirHeader.Name = "defaultDirHeader";
            this.defaultDirHeader.Size = new System.Drawing.Size(453, 64);
            this.defaultDirHeader.TabIndex = 0;
            this.defaultDirHeader.Title = "Default Directory";
            // 
            // nameInfoPage
            // 
            this.nameInfoPage.Controls.Add(this.nameLab1);
            this.nameInfoPage.Controls.Add(this.nameHeader);
            this.nameInfoPage.Controls.Add(this.nameLab2);
            this.nameInfoPage.Controls.Add(this.nameTxtBox);
            this.nameInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.nameInfoPage.IsFinishPage = false;
            this.nameInfoPage.Location = new System.Drawing.Point(0, 0);
            this.nameInfoPage.Name = "nameInfoPage";
            this.nameInfoPage.Size = new System.Drawing.Size(453, 272);
            this.nameInfoPage.TabIndex = 3;
            this.nameInfoPage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.wizardNamePage_CloseFromNext);
            // 
            // nameLab1
            // 
            this.nameLab1.AutoSize = true;
            this.nameLab1.Location = new System.Drawing.Point(9, 83);
            this.nameLab1.Name = "nameLab1";
            this.nameLab1.Size = new System.Drawing.Size(343, 39);
            this.nameLab1.TabIndex = 4;
            this.nameLab1.Text = "Type a location for you virtual machine. This is the location which your\r\nvirtual" +
                " machine image and it\'s corresponding configuration settings wil\r\n be stored.";
            // 
            // nameHeader
            // 
            this.nameHeader.BackColor = System.Drawing.SystemColors.Control;
            this.nameHeader.CausesValidation = false;
            this.nameHeader.Description = "Choose a name for you virtual machine which will be used to identify it via the c" +
                "onsole";
            this.nameHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.nameHeader.Image = ((System.Drawing.Image)(resources.GetObject("nameHeader.Image")));
            this.nameHeader.Location = new System.Drawing.Point(0, 0);
            this.nameHeader.Name = "nameHeader";
            this.nameHeader.Size = new System.Drawing.Size(453, 64);
            this.nameHeader.TabIndex = 2;
            this.nameHeader.Title = "Virtual Machine name";
            // 
            // nameLab2
            // 
            this.nameLab2.AutoSize = true;
            this.nameLab2.Location = new System.Drawing.Point(12, 137);
            this.nameLab2.Name = "nameLab2";
            this.nameLab2.Size = new System.Drawing.Size(188, 13);
            this.nameLab2.TabIndex = 1;
            this.nameLab2.Text = "Enter a name for your virtual machine";
            // 
            // nameTxtBox
            // 
            this.nameTxtBox.Location = new System.Drawing.Point(12, 157);
            this.nameTxtBox.Name = "nameTxtBox";
            this.nameTxtBox.Size = new System.Drawing.Size(429, 21);
            this.nameTxtBox.TabIndex = 0;
            // 
            // optionInfoPage
            // 
            this.optionInfoPage.Controls.Add(this.optionLab3);
            this.optionInfoPage.Controls.Add(this.optionLab2);
            this.optionInfoPage.Controls.Add(this.optionLab1);
            this.optionInfoPage.Controls.Add(this.optionRadDefault);
            this.optionInfoPage.Controls.Add(this.optionRadExist);
            this.optionInfoPage.Controls.Add(this.optionRadNew);
            this.optionInfoPage.Controls.Add(this.optionHeader);
            this.optionInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.optionInfoPage.IsFinishPage = false;
            this.optionInfoPage.Location = new System.Drawing.Point(0, 0);
            this.optionInfoPage.Name = "optionInfoPage";
            this.optionInfoPage.Size = new System.Drawing.Size(453, 272);
            this.optionInfoPage.TabIndex = 2;
            // 
            // optionLab3
            // 
            this.optionLab3.AutoSize = true;
            this.optionLab3.Location = new System.Drawing.Point(64, 220);
            this.optionLab3.Name = "optionLab3";
            this.optionLab3.Size = new System.Drawing.Size(277, 26);
            this.optionLab3.TabIndex = 6;
            this.optionLab3.Text = "This option will set up a virtual machine using the default\r\nrecommended settings" +
                " for your hardware";
            // 
            // optionLab2
            // 
            this.optionLab2.AutoSize = true;
            this.optionLab2.Location = new System.Drawing.Point(64, 165);
            this.optionLab2.Name = "optionLab2";
            this.optionLab2.Size = new System.Drawing.Size(298, 13);
            this.optionLab2.TabIndex = 5;
            this.optionLab2.Text = "This option will add an existing virtual machine to the console";
            // 
            // optionLab1
            // 
            this.optionLab1.AutoSize = true;
            this.optionLab1.Location = new System.Drawing.Point(64, 110);
            this.optionLab1.Name = "optionLab1";
            this.optionLab1.Size = new System.Drawing.Size(297, 26);
            this.optionLab1.TabIndex = 4;
            this.optionLab1.Text = "This option will take you through the process of setting up a \r\nnew virtual machi" +
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
            this.optionRadExist.Location = new System.Drawing.Point(46, 145);
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
            // optionHeader
            // 
            this.optionHeader.BackColor = System.Drawing.SystemColors.Control;
            this.optionHeader.CausesValidation = false;
            this.optionHeader.Description = "Create a new virtual machine or hook up to an existing one";
            this.optionHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.optionHeader.Image = ((System.Drawing.Image)(resources.GetObject("optionHeader.Image")));
            this.optionHeader.Location = new System.Drawing.Point(0, 0);
            this.optionHeader.Name = "optionHeader";
            this.optionHeader.Size = new System.Drawing.Size(453, 64);
            this.optionHeader.TabIndex = 0;
            this.optionHeader.Title = "Choose a setup option";
            // 
            // wizardStartPage
            // 
            this.wizardStartPage.Controls.Add(this.startInfoPage);
            this.wizardStartPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardStartPage.IsFinishPage = false;
            this.wizardStartPage.Location = new System.Drawing.Point(0, 0);
            this.wizardStartPage.Name = "wizardStartPage";
            this.wizardStartPage.Size = new System.Drawing.Size(453, 272);
            this.wizardStartPage.TabIndex = 1;
            // 
            // startInfoPage
            // 
            this.startInfoPage.BackColor = System.Drawing.Color.White;
            this.startInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.startInfoPage.Image = ((System.Drawing.Image)(resources.GetObject("startInfoPage.Image")));
            this.startInfoPage.Location = new System.Drawing.Point(0, 0);
            this.startInfoPage.Name = "startInfoPage";
            this.startInfoPage.PageText = "This wizard enables you to quickly set up a new virtual machine";
            this.startInfoPage.PageTitle = "Welcome to the New Virtual Machine wizard";
            this.startInfoPage.Size = new System.Drawing.Size(453, 272);
            this.startInfoPage.TabIndex = 0;
            // 
            // wizardFinishPage
            // 
            this.wizardFinishPage.Controls.Add(this.finishInfoPage);
            this.wizardFinishPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.wizardFinishPage.IsFinishPage = false;
            this.wizardFinishPage.Location = new System.Drawing.Point(0, 0);
            this.wizardFinishPage.Name = "wizardFinishPage";
            this.wizardFinishPage.Size = new System.Drawing.Size(453, 272);
            this.wizardFinishPage.TabIndex = 4;
            this.wizardFinishPage.CloseFromBack += new Gui.Wizard.PageEventHandler(this.wizardFinishPage_CloseFromBack);
            // 
            // finishInfoPage
            // 
            this.finishInfoPage.BackColor = System.Drawing.Color.White;
            this.finishInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.finishInfoPage.Image = ((System.Drawing.Image)(resources.GetObject("finishInfoPage.Image")));
            this.finishInfoPage.Location = new System.Drawing.Point(0, 0);
            this.finishInfoPage.Name = "finishInfoPage";
            this.finishInfoPage.PageText = "You new virtual machine is now ready to use via the console. Use the settings dia" +
                "log for any additional fine tuning";
            this.finishInfoPage.PageTitle = "Completed the New Virtual Machine wizard";
            this.finishInfoPage.Size = new System.Drawing.Size(453, 272);
            this.finishInfoPage.TabIndex = 0;
            // 
            // memoryInfoPage
            // 
            this.memoryInfoPage.Controls.Add(this.memoryLab7);
            this.memoryInfoPage.Controls.Add(this.memoryLab1);
            this.memoryInfoPage.Controls.Add(this.memoryHorizRuleLight);
            this.memoryInfoPage.Controls.Add(this.memoryHorizRuleDark);
            this.memoryInfoPage.Controls.Add(this.memoryLab6);
            this.memoryInfoPage.Controls.Add(this.memoryPhyRam);
            this.memoryInfoPage.Controls.Add(this.memoryLab2);
            this.memoryInfoPage.Controls.Add(this.memoryRecMax);
            this.memoryInfoPage.Controls.Add(this.memoryRec);
            this.memoryInfoPage.Controls.Add(this.memoryRecMin);
            this.memoryInfoPage.Controls.Add(this.memoryLab5);
            this.memoryInfoPage.Controls.Add(this.memoryLab4);
            this.memoryInfoPage.Controls.Add(this.memoryLab3);
            this.memoryInfoPage.Controls.Add(this.memoryUpDwn);
            this.memoryInfoPage.Controls.Add(this.memoryTrkBar);
            this.memoryInfoPage.Controls.Add(this.memoryheader);
            this.memoryInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.memoryInfoPage.IsFinishPage = false;
            this.memoryInfoPage.Location = new System.Drawing.Point(0, 0);
            this.memoryInfoPage.Name = "memoryInfoPage";
            this.memoryInfoPage.Size = new System.Drawing.Size(453, 272);
            this.memoryInfoPage.TabIndex = 5;
            this.memoryInfoPage.Enter += new System.EventHandler(this.wizardMemoryPage_Enter);
            this.memoryInfoPage.CloseFromBack += new Gui.Wizard.PageEventHandler(this.memoryInfoPage_CloseFromBack);
            // 
            // memoryLab7
            // 
            this.memoryLab7.AutoSize = true;
            this.memoryLab7.Location = new System.Drawing.Point(421, 127);
            this.memoryLab7.Name = "memoryLab7";
            this.memoryLab7.Size = new System.Drawing.Size(21, 13);
            this.memoryLab7.TabIndex = 38;
            this.memoryLab7.Text = "MB";
            // 
            // memoryLab1
            // 
            this.memoryLab1.AutoSize = true;
            this.memoryLab1.Location = new System.Drawing.Point(12, 82);
            this.memoryLab1.Name = "memoryLab1";
            this.memoryLab1.Size = new System.Drawing.Size(323, 13);
            this.memoryLab1.TabIndex = 26;
            this.memoryLab1.Text = "Select the size of memory to make available to the virtual machine";
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
            // memoryLab6
            // 
            this.memoryLab6.AutoSize = true;
            this.memoryLab6.Location = new System.Drawing.Point(266, 219);
            this.memoryLab6.Name = "memoryLab6";
            this.memoryLab6.Size = new System.Drawing.Size(181, 39);
            this.memoryLab6.TabIndex = 11;
            this.memoryLab6.Text = "Setting the virtual memory above\r\nthe recomended maximum may \r\nresult in signific" +
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
            // memoryLab2
            // 
            this.memoryLab2.AutoSize = true;
            this.memoryLab2.Location = new System.Drawing.Point(38, 173);
            this.memoryLab2.Name = "memoryLab2";
            this.memoryLab2.Size = new System.Drawing.Size(110, 13);
            this.memoryLab2.TabIndex = 22;
            this.memoryLab2.Text = "Current physical RAM";
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
            // memoryLab5
            // 
            this.memoryLab5.AutoSize = true;
            this.memoryLab5.Location = new System.Drawing.Point(38, 245);
            this.memoryLab5.Name = "memoryLab5";
            this.memoryLab5.Size = new System.Drawing.Size(116, 13);
            this.memoryLab5.TabIndex = 17;
            this.memoryLab5.Text = "Recomended maximum";
            // 
            // memoryLab4
            // 
            this.memoryLab4.AutoSize = true;
            this.memoryLab4.Location = new System.Drawing.Point(38, 225);
            this.memoryLab4.Name = "memoryLab4";
            this.memoryLab4.Size = new System.Drawing.Size(69, 13);
            this.memoryLab4.TabIndex = 16;
            this.memoryLab4.Text = "Recomended";
            // 
            // memoryLab3
            // 
            this.memoryLab3.AutoSize = true;
            this.memoryLab3.Location = new System.Drawing.Point(38, 202);
            this.memoryLab3.Name = "memoryLab3";
            this.memoryLab3.Size = new System.Drawing.Size(112, 13);
            this.memoryLab3.TabIndex = 15;
            this.memoryLab3.Text = "Recomended minimum";
            // 
            // memoryUpDwn
            // 
            this.memoryUpDwn.Location = new System.Drawing.Point(355, 125);
            this.memoryUpDwn.Name = "memoryUpDwn";
            this.memoryUpDwn.Size = new System.Drawing.Size(60, 21);
            this.memoryUpDwn.TabIndex = 2;
            this.memoryUpDwn.ValueChanged += new System.EventHandler(this.memoryUpDwn_ValueChanged);
            // 
            // memoryTrkBar
            // 
            this.memoryTrkBar.Location = new System.Drawing.Point(12, 119);
            this.memoryTrkBar.Name = "memoryTrkBar";
            this.memoryTrkBar.Size = new System.Drawing.Size(337, 45);
            this.memoryTrkBar.TabIndex = 1;
            this.memoryTrkBar.Scroll += new System.EventHandler(this.memoryTrkBar_Scroll);
            // 
            // memoryheader
            // 
            this.memoryheader.BackColor = System.Drawing.SystemColors.Control;
            this.memoryheader.CausesValidation = false;
            this.memoryheader.Description = "Choose the size of the virtual memory which will be available to your virtual mac" +
                "hine";
            this.memoryheader.Dock = System.Windows.Forms.DockStyle.Top;
            this.memoryheader.Image = ((System.Drawing.Image)(resources.GetObject("memoryheader.Image")));
            this.memoryheader.Location = new System.Drawing.Point(0, 0);
            this.memoryheader.Name = "memoryheader";
            this.memoryheader.Size = new System.Drawing.Size(453, 64);
            this.memoryheader.TabIndex = 0;
            this.memoryheader.Title = "Virtual Memory";
            // 
            // imageInfoPage
            // 
            this.imageInfoPage.Controls.Add(this.imageHeader);
            this.imageInfoPage.Controls.Add(this.imageLab2);
            this.imageInfoPage.Controls.Add(this.imageLab1);
            this.imageInfoPage.Controls.Add(this.imageFileBrowse);
            this.imageInfoPage.Controls.Add(this.imageLocTxtBox);
            this.imageInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageInfoPage.IsFinishPage = false;
            this.imageInfoPage.Location = new System.Drawing.Point(0, 0);
            this.imageInfoPage.Name = "imageInfoPage";
            this.imageInfoPage.Size = new System.Drawing.Size(453, 272);
            this.imageInfoPage.TabIndex = 8;
            this.imageInfoPage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.imageInfoPage_CloseFromNext);
            this.imageInfoPage.CloseFromBack += new Gui.Wizard.PageEventHandler(this.imageInfoPage_CloseFromBack);
            // 
            // imageHeader
            // 
            this.imageHeader.BackColor = System.Drawing.SystemColors.Control;
            this.imageHeader.CausesValidation = false;
            this.imageHeader.Description = "Select an existing image to be used via the console";
            this.imageHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.imageHeader.Image = ((System.Drawing.Image)(resources.GetObject("imageHeader.Image")));
            this.imageHeader.Location = new System.Drawing.Point(0, 0);
            this.imageHeader.Name = "imageHeader";
            this.imageHeader.Size = new System.Drawing.Size(453, 64);
            this.imageHeader.TabIndex = 9;
            this.imageHeader.Title = "Existing image";
            // 
            // imageLab2
            // 
            this.imageLab2.AutoSize = true;
            this.imageLab2.Location = new System.Drawing.Point(12, 137);
            this.imageLab2.Name = "imageLab2";
            this.imageLab2.Size = new System.Drawing.Size(80, 13);
            this.imageLab2.TabIndex = 8;
            this.imageLab2.Text = "Image Location";
            // 
            // imageLab1
            // 
            this.imageLab1.AutoSize = true;
            this.imageLab1.Location = new System.Drawing.Point(9, 83);
            this.imageLab1.Name = "imageLab1";
            this.imageLab1.Size = new System.Drawing.Size(363, 26);
            this.imageLab1.TabIndex = 7;
            this.imageLab1.Text = "Type a location for your existing virtual machine image. This is the location\r\nin" +
                " which the virtual machine image configuration settings will  be stored.";
            // 
            // imageFileBrowse
            // 
            this.imageFileBrowse.Location = new System.Drawing.Point(366, 155);
            this.imageFileBrowse.Name = "imageFileBrowse";
            this.imageFileBrowse.Size = new System.Drawing.Size(75, 23);
            this.imageFileBrowse.TabIndex = 6;
            this.imageFileBrowse.Text = "Browse...";
            this.imageFileBrowse.UseVisualStyleBackColor = true;
            this.imageFileBrowse.Click += new System.EventHandler(this.imageFileBrowse_Click);
            // 
            // imageLocTxtBox
            // 
            this.imageLocTxtBox.Location = new System.Drawing.Point(12, 157);
            this.imageLocTxtBox.Name = "imageLocTxtBox";
            this.imageLocTxtBox.Size = new System.Drawing.Size(348, 21);
            this.imageLocTxtBox.TabIndex = 5;
            // 
            // harddiskInfoPage
            // 
            this.harddiskInfoPage.Controls.Add(this.label5);
            this.harddiskInfoPage.Controls.Add(this.harddiskDiskLab);
            this.harddiskInfoPage.Controls.Add(this.harddiskLab2);
            this.harddiskInfoPage.Controls.Add(this.harddiskLab1);
            this.harddiskInfoPage.Controls.Add(this.harddiskDiskSizeLab);
            this.harddiskInfoPage.Controls.Add(this.harddiskLab3);
            this.harddiskInfoPage.Controls.Add(this.harddiskRecMinLab);
            this.harddiskInfoPage.Controls.Add(this.harddiskLab4);
            this.harddiskInfoPage.Controls.Add(this.harddiskUpDwn);
            this.harddiskInfoPage.Controls.Add(this.harddiskTrkBar);
            this.harddiskInfoPage.Controls.Add(this.harddiskHeader);
            this.harddiskInfoPage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.harddiskInfoPage.IsFinishPage = false;
            this.harddiskInfoPage.Location = new System.Drawing.Point(0, 0);
            this.harddiskInfoPage.Name = "harddiskInfoPage";
            this.harddiskInfoPage.Size = new System.Drawing.Size(453, 272);
            this.harddiskInfoPage.TabIndex = 6;
            this.harddiskInfoPage.Enter += new System.EventHandler(this.wizardHardDiskPage_Enter);
            this.harddiskInfoPage.CloseFromNext += new Gui.Wizard.PageEventHandler(this.harddiskInfoPage_CloseFromNext);
            this.harddiskInfoPage.CloseFromBack += new Gui.Wizard.PageEventHandler(this.harddiskInfoPage_CloseFromBack);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(421, 142);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(21, 13);
            this.label5.TabIndex = 37;
            this.label5.Text = "MB";
            // 
            // harddiskDiskLab
            // 
            this.harddiskDiskLab.AutoSize = true;
            this.harddiskDiskLab.Location = new System.Drawing.Point(197, 190);
            this.harddiskDiskLab.Name = "harddiskDiskLab";
            this.harddiskDiskLab.Size = new System.Drawing.Size(18, 13);
            this.harddiskDiskLab.TabIndex = 36;
            this.harddiskDiskLab.Text = "C:";
            // 
            // harddiskLab2
            // 
            this.harddiskLab2.AutoSize = true;
            this.harddiskLab2.Location = new System.Drawing.Point(39, 190);
            this.harddiskLab2.Name = "harddiskLab2";
            this.harddiskLab2.Size = new System.Drawing.Size(66, 13);
            this.harddiskLab2.TabIndex = 35;
            this.harddiskLab2.Text = "Physical disk";
            // 
            // harddiskLab1
            // 
            this.harddiskLab1.AutoSize = true;
            this.harddiskLab1.Location = new System.Drawing.Point(9, 83);
            this.harddiskLab1.Name = "harddiskLab1";
            this.harddiskLab1.Size = new System.Drawing.Size(352, 26);
            this.harddiskLab1.TabIndex = 34;
            this.harddiskLab1.Text = "Select a hard disk size for the virtual machine. The location of this image\r\ncorr" +
                "esponds to the default directory previously chosen";
            // 
            // harddiskDiskSizeLab
            // 
            this.harddiskDiskSizeLab.AutoSize = true;
            this.harddiskDiskSizeLab.Location = new System.Drawing.Point(197, 215);
            this.harddiskDiskSizeLab.Name = "harddiskDiskSizeLab";
            this.harddiskDiskSizeLab.Size = new System.Drawing.Size(25, 13);
            this.harddiskDiskSizeLab.TabIndex = 33;
            this.harddiskDiskSizeLab.Text = "size";
            // 
            // harddiskLab3
            // 
            this.harddiskLab3.AutoSize = true;
            this.harddiskLab3.Location = new System.Drawing.Point(39, 215);
            this.harddiskLab3.Name = "harddiskLab3";
            this.harddiskLab3.Size = new System.Drawing.Size(104, 13);
            this.harddiskLab3.TabIndex = 32;
            this.harddiskLab3.Text = "Available free space";
            // 
            // harddiskRecMinLab
            // 
            this.harddiskRecMinLab.AutoSize = true;
            this.harddiskRecMinLab.Location = new System.Drawing.Point(199, 238);
            this.harddiskRecMinLab.Name = "harddiskRecMinLab";
            this.harddiskRecMinLab.Size = new System.Drawing.Size(23, 13);
            this.harddiskRecMinLab.TabIndex = 29;
            this.harddiskRecMinLab.Text = "min";
            // 
            // harddiskLab4
            // 
            this.harddiskLab4.AutoSize = true;
            this.harddiskLab4.Location = new System.Drawing.Point(39, 238);
            this.harddiskLab4.Name = "harddiskLab4";
            this.harddiskLab4.Size = new System.Drawing.Size(112, 13);
            this.harddiskLab4.TabIndex = 26;
            this.harddiskLab4.Text = "Recomended minimum";
            // 
            // harddiskUpDwn
            // 
            this.harddiskUpDwn.Location = new System.Drawing.Point(355, 140);
            this.harddiskUpDwn.Name = "harddiskUpDwn";
            this.harddiskUpDwn.Size = new System.Drawing.Size(60, 21);
            this.harddiskUpDwn.TabIndex = 25;
            this.harddiskUpDwn.ValueChanged += new System.EventHandler(this.harddiskUpdown_ValueChanged);
            // 
            // harddiskTrkBar
            // 
            this.harddiskTrkBar.Location = new System.Drawing.Point(12, 140);
            this.harddiskTrkBar.Name = "harddiskTrkBar";
            this.harddiskTrkBar.Size = new System.Drawing.Size(337, 45);
            this.harddiskTrkBar.SmallChange = 100;
            this.harddiskTrkBar.TabIndex = 24;
            this.harddiskTrkBar.Scroll += new System.EventHandler(this.harddiskTrkBar_Scroll);
            // 
            // harddiskHeader
            // 
            this.harddiskHeader.BackColor = System.Drawing.SystemColors.Control;
            this.harddiskHeader.CausesValidation = false;
            this.harddiskHeader.Description = "Choose the size of hard diskfor your virtual machine";
            this.harddiskHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.harddiskHeader.Image = ((System.Drawing.Image)(resources.GetObject("harddiskHeader.Image")));
            this.harddiskHeader.Location = new System.Drawing.Point(0, 0);
            this.harddiskHeader.Name = "harddiskHeader";
            this.harddiskHeader.Size = new System.Drawing.Size(453, 64);
            this.harddiskHeader.TabIndex = 0;
            this.harddiskHeader.Title = "Hard Disk Size";
            // 
            // NewVMWizard
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(453, 320);
            this.ControlBox = false;
            this.Controls.Add(this.wizardNewVM);
            this.Name = "NewVMWizard";
            this.Text = "New Virtual Machine Wizard";
            this.wizardNewVM.ResumeLayout(false);
            this.defaultDirInfoPage.ResumeLayout(false);
            this.defaultDirInfoPage.PerformLayout();
            this.nameInfoPage.ResumeLayout(false);
            this.nameInfoPage.PerformLayout();
            this.optionInfoPage.ResumeLayout(false);
            this.optionInfoPage.PerformLayout();
            this.wizardStartPage.ResumeLayout(false);
            this.wizardFinishPage.ResumeLayout(false);
            this.memoryInfoPage.ResumeLayout(false);
            this.memoryInfoPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.memoryUpDwn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.memoryTrkBar)).EndInit();
            this.imageInfoPage.ResumeLayout(false);
            this.imageInfoPage.PerformLayout();
            this.harddiskInfoPage.ResumeLayout(false);
            this.harddiskInfoPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.harddiskUpDwn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.harddiskTrkBar)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Gui.Wizard.Wizard wizardNewVM;
        private Gui.Wizard.WizardPage wizardStartPage;
        private Gui.Wizard.InfoPage startInfoPage;
        private Gui.Wizard.WizardPage optionInfoPage;
        private System.Windows.Forms.RadioButton optionRadDefault;
        private System.Windows.Forms.RadioButton optionRadExist;
        private System.Windows.Forms.RadioButton optionRadNew;
        private Gui.Wizard.Header optionHeader;
        private System.Windows.Forms.Label optionLab3;
        private System.Windows.Forms.Label optionLab2;
        private System.Windows.Forms.Label optionLab1;
        private Gui.Wizard.WizardPage nameInfoPage;
        private Gui.Wizard.WizardPage wizardFinishPage;
        private Gui.Wizard.InfoPage finishInfoPage;
        private Gui.Wizard.Header nameHeader;
        private System.Windows.Forms.Label nameLab2;
        private System.Windows.Forms.TextBox nameTxtBox;
        private Gui.Wizard.WizardPage defaultDirInfoPage;
        private Gui.Wizard.WizardPage harddiskInfoPage;
        private Gui.Wizard.WizardPage memoryInfoPage;
        private Gui.Wizard.Header memoryheader;
        private Gui.Wizard.Header harddiskHeader;
        private Gui.Wizard.Header defaultDirHeader;
        private System.Windows.Forms.NumericUpDown memoryUpDwn;
        private System.Windows.Forms.TrackBar memoryTrkBar;
        private System.Windows.Forms.Label memoryLab6;
        private System.Windows.Forms.Label memoryPhyRam;
        private System.Windows.Forms.Label memoryLab2;
        private System.Windows.Forms.Label memoryRecMax;
        private System.Windows.Forms.Label memoryRec;
        private System.Windows.Forms.Label memoryRecMin;
        private System.Windows.Forms.Label memoryLab5;
        private System.Windows.Forms.Label memoryLab4;
        private System.Windows.Forms.Label memoryLab3;
        private System.Windows.Forms.Panel memoryHorizRuleDark;
        private System.Windows.Forms.Panel memoryHorizRuleLight;
        private System.Windows.Forms.Label harddiskDiskSizeLab;
        private System.Windows.Forms.Label harddiskLab3;
        private System.Windows.Forms.Label harddiskRecMinLab;
        private System.Windows.Forms.Label harddiskLab4;
        private System.Windows.Forms.NumericUpDown harddiskUpDwn;
        private System.Windows.Forms.TrackBar harddiskTrkBar;
        private System.Windows.Forms.Button defaultDirBrowse;
        private System.Windows.Forms.TextBox defaultDirTxtBox;
        private System.Windows.Forms.Label defaultDirLab1;
        private System.Windows.Forms.Label defaultDirLab2;
        private System.Windows.Forms.Label memoryLab1;
        private System.Windows.Forms.Label harddiskDiskLab;
        private System.Windows.Forms.Label harddiskLab2;
        private System.Windows.Forms.Label harddiskLab1;
        private System.Windows.Forms.Label nameLab1;
        private System.Windows.Forms.FolderBrowserDialog wizardFolderBrowser;
        private System.Windows.Forms.Label memoryLab7;
        private System.Windows.Forms.Label label5;
        private Gui.Wizard.WizardPage imageInfoPage;
        private Gui.Wizard.Header imageHeader;
        private System.Windows.Forms.Label imageLab2;
        private System.Windows.Forms.Label imageLab1;
        private System.Windows.Forms.Button imageFileBrowse;
        private System.Windows.Forms.TextBox imageLocTxtBox;
        private System.Windows.Forms.OpenFileDialog wizardOpenFile;
    }
}
