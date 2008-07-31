using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DebugProtocol
{
    public class MemoryBlock
    {
        public readonly long Address;
        public readonly byte[] Block;
        static int mMemoryBlockSize = 256;
        public MemoryBlock(long address)
        {
            Address = address;
            Block = new byte[mMemoryBlockSize];
        }
    }
}
