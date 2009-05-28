using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using AbstractPipe;

namespace RosDBG
{
    public partial class Connect : Form
    {
        public enum ConnectionType {Serial, Pipe, Socket};
        public ConnectionType Type = Settings.SelectedConnType;
        private string defaultPipeName = Settings.Pipe;
        private string pipeName;
        public ConnectionMode pipeMode;

        public Connect()
        {
            InitializeComponent();
        }

#region Socket
        public string Host
        {
            get { return HostChoice.Text; }
            set { HostChoice.Text = value; }
        }

        public int Port
        {
            get { return (int)PortNumber.Value; }
            set { PortNumber.Value = value; }
        }
#endregion

#region Pipe
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
#endregion

#region Serial
        public String ComPort
        {
            get { return cPort.Text; }
            set { ComPort = value; }
        }

        public int Baudrate
        {
            get { return Convert.ToInt32(cBaud.Text); }
            set { Baudrate = value; }
        }

        private void SelectComboItem(ComboBox obj, string text)
        {
            obj.SelectedIndex = 0;
            foreach (object item in obj.Items)
            {
                if (item.ToString() == text)
                {
                    obj.SelectedItem = item;
                    break;
                }
            }
        }
#endregion

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (DefaultRadioBtn.Checked)
                pipeName = defaultPipeName;
            else
                pipeName = PipeNameTextBox.Text;

            if (cType.SelectedItem.ToString().CompareTo("Client") == 0)
                pipeMode = ConnectionMode.MODE_CLIENT;
            else if (cType.SelectedItem.ToString().CompareTo("Server") == 0)
                pipeMode = ConnectionMode.MODE_SERVER;
            else if (cType.SelectedItem.ToString().CompareTo("Automatic") == 0)
                pipeMode = ConnectionMode.MODE_AUTO;

            Settings.SelectedConnType = Type;
            Settings.Save();

            Close();
        }

        private void Connect_Load(object sender, EventArgs e)
        {
            int i = 0;

            /* Serial */
            foreach (string s in SerialPort.GetPortNames())
            {
                cPort.Items.Add(s);
            }
            SelectComboItem(cPort, Settings.ComPort);
            SelectComboItem(cBaud, Settings.Baudrate);

            /* Pipe */
            DefaultRadioBtn.Text += " [" + defaultPipeName + "]";
            for (i = 0; i < cType.Items.Count - 1; i++)
            {
                if (cType.Items[i].ToString().CompareTo(Settings.Mode) == 0)
                    break;
            }
            cType.SelectedIndex = i;

            tabControl.SelectedIndex = (int) Settings.SelectedConnType;
        }

        private void CustomRadioBtn_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rb = (RadioButton)sender;
            PipeNameTextBox.Enabled = rb.Checked;
        }

        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            Type = (ConnectionType) ((TabControl)sender).SelectedIndex;
        }
    }
}
