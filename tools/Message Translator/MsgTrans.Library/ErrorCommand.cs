using System;
using System.Xml;
using System.Collections;
using System.Collections.Generic;

namespace MsgTrans.Library
{
    public class ErrorCommand : Command
    {
        private List<Command> errorCommands = new List<Command>();

        public ErrorCommand(MessageTranslator msgTrans,
                            string ntstatusXml,
                            string winerrorXml,
                            string hresultXml,
                            string bugcheckXml)
            : base(msgTrans)
        {
            errorCommands.Add(new WinerrorCommand(msgTrans, winerrorXml));
            errorCommands.Add(new NtStatusCommand(msgTrans, ntstatusXml));
            errorCommands.Add(new HResultCommand(msgTrans, hresultXml));
            errorCommands.Add(new BugCheckCommand(msgTrans, bugcheckXml));
            errorCommands.Add(new CustomCommand(msgTrans));
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "error" }; }
        }

        public override  bool Handle(MessageContext context,
                                     string commandName,
                                     string parameters)
        {
            string errorText = parameters.Trim();

        retry:

            foreach (Command command in errorCommands)
            {
                command.Handle(context,
                               commandName,
                               errorText);
            }

            if (MsgTrans.Messages.Count == 0)
            {
                // Last chance heuristics: attempt to parse a 8-digit decimal as hexadecimal
                if (errorText.Length == 8)
                {
                    errorText = "0x" + errorText;
                    goto retry;
                }

                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("[{0}] is not a valid error code",
                                                        parameters.Trim()));
                return false;
            }
            
            return true;
        }

        public override string Help()
        {
            return "error <value>";
        }
    }
}
