using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using System.Threading;
using DebugProtocol;

namespace RosDBG
{
    [DebugControl]
    public partial class MemoryWindow : UserControl, IUseDebugConnection
    {
        ulong mAddress;
        DebugConnection mConnection;
        bool mRunning = true;
        Dictionary<ulong, ListViewItem> mStoredBytes = new Dictionary<ulong, ListViewItem>();

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            conn.DebugRunningChangeEvent += DebugRunningChangeEvent;
            mRunning = conn.Running;
        }

        void UpdateMemoryWindow()
        {
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "RefreshView"));
        }

        void RefreshView()
        {
            MemoryView.EnsureVisible(0);
            MemoryView.Refresh();
        }

        void DebugRunningChangeEvent(object sender, DebugRunningChangeEventArgs args)
        {
            ulong address;

            mRunning = args.Running;

            if (!mRunning && ulong.TryParse(MemoryAddress.Text, NumberStyles.HexNumber, null, out address))
            {
                mStoredBytes.Clear();
                mAddress = address & ~15UL;
                UpdateMemoryWindow();
            }
        }

        public MemoryWindow()
        {
            InitializeComponent();
        }

        private void MemoryView_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            ListViewItem theItem;
            int i;
            ulong toRead = mAddress + (ulong)(e.ItemIndex << 4);

            if (mStoredBytes.TryGetValue(toRead, out theItem))
            {
                e.Item = theItem;
                return;
            }

            StringBuilder resultName = new StringBuilder(string.Format("{0:X8}:", toRead));

            for (i = 0; i < 16; i++)
                resultName.Append(" ??");

            e.Item = new ListViewItem(resultName.ToString());
            mStoredBytes[toRead] = e.Item;
            ThreadPool.QueueUserWorkItem(UpdateRow, toRead);
        }

        void UpdateRow(object toReadObj)
        {
            ulong toRead = (ulong)toReadObj;
            DebugMemoryStream mem = mConnection.NewMemoryStream();
            StringBuilder resultName = new StringBuilder(string.Format("{0:X8}:", toRead));

            int width = 16;
            byte[] readBuf = new byte[width];

            mem.Seek((long)toRead, System.IO.SeekOrigin.Begin);

            int i, result = mRunning ? 0 : mem.Read(readBuf, 0, readBuf.Length);

            if (result == 0) return;

            for (i = 0; i < readBuf.Length; i++)
                resultName.Append(string.Format(" {0:X2}", (int)readBuf[i]));
            resultName.Append(" | ");
            for (i = 0; i < readBuf.Length; i++)
                resultName.Append((!char.IsControl((char)readBuf[i]) && (int)readBuf[i] < 128) ? (char)readBuf[i] : '\xb7');

            mStoredBytes[toRead] = new ListViewItem(resultName.ToString());
            UpdateMemoryWindow();
        }

        private void MemoryAddress_TextChanged(object sender, EventArgs e)
        {
            ulong address;
            if (ulong.TryParse(MemoryAddress.Text, NumberStyles.HexNumber, null, out address))
            {
                mAddress = address & ~15UL;
                UpdateMemoryWindow();
            }
        }
    }
}
