using System;
using System.Text;
using System.IO.Ports;

namespace AbstractPipe
{
    public class SerialPipe : Pipe
    {
        SerialPort mSerialPort;
        int mBufSize;

        public event PipeReceiveEventHandler PipeReceiveEvent;
        public event PipeErrorEventHandler PipeErrorEvent;

        public SerialPipe(SerialPort port)
        {
            mSerialPort = port;
            mBufSize = mSerialPort.WriteBufferSize;
            //try to get older input once without timeout (to avoid blocking behaviour)
            mSerialPort.ReadTimeout = 10;
            try
            {
                String buf = mSerialPort.ReadLine();
                if (buf.Length > 0 && PipeReceiveEvent != null)
                    PipeReceiveEvent.Invoke(this, new PipeReceiveEventArgs(buf));
            }
            catch (TimeoutException) { }
            catch (Exception e)
            {
                if (PipeErrorEvent != null)
                    PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(e.Message));
            }
            mSerialPort.ReadTimeout = -1;
            //set up internal handlers
            mSerialPort.DataReceived += SerialPortDataReceived;
            mSerialPort.ErrorReceived += SerialPortErrorReceived;
        }
        
        public bool Write(string output)
        {
            lock (this)
            {
                if (mSerialPort == null) return false;
                try
                {
                    byte[] outbuf = UTF8Encoding.UTF8.GetBytes(output);
                    int off = 0, len;
                    //supply the output according to the buffer size, might not be needed
                    do
                    {
                        if (output.Length - off > mBufSize)
                        {
                            len = mBufSize;
                        }
                        else
                        {
                            len = output.Length - off;
                        }
                        mSerialPort.Write(outbuf, off, len);
                        off += len;
                    }
                    while (off < outbuf.Length);
                    return off == outbuf.Length;
                }
                catch (Exception e)
                {
                    mSerialPort.Close();
                    if (PipeErrorEvent != null)
                        PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(e.Message));
                    return false;
                }
            }
        }

        public void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs args)
        {
            if (PipeReceiveEvent != null)
                PipeReceiveEvent.Invoke(this, new PipeReceiveEventArgs(mSerialPort.ReadExisting()));
        }

        public void SerialPortErrorReceived(object sender, SerialErrorReceivedEventArgs args)
        {
            /* port will be closed by debug connection */
            if (PipeErrorEvent != null)
                PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(args.EventType.ToString()));
        }
    }
}
