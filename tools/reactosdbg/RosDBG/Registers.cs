using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace RosDBG
{
    public class Registers
    {
        public ulong Eax { get { return RegisterSet[0]; } set { RegisterSet[0] = value; } }
        public ulong Ecx { get { return RegisterSet[1]; } set { RegisterSet[1] = value; } }
        public ulong Edx { get { return RegisterSet[2]; } set { RegisterSet[2] = value; } }
        public ulong Ebx { get { return RegisterSet[3]; } set { RegisterSet[3] = value; } }
        public ulong Esp { get { return RegisterSet[4]; } set { RegisterSet[4] = value; } }
        public ulong Ebp { get { return RegisterSet[5]; } set { RegisterSet[5] = value; } }
        public ulong Esi { get { return RegisterSet[6]; } set { RegisterSet[6] = value; } }
        public ulong Edi { get { return RegisterSet[7]; } set { RegisterSet[7] = value; } }
        public ulong Eip { get { return RegisterSet[8]; } set { RegisterSet[8] = value; } }
        public ulong Eflags { get { return RegisterSet[9]; } set { RegisterSet[9] = value; } }
        public ulong Cs { get { return RegisterSet[10]; } set { RegisterSet[10] = value; } }
        public ulong Ds { get { return RegisterSet[11]; } set { RegisterSet[11] = value; } }
        public ulong Es { get { return RegisterSet[12]; } set { RegisterSet[12] = value; } }
        public ulong Fs { get { return RegisterSet[13]; } set { RegisterSet[13] = value; } }
        public ulong Gs { get { return RegisterSet[14]; } set { RegisterSet[14] = value; } }
        public ulong Ss { get { return RegisterSet[15]; } set { RegisterSet[15] = value; } }
        public ulong[] RegisterSet = new ulong[32];
    }
}
