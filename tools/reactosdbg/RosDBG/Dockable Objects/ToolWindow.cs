using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace RosDBG
{
    public partial class ToolWindow : DockContent
    {

        public enum Commands {Copy, Cut, Paste, Save, SaveAs, Print};

        public ToolWindow()
        {
            InitializeComponent();
        }

        public virtual void SaveAs(string FileName)
        {
            MessageBox.Show("Not implemented!");
        }

        public virtual void Save(string FileName)
        {
            SaveAs(FileName);
        }

        public virtual string GetDocumentName()
        {
            return "";
        }

        public virtual bool IsCmdEnabled(Commands Cmd)
        {
            return false;
        }

    }
}
