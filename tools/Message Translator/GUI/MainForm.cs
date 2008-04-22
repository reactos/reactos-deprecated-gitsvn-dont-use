using System;
using System.Windows.Forms;
using System.Configuration;
using MsgTrans.Library;
using System.Resources;
using System.Reflection;
using Microsoft.Win32;

namespace MsgTranslator
{
    public partial class MainForm : Form
    {
        private static string regPath = @"Software\ReactOS\MsgTrans";
        private string[] msgTypes = { "error", "wm", "bug" };

        #region properties
        private static bool HideOnMin
        {
            get
            {
                string keyName = "HideOnMin";
                RegistryKey rk = Registry.CurrentUser.CreateSubKey(regPath);
                return Convert.ToBoolean(rk.GetValue(keyName));
            }
            set
            {
                string keyName = "HideOnMin";
                RegistryKey rk = Registry.CurrentUser.OpenSubKey(regPath, true);
                rk.SetValue(keyName, value);
            }
        }
        private static bool RunOnStart
        {
            get
            {
                string keyName = "RunOnStart";
                RegistryKey rk = Registry.CurrentUser.CreateSubKey(regPath);
                return Convert.ToBoolean(rk.GetValue(keyName));
            }
            set
            {
                string keyName = "RunOnStart";
                RegistryKey rk = Registry.CurrentUser.OpenSubKey(regPath, true);
                rk.SetValue(keyName, value);
            }
        }

        private static void VerifyRequiredOption(string optionName,
                                 string optionValue)
        {
            if (optionValue == null)
            {
                throw new Exception(String.Format("Option '{0}' not set.",
                                                  optionName));
            }
        }
        private static string NtstatusXml
        {
            get
            {
                string optionName = "NtstatusXml";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                return s;
            }
        }
        private static string WinerrorXml
        {
            get
            {
                string optionName = "WinerrorXml";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                return s;
            }
        }
        private static string HresultXml
        {
            get
            {
                string optionName = "HresultXml";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                return s;
            }
        }
        private static string WmXml
        {
            get
            {
                string optionName = "WmXml";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                return s;
            }
        }
        private static string BugUrl
        {
            get
            {
                string optionName = "BugUrl";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                return s;
            }
        }
        private static string BugHomepage
        {
            get
            {
                string optionName = "BugHomepage";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                return s;
            }
        }
        #endregion

        private void SetAutoStart(bool bStart)
        {
            string path = @"Software\Microsoft\Windows\CurrentVersion\Run";
            string keyName = "MsgTrans";

            try
            {
                if (bStart)
                {
                    RegistryKey rk = Registry.CurrentUser.CreateSubKey(path);
                    string dir = Assembly.GetEntryAssembly().Location;
                    rk.SetValue(keyName, dir);
                }
                else
                {
                    RegistryKey rk = Registry.CurrentUser.CreateSubKey(path);
                    rk.DeleteValue(keyName);
                }
            }
            catch (Exception ex)
            {
                MessageOutput mo = new MessageOutput();
                mo.MsgOut(null, ex.Message);
            }
        }

        private void SendToTray()
        {
            Hide();
            notifyIcon.Visible = true;
        }

        private string GetMessageType()
        {
            try
            {
                return msgTypes[mainTabControl.SelectedIndex];
            }
            catch (ArgumentOutOfRangeException)
            {
                return null;
            }
        }

        private void SetBugurlPosition()
        {
            int hCenter = (mainTabControl.Height / 2) - (bugLinkLabel.Height / 2);
            int wCenter = (mainTabControl.Width / 2) - (bugLinkLabel.Width / 2);
            bugLinkLabel.Location = new System.Drawing.Point(wCenter, hCenter);
        }

