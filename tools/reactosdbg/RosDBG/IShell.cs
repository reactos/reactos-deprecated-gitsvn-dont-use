using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RosDBG
{
    public interface IShell
    {
        void FocusAddress(ulong address);
    }

    public interface IUseShell
    {
        void SetShell(IShell shell);
    }
}
