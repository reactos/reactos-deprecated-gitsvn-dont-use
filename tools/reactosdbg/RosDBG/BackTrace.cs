using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Globalization;
using System.Windows.Forms;
using DbgHelpAPI;
using DebugProtocol;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class BackTrace : UserControl, IUseDebugConnection, IUseSymbols, IUseShell
    {
        DebugConnection mConnection;
        SymbolContext mSymbols;
        IShell mShell;
        ulong mSelectedAddr;
        ulong mCurrentEbp, mCurrentEip, mChaseEbpValue;
        bool mRunning = true, mInProgress;
        List<ulong> mStackFramesToAdd = new List<ulong>();

        public BackTrace()
        {
            InitializeComponent();
            this.Tag = "Backtrace";
        }

        public void SetShell(IShell shell)
        {
            mShell = shell;
            UpdateBackTrace();
        }

        void DumpStackFrames()
        {
            lock (mStackFramesToAdd)
            {
                foreach (ulong addr in mStackFramesToAdd)
                {
                    KeyValuePair<string, int> sourceLine = mSymbols.GetFileAndLine(addr);
                    if (sourceLine.Key != "unknown")
                        StackFrames.Items.Add(string.Format("{0:X8} {1}:{2}", addr, sourceLine.Key, sourceLine.Value));
                    else
                        StackFrames.Items.Add(string.Format("{0:X8} ?", addr));
                }
                mStackFramesToAdd.Clear();
            }
        }

        void TraceOneFrame(object dummy)
        {
            DebugMemoryStream mem = mConnection.NewMemoryStream();
            BinaryReader rdr = new BinaryReader(mem);
            try
            {
                mem.Seek((long)mChaseEbpValue, SeekOrigin.Begin);
                ulong chaseEbpValue = rdr.ReadUInt32();
                ulong returnAddr = rdr.ReadUInt32();
                lock (mStackFramesToAdd)
                {
                    mStackFramesToAdd.Add(returnAddr);
                }
                Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "DumpStackFrames"));
                if (chaseEbpValue != 0 && chaseEbpValue > mChaseEbpValue)
                {
                    mChaseEbpValue = chaseEbpValue;
                    ThreadPool.QueueUserWorkItem(TraceOneFrame);
                    return;
                }
            }
            catch (Exception)
            {
            }
            mInProgress = false;
        }

        void PerformBacktrace()
        {
            mInProgress = true;
            StackFrames.Items.Clear();
            mChaseEbpValue = mCurrentEbp;
            lock (mStackFramesToAdd)
            {
                mStackFramesToAdd.Add(mCurrentEip);
            }
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "DumpStackFrames"));
            ThreadPool.QueueUserWorkItem(TraceOneFrame);
        }

        public void UpdateBackTrace()
        {
            if (!(mConnection == null || mSymbols == null || mShell == null || mRunning || mInProgress))
            {
                mInProgress = true;
                Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "PerformBacktrace"));
            }
        }

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            mConnection.DebugRegisterChangeEvent += DebugRegisterChangeEvent;
            mConnection.DebugRunningChangeEvent += DebugRunningChangeEvent;
            mConnection.DebugModuleChangedEvent += DebugModuleChangedEvent;
            UpdateBackTrace();
        }

        void DebugModuleChangedEvent(object sender, DebugModuleChangedEventArgs args)
        {
            UpdateBackTrace();
        }

        void DebugRegisterChangeEvent(object sender, DebugRegisterChangeEventArgs args)
        {
            mCurrentEip = args.Registers.Eip;
            mCurrentEbp = args.Registers.Ebp;
            UpdateBackTrace();
        }

        void DebugRunningChangeEvent(object sender, DebugRunningChangeEventArgs args)
        {
            mRunning = args.Running;
        }

        public void SetSymbolProvider(SymbolContext context)
        {
            mSymbols = context;
            UpdateBackTrace();
        }

        private void StackFrames_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (StackFrames.SelectedItem != null) //crashed with argumentnullexception
            {
                string[] parsedEntry = ((string)StackFrames.SelectedItem).Split(new char[] { ' ' });
                mSelectedAddr = ulong.Parse(parsedEntry[0], NumberStyles.HexNumber);
            }
        }

        private void StackFrames_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            mShell.FocusAddress(mSelectedAddr);
        }
    }
}
