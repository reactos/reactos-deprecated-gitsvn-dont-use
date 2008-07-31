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
    [DebugControl]
    public partial class Modules : UserControl, IUseDebugConnection
    {
        public SortedList<uint, string> mModules = new SortedList<uint, string>();

        public void SetDebugConnection(DebugConnection conn)
        {
            conn.DebugModuleChangedEvent += DebugModuleChangedEvent;
            if (!conn.Running)
                conn.Debugger.GetModuleUpdate();
        }

        void UpdateList()
        {
            lock (mModules)
            {
                ModulesList.VirtualListSize = mModules.Count;
                ModulesList.Refresh();
            }
        }

        void DebugModuleChangedEvent(object sender, DebugModuleChangedEventArgs args)
        {
            lock (mModules)
            {
                if (args.ModuleName.ToLower() == "ntoskrnl.exe")
                    mModules.Clear();
                mModules[args.ModuleAddr] = args.ModuleName;
            }
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateList"));
        }

        public Modules()
        {
            InitializeComponent();
        }

        private void ModulesList_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            KeyValuePair<uint, string> modent = mModules.ElementAt(e.ItemIndex);
            e.Item = new ListViewItem(new string[] { modent.Key.ToString("X8"), modent.Value });
        }
    }
}
