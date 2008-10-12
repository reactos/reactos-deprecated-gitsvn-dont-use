using AbstractPipe;
using System;
using System.Windows.Forms;

namespace RosDBG
{
    public partial class PipeTargetSelect : Form
    {
        private string defaultPipeName = Settings.Pipe;
        private string pipeName;
        public ConnectionMode pipeMode;

        public string PipeName
        {
            get { return pipeName; }
        }

        public ConnectionMode PipeMode
        {
            get { return (ConnectionMode)pipeMode; }
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

            if (cType.SelectedItem.ToString().CompareTo("Client") == 0)
            {
                pipeMode = ConnectionMode.MODE_CLIENT;
            }
            else if (cType.SelectedItem.ToString().CompareTo("Server") == 0)
            {
                pipeMode = ConnectionMode.MODE_SERVER;
            }
            else if (cType.SelectedItem.ToString().CompareTo("Automatic") == 0)
            {
                pipeMode = ConnectionMode.MODE_AUTO;
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
            DefaultRadioBtn.Text += " [" + defaultPipeName + "]";
            cType.SelectedIndex = 0;
        }
    }
}
