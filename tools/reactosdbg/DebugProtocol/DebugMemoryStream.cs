using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace DebugProtocol
{
    public class DebugMemoryStream : Stream
    {
        DebugConnection mConnection;
        ulong mReadAddr;
        int mCopyOffset, mCopyCount;
        byte[] mReadBuffer;
        long[] mBytesReceived;
        EventWaitHandle mReadComplete = new EventWaitHandle(false, EventResetMode.AutoReset);

        public override bool CanRead
        {
            get { return true; }
        }

        public override bool CanSeek
        {
            get { return true; }
        }

        public override bool CanWrite
        {
            get { return true; }
        }

        public override long Length
        {
            get { return 1L << 32; }
        }

        long mPosition;
        public override long Position
        {
            get { return mPosition; }
            set { mPosition = value; }
        }

        public override void Flush() { }

        public override long Seek(long position, SeekOrigin origin)
        {
            long prev = mPosition;
            switch (origin)
            {
                case SeekOrigin.Begin:
                    mPosition = position;
                    break;

                case SeekOrigin.Current:
                    mPosition += position;
                    break;

                case SeekOrigin.End:
                    mPosition = ((1L << 32) - position) & ((1L << 32) - 1);
                    break;
            }
            return prev;
        }

        public override void SetLength(long len) { }

        int RoundUp(int count, int factor)
        {
            return (count + (factor - 1)) & ~(factor - 1);
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            lock (this)
            {
                mReadAddr = (ulong)mPosition;
                mBytesReceived = new long[RoundUp(count, 64) / 64];
                SetBits(count, RoundUp(count, 64));
                mCopyOffset = offset;
                mCopyCount = count;
                mReadBuffer = buffer;
                mConnection.Debugger.GetMemoryUpdate(mReadAddr, count);
            }
            mReadComplete.WaitOne();
            lock (this)
            {
                mBytesReceived = null;
                return mCopyCount == 0 ? -1 : mCopyCount;
            }
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
        }

        void SetBits(int start, int end)
        {
            int longOfStart = start >> 6, longOfEnd = (end - 1) >> 6;
            if (longOfStart == longOfEnd)
            {
                end -= start & ~63;
                start &= 63;
                long mask = ~((1L << start) - 1);
                mask &= end == 64 ? -1 : ((1L << end) - 1);
                mBytesReceived[longOfStart] |= mask;
            }
            else
            {
                int nextStart = (start + 64) & ~63;
                SetBits(start, nextStart);
                while (nextStart >> 6 != longOfEnd)
                {
                    start = nextStart;
                    nextStart = start + 64;
                    SetBits(start, nextStart);
                }
            }
        }

        bool ReadComplete()
        {
            foreach (long l in mBytesReceived) if (l != -1) return false;
            if (mCopyCount > 0) mPosition += mCopyCount;
            return true;
        }

        public void Update(ulong Address, byte[] Memory)
        {
            lock (this)
            {
                if (mBytesReceived == null) return;
                if (Address >= mReadAddr)
                {
                    long diff = (long)(Address - mReadAddr);

                    if (Memory == null)
                    {
                        if (diff >= mCopyCount) return;
                        SetBits((int)diff, mCopyCount);
                        mCopyCount = Math.Min(mCopyCount, (int)diff);
                    }
                    else
                    {
                        long toCopy = Math.Min(Memory.Length, mCopyCount - diff);
                        if (toCopy <= 0 || diff >= mCopyCount) return;

                        Array.Copy(Memory, 0, mReadBuffer, mCopyOffset + diff, toCopy);
                        SetBits(mCopyOffset + (int)diff, (int)(mCopyOffset + diff + toCopy));
                    }
                }
                else
                {
                    long diff = (long)(mReadAddr - Address);

                    if (Memory == null)
                    {
                        if (diff >= 4096) return;
                        mCopyCount = 0;
                    }
                    else
                    {
                        long toCopy = Math.Min(mReadBuffer.Length - mCopyOffset, Math.Min(mCopyCount - diff, Memory.Length));
                        if (toCopy <= 0 || diff >= Memory.Length) return;

                        Array.Copy(Memory, (int)diff, mReadBuffer, mCopyOffset, (int)toCopy);
                        SetBits(0, (int)toCopy);
                    }
                }
                if (ReadComplete()) mReadComplete.Set();
            }
        }

        public DebugMemoryStream(DebugConnection conn)
        {
            mConnection = conn;
        }
    }
}
