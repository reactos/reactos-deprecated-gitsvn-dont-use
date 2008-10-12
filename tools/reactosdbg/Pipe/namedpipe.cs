using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace AbstractPipe
{
    public enum ConnectionMode
    {
        MODE_CLIENT = 0x00000000,
        MODE_SERVER = 0x00000001,
        MODE_AUTO   = 0x00000002
    }

    public class NamedPipe : Pipe
    {
        public const int PIPE_SIZE = 1024;

        private PipeStream ioStream; /* stream for io operations */
        private String wCommand; /* buffer of a single command line */
        private List<String> cmdList; /*list of commands pending to be written */

        public event PipeReceiveEventHandler PipeReceiveEvent;
        public event PipeErrorEventHandler PipeErrorEvent;

        private static ManualResetEvent newWriteData = new ManualResetEvent(false);

        public NamedPipe()
        {
            cmdList = new List<string>();
        }

        public bool CreateServerPipe(string name)
        {
            /* create a pipe and wait for a client */
            NamedPipeServerStream sStream = new NamedPipeServerStream(name, PipeDirection.InOut, 1, 
                PipeTransmissionMode.Byte, PipeOptions.Asynchronous, PIPE_SIZE, PIPE_SIZE);
            sStream.WaitForConnection();

            if (sStream.IsConnected)
            {
                ioStream = sStream;
                return true;
            }
            else
            {
                return false;
            }
        }
       
        public bool CreateClientPipe(string name)
        {
            /* try to connect as a client */
            /* (QEMU -serial pipe or VMware in pipe server mode) */
            try
            {
                NamedPipeClientStream cStream = new NamedPipeClientStream(".", name, PipeDirection.InOut, PipeOptions.Asynchronous);
                cStream.Connect(100);

                if (cStream.IsConnected)
                {
                    ioStream = cStream;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            catch (Exception)
            {
                return false;
            }
        }

        public bool CreatePipe(string name, ConnectionMode mode)
        {
            if (name == "" || name == null)
            {
                return false;
            }
            switch (mode)
            {
                case ConnectionMode.MODE_AUTO:
                    //check if pipe exists, if not create server pipe, wait certain time, check if pipe...
                    //TODO: server-client lookup should time out
                    while (true)
                    {
                        if (CreateClientPipe(name))
                        {
                            break;
                        }
                        if (CreateServerPipe(name))
                        {
                            break;
                        }
                    }
                    return true; 

                case ConnectionMode.MODE_CLIENT:
                    while (!CreateClientPipe(name)) ;
                    
                    /* pipe open, everything fine */
                    return true;

                case ConnectionMode.MODE_SERVER:
                    if (CreateServerPipe(name))
                    {
                        /* wait for a client*/
                        
                        return true;
                    }
                    break;
            }
            return false;
        }

        public void Close()
        {
            if (ioStream != null)
                ioStream.Close();
        }

        public void WriteLoop()
        {
            try
            {
                while (true)
                {
                    if (cmdList.Count > 0)
                    {
                        byte[] wBuf = new byte[cmdList[0].Length];
                        UTF8Encoding.UTF8.GetBytes(cmdList[0], 0, cmdList[0].Length, wBuf, 0);

                        ioStream.Write(wBuf, 0, cmdList[0].Length);

                        /* remove written data from commandlist */
                        cmdList.RemoveAt(0);
                    }
                    else if (cmdList.Count == 0)
                    {
                        /* wait until new data is signaled */
                        newWriteData.Reset();
                        newWriteData.WaitOne();
                    }
                }
            }
            catch (Exception e)
            {
                if (PipeErrorEvent != null)
                    PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(e.Message));
            }

        }

        public void ReadLoop()
        {
            byte[] buf = new byte[PIPE_SIZE];
            int read = 0;

            try
            {
                while(true)
                {
                    read = ioStream.Read(buf, 0, PIPE_SIZE);
                    if (read > 0)
                    {
                        if (PipeReceiveEvent != null)
                            PipeReceiveEvent.Invoke(this, new PipeReceiveEventArgs(UTF8Encoding.UTF8.GetString(buf, 0, read)));
                    }
                    else
                    {
                        /* connecion closed */
                        break;
                    }
                }
            }
            catch (Exception e)
            {
                if (PipeErrorEvent != null)
                    PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(e.Message));
            }
        }

        public bool Write(string str)
        {
            /* only forward a complete line */
            wCommand += str;

            if (str[str.Length-1] == '\r')
            {
                cmdList.Add(wCommand);
                wCommand = null;

                /* wake up the write thread */
                newWriteData.Set();
            }
            return true;
        }
    }
}
