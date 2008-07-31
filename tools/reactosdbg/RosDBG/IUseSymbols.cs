using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DbgHelpAPI;

namespace RosDBG
{
    public interface IUseSymbols
    {
        void SetSymbolProvider(SymbolContext symcon);
    }
}
