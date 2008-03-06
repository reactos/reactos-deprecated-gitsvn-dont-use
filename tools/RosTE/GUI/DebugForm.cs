using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public partial class DebugForm : Form
    {
        public DebugForm()
        {
            InitializeComponent();
        }

        public void AddMessage(string message, string exception, string trace)
        {
            errorText.Text = message + " : " + exception + "\n\t" + trace;
        }

        private void errorCloseBtn_Click(object sender, EventArgs e)
        {
            Debug.DoDebug = false;
            Close();
        }

        private void errorCopyBtn_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(errorText.Text);
        }
    }

    public static class Debug
    {
        static DebugForm df = null;
        static bool doDebug = false;

        public static bool DoDebug
        {
            set { doDebug = value; }
            get { return doDebug; }
        }

        public static void TurnDebuggingOn()
        {
            if (df == null)
            {
                df = new DebugForm();
                df.Show();
            }

            DoDebug = true;
        }

        public static void TurnDebuggingOff()
        {
            if (df != null)
            {
                df.Close();
                df = null;
            }

            DoDebug = false;
        }

        public static void LogMessage(string message)
        {
            LogMessage(message, null, null, false);
        }

        public static void LogMessage(string message, string exception)
        {
            LogMessage(message, exception, null, false);
        }

        public static void LogMessage(string message, string exception, string trace)
        {
            LogMessage(message, exception, trace, false);
        }

        public static void LogMessage(string message, string exception, string trace, bool bForce)
        {
            if (df == null && bForce)
            {
                df = new DebugForm();
                df.Show();
                df.AddMessage(message, exception, trace);
            }

            if (DoDebug)
                df.AddMessage(message, null, null);
        }
    }
}