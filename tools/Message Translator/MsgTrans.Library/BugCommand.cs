using System;

namespace MsgTrans.Library
{
    public class BugCommand : Command
    {
        string bugUrl;

        public BugCommand(MessageTranslator msgTrans,
                          string bugUrl)
            : base(msgTrans)
        {
            this.bugUrl = bugUrl;
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "bug" }; }
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string bugText = parameters;
            if (bugText.Equals(String.Empty))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          "Please provide a valid bug number.");
                return false;
            }

            NumberParser np = new NumberParser();
            if (!np.Parse(bugText))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("{0} is not a valid bug number.",
                                                        bugText));
                return false;
            }

            string url = String.Format(bugUrl, np.Decimal);

            MsgType = MessageType.BugUrl;
            Number = np.Decimal;
            Hex = np.Hex;
            Code = url;
            Message = null;
            MsgTrans.Messages.Add(this);

            return true;
        }
    }
}
