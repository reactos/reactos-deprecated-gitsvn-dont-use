using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace RosDBG
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
            return (count + (factor - 1)) & (~factor - 1);
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
                mConnection.RequestMemory(mReadAddr, count);
                mReadComplete.WaitOne();
                return mCopyCount;
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
                start &= 63;
                end &= 63;
                mBytesReceived[longOfStart] |= -1 & ~((1L << start) - 1) & ((1L << end) - 1);
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
            return true;
        }

        public void Update(ulong Address, byte[] Memory)
        {
            if (Address >= mReadAddr && Address < mReadAddr + (ulong)mCopyCount)
            {
                if (Memory == null)
                {
                    int newCount = Math.Min((int)mCopyCount, (int)(Address - mReadAddr));
                    SetBits(newCount, mCopyCount);
                    mCopyCount = newCount;
                }
                else
                {
                    int bytesOffset = (int)(Address - (ulong)mPosition);
                    int bytesCount = (int)Math.Min(mPosition + mCopyCount - (long)Address, Memory.Length);
                    Array.Copy(Memory, 0, mReadBuffer, mCopyOffset, bytesCount);
                    SetBits(bytesOffset, bytesOffset + bytesCount);
                    if (ReadComplete()) mReadComplete.Set();
                }
            }
        }

        public DebugMemoryStream(DebugConnection conn)
        {
            mConnection = conn;
        }
    }
}