        private void GetMessage()
        {
            string msgType = GetMessageType();

            if (msgType != null)
            {
                MessageTranslator msgTran = new MessageTranslator(new MessageOutput(),
                                                                  NtstatusXml,
                                                                  WinerrorXml,
                                                                  HresultXml,
                                                                  WmXml,
                                                                  BugUrl);
                string message = msgType + " " + mainErrTxtBox.Text;
                if (msgTran.ParseCommandMessage(null, message))
                {
                    foreach (Command cmd in msgTran.Messages)
                    {
                        if (cmd.MsgType == MessageType.WinError ||
                            cmd.MsgType == MessageType.HResult ||
                            cmd.MsgType == MessageType.NTStatus ||
                            cmd.MsgType == MessageType.Custom)
                        {
                            errorTypeValueLabel.Text = cmd.MsgType.ToString();
                            errorDecimalTxtBox.Text = cmd.Number.ToString();
                            errorHexTxtBox.Text = "0x" + cmd.Hex;
                            errorCodeTxtBox.Text = cmd.Code;
                            errorMessageTxtBox.Text = cmd.Message;
                        }
                        else if (cmd.MsgType == MessageType.WinMsg)
                        {
                            wndmsgDecimalTxtBox.Text = cmd.Number.ToString();
                            wndmsgHexTxtBox.Text = "0x" + cmd.Hex;
                            wndmsgCodeTxtBox.Text = cmd.Code;
                        }
                        else if (cmd.MsgType == MessageType.BugUrl)
                        {
                            bugLinkLabel.Text = "Click here for bug " + cmd.Number;
                            bugLinkLabel.Links[0].LinkData = cmd.Code;
                            bugLinkLabel.Links[0].Visited = false;

                            SetBugurlPosition();
                        }
                    }
                }
            }
        }

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            // setup error page
            errorBackButton.Enabled = false;
            errorForwardButton.Enabled = false;

            // setup bug page
            bugLinkLabel.Links[0].LinkData = BugHomepage;
            SetBugurlPosition();

            // setup options page
            optionsMinimizeChkBox.Checked = HideOnMin;
            optionsRunStartChkBox.Checked = RunOnStart;
            notifyIcon.Visible = false;
            /*
            if (HideOnMin)
            {
                // FIXME: hide correctly
                this.WindowState = FormWindowState.Minimized;
                SendToTray();
            }*/

          //toolTip.SetToolTip(mainErrTxtBox, Properties.Resources.tooltipErrMsg);
          //toolTip.SetToolTip(mainWndMsgRadio, Properties.Resources.tooltipWndMsg);
          //toolTip.SetToolTip(mainBugMsgRadio, Properties.Resources.tooltipBug);
          //toolTip.SetToolTip(mainOptionsRadio, Properties.Resources.tooltipOpt);
        }

        private void bugLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            bugLinkLabel.Links[bugLinkLabel.Links.IndexOf(e.Link)].Visited = true;

            System.Diagnostics.Process.Start(e.Link.LinkData.ToString());
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState && HideOnMin)
            {
                SendToTray();
            }
        }

        private void notifyIcon_DoubleClick(object sender, EventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;
            notifyIcon.Visible = false;
        }

        private void mainLookupButton_Click(object sender, EventArgs e)
        {
            GetMessage();
            mainErrTxtBox.Focus();
            mainErrTxtBox.SelectAll();
        }

        private void mainErrTxtBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            TextBox tb = (TextBox)sender;

            if (e.KeyChar == (char)Keys.Enter)
            {
                GetMessage();
                tb.SelectAll();
                e.Handled = true;
            }
        }

        private void OptionsHaveChanged(object sender, EventArgs e)
        {
            CheckBox cb = (CheckBox)sender;

            if (cb.Name == optionsMinimizeChkBox.Name)
            {
                HideOnMin = optionsMinimizeChkBox.Checked;
            }
            else if (cb.Name == optionsRunStartChkBox.Name)
            {
                SetAutoStart(cb.Checked);
                RunOnStart = optionsRunStartChkBox.Checked;
            }
        }

        private void mainTabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            TabControl tc = (TabControl)sender;

            if (tc.SelectedIndex == 3)
            {
                mainErrLabel.Enabled = false;
                mainErrTxtBox.Enabled = false;
                mainLookupButton.Enabled = false;
            }
            else
            {
                mainErrLabel.Enabled = true;
                mainErrTxtBox.Enabled = true;
                mainLookupButton.Enabled = true;

                if (tc.SelectedIndex == 2)
                {
                    mainErrLabel.Text = "Bug Num:";
                }
                else
                {
                    mainErrLabel.Text = "Message:";
                }
            }
        }
    }

    public class MessageOutput : IMsgOutput
    {
        public void MsgOut(MessageContext context,
                           string message)
        {
            MessageBox.Show(message);
        }
    }
}