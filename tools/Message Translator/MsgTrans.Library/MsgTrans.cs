using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Data;
using System.Threading;

namespace MsgTrans.Library
{
    // Summary:
    //     Specifies the type of message returned
    public enum MessageType
    {
        // Summary:
        //     A Win32 error
        WinError = 0,
        //
        // Summary:
        //     A HRESULT status code.
        HRESULT = 1,
        //
        // Summary:
        //     An NTSTATUS status code
        NTSTATUS = 2,
        //
        // Summary:
        //     a STOP/Bug Check code
        BugCheck = 3,
        //
        // Summary:
        //     a Windows Message code
        WinMsg = 4,
        //
        // Summary:
        //     a Bug Url
        BugUrl = 5,
        //
        // Summary:
        //     a custom Check code
        Custom = 6
    }

    public class MessageTranslator
    {
        private IMsgOutput msgOutput;
        private List<Command> commands = new List<Command>();
        private List<Command> messages = new List<Command>();
        private string type;


        #region properties
        public string Type
        {
            get { return type; }
            set { type = value; }
        }
        public IMsgOutput MsgOutput
        {
            get { return msgOutput; }
        }
        public List<Command> Messages
        {
            get { return messages; }
        }
        #endregion

        public MessageTranslator(IMsgOutput msgOutput,
                                 string ntstatusXml,
                                 string winerrorXml,
                                 string hresultXml,
                                 string bugcheckXml,
                                 string wmXml,
                                 string bugUrl)
        {
            this.msgOutput = msgOutput;

            commands.Add(new ErrorCommand(this,
                                          ntstatusXml,
                                          winerrorXml,
                                          hresultXml,
                                          bugcheckXml));
            commands.Add(new WMCommand(this, wmXml));
            commands.Add(new BugCommand(this, bugUrl));
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
