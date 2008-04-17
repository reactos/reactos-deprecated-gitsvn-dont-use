using System;

namespace MsgTrans.Library
{

    public abstract class Command
    {
        protected MessageTranslator msgTrans = null;

        public Command(MessageTranslator msgTrans)
        {
            this.msgTrans = msgTrans;
        }

        public MessageTranslator MsgTrans
        {
            get { return msgTrans; }
        }

        public abstract string[] AvailableCommands { get; }

        public abstract bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters);

        public virtual string Help()
        {
            return "No help is available for this command";
        }
    }
}
