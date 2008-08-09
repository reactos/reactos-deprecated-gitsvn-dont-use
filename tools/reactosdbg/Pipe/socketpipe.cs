using System;
using System.Text;
using System.Net.Sockets;

namespace AbstractPipe
{
    public class SocketPipe : Pipe
    {
        Socket mSocket;
        byte []mBuf = new byte[4096];
        IAsyncResult mResult;

        public event PipeReceiveEventHandler PipeReceiveEvent;
        public event PipeErrorEventHandler PipeErrorEvent;

        public bool Write(string output)
        {
            lock (this)
            {
                if (mSocket == null) return false;
                try
                {
                    byte[] outbuf = UTF8Encoding.UTF8.GetBytes(output);
                    int off = 0, res;
                    do
                    {
                        res = mSocket.Send(outbuf, off, outbuf.Length - off, SocketFlags.None);
                        if (res > 0)
                            off += res;
                    }
                    while (off < outbuf.Length && res != -1);
                    return off == outbuf.Length;
                }
                catch (System.Net.Sockets.SocketException se)
                {
                    mSocket.Close();
                    if (PipeErrorEvent != null)
                        PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(se.Message));
                    return false;
                }
                catch (Exception) { return false; }
            }
        }

        public void TriggerReadable(IAsyncResult result)
        {
            try
            {
                int bytes = mSocket.EndReceive(result);
                string datastr = UTF8Encoding.UTF8.GetString(mBuf, 0, bytes);
                if (PipeReceiveEvent != null)
                    PipeReceiveEvent.Invoke(this, new PipeReceiveEventArgs(datastr));
                do
                {
                    mResult = mSocket.BeginReceive
                        (mBuf, 0, mBuf.Length, SocketFlags.None, TriggerReadable, this);
                } while (mResult.CompletedSynchronously);
                
            }
            catch (System.Net.Sockets.SocketException se)
            {
                mSocket.Close();
                if (PipeErrorEvent != null)
                    PipeErrorEvent.Invoke(this, new PipeErrorEventArgs(se.Message));
            }
        }

        public SocketPipe(Socket socket) 
        { 
            mSocket = socket;
            do
            {
                mResult = mSocket.BeginReceive
                    (mBuf, 0, mBuf.Length, SocketFlags.None, TriggerReadable, this);
            } while (mResult.CompletedSynchronously);
        }
    }
}
