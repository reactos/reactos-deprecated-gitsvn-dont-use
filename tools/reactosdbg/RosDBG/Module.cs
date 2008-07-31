using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DbgHelpAPI;

namespace RosDBG
{
    class Module
    {
        ulong mBaseAddress;
        public ulong BaseAddress
        {
            get { return mBaseAddress; }
        }
        string mShortName;
        public string ShortName
        {
            get { return mShortName; }
        }

        public Module(ulong addr, string shortName)
        {
            mBaseAddress = addr;
            mShortName = shortName.ToLower();
        }
    }
}
