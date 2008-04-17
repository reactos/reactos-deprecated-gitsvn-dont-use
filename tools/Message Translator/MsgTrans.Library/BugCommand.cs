using System;

namespace MsgTrans.Library
{
    public abstract class BugCommand : Command//, ICommand
    {
        string bugUrl;

        public BugCommand(MessageTranslator msgTrans,
                          string bugUrl)
            : base(msgTrans)
        {
            this.bugUrl = bugUrl;
        }

        public string BugUrl
        {
            get { return bugUrl; }
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

            MsgTrans.Number = np.Decimal;
            MsgTrans.BugUrl = String.Format(BugUrl, np.Decimal);

            return true;
        }
    }
}
