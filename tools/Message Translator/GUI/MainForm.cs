using System;
using System.Windows.Forms;
using System.Configuration;
using System.Collections.Generic;
using MsgTrans.Library;
using System.Resources;
using System.Reflection;
using System.IO;
using Microsoft.Win32;

namespace MsgTranslator
{
    public partial class MainForm : Form
    {
        private static string regPath = @"Software\ReactOS\MsgTrans";
        private string[] msgTypes = { "error", "wm", "bug" };
        private List<Command> errMessages;
        private int curErrorPage;

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
        private static string PrependWorkingDirectory(string file)
        {
            string full;
            string workDir;

            try
            {
                workDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                full = workDir + '\\' + file;
                return full;
            }
            catch (Exception)
            {
                return file;
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
                s = PrependWorkingDirectory(s);
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
                s = PrependWorkingDirectory(s);
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
                s = PrependWorkingDirectory(s);
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
                s = PrependWorkingDirectory(s);
                return s;
            }
        }
        private static string BugCheckXml
        {
            get
            {
                string optionName = "BugcheckXml";
                string s = ConfigurationSettings.AppSettings[optionName];
                VerifyRequiredOption(optionName,
                                     s);
                s = PrependWorkingDirectory(s);
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

        private void UpdateErrorPage(int pageNum)
        {
            if (errMessages != null && errMessages.Count > 1)
            {
                if (pageNum > errMessages.Count || pageNum < 0)
                    return;

                errorTypeValueLabel.Text = errMessages[pageNum - 1].MsgType.ToString();
                errorDecimalTxtBox.Text = errMessages[pageNum - 1].Number.ToString();
                errorHexTxtBox.Text = "0x" + errMessages[pageNum - 1].Hex;
                errorCodeTxtBox.Text = errMessages[pageNum - 1].Code;
                errorMessageTxtBox.Text = errMessages[pageNum - 1].Message;

                curErrorPage = pageNum;

                // set navigation buttons
                errorBackButton.Enabled = errorForwardButton.Enabled = false;
                if (curErrorPage < errMessages.Count)
                    errorForwardButton.Enabled = true;
                if (curErrorPage > 1)
                    errorBackButton.Enabled = true;
            }
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
                                                                  BugCheckXml,
                                                                  WmXml,
                                                                  BugUrl);
                string message = msgType + " " + mainErrTxtBox.Text;
                if (msgTran.ParseCommandMessage(null, message))
                {
                    if (msgTran.Messages.Count > 1)
                    {
                        // if we have more than one, they must be error messages
                        errMessages = msgTran.Messages;
                        UpdateErrorPage(1);
                    }
                    else
                    {
                        Command cmd = msgTran.Messages[0];

                        if (cmd.MsgType == MessageType.WinError ||
                            cmd.MsgType == MessageType.HRESULT ||
                            cmd.MsgType == MessageType.NTSTATUS ||
                            cmd.MsgType == MessageType.BugCheck ||
                            cmd.MsgType == MessageType.Custom)
                        {
                            if (errMessages != null)
                                errMessages.Clear();

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
            curErrorPage = 1;
            errorBackButton.Enabled = false;
            errorForwardButton.Enabled = false;

            // setup bug page
            bugLinkLabel.Links[0].LinkData = BugHomepage;
            SetBugurlPosition();

            // setup options page
            optionsMinimizeChkBox.Checked = HideOnMin;
            optionsRunStartChkBox.Checked = RunOnStart;


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
                this.Close();
            }
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

        private void NavigateErrorPage(object sender, EventArgs e)
        {
            Button btn = (Button)sender;

            if (btn.Name == "errorForwardButton")
            {
                UpdateErrorPage(curErrorPage + 1);
            }
            else if (btn.Name == "errorBackButton")
            {
                UpdateErrorPage(curErrorPage - 1);
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
