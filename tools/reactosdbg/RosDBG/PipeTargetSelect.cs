using System;
using System.Windows.Forms;

namespace RosDBG
{
    public partial class PipeTargetSelect : Form
    {
        private string defaultPipeName = Settings.Pipe;
        private string pipeName;

        public string PipeName
        {
            get { return pipeName; }
        }

        public bool UseDefault
        {
            get { return DefaultRadioBtn.Checked; }
        }

        public PipeTargetSelect()
        {
            InitializeComponent();
        }

        private void bOK_Click(object sender, EventArgs e)
        {
            if (DefaultRadioBtn.Checked)
            {
                pipeName = defaultPipeName;
            }
            else
            {
                pipeName = PipeNameTextBox.Text;
            }

            DialogResult = DialogResult.OK;
            Close();
        }

        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rb = (RadioButton)sender;

            PipeNameTextBox.Enabled = rb.Checked;
        }

        private void PipeTargetSelect_Load(object sender, EventArgs e)
        {
            DefaultNameLabel.Text = "[" + defaultPipeName + "]";
        }
    }
}
