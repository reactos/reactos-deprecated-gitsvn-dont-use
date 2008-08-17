using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DebugProtocol;
using DbgHelpAPI;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class ProcThread : UserControl, IUseDebugConnection, IUseSymbols
    {
        DebugConnection mConnection;
        SymbolContext mSymcon;
        Dictionary<ulong, ProcessElement> mProcesses = new Dictionary<ulong,ProcessElement>();

        public ProcThread()
        {
            InitializeComponent();
        }

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            mConnection.DebugProcessThreadChangeEvent += DebugProcessThreadChangeEvent;
        }

        public void SetSymbolProvider(SymbolContext symcon)
        {
            mSymcon = symcon;
        }

        void DebugProcessThreadChangeEvent(object sender, DebugProcessThreadChangeEventArgs args)
        {
            mProcesses = args.Processes;
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "RefreshProcThreads"));
        }

        void RefreshProcThreads()
        {
            Processes.DataSource = new List<ProcessElement>(mProcesses.Values);
            foreach (ProcessElement pe in mProcesses.Values)
            {
                if (pe.Current)
                {
                    List<ThreadElement> telist = new List<ThreadElement>(mProcesses[pe.ProcessId].Threads.Values);
                    foreach (ThreadElement te in telist)
                    {
                        KeyValuePair<string, int> fileLine = mSymcon.GetFileAndLine(te.Eip);
                        te.Description = fileLine.Key + ":" + fileLine.Value;
                    }
                    Threads.DataSource = telist;
                }
            }
        }

        void DebugRunningChangeEvent(object sender, DebugRunningChangeEventArgs args)
        {
            if (!args.Running)
            {
                mConnection.Debugger.GetProcesses();
            }
        }

        private void Processes_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            mConnection.SetProcess(((ProcessElement)Processes.Rows[e.RowIndex].DataBoundItem).ProcessId);
        }

        private void Threads_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            mConnection.SetThread(((ThreadElement)Threads.Rows[e.RowIndex].DataBoundItem).ThreadId);
        }
    }
}
