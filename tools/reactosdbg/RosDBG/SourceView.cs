using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RosDBG
{
    public partial class SourceView : UserControl
    {
        string mSourceFile;
        public string SourceFile
        {
            get { return mSourceFile; }
            set
            {
                mSourceFile = value;
                UpdateSourceCode();
            }
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            SourceCode.BackColor = Color.FromKnownColor(KnownColor.Window);
        }

        public void ScrollTo(int line)
        {
        }

        Dictionary<int, Color> mHighlightedLines = new Dictionary<int, Color>();
        public void ClearHighlight()
        {
            SourceCode.SelectAll();
            SourceCode.SelectionBackColor = Color.FromKnownColor(KnownColor.Window);
            SourceCode.SelectionColor = Color.FromKnownColor(KnownColor.WindowText);
        }

        public void AddHighlight(int line, Color backColor, Color foreColor)
        {
            SourceCode.SelectionStart = SourceCode.GetFirstCharIndexFromLine(line);
            SourceCode.SelectionLength = SourceCode.GetFirstCharIndexFromLine(line + 1) - SourceCode.SelectionStart;
            SourceCode.SelectionBackColor = backColor;
            SourceCode.SelectionColor = foreColor;
        }

        public void RemoveHighlight(int line)
        {
            AddHighlight(line, Color.FromKnownColor(KnownColor.Window), Color.FromKnownColor(KnownColor.WindowText));
        }

        public SourceView()
        {
            InitializeComponent();
        }

        public SourceView(string FileName)
        {
            InitializeComponent();
            this.Tag = FileName;
        }

        public void UpdateSourceCode()
        {
            SourceCode.Clear();
            ClearHighlight();

            if (mSourceFile == null) return;
            try
            {
                SourceCode.Lines = File.ReadAllLines(mSourceFile);
            }
            catch (IOException ex)
            {
                SourceCode.Text = "Could not load " + mSourceFile + ": " + ex.Message + "\n";
            }
        }

        private void SourceCode_SelectionChanged(object sender, EventArgs e)
        {
            btnCopy.Enabled = (SourceCode.SelectionLength > 0);
         }

        private void btnCopy_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(SourceCode.SelectedText);   
        }
    }
}
