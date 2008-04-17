using System;
using System.Xml;

namespace MsgTrans.Library
{
    public class WMCommand : XmlCommand
    {

        public WMCommand(MessageTranslator msgTrans,
                         string wmXml)
            : base(msgTrans, wmXml)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "wm" }; }
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string wmText = parameters;
            if (wmText.Equals(String.Empty))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          "Please provide a valid window message value or name.");
                return false;
            }

            NumberParser np = new NumberParser();

            string code;
            long num;
            if (np.Parse(wmText))
            {
                num = np.Decimal;
                code = GetWmDescription(np.Decimal);
            }
            else
            {
                // Possibly in "wm <name>" form.
                num = GetWmNumber(wmText);
                code = wmText;
            }

            if (code != null)
            {
                MsgTrans.Number = num;
                MsgTrans.Code = code;
                return true;
            }
            else
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("I don't know about window message {0}.",
                                                        wmText));
                return false;
            }
        }

        public override string Help()
        {
            return "!wm <value> or !wm <name>";
        }
        
        private string GetWmDescription(long wm)
        {
            XmlElement root = base.m_XmlDocument.DocumentElement;
            XmlNode node = root.SelectSingleNode(String.Format("WindowMessage[@value='{0}']",
                                                               wm));
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
        
        private long GetWmNumber(string wmName)
        {
            XmlElement root = base.m_XmlDocument.DocumentElement;
            XmlNode node = root.SelectSingleNode(String.Format("WindowMessage[@text='{0}']",
                                                               wmName));
            if (node != null)
            {
                XmlAttribute value = node.Attributes["value"];
                if (value == null)
                    throw new Exception("Node has no value attribute.");
                return Convert.ToInt64(value.Value);
            }
            else
                return -1;
        }
    }
}
