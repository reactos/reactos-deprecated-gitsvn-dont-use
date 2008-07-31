using System;
using System.Text;
using System.Net.Sockets;

namespace Gdb
{
    public class SocketPipe : Pipe
    {
        Socket mSocket;
        byte []mBuf = new byte[4096];
        IAsyncResult mResult;

        public event PipeReceiveEventHandler PipeReceiveEvent;
        public bool Write(string output)
        {
            byte []outbuf = UTF8Encoding.UTF8.GetBytes(output);
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

        public void TriggerReadable(IAsyncResult result)
        {
            int bytes = mSocket.EndReceive(mResult);
            string datastr = UTF8Encoding.UTF8.GetString(mBuf, 0, bytes);
            if (PipeReceiveEvent != null)
                PipeReceiveEvent(this, new PipeReceiveEventArgs(datastr));
            mResult = mSocket.BeginReceive
                (mBuf, 0, mBuf.Length, SocketFlags.None, TriggerReadable, this);
        }

        public SocketPipe(Socket socket) 
        { 
            mSocket = socket; 
            mResult = mSocket.BeginReceive
                (mBuf, 0, mBuf.Length, SocketFlags.None, TriggerReadable, this);
        }
    }
}
