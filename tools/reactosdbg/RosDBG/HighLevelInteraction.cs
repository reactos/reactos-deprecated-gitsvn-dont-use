using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RosDBG
{
    public class BytesEventArgs : EventArgs
    {
        byte []mBytes;
        public byte []Bytes { get { return mBytes; } }
        public BytesEventArgs(byte []bytes)
        {
            mBytes = bytes;
        }
    }
    public interface BidirectionalChannel
    {
    }

    public class HighLevelInteraction
    {

    }
}
