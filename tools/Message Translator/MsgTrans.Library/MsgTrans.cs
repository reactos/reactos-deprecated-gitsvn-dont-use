using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Data;
using System.Threading;

namespace MsgTrans.Library
{
    public class MessageTranslator
    {
        private IMsgOutput msgOutput;
        private List<Command> commands = new List<Command>();
        private string bugUrl;
        private string type;
        private long number;
        private string hex;
        private string code;
        private string message;

        #region properties
        public string Type
        {
            get { return type; }
            set { type = value; }
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
        public string BugUrl
        {
            get { return bugUrl; }
            set { bugUrl = value; }
        }
        public IMsgOutput MsgOutput
        {
            get { return msgOutput; }
        }
        public IList<Command> Commands
        {
            get { return commands; }
        }
        #endregion

        public MessageTranslator(IMsgOutput msgOutput,
                                 string ntstatusXml,
                                 string winerrorXml,
                                 string hresultXml,
                                 string wmXml,
                                 string bugUrl)
        {
            this.msgOutput = msgOutput;

            commands.Add(new ErrorCommand(this,
                                          ntstatusXml,
                                          winerrorXml,
                                          hresultXml));
            commands.Add(new WMCommand(this, wmXml));
            commands.Add(new BugUrl(this, bugUrl));
        }

        public bool ParseCommandMessage(MessageContext context,
                                        string message)
        {
            int index = message.IndexOf(' ');
            string commandName;
            string commandParams = "";
            if (index != -1)
            {
                commandName = message.Substring(0, index).Trim();
                commandParams = message.Substring(index).Trim();
            }
            else
                commandName = message.Trim();

            foreach (Command command in commands)
            {
                foreach (string cmd in command.AvailableCommands)
                {
                    if (cmd == commandName)
                    {
                        return command.Handle(context,
                                              commandName, 
                                              commandParams);
                    }
                }
            }

            return false;
        }
    }
}
