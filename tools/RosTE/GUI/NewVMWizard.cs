using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace RosTEGUI
{
    public partial class NewVMWizard : Form
    {
        public NewVMWizard()
        {
            InitializeComponent();
        }

        private void wizard1_CloseFromCancel(object sender, CancelEventArgs e)
        {
            if (MessageBox.Show(this,
                                "Are you sure you want to close the wizard?",
                                "Wizard Cancelled", 
                                MessageBoxButtons.YesNo
                                ) == DialogResult.No)
            {
                e.Cancel = true;
            }
        }

        private void wizardSetupOptionPage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (optionRadDefault.Checked == true)
                e.Page = wizardFinishPage;
            else
                e.Page = wizardNamePage;
        }

        private void wizardNamePage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (wizardNameTxtBox.Text == "")
            {
                MessageBox.Show("You must enter a name", "Error");
                e.Page = wizardNamePage;
            }
        }

        private void wizardDefaultDirPage_CloseFromNext(object sender, Gui.Wizard.PageEventArgs e)
        {
            if (wizardDefaultDirTextBox.Text == "")
            {
                MessageBox.Show("You must enter a name", "Error");
                e.Page = wizardDefaultDirPage;
            }
            else if (Directory.Exists(wizardDefaultDirTextBox.Text) == false)
            {
                MessageBox.Show("Directory does not exist", "Error");
                e.Page = wizardDefaultDirPage;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult result = wizardFolderBrowser.ShowDialog();

            if (result == DialogResult.OK)
            {
                wizardDefaultDirTextBox.Text = wizardFolderBrowser.SelectedPath;
            }
        }
    }
}