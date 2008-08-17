using System;
using System.Xml;

namespace MsgTrans.Library
{
    public class BugCheckCommand : XmlCommand
    {
        public BugCheckCommand(MessageTranslator msgTrans,
                               string bugcheckXml)
            : base(msgTrans, bugcheckXml)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "bugcheck" }; }
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string bugcheckText = parameters;
            if (bugcheckText.Equals(String.Empty))
            {
                return false;
            }

            NumberParser np = new NumberParser();
            if (!np.Parse(bugcheckText))
            {
                return false;
            }
            
            string description = GetBugCheckDescription(np.Decimal);
            if (description != null)
            {
                AddMessage(MessageType.BugCheck,
                           np.Decimal,
                           np.Hex,
                           description,
                           null);

                return true;
            }

            return false;
        }

        public override string Help()
        {
            return "ntstatus <value>";
        }
        
        public string GetBugCheckDescription(long stopcode)
        {
            XmlElement root = base.m_XmlDocument.DocumentElement;
            XmlNode node = root.SelectSingleNode(String.Format("BugCheck[@value='{0}']",
                                                               stopcode.ToString("X8")));
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
