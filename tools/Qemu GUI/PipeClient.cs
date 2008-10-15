using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.IO;
using System.Collections;

namespace Qemu_GUI
{
    public class PipeReceiveEventArgs
    {
        public readonly string Received;
        public PipeReceiveEventArgs(string received)
        {
            Received = received;
        }
    }
    public delegate void PipeReceiveEventHandler(object sender, PipeReceiveEventArgs args);

    public class PipeErrorEventArgs
    {
        public readonly string ErrorDesc;
        public PipeErrorEventArgs(string error)
        {
            ErrorDesc = error;
        }
    }
    public delegate void PipeErrorEventHandler(object sender, PipeErrorEventArgs args);

    public interface Pipe
    {
        event PipeReceiveEventHandler PipeReceiveEvent;
        event PipeErrorEventHandler PipeErrorEvent;
        bool Write(string output);
    }

}
