using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO.Ports;
using AbstractPipe;
using DebugProtocol;
using KDBGProtocol;

namespace DebugProtocol
{
    public class DebugConnectedEventArgs : EventArgs
    {
        public DebugConnectedEventArgs()
        {
        }
    }

    public delegate void DebugConnectedEventHandler(object sender, DebugConnectedEventArgs args);

    public class DebugConnectionModeChangedEventArgs : EventArgs
    {
        public readonly DebugConnection.Mode Mode;
        public DebugConnectionModeChangedEventArgs(DebugConnection.Mode mode)
        {
            Mode = mode;
        }
    }
    public delegate void DebugConnectionModeChangedEventHandler(object sender, DebugConnectionModeChangedEventArgs args);

    public class DebugRegisterChangeEventArgs : EventArgs
    {
        public readonly Registers Registers;
        public DebugRegisterChangeEventArgs(Registers regs)
        {
            Registers = regs;
        }
    }
    public delegate void DebugRegisterChangeEventHandler(object sender, DebugRegisterChangeEventArgs args);

    public class DebugRunningChangeEventArgs : EventArgs
    {
        public readonly bool Running;
        public DebugRunningChangeEventArgs(bool running) { Running = running; }
    }
    public delegate void DebugRunningChangeEventHandler(object sender, DebugRunningChangeEventArgs args);

    public class DebugModuleChangedEventArgs : EventArgs
    {
        public readonly uint ModuleAddr;
        public readonly string ModuleName;
        public DebugModuleChangedEventArgs(uint moduleAddr, string moduleName)
        {
            ModuleAddr = moduleAddr;
            ModuleName = moduleName;
        }
    }
    public delegate void DebugModuleChangedEventHandler(object sender, DebugModuleChangedEventArgs args);

    public class DebugRawTrafficEventArgs : EventArgs
    {
        public readonly string Data;
        public DebugRawTrafficEventArgs(string data) { Data = data; }
    }
    public delegate void DebugRawTrafficEventHandler(object sender, DebugRawTrafficEventArgs args);

    public class ThreadElement
    {
        bool mCurrent;
        public bool Current { get { return mCurrent; } set { mCurrent = value; } }
        ulong mTid;
        public ulong ThreadId { get { return mTid; } set { mTid = value; } }
        ulong mEip;
        public ulong Eip { get { return mEip; } set { mEip = value; } }
        string mDescription;
        public string Description { get { return mDescription; } set { mDescription = value; } }
        public ThreadElement(ulong tid, bool current, ulong eip) { mTid = tid; mCurrent = current; mEip = eip; }
    }

    public class ProcessElement
    {
        bool mCurrent;
        public bool Current { get { return mCurrent; } set { mCurrent = value; } }
        ulong mPid;
        public ulong ProcessId { get { return mPid; } set { mPid = value; } }
        public Dictionary<ulong, ThreadElement> Threads = new Dictionary<ulong,ThreadElement>();
        public ProcessElement(ulong pid, bool current) { mPid = pid; mCurrent = current; }
    }

    public class DebugProcessThreadChangeEventArgs : EventArgs
    {
        public readonly Dictionary<ulong, ProcessElement> Processes;
        public DebugProcessThreadChangeEventArgs(Dictionary<ulong, ProcessElement> processes)
        {
            Processes = processes;
        }
    }
    public delegate void DebugProcessThreadChangeEventHandler(object sender, DebugProcessThreadChangeEventArgs args);

    public class DebugConnection
    {
        #region Primary State
        public enum Mode { ClosedMode, SocketMode, SerialMode, PipeMode }
        public Mode ConnectionMode
        {
            get { return mConnectionMode; }
            set
            {
                mConnectionMode = value;
                if (DebugConnectionModeChangedEvent != null)
                    DebugConnectionModeChangedEvent(this, new DebugConnectionModeChangedEventArgs(value));
            }
        }

        bool mRunning = true;
        public bool Running
        {
            get { return mRunning; }
            set
            {
                mRunning = value;
                if (DebugRunningChangeEvent != null)
                    DebugRunningChangeEvent(this, new DebugRunningChangeEventArgs(value));
            }
        }

        ulong mCurrentProcess;
        public ulong CurrentProcess
        {
            get { return mCurrentProcess; }
            set { mKdb.SetProcess(value); }
        }

        ulong mCurrentThread;
        public ulong CurrentThread
        {
            get { return mCurrentThread; }
            set { mKdb.SetThread(value); }
        }

