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
        MessageTranslator msgTrans;
        private Panel[] panels;
        private static string regPath = @"Software\AppSense Technologies\MsgTrans";

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
                    if (msgType == msgTypes[0]) // error
                    {
                        errorTypeValueLabel.Text = msgTran.Type;
                        errorDecimalTxtBox.Text = msgTran.Number.ToString();
                        errorHexTxtBox.Text = "0x" + msgTran.Hex;
                        errorCodeTxtBox.Text = msgTran.Code;
                        errorMessageTxtBox.Text = msgTran.Message;
                    }
                    else if (msgType == msgTypes[1]) // wm
                    {
                        wndmsgNumberTxtBox.Text = msgTran.Number.ToString();
                        wndmsgCodeTxtBox.Text = msgTran.Code;
                    }
                    else if (msgType == msgTypes[2]) // bug
                    {
                        bugLinkLabel.Text = "Click here for bug " + msgTran.Number;
                        bugLinkLabel.Links[0].LinkData = msgTran.BugUrl;
                        bugLinkLabel.Links[0].Visited = false;

                        SetBugurlPosition();
                    }
                }
            }
        }

        public MainForm()
        {
            InitializeComponent();
        }

        public void MsgOut(string message)
        {
            MessageBox.Show(message);
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
            optionsOKButton.Enabled = false;
            optionsOKButton.Text = "Done";
            notifyIcon.Visible = false;

            toolTip.SetToolTip(mainErrTxtBox, Properties.Resources.tooltipErrMsg);
          //toolTip.SetToolTip(mainWndMsgRadio, Properties.Resources.tooltipWndMsg);
          //toolTip.SetToolTip(mainBugMsgRadio, Properties.Resources.tooltipBug);
          //toolTip.SetToolTip(mainOptionsRadio, Properties.Resources.tooltipOpt);
        }

        private void bugLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            bugLinkLabel.Links[bugLinkLabel.Links.IndexOf(e.Link)].Visited = true;

            System.Diagnostics.Process.Start(e.Link.LinkData.ToString());
        }

        private void optionsOKButton_Click(object sender, EventArgs e)
        {
            Button btn = (Button)sender;

            HideOnMin = optionsMinimizeChkBox.Checked;
            RunOnStart = optionsRunStartChkBox.Checked;

            btn.Text = "Done";
            btn.Enabled = false;
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
            {
                if (HideOnMin)
                {
                    Hide();
                    notifyIcon.Visible = true;
                }
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
            optionsOKButton.Text = "OK";
            optionsOKButton.Enabled = true;
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