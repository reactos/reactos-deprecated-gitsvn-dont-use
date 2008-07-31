using System;
using System.IO;
using System.Text;
using System.Globalization;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace Gdb
{
    #region Event defs
    public class ConsoleOutputEventArgs : EventArgs
    {
        public readonly string Line;
        public ConsoleOutputEventArgs(string line)
        {
            Line = line;
        }
    }
    public delegate void ConsoleOutputEventHandler(object sender, ConsoleOutputEventArgs args);
    
    public class RegisterChangeEventArgs : EventArgs
    {
        public readonly List<long> Registers;
        public RegisterChangeEventArgs(IEnumerable<long> registers)
        {
            Registers = new List<long>(registers);
        }
    }
    public delegate void RegisterChangeEventHandler(object sender, RegisterChangeEventArgs args);
    
    public class SignalDeliveredEventArgs : EventArgs
    {
        public readonly int Signal;
        public SignalDeliveredEventArgs(int sig)
        {
            Signal = sig;
        }
    }
    public delegate void SignalDeliveredEventHandler(object sender, SignalDeliveredEventArgs args);
    
    public class RemoteGDBErrorArgs : EventArgs
    {
        public readonly int Error;
        public RemoteGDBErrorArgs(int error)
        {
            Error = error;
        }
    }
    public delegate void RemoteGDBErrorHandler(object sender, RemoteGDBErrorArgs args);
    
    public class MemoryUpdateEventArgs : EventArgs
    {
        public readonly long Address;
        public readonly byte []Memory;
        public MemoryUpdateEventArgs(long address, byte []memory)
        {
            Address = address;
            Memory = memory;
        }
    }
    public delegate void MemoryUpdateEventHandler(object sender, MemoryUpdateEventArgs args);
    
    public class ModuleListEventArgs : EventArgs
    {
        public readonly long Address;
        public readonly string Module;
        public ModuleListEventArgs(string module, long address)
        {
            Module = module;
            Address = address;
        }
    }
    public delegate void ModuleListEventHandler(object sender, ModuleListEventArgs args);
    #endregion

    public class Gdb : GdbProtocol
    {
        string []X86RegisterNames = new string[] {
            "eax",   "ecx",    "edx",   "ebx",
            "esp",   "ebp",    "esi",   "edi",
            "eip",   "eflags", "cs",    "ss",
            "ds",    "es",     "fs",    "gs",
            "st0",   "st1",    "st2",   "st3",
            "st4",   "st5",    "st6",   "st7",
            "fctrl", "fstat",  "ftag",  "fiseg",
            "fioff", "foseg",  "fooff", "fop",
            "xmm0",  "xmm1",   "xmm2",  "xmm3",
            "xmm4",  "xmm5",   "xmm6",  "xmm7",
            "mxcsr"            
        };

        enum ExpectedReplyPacket
        {
            gPacketReply,
            pPacketReply,
            SuccessOrFailure
        }

        bool mRunning;
        public bool Running { get { return mRunning; } }

        int mSignal;
        long []mRegisters = new long[32];

        #region Memory updates
        public class MemoryBlock
        {
            public readonly long Address;
            public readonly byte[] Block;
            public MemoryBlock(long address)
            {
                Address = address;
                Block = new byte[mMemoryBlockSize];
            }
        }

        long mNextMemoryResult;
        static int mMemoryBlockSize = 0x100;
        Dictionary<long, MemoryBlock> mMemState = new Dictionary<long, MemoryBlock>();

        public MemoryBlock GetMemoryBlock(long addr)
        {
            MemoryBlock result;
            addr &= mMemoryBlockSize - 1;
            if (!mMemState.TryGetValue(addr, out result))
            {
                result = new MemoryBlock(addr);
                mMemState[addr] = result;
            }
            return result;
        }
        #endregion

        #region Breakpoints
        public class Breakpoint
        {
            public enum BPType { Software, Hardware, WriteWatch, ReadWatch, AccessWatch };
            public readonly BPType BreakpointType;
            public readonly long Address;
            public readonly int Length;
            public Breakpoint(BPType type, long addr, int len)
            {
                BreakpointType = type;
                Address = addr;
                Length = len;
            }

            public override int GetHashCode()
            {
                return (int)(((int)BreakpointType) ^ Address ^ (Length << 28));
            }

            public override bool Equals(object other)
            {
                Breakpoint otherbp = other as Breakpoint;
                if (otherbp == null) return false;
                return 
                    (otherbp.BreakpointType == BreakpointType) &&
                    (otherbp.Address == Address) &&
                    (otherbp.Length == Length);
            }
        }

        Dictionary<Breakpoint,Breakpoint> mBreakpoints = new Dictionary<Breakpoint,Breakpoint>();

        public void SetBreakpoint(Breakpoint bp)
        {
            if (!mBreakpoints.ContainsKey(bp))
            {
                mBreakpoints[bp] = bp;
                SendBPCommand('Z', bp);
            }
        }

        public void RemoveBreakpoint(Breakpoint bp)
        {
            mBreakpoints.Remove(bp);
            SendBPCommand('z', bp);
        }

        void SendBPCommand(char ch, Breakpoint bp)
        {
            SendMessage
                (string.Format
                 ("{0}{1},{2:X},{3:X}",
                  ch, (int)bp.BreakpointType, bp.Address, bp.Length));
        }
        #endregion

        #region Events
        public event ConsoleOutputEventHandler ConsoleOutputEvent;
        public event RegisterChangeEventHandler RegisterChangeEvent;
        public event SignalDeliveredEventHandler SignalDeliveredEvent;
        public event RemoteGDBErrorHandler RemoteGDBError;
        public event MemoryUpdateEventHandler MemoryUpdateEvent;
        public event ModuleListEventHandler ModuleListEvent;
        #endregion

        #region Packet receive
        public virtual void EmptyReply()
        {
        }

        string hexdig = "[0-9a-fA-F]";
        Regex mAllHex = new Regex("[0-9a-fA-F]+");
        Regex mModuleLoaded = new Regex("M(?<sectaddr>[0-9a-fA-F]+),(?<modname>.+)");

        protected override void ReceivePacket(string payload)
        {
            // An empty reply can be a handshake response; treat accordingly
            if (payload.Length == 0)
            {
                EmptyReply();
                return;
            }
            else if (payload == "OK")
            {
                // Maybe have an OK event?
                return;
            }
            // Output doesn't change state
            else if (payload[0] == 'O')
            {
                if (ConsoleOutputEvent != null)
                    ConsoleOutputEvent(this, new ConsoleOutputEventArgs(payload.Substring(1)));
                return;
            }
            else if (payload[0] == 'S')
            {
                if (SignalDeliveredEvent != null)
                    SignalDeliveredEvent(this, new SignalDeliveredEventArgs(int.Parse(payload.Substring(1,2), NumberStyles.HexNumber)));
                return;
            }
            else if (payload[0] == 'E')
            {
                if (RemoteGDBError != null)
                    RemoteGDBError(this, new RemoteGDBErrorArgs(int.Parse(payload.Substring(1,2), NumberStyles.HexNumber)));
                return;
            }
            else if (payload[0] == 'M')
            {
                Match modmatch = mModuleLoaded.Match(payload);
                Console.WriteLine("Module [" + payload + "]");
                if (modmatch.Success)
                {
                    if (ModuleListEvent != null)
                        ModuleListEvent
                            (this, 
                             new ModuleListEventArgs
                             (modmatch.Groups["modname"].ToString(),
                              int.Parse
                              (modmatch.Groups["sectaddr"].ToString(), 
                               NumberStyles.HexNumber)));
                }
            }
            else if (payload[0] == 'R')
            {
                int i;

                Console.WriteLine("Got Register Update");

                for (i = 1; i < payload.Length; i += 8)
                {
                    string regdata = 
                        payload.Substring(i+6,2) + payload.Substring(i+4,2) + 
                        payload.Substring(i+2,2) + payload.Substring(i,2);
                    mRegisters[i>>3] = long.Parse(regdata, NumberStyles.HexNumber);
                }
                if (RegisterChangeEvent != null)
                    RegisterChangeEvent(this, new RegisterChangeEventArgs(mRegisters));
            }
            else if (payload[0] == 'X')
            {
                int i;
                MemoryBlock mb = GetMemoryBlock(mNextMemoryResult);
                int blockOffset = (int)(mNextMemoryResult & (mMemoryBlockSize - 1));
                for (i = 0; i < payload.Length; i += 2)
                {
                    int by = int.Parse(payload.Substring(i,2), NumberStyles.HexNumber);
                    mb.Block[blockOffset++] = (byte)by;

                    if (blockOffset == mMemoryBlockSize)
                    {
                        if (MemoryUpdateEvent != null)
                            MemoryUpdateEvent(this, new MemoryUpdateEventArgs(mb.Address, mb.Block));
                        mNextMemoryResult += blockOffset;
                        blockOffset = 0;
                        mb = GetMemoryBlock(mNextMemoryResult);
                    }
                }
                if (blockOffset != 0)
                {
                    if (MemoryUpdateEvent != null)
                        MemoryUpdateEvent(this, new MemoryUpdateEventArgs(mb.Address, mb.Block));
                }
            }
        }
        #endregion

        #region Actions on the target
        public void GetRegisterUpdate()
        {
            SendMessage("g");
        }

        public void GetMemoryUpdate(long address, int len)
        {
            SendMessage
                (string.Format
                 ("m{0:X},{1:X}", address, len));
        }

        public void WriteMemory(long address, byte[] buf)
        {
            StringBuilder sb = new StringBuilder();
            foreach (byte b in buf)
                sb.Append(string.Format("{0:X2}", b));
            SendMessage
                (string.Format
                 ("M{0:X},{1:X}:{2}", address, buf.Length, sb.ToString()));
        }

        public void Step()
        {
            SendMessage("s");
        }

        public void Break()
        {
            SendBreak();
        }

        public void Go(long address)
        {
            if (address != 0)
                SendMessage(string.Format("c{0:X2}", address));
            else
                SendMessage("c");
        }
        #endregion

        public Gdb(Pipe p) : base(p) { }
    }
}
