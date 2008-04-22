using System;
using System.Xml;

namespace MsgTrans.Library
{
    public class WinerrorCommand : XmlCommand
    {
        public WinerrorCommand(MessageTranslator msgTrans,
                               string winerrorXml)
            : base(msgTrans, winerrorXml)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "winerror" }; }
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string winerrorText = parameters;
            if (winerrorText.Equals(String.Empty))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          "Please provide a valid System Error Code value.");
                return false;
            }

            NumberParser np = new NumberParser();
            if (!np.Parse(winerrorText))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("{0} is not a valid System Error Code value.",
                                                        winerrorText));
                return false;
            }
            
            string description = GetWinerrorDescription(np.Decimal);
            if (description != null)
            {
                Number = np.Decimal;
                Code = description;
                return true;
            }
            else
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("I don't know about System Error Code {0}.",
                                                        winerrorText));
                return false;
            }
        }

        public override string Help()
        {
            return "!winerror <value>";
        }
        
        public string GetWinerrorDescription(long winerror)
        {
            XmlElement root = base.m_XmlDocument.DocumentElement;
            XmlNode node = root.SelectSingleNode(String.Format("Winerror[@value='{0}']",
                                                               winerror));
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
