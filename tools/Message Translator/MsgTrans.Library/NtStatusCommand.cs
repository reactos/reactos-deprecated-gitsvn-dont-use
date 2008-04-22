using System;
using System.Xml;

namespace MsgTrans.Library
{
    public class NtStatusCommand : XmlCommand
    {
        public NtStatusCommand(MessageTranslator msgTrans,
                               string ntstatusXml)
            : base(msgTrans, ntstatusXml)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "ntstatus" }; }
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string ntstatusText = parameters;
            if (ntstatusText.Equals(String.Empty))
            {
                return false;
            }

            NumberParser np = new NumberParser();
            if (!np.Parse(ntstatusText))
            {
                return false;
            }
            
            string description = GetNtstatusDescription(np.Decimal);
            if (description != null)
            {
                AddMessage(MessageType.NTSTATUS,
                           np.Decimal,
                           np.Hex,
                           description,
                           null);

                return true;
            }/*
            else
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("I don't know about NTSTATUS {0}.",
                                                        ntstatusText));

                return false;
            }*/

            return false;
        }

        public override string Help()
        {
            return "ntstatus <value>";
        }
        
        public string GetNtstatusDescription(long ntstatus)
        {
            XmlElement root = base.m_XmlDocument.DocumentElement;
            XmlNode node = root.SelectSingleNode(String.Format("Ntstatus[@value='{0}']",
                                                               ntstatus.ToString("X8")));
            if (node != null)
            {
                XmlAttribute text = node.Attributes["text"];
                if (text == null)
                    throw new Exception("Node has no text attribute.");
                return text.Value;
            }
            else
                return null;
        }
    }
}