        KDBG mKdb;
        Registers mRegisters = new Registers();
        ulong mNewCurrentThread, mNewCurrentProcess;
        public IDebugProtocol Debugger { get { return mKdb; } }
        Pipe mMedium;
        Mode mConnectionMode;
        List<WeakReference> mMemoryReaders = new List<WeakReference>();
        Dictionary<ulong, ProcessElement> mProcesses = new Dictionary<ulong, ProcessElement>();
        Dictionary<ulong, ProcessElement> mAccumulateProcesses = new Dictionary<ulong, ProcessElement>();
        #endregion

        #region Socket Mode State
        int mRemotePort;
        Socket mSocket;
        SocketAsyncEventArgs mAsyncConnect;
        AsyncCallback mDnsLookup;
        IAsyncResult mDnsAsyncResult;
        #endregion

        #region Serial Mode Members
        SerialPort mSerialPort;
        #endregion

        #region Named Pipe Members
        NamedPipe mNamedPipe;
        #endregion

        public event DebugRegisterChangeEventHandler DebugRegisterChangeEvent;
        public event DebugConnectedEventHandler DebugConnectionConnectedEvent;
        public event DebugConnectionModeChangedEventHandler DebugConnectionModeChangedEvent;
        public event DebugRunningChangeEventHandler DebugRunningChangeEvent;
        public event DebugModuleChangedEventHandler DebugModuleChangedEvent;
        public event DebugProcessThreadChangeEventHandler DebugProcessThreadChangeEvent;
        public event DebugRawTrafficEventHandler DebugRawTrafficEvent;

        public DebugConnection()
        {
        }

        void DnsLookupResult(IAsyncResult result)
        {
            if (result.IsCompleted)
            {
                IPHostEntry entry = Dns.EndGetHostEntry(mDnsAsyncResult);
                if (entry.AddressList.Length > 0)
                {
                    int i;
                    // Check for an ipv4 target
                    for (i = 0; i < entry.AddressList.Length; i++)
                        if (entry.AddressList[i].AddressFamily == AddressFamily.InterNetwork)
                            break;
                    // Otherwise just fall back to the first one
                    if (i == entry.AddressList.Length) i = 0;
                    mAsyncConnect.RemoteEndPoint = new IPEndPoint(entry.AddressList[i], mRemotePort);
                    mSocket.ConnectAsync(mAsyncConnect);
                }
                else
                {
                    Close();
                }
            }
        }

        void ConnectEventHandlers()
        {
            //set up tab handlers
            mKdb.RegisterChangeEvent += RegisterChangeEvent;
            mKdb.ModuleListEvent += ModuleListEvent;
            mKdb.MemoryUpdateEvent += MemoryUpdateEvent;
            mKdb.ProcessListEvent += ProcessListEvent;
            mKdb.ThreadListEvent += ThreadListEvent;
        }

        public void Start(string host, int port)
        {
            Close();
            mRemotePort = port;
            ConnectionMode = Mode.SocketMode;
            mSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            mAsyncConnect = new SocketAsyncEventArgs();
            mAsyncConnect.UserToken = this;
            mAsyncConnect.Completed += SocketConnectCompleted;
            mDnsLookup = new AsyncCallback(DnsLookupResult);
            mDnsAsyncResult = Dns.BeginGetHostEntry(host, mDnsLookup, this);
        }

        public void Start(string pipeName)
        {
            Close();
            ConnectionMode = Mode.PipeMode;
            mNamedPipe = new NamedPipe();
            mNamedPipe.Create(pipeName);
            mNamedPipe.Listen();
        }

        public void Start(int baudrate, string port)
        {
            Close();
            ConnectionMode = Mode.SerialMode;
            mSerialPort = new SerialPort(port, baudrate, Parity.None, 8, StopBits.One);
            mSerialPort.Handshake = Handshake.None;
            try
            {
                //opening could fail for occupied, not existing or misconfigured ports
                mSerialPort.Open();
                //create pipe and kdb instances, connect internal receive pipe 
                mMedium = new SerialPipe(mSerialPort);
                mMedium.PipeReceiveEvent += PipeReceiveEvent;
                mKdb = new KDBG(mMedium);
                ConnectEventHandlers();
                Running = true;
            }
            catch (Exception)
            {
                ConnectionMode = Mode.ClosedMode;
                //error signal?
            }
        }

        void ProcessListEvent(object sender, ProcessListEventArgs args)
        {
            if (args.Reset)
            {
                mAccumulateProcesses.Clear();
            }
            else if (args.End)
            {
                mKdb.GetThreads(mNewCurrentProcess);
            }
            else
                mAccumulateProcesses[args.Pid] = new ProcessElement(args.Pid, args.Current);

            if (args.Current)
                mNewCurrentProcess = args.Pid;
        }

