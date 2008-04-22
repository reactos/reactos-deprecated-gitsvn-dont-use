using System;

namespace MsgTrans.Library
{

    public abstract class Command
    {
        protected MessageTranslator msgTrans = null;
        private MessageType messageType;
        private long number;
        private string hex;
        private string code;
        private string message;

        #region properties
        public MessageType MsgType
        {
            get { return messageType; }
            set { messageType = value; }
        }
        public long Number
        {
            get { return number; }
            set { number = value; }
        }
        public string Hex
        {
            get { return hex; }
            set { hex = value; }
        }
        public string Code
        {
            get { return code; }
            set { code = value; }
        }
        public string Message
        {
            get { return message; }
            set { message = value; }
        }
        public MessageTranslator MsgTrans
        {
            get { return msgTrans; }
        }
        #endregion

        public abstract string[] AvailableCommands { get; }

        public Command(MessageTranslator msgTrans)
        {
            this.msgTrans = msgTrans;
        }

        protected void AddMessage(MessageType msgType,
                                long dec,
                                string hex,
                                string code,
                                string msg)
        {
            MsgType = msgType;
            Number = dec;
            Hex = hex;
            Code = code;
            Message = msg;
            MsgTrans.Messages.Add(this);
        }

        public abstract bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters);

        public virtual string Help()
        {
            return "No help is available for this command";
        }
    }
}
