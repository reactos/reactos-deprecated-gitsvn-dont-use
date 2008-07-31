using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DebugProtocol;

namespace RosDBG
{
    public interface IUseDebugConnection
    {
        void SetDebugConnection(DebugConnection conn);
    }
}
