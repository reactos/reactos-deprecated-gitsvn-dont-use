using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RosDBG
{
    public partial class HostWindow : Form
    {
        public HostWindow()
        {
            InitializeComponent();
        }

        public delegate void RedockControlEventHandler(object sender, RedockControlEventArgs args);
        public event RedockControlEventHandler RedockControlEvent;

        public Control Content
        {
            get 
            { 
                if (ContentSplitter.Panel2.Controls.Count > 0)
                    return ContentSplitter.Panel2.Controls[0];
                else
                    return null;
            }
            set
            {
                ContentSplitter.Panel2.Controls.Clear();
                if (value != null)
                    ContentSplitter.Panel2.Controls.Add(value);
            }
        }

        private void redockToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RedockControlEventArgs args = new RedockControlEventArgs(Content);
            Content = null;
            if (RedockControlEvent != null)
                RedockControlEvent(this, args);
            Close();
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }
    }

    public class RedockControlEventArgs : EventArgs
    {
        Control mControl;
        public Control Control { get { return mControl; } }
        public RedockControlEventArgs(Control ctrl) { mControl = ctrl; }
    }
}
