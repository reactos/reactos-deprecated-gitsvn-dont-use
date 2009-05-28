using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using DebugProtocol;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class RawTraffic : ToolWindow, IUseDebugConnection
    {
        DebugConnection mConnection;
        List<string> textToAdd = new List<string>();
        // public event CanCopyChangedEventHandler CanCopyChangedEvent; 

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
           // ((MainWindow)this.ParentForm).CopyEvent += CopyEvent;
        }

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            conn.DebugRawTrafficEvent += DebugRawTrafficEvent;
        }

        void UpdateText()
        {
            StringBuilder toAdd = new StringBuilder();
            lock (textToAdd)
            {
                foreach (string s in textToAdd)
                    toAdd.Append(s);
                textToAdd.Clear();
                //TODO: skip backspace signs
            }
            RawTrafficText.AppendText(toAdd.ToString());
            InputLabel.Location = RawTrafficText.GetPositionFromCharIndex(RawTrafficText.Text.Length -1);
            InputLabel.Top += 2;
            InputLabel.Left += 2;
        }

        void DebugRawTrafficEvent(object sender, DebugRawTrafficEventArgs args)
        {
            lock (textToAdd)
            {
                textToAdd.Add(args.Data);
            }
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateText"));
        }

      /*  void CopyEvent(object sender, CopyEventArgs args)
        {
            if (args.Obj == this && RawTrafficText.SelectedText != null)
                Clipboard.SetText(RawTrafficText.SelectedText);
        } */

        public RawTraffic()
        {
            InitializeComponent();
            this.Tag = "Raw Traffic";
            InputLabel.Location = new Point(2, 2);
        }

        private void RawTrafficText_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((mConnection.ConnectionMode != DebugConnection.Mode.ClosedMode) && (!mConnection.Running))
            {
                switch ((int)e.KeyChar)
                {
                    case 8: /* Backspace */
                        if (InputLabel.Text.Length > 0)
                            InputLabel.Text = InputLabel.Text.Substring(0, InputLabel.Text.Length - 1);
                        break;
                    case 13: /* Return */
                        if (InputLabel.Text.ToLower().CompareTo("cont") == 0)
                            mConnection.Running = true; 
                        InputLabel.Text = "";
                        break;
                    default:
                        InputLabel.Text += e.KeyChar;
                        break;
                }

                mConnection.Debugger.Write("" + e.KeyChar);
            }
        }

        private void RawTrafficText_MouseUp(object sender, MouseEventArgs e)
        {
            copyToolStripMenuItem.Enabled = (RawTrafficText.SelectionLength > 0);
         /*   if (CanCopyChangedEvent != null)
                CanCopyChangedEvent(this, new CanCopyChangedEventArgs(RawTrafficText.SelectionLength != 0)); */
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (RawTrafficText.SelectionLength != 0)
                Clipboard.SetText(RawTrafficText.SelectedText);
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RawTrafficText.SelectAll(); 
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {
            selectAllToolStripMenuItem.Enabled = (RawTrafficText.Text.Length != 0);
        }

        public override bool IsCmdEnabled(Commands Cmd)
        {
            switch (Cmd)
            {
                case Commands.SaveAs:
                    return true;
            }
            return false;
        }

        public override void SaveAs(string FileName)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.FileName = "debuglog-" + DateTime.Now.Date.ToShortDateString() + ".txt";
            saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            saveFileDialog.Filter = "Textfiles (*.txt)|*.txt|All Files (*.*)|*.*";
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                StreamWriter sw = new StreamWriter(saveFileDialog.FileName);
                sw.Write(RawTrafficText.Text);
                sw.Close(); 
            }
        }

    }
}
