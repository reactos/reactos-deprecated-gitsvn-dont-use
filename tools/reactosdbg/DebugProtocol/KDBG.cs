using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Globalization;
using AbstractPipe;
using DebugProtocol;

namespace KDBGProtocol
{
    public class KDBG : IDebugProtocol
    {
        bool mRunning = true;
        Pipe mConnection;
        List<string> mCommandBuffer = new List<string>();
        Dictionary<string, ulong> mModuleList = new Dictionary<string, ulong>();
        ulong []mRegisters = new ulong[32];

        public KDBG(Pipe connection) 
        { 
            mConnection = connection;
            mConnection.PipeReceiveEvent += PipeReceiveEvent;
            mConnection.PipeErrorEvent += PipeErrorEvent;
        }

        void PipeErrorEvent(object sender, PipeErrorEventArgs args)
        {
        }

        static Regex mMemoryRowUpdate = new Regex("<(?<addr>[^>]+)>: (?<row>[0-9a-fA-F? ]*)");
        static Regex mModuleOffset = new Regex("(?<modname>[^:]+):(?<offset>[0-9a-fA-f]+).*");
        static Regex mModuleUpdate = new Regex("(?<base>[0-9a-fA-F]+)  (?<size>[0-9a-fA-F]+)  (?<modname>\\w+\\.\\w+)");
        static Regex mRegLineCS_EIP = new Regex("CS:EIP  0x(?<cs>[0-9a-fA-F]+):0x(?<eip>[0-9a-fA-F]+).*");
        static Regex mRegLineSS_ESP = new Regex("SS:ESP  0x(?<ss>[0-9a-fA-F]+):0x(?<esp>[0-9a-fA-F]+).*");
        static Regex mRegLineEAX_EBX = new Regex("EAX  0x(?<eax>[0-9a-fA-F]+)[ \t]+EBX  0x(?<ebx>[0-9a-fA-F]+).*");
        static Regex mRegLineECX_EDX = new Regex("ECX  0x(?<ecx>[0-9a-fA-F]+)[ \t]+EDX  0x(?<edx>[0-9a-fA-F]+).*");
        static Regex mRegLineEBP = new Regex("EBP  0x(?<ebp>[0-9a-fA-F]+).*");
        static Regex mRegLineEFLAGS = new Regex("EFLAGS  0x(?<eflags>[0-9a-fA-F]+).*");
        static Regex mSregLine = new Regex("[CDEFGS]S  0x(?<seg>[0-9a-fA-F]+).*");
        static Regex mProcListHeading = new Regex("PID[ \\t]+State[ \\t]+Filename.*");
        static Regex mThreadListHeading = new Regex("TID[ \\t]+State[ \\t]+Prio.*");
        static Regex mProcListEntry = new Regex("^(?<cur>([*]|))0x(?<pid>[0-9a-fA-F]+)[ \\t]+(?<state>.*)");
        static Regex mThreadListEntry = new Regex("^(?<cur>([*]|))0x(?<tid>[0-9a-fA-F]+)[ \\t]+(?<state>.*)0x(?<eip>[0-9a-fA-F]*)");

        bool mFirstModuleUpdate = false;
        bool mReceivingProcs = false;
        bool mReceivingThreads = false;
        StringBuilder mInputBuffer = new StringBuilder();
        int mUsedInput;

