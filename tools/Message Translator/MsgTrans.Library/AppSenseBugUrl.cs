using System;
using System.Collections.Generic;
using System.Text;

namespace MsgTrans.Library
{
    class BugUrl : BugCommand
    {
        public BugUrl(MessageTranslator msgTrans,
                      string bugUrl)
            : base(msgTrans, bugUrl)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "bug" }; }
        }

        public override string Help()
        {
            return "bug <number>";
        }
    }
}
