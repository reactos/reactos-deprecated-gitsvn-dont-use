using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RosDBG
{
    public class DebugInfoFile
    {
        BinaryReader mTheFile;
        public DebugInfoFile(BinaryReader reader)
        {
            mTheFile = reader;
        }

        Int32 mImageBase;
        public Int32 ImageBase
        {
            get { return mImageBase; }
            set { mImageBase = value; }
        }
    }
}
