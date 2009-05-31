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
        PrintDialog pd;
        PrintDocument printDoc;

        DebugConnection mConnection;
        List<string> textToAdd = new List<string>();

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
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

        public RawTraffic()
        {
            InitializeComponent();
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

    }
}