        void PipeReceiveEvent(object sender, PipeReceiveEventArgs args)
        {
            bool tookText = false;
            string inbufStr;
            int promptIdx;

            mInputBuffer.Append(args.Received);

            if (mInputBuffer.ToString().Substring(mUsedInput).Contains("key to continue"))
            {
                mConnection.Write("\r");
                mUsedInput = mInputBuffer.Length;
            }

            while ((promptIdx = (inbufStr = mInputBuffer.ToString()).IndexOf("kdb:> ")) != -1)
            {
                string pretext = inbufStr.Substring(0, promptIdx);
                string[] theInput = pretext.Split(new char[] { '\n' });
                int remove = pretext.Length + "kdb:> ".Length;

                mInputBuffer.Remove(0, remove);
                mUsedInput = Math.Max(0, mUsedInput - remove);

                if (!tookText)
                {
                    if (mRunning)
                    {
                        mRunning = false;
                        GetRegisterUpdate();
                        GetProcesses();
                    }
                    tookText = true;
                }

                foreach (string line in theInput)
                {
                    string cleanedLine = line.Trim();
                    try 
                    {
                        if (cleanedLine.StartsWith("Entered debugger on "))
                        {
                            mReceivingProcs = false;
                            mReceivingThreads = false;
                            GetRegisterUpdate();
                            GetProcesses();
                            continue;
                        }

                        if (!mFirstModuleUpdate)
                        {
                            GetModuleUpdate();
                            mFirstModuleUpdate = true;
                        }

                        Match memoryMatch = mMemoryRowUpdate.Match(cleanedLine);
                        if (memoryMatch.Success)
                        {
                            string addrStr = memoryMatch.Groups["addr"].ToString();
                            Match modOffset = mModuleOffset.Match(addrStr);
                            ulong updateAddress;
                            if (modOffset.Success)
                            {
                                string modname = modOffset.Groups["modname"].ToString();
                                ulong offset = ulong.Parse(modOffset.Groups["offset"].ToString(), NumberStyles.HexNumber);
                                ulong modbase;
                                if (mModuleList.TryGetValue(modname.ToUpper(), out modbase))
                                    updateAddress = modbase + offset;
                                else
                                    continue; // Couldn't resolve the address of the named module ...
                            }
                            else
                            {
                                updateAddress = ulong.Parse(addrStr, NumberStyles.HexNumber);
                            }
                            string[] memWords = memoryMatch.Groups["row"].ToString().Split(new char[] { ' ' });
                            byte []updateBytes = new byte[4 * memWords.Length];
                            int ctr = 0;
                            foreach (string word in memWords)
                            {
                                if (word[0] == '?')
                                {
                                    if (MemoryUpdateEvent != null)
                                        MemoryUpdateEvent(this, new MemoryUpdateEventArgs((updateAddress & ~0xfffUL), null));
                                }
                                else
                                {
                                    int wordParsed = int.Parse(word, NumberStyles.HexNumber);
                                    int curCtr = ctr;
                                    for (ctr = curCtr; ctr < curCtr + 4; ctr++)
                                    {
                                        updateBytes[ctr] = (byte)(wordParsed & 0xff);
                                        wordParsed >>= 8;
                                    }
                                }
                            }
                            if (MemoryUpdateEvent != null)
                                MemoryUpdateEvent(this, new MemoryUpdateEventArgs(updateAddress, updateBytes));

                            continue;
                        }

                        Match moduleMatch = mModuleUpdate.Match(cleanedLine);
                        if (moduleMatch.Success)
                        {
                            ulong baseAddress = ulong.Parse(moduleMatch.Groups["base"].ToString(), NumberStyles.HexNumber);
                            uint moduleSize = uint.Parse(moduleMatch.Groups["size"].ToString(), NumberStyles.HexNumber);
                            string moduleName = moduleMatch.Groups["modname"].ToString();
                            mModuleList[moduleName.ToUpper()] = baseAddress;
                            if (ModuleListEvent != null)
                                ModuleListEvent(this, new ModuleListEventArgs(moduleName, baseAddress));
                            continue;
                        }

                        Match csEipMatch = mRegLineCS_EIP.Match(cleanedLine);
                        if (csEipMatch.Success)
                        {
                            uint cs = uint.Parse(csEipMatch.Groups["cs"].ToString(), NumberStyles.HexNumber);
                            ulong eip = ulong.Parse(csEipMatch.Groups["eip"].ToString(), NumberStyles.HexNumber);
                            mRegisters[8] = eip;
                            mRegisters[10] = cs;
                            continue;
                        }

                        Match ssEspMatch = mRegLineSS_ESP.Match(cleanedLine);
                        if (ssEspMatch.Success)
                        {
                            uint ss = uint.Parse(ssEspMatch.Groups["ss"].ToString(), NumberStyles.HexNumber);
                            ulong esp = ulong.Parse(ssEspMatch.Groups["esp"].ToString(), NumberStyles.HexNumber);
                            mRegisters[4] = esp;
                            mRegisters[15] = ss;
                            continue;
                        }

                        Match eaxEbxMatch = mRegLineEAX_EBX.Match(cleanedLine);
                        if (eaxEbxMatch.Success)
                        {
                            ulong eax = ulong.Parse(eaxEbxMatch.Groups["eax"].ToString(), NumberStyles.HexNumber);
                            ulong ebx = ulong.Parse(eaxEbxMatch.Groups["ebx"].ToString(), NumberStyles.HexNumber);
                            mRegisters[0] = eax;
                            mRegisters[3] = ebx;
                            continue;
                        }

                        Match ecxEdxMatch = mRegLineECX_EDX.Match(cleanedLine);
                        if (ecxEdxMatch.Success)
                        {
                            ulong ecx = ulong.Parse(ecxEdxMatch.Groups["ecx"].ToString(), NumberStyles.HexNumber);
                            ulong edx = ulong.Parse(ecxEdxMatch.Groups["edx"].ToString(), NumberStyles.HexNumber);
                            mRegisters[1] = ecx;
                            mRegisters[2] = edx;
                            continue;
                        }

                        Match ebpMatch = mRegLineEBP.Match(cleanedLine);
                        if (ebpMatch.Success)
                        {
                            ulong ebp = ulong.Parse(ebpMatch.Groups["ebp"].ToString(), NumberStyles.HexNumber);
                            mRegisters[5] = ebp;
                            continue;
                        }

                        Match eflagsMatch = mRegLineEFLAGS.Match(cleanedLine);
                        if (eflagsMatch.Success)
                        {
                            ulong eflags = ulong.Parse(eflagsMatch.Groups["eflags"].ToString(), NumberStyles.HexNumber);
                            mRegisters[9] = eflags;
                            if (RegisterChangeEvent != null)
                                RegisterChangeEvent(this, new RegisterChangeEventArgs(mRegisters));
                            continue;
                        }

                        Match sregMatch = mSregLine.Match(cleanedLine);
                        if (sregMatch.Success)
                        {
                            char []segmap = new char[] { 'C','D','E','F','G','S' };
                            uint sreg = uint.Parse(sregMatch.Groups["seg"].ToString(), NumberStyles.HexNumber);
                            int findSeg;
                            for (findSeg = 0; findSeg < segmap.Length; findSeg++)
                            {
                                if (segmap[findSeg] == cleanedLine[0])
                                {
                                    mRegisters[10 + findSeg] = sreg;
                                    if (segmap[findSeg] == 'S' && RegisterChangeEvent != null)
                                        RegisterChangeEvent(this, new RegisterChangeEventArgs(mRegisters));
                                    break;
                                }
                            }
                            continue;
                        }

                        Match pidHeadMatch = mProcListHeading.Match(cleanedLine);
                        if (pidHeadMatch.Success)
                        {
                            mReceivingThreads = false;
                            mReceivingProcs = true;
                            if (ProcessListEvent != null)
                                ProcessListEvent(this, new ProcessListEventArgs());
                            continue;
                        }
                        else
                        {
                            Match pidEntryMatch = mProcListEntry.Match(cleanedLine);
                            if (pidEntryMatch.Success && mReceivingProcs)
                            {
                                if (ProcessListEvent != null)
                                    ProcessListEvent(this, new ProcessListEventArgs(ulong.Parse(pidEntryMatch.Groups["pid"].ToString(), NumberStyles.HexNumber), pidEntryMatch.Groups["cur"].Length > 0));
                            }
                            else
                            {
                                if ((mReceivingProcs || cleanedLine.Contains("No processes")) && ProcessListEvent != null)
                                    ProcessListEvent(this, new ProcessListEventArgs(true));
                            }
                        }

                        Match tidHeadMatch = mThreadListHeading.Match(cleanedLine);
                        if (tidHeadMatch.Success)
                        {
                            mReceivingThreads = true;
                            mReceivingProcs = false;
                            if (ThreadListEvent != null)
                                ThreadListEvent(this, new ThreadListEventArgs());
                            continue;
                        }
                        else
                        {
                            Match tidEntryMatch = mThreadListEntry.Match(cleanedLine);
                            if (tidEntryMatch.Success && mReceivingThreads)
                            {
                                if (ThreadListEvent != null)
                                    ThreadListEvent(this, new ThreadListEventArgs(ulong.Parse(tidEntryMatch.Groups["tid"].ToString(), NumberStyles.HexNumber), tidEntryMatch.Groups["cur"].Length > 0, ulong.Parse(tidEntryMatch.Groups["eip"].ToString(), NumberStyles.HexNumber)));
                            }
                            else
                            {
                                if (mReceivingThreads && ThreadListEvent != null)
                                    ThreadListEvent(this, new ThreadListEventArgs(true));
                            }
                        }
                    }
                    catch (Exception) { /* Error line ... we'll ignore it for now */ }
                }
            }

            if (tookText)
            {
                lock (mCommandBuffer)
                {
                    if (mCommandBuffer.Count > 0)
                    {
                        string firstCommand = mCommandBuffer[0];
                        mCommandBuffer.RemoveAt(0);
                        mConnection.Write(firstCommand + "\r");
                    }
                }
            }
        }

