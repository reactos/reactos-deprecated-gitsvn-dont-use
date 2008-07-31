using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DebugProtocol;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class RawTraffic : UserControl, IUseDebugConnection
    {
        DebugConnection mConnection;
        List<string> textToAdd = new List<string>();

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

        public RawTraffic()
        {
            InitializeComponent();
        }

        private void RawTrafficText_KeyPress(object sender, KeyPressEventArgs e)
        {
            mConnection.Debugger.Write("" + e.KeyChar);
        }
    }
}
