using System;
using System.Xml;

namespace MsgTrans.Library
{
    public class HResultCommand : XmlCommand
    {

        public HResultCommand(MessageTranslator msgTrans,
                              string hresultXml)
            : base(msgTrans, hresultXml)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "hresult" }; }
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string hresultText = parameters;
            if (hresultText.Equals(String.Empty))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          "Please provide a valid HRESULT value.");
                return false;
            }

            NumberParser np = new NumberParser();
            if (!np.Parse(hresultText))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("{0} is not a valid HRESULT value.",
                                                        hresultText));
                return false;
            }
            
            string description = GetHresultDescription(np.Decimal);
            if (description != null)
            {
                Number = np.Decimal;
                Code = description;
                return true;
            }
            else
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("I don't know about HRESULT {0}.",
                                                        hresultText));
                return false;
            }
        }
        
        public override string Help()
        {
            return "hresult <value>";
        }
        
        public string GetHresultDescription(long hresult)
        {
            XmlElement root = base.m_XmlDocument.DocumentElement;
            XmlNode node = root.SelectSingleNode(String.Format("Hresult[@value='{0}']",
                                                               hresult.ToString("X8")));
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
