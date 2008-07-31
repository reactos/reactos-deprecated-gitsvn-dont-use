using System;
using System.IO;
using System.Text;
using System.Globalization;
using System.Text.RegularExpressions;
using System.Timers;
using System.Collections.Generic;

namespace Gdb 
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
    public interface Pipe
    {
        event PipeReceiveEventHandler PipeReceiveEvent;
        bool Write(string output);
    }

    public class GdbProtocol
    {
        Pipe mPipe;
        List<string> mSendBuffer = new List<string>();
        StringBuilder mInputBuffer = new StringBuilder();
        Timer mTimer = new Timer();
        int mMessageId = 0;

        public void SendMessage(string msg)
        {
            byte csum = 0;
            foreach (char addend in msg) { csum += (byte)addend; }
            mSendBuffer.Add(string.Format("${0}#{1:X2}#{1:X3}", msg, csum, mMessageId++));
            mPipe.Write(mSendBuffer[0]);
        }

        // Break isn't a packet... its effect is immediate
        public void SendBreak()
        {
            mPipe.Write("\x03\x03\x03\x03\x03");
        }

        public void SendStart()
        {
            mPipe.Write("---");
        }

	bool CheckSum(string payload, int checksum)
	{
		byte newcsum = 0;
		foreach (char p in payload)
		{
			newcsum += (byte)p;
		}
		return checksum == newcsum;
	}

        static Regex mPacketRegex = 
            new Regex("[^$]*\\$(?<payload>[^#]*)\\#(?<checksum>[0-9a-fA-F][0-9a-fA-F])\\#(?<mid>[0-9a-fA-F][0-9a-fA-F])(?<tail>.*)");
        int mLastId;

        void RecvFired(object sender, PipeReceiveEventArgs args)
        {
            string msg = args.Received;

            // Must get a confirmation as a single message
            if (msg.Length > 0 && msg[0] == '+')
            {
                if (mSendBuffer.Count > 0) mSendBuffer.RemoveAt(0);
                msg = msg.Substring(1);
            }

            mInputBuffer.Append(msg);
            string inbuf = mInputBuffer.ToString();
            bool matched = false;
            bool parsed = false;

	    // Leave if there's no '$' in the buffer.  We don't need to 
	    // process further
	    if (inbuf.IndexOf('$') == -1)
	    {
	        mInputBuffer = new StringBuilder("");
		return;
	    }

            Match m = mPacketRegex.Match(inbuf);

            while (m.Success)
            {
                string payload = m.Groups["payload"].ToString();
                string checksum = m.Groups["checksum"].ToString();
                string midstr = m.Groups["mid"].ToString();
                int mid = int.Parse(midstr, NumberStyles.HexNumber);
                matched = true;

                if (CheckSum(payload, int.Parse(checksum, NumberStyles.HexNumber)))
                {
                    parsed = true;
                    try
                    {
                        if (mid != mLastId)
                        {
                            mLastId = mid;
                            ReceivePacket(payload);
                        }
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Error parsing [{0}]: {1}", payload, e.Message);
                    }
                    mPipe.Write("+");
                }
                mInputBuffer = new StringBuilder("");
                mInputBuffer.Append(inbuf = m.Groups["tail"].ToString());
                m = mPacketRegex.Match(inbuf);
            }
            
            // On the safe side, send a nak if we got a packet, but it didn't
            // check
            if (matched && !parsed)
            {
                mPipe.Write("-");
            }
        }

        protected virtual void ReceivePacket(string payload)
        {
            Console.WriteLine("got payload: " + payload);
        }

        void ResendTimer(object sender, ElapsedEventArgs args)
        {
            if (mSendBuffer.Count > 0)
            {
                mPipe.Write(mSendBuffer[0]);
            }
        }

        public GdbProtocol(Pipe pipe)
        {
            mPipe = pipe;
            mPipe.PipeReceiveEvent += RecvFired;
            mTimer.Elapsed += ResendTimer;
            mTimer.Start();
        }
    }
}
