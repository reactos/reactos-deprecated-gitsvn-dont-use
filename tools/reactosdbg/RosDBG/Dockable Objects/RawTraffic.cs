using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Printing;
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
        private int startPage;
        private int totalPages;
        private int pageNumber;
        private string strPrintText;
        private List<string> prevCommand;
        private int maxCommands = 10;
        PrintDialog pd;
        PrintDocument printDoc;
        DebugConnection mConnection;
        List<string> textToAdd = new List<string>();
        string kdbPrompt = "\r\nkdb:> ";

        private string[] builtinCommands = { ".cls", ".reload" };


        public RawTraffic()
        {
            InitializeComponent();

            prevCommand = new List<string>(maxCommands);
            RawTrafficTextBox.Tag = 0;
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
        }

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            conn.DebugRawTrafficEvent += DebugRawTrafficEvent;
        }

        private void ParseBuiltinCommand(string command)
        {
            if (command == ".cls")
            {
                AddCommandToList(command);
                RawTrafficText.Text = string.Empty;
            }
        }

        private void ParseGdbCommand(string command)
        {
            AddCommandToList(command);
            RawTrafficTextBox.Text += '\r'; //FIXME: remove this
            SendCommandToDebugger();
            RawTrafficTextBox.Text = string.Empty;
        }

        private void ParseCommand(string command)
        {
            if (command.StartsWith("."))
            {
                ParseBuiltinCommand(command);
            }
            else
            {
                ParseGdbCommand(command);
            }
        }


        void UpdateText()
        {
            StringBuilder toAdd = new StringBuilder();
            lock (textToAdd)
            {
                foreach (string s in textToAdd)
                {
                    if (s.EndsWith(kdbPrompt))
                    {
                        toAdd.Append(s.Remove(s.Length - kdbPrompt.Length));
                    }
                    else
                    {
                        toAdd.Append(s);
                    }
                }
                textToAdd.Clear();
            }
            RawTrafficText.AppendText(toAdd.ToString());
        }
        
        void DebugRawTrafficEvent(object sender, DebugRawTrafficEventArgs args)
        {
            lock (textToAdd)
            {
                textToAdd.Add(args.Data);
            }
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateText"));
        }

        private void AddCommandToList(string cmd)
        {
            if (prevCommand.Count > 0 &&
                prevCommand[prevCommand.Count - 1] == cmd)
            {
                return;
            }

            if (prevCommand.Count == maxCommands)
            {
                for (int i = 0; i < prevCommand.Count; i++)
                {
                    if (i < prevCommand.Count - 1)
                        prevCommand[i] = prevCommand[i + 1];
                }

                prevCommand.RemoveAt(prevCommand.Count - 1);
            }

            prevCommand.Add(cmd);

            RawTrafficTextBox.Tag = prevCommand.Count;
        }


        private void SendCommandToDebugger()
        {
            if (RawTrafficTextBox.Text.Length > 0 && mConnection.Debugger != null)
            {
                RawTrafficText.AppendText(kdbPrompt);
                mConnection.Debugger.Write(RawTrafficTextBox.Text);
            }
        }


        #region override methods
        public override bool IsCmdEnabled(Commands Cmd)
        {
            switch (Cmd)
            {
                case Commands.Save:
                case Commands.SaveAs:
                case Commands.Print:
                    return true;
            }
            return false;
        }

        public override void Save(string FileName)
        {
            SaveAs(FileName);
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

        public override void Print(bool ShowDialog)
        {
                printDoc = new PrintDocument();
                printDoc.PrintPage += new PrintPageEventHandler(printDoc_PrintPage);
                pd = new PrintDialog();
                pd.Document = printDoc;
                pd.AllowSomePages = true; 
                if ((!ShowDialog) || (pd.ShowDialog() == DialogResult.OK))
                {
                    printDoc.DocumentName = "debuglog-" + DateTime.Now.Date.ToShortDateString() + ".txt";
                    strPrintText = RawTrafficText.Text.Length > 0 ? RawTrafficText.Text : " ";
                    switch (pd.PrinterSettings.PrintRange) 
                    { 
                        case PrintRange.AllPages: 
                            startPage = 1; 
                            totalPages = pd.PrinterSettings.MaximumPage;
                            break; 
                        case PrintRange.SomePages: 
                            startPage = pd.PrinterSettings.FromPage;
                            totalPages = pd.PrinterSettings.ToPage - startPage + 1;
                            break; 
                    }
                    // start printing
                    pageNumber = 1;
                    printDoc.Print();
                }
        }
        #endregion

        #region events
        private void RawTrafficTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Enter:
                    if (RawTrafficTextBox.Text.Length > 0)
                    {
                        ParseCommand(RawTrafficTextBox.Text);
                    }
                    break;

                case Keys.Up:
                case Keys.Down:
                    int prevCmd = (int)RawTrafficTextBox.Tag;

                    if (e.KeyCode == Keys.Up)
                        prevCmd--;
                    else
                        prevCmd++;

                    if (prevCmd >= 0 && prevCmd < prevCommand.Count)
                    {
                        RawTrafficTextBox.Text = prevCommand[prevCmd];
                        RawTrafficTextBox.Tag = prevCmd;
                        RawTrafficTextBox.SelectionStart = RawTrafficTextBox.Text.Length;
                    }
                    break;

                case Keys.Escape:
                    RawTrafficTextBox.Text = string.Empty;
                    RawTrafficTextBox.Tag = prevCommand.Count;
                    break;
            }

        }

        private void RawTrafficText_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((mConnection.ConnectionMode != DebugConnection.Mode.ClosedMode) && (!mConnection.Running))
            {
                switch ((int)e.KeyChar)
                {
                    case 8: /* Backspace */
                        if (RawTrafficTextBox.Text.Length > 0)
                            RawTrafficTextBox.Text = RawTrafficTextBox.Text.Substring(0, RawTrafficTextBox.Text.Length - 1);
                        break;
                    case 13: /* Return */
                        if (RawTrafficTextBox.Text.ToLower().CompareTo("cont") == 0)
                            mConnection.Running = true;
                        RawTrafficTextBox.Text += e.KeyChar;
                        SendCommandToDebugger();
                        break;
                    default:
                        RawTrafficTextBox.Text += e.KeyChar;
                        break;
                }
            }
        }

        private void RawTrafficText_MouseUp(object sender, MouseEventArgs e)
        {
            copyToolStripMenuItem.Enabled = (RawTrafficText.SelectionLength > 0);
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

        private void printDoc_PrintPage(object sender, PrintPageEventArgs e)
        {
            StringFormat stringFormat = new StringFormat();
            RectangleF rectFPaper, rectFText;
            int intChars, intLines;

            rectFPaper = e.MarginBounds;
            rectFText = RectangleF.Inflate(rectFPaper, 0,
                                -2 * RawTrafficText.Font.GetHeight(e.Graphics));
            int totalLines = (int)Math.Floor(rectFText.Height /
                                     RawTrafficText.Font.GetHeight(e.Graphics));
            rectFText.Height = totalLines *
                                      RawTrafficText.Font.GetHeight(e.Graphics);
            stringFormat.Trimming = StringTrimming.Word;

            while ((pageNumber < startPage) && (strPrintText.Length > 0))
            {
                e.Graphics.MeasureString(strPrintText, RawTrafficText.Font,
                                     rectFText.Size, stringFormat,
                                     out intChars, out intLines);
                strPrintText = strPrintText.Substring(intChars);
                pageNumber++;
            }

            // Cancel job when nothing to print is left
            if (strPrintText.Length == 0)
            {
                e.Cancel = true;
                return;
            }

            e.Graphics.DrawString(strPrintText, RawTrafficText.Font, Brushes.Black, rectFText, stringFormat);
            e.Graphics.MeasureString(strPrintText, RawTrafficText.Font,
                                     rectFText.Size, stringFormat,
                                     out intChars, out intLines);
            strPrintText = strPrintText.Substring(intChars);
            stringFormat = new StringFormat();

            // show filename in header
            stringFormat.Alignment = StringAlignment.Center;

            e.Graphics.DrawString(printDoc.DocumentName, RawTrafficText.Font,
                                  Brushes.Black, rectFPaper, stringFormat);

            // show page number in footer
            stringFormat.LineAlignment = StringAlignment.Far;

            e.Graphics.DrawString("Page " + pageNumber, RawTrafficText.Font,
                                 Brushes.Black, rectFPaper, stringFormat);

            pageNumber++;
            e.HasMorePages = (strPrintText.Length > 0) &&
                             (pageNumber < startPage + totalPages);

            //re-init
            if (!e.HasMorePages)
            {
                strPrintText = RawTrafficText.Text;
                startPage = 1;
                totalPages = pd.PrinterSettings.MaximumPage;
                pageNumber = 1;
            }

        }
        #endregion

    }
}