        public void SetBreakpoint(Breakpoint bp)
        {
        }

        public void RemoveBreakpoint(Breakpoint bp)
        {
        }

        public event ConsoleOutputEventHandler ConsoleOutputEvent;
        public event RegisterChangeEventHandler RegisterChangeEvent;
        public event SignalDeliveredEventHandler SignalDeliveredEvent;
        public event RemoteGDBErrorHandler RemoteGDBError;
        public event MemoryUpdateEventHandler MemoryUpdateEvent;
        public event ModuleListEventHandler ModuleListEvent;
        public event ProcessListEventHandler ProcessListEvent;
        public event ThreadListEventHandler ThreadListEvent;

        public void GetRegisterUpdate()
        {
            QueueCommand("regs");
            QueueCommand("sregs");
        }

        public void GetModuleUpdate()
        {
            QueueCommand("mod");
        }

        public void GetMemoryUpdate(ulong address, int len)
        {
            QueueCommand(string.Format("x 0x{0:X} L {1}", address, len));
        }

        public void WriteMemory(ulong address, byte[] buf)
        {
        }

        public void QueueCommand(string command)
        {
            lock (mCommandBuffer)
            {
                mCommandBuffer.Add(command);
                if (mCommandBuffer.Count == 1)
                {
                    mConnection.Write(command + "\r");
                    mCommandBuffer.RemoveAt(0); //useful???
                }
            }
        }

        public void Step()
        {
            QueueCommand("step");
            GetRegisterUpdate();
            GetModuleUpdate();
            GetProcesses();
        }

        public void Next()
        {
            QueueCommand("next");
            GetRegisterUpdate();
            GetModuleUpdate();
            GetProcesses();
        }

        public void Break()
        {
            mConnection.Write("\r");
            GetRegisterUpdate();
            GetModuleUpdate();
        }

        public void Go(ulong address)
        {
            mRunning = true;
            mFirstModuleUpdate = false;
            QueueCommand("cont");
        }

        public void GetProcesses()
        {
            QueueCommand("proc list");
        }

        public void GetThreads(ulong pid)
        {
            QueueCommand(string.Format("thread list 0x{0:X8}", pid));
        }

        public void SetProcess(ulong pid)
        {
            QueueCommand(string.Format("proc attach 0x{0:X8}", pid));
            GetRegisterUpdate();
        }

        public void SetThread(ulong tid)
        {
            QueueCommand(string.Format("thread attach 0x{0:X8}", tid));
            GetRegisterUpdate();
        }

        public void Write(string wr) { mConnection.Write(wr); }

        public void Close()
        {
        }
    }
}
