using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DebugProtocol
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
        public readonly List<ulong> Registers;
        public RegisterChangeEventArgs(IEnumerable<ulong> registers)
        {
            Registers = new List<ulong>(registers);
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
        public readonly ulong Address;
        public readonly byte[] Memory;
        public MemoryUpdateEventArgs(ulong address, byte[] memory)
        {
            Address = address;
            Memory = memory;
        }
    }
    public delegate void MemoryUpdateEventHandler(object sender, MemoryUpdateEventArgs args);

    public class ModuleListEventArgs : EventArgs
    {
        public readonly ulong Address;
        public readonly string Module;
        public ModuleListEventArgs(string module, ulong address)
        {
            Module = module;
            Address = address;
        }
    }
    public delegate void ModuleListEventHandler(object sender, ModuleListEventArgs args);

    public class ProcessListEventArgs : EventArgs
    {
        public readonly bool Reset, Current, End;
        public readonly ulong Pid;
        public ProcessListEventArgs() { Reset = true; }
        public ProcessListEventArgs(bool end) { End = true; }
        public ProcessListEventArgs(ulong pid, bool current) { Current = current; Pid = pid; }
    }

    public delegate void ProcessListEventHandler(object sender, ProcessListEventArgs args);

    public class ThreadListEventArgs : EventArgs
    {
        public readonly bool Reset, Current, End;
        public readonly ulong Tid;
        public ThreadListEventArgs() { Reset = true; }
        public ThreadListEventArgs(bool end) { End = true; }
        public ThreadListEventArgs(ulong tid, bool current) { Current = current; Tid = tid; }
    }

    public delegate void ThreadListEventHandler(object sender, ThreadListEventArgs args);
    #endregion

    public interface IDebugProtocol
    {
        void SetBreakpoint(Breakpoint bp);
        void RemoveBreakpoint(Breakpoint bp);

        event ConsoleOutputEventHandler ConsoleOutputEvent;
        event RegisterChangeEventHandler RegisterChangeEvent;
        event SignalDeliveredEventHandler SignalDeliveredEvent;
        event RemoteGDBErrorHandler RemoteGDBError;
        event MemoryUpdateEventHandler MemoryUpdateEvent;
        event ModuleListEventHandler ModuleListEvent;
        event ProcessListEventHandler ProcessListEvent;
        event ThreadListEventHandler ThreadListEvent;

        void GetRegisterUpdate();
        void GetModuleUpdate();
        void GetMemoryUpdate(ulong address, int len);
        void GetProcesses();
        void GetThreads(ulong pid);
        void WriteMemory(ulong address, byte[] buf);
        void Step();
        void Next();
        void Break();
        void Go(ulong address);

        void SetProcess(ulong pid);
        void SetThread(ulong tid);

        void Write(string wr);

        void Close();
    }
}
