using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DebugProtocol
{
    public class Breakpoint
    {
        public enum BPType { Software, Hardware, WriteWatch, ReadWatch, AccessWatch };
        public readonly BPType BreakpointType;
        public readonly long Address;
        public readonly int Length;
        public Breakpoint(BPType type, long addr, int len)
        {
            BreakpointType = type;
            Address = addr;
            Length = len;
        }

        public override int GetHashCode()
        {
            return (int)(((int)BreakpointType) ^ Address ^ (Length << 28));
        }

        public override bool Equals(object other)
        {
            Breakpoint otherbp = other as Breakpoint;
            if (otherbp == null) return false;
            return
                (otherbp.BreakpointType == BreakpointType) &&
                (otherbp.Address == Address) &&
                (otherbp.Length == Length);
        }
    }
}