        void ThreadListEvent(object sender, ThreadListEventArgs args)
        {
            if (args.Reset)
            {
                mAccumulateProcesses[mNewCurrentProcess].Threads.Clear();
            }
            else if (args.End)
            {
                mCurrentProcess = mNewCurrentProcess;
                mCurrentThread = mNewCurrentThread;
                mProcesses = mAccumulateProcesses;
                if (DebugProcessThreadChangeEvent != null)
                    DebugProcessThreadChangeEvent(this, new DebugProcessThreadChangeEventArgs(mProcesses));
            }
            else
            {
                mAccumulateProcesses[mNewCurrentProcess].Threads[args.Tid] = new ThreadElement(args.Tid, args.Current, args.Eip);
            }

            if (args.Current)
                mNewCurrentThread = args.Tid;
        }

        public void Close()
        {
            switch (ConnectionMode)
            {
                case Mode.SocketMode:
                    mSocket.Close();
                    mSocket = null;
                    break;
                case Mode.SerialMode:
                    mSerialPort.Close();
                    mSerialPort = null;
                    Running = false;
                    break;
                case Mode.PipeMode:
                    mNamedPipe.Close();
                    mNamedPipe = null;
                    Running = false;
                    break;
            }

            mMedium = null;
            if (mKdb != null)
                mKdb.Close();
            mKdb = null;
            ConnectionMode = Mode.ClosedMode;
        }

        void MediumError(object sender, PipeErrorEventArgs args)
        {
            Close();
        }

        void SocketConnectCompleted(object sender, SocketAsyncEventArgs e)
        {
            if (mAsyncConnect.SocketError == SocketError.Success)
            {
                mMedium = new SocketPipe(mSocket);
                mMedium.PipeErrorEvent += MediumError;
                mMedium.PipeReceiveEvent += PipeReceiveEvent;
                mKdb = new KDBG(mMedium);
                ConnectEventHandlers();
                Running = true;
                if (DebugConnectionConnectedEvent != null)
                    DebugConnectionConnectedEvent(this, new DebugConnectedEventArgs());
            }
            else
                Close();
        }

        void MemoryUpdateEvent(object sender, MemoryUpdateEventArgs args)
        {
            List<WeakReference> deadStreams = new List<WeakReference>();

            lock (mMemoryReaders)
            {
                foreach (WeakReference memStreamRef in mMemoryReaders)
                {
                    DebugMemoryStream memStream = memStreamRef.Target as DebugMemoryStream;

                    if (memStream == null)
                    {
                        deadStreams.Add(memStreamRef);
                        continue;
                    }

                    memStream.Update(args.Address, args.Memory);
                }

                foreach (WeakReference deadref in deadStreams)
                    mMemoryReaders.Remove(deadref);
            }
        }

        void PipeReceiveEvent(object sender, PipeReceiveEventArgs args)
        {
            if (DebugRawTrafficEvent != null)
                DebugRawTrafficEvent(this, new DebugRawTrafficEventArgs(args.Received));
        }

        void ModuleListEvent(object sender, ModuleListEventArgs args)
        {
            if (DebugModuleChangedEvent != null)
                DebugModuleChangedEvent(this, new DebugModuleChangedEventArgs((uint)args.Address, args.Module));
        }

        void RegisterChangeEvent(object sender, RegisterChangeEventArgs args)
        {
            args.Registers.CopyTo(mRegisters.RegisterSet);
            Running = false;
            if (DebugRegisterChangeEvent != null)
                DebugRegisterChangeEvent(this, new DebugRegisterChangeEventArgs(mRegisters));
        }

        public void Break()
        {
            if (mKdb != null)
                mKdb.Break();
        }

        public void Step()
        {
            Running = true;
            if (mKdb != null) mKdb.Step();
            Running = false;
        }

        public void Go()
        {
            if (mKdb != null)
            {
                mKdb.Go(0);
                Running = true;
            }
        }

        public void SetProcess(ulong pid)
        {
            mKdb.SetProcess(pid);
        }

        public void SetThread(ulong tid)
        {
            mKdb.SetThread(tid);
        }

        public DebugMemoryStream NewMemoryStream()
        {
            lock (mMemoryReaders)
            {
                DebugMemoryStream dms = new DebugMemoryStream(this);
                mMemoryReaders.Add(new WeakReference(dms));
                return dms;
            }
        }
    }
}
