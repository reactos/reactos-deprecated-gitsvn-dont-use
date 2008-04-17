using System;
using System.Xml;
using System.Collections.Generic;
using System.Text;

namespace MsgTrans.Library
{
    public abstract class XmlCommand : Command
    {
        protected XmlDocument m_XmlDocument;
        private string xmlFile;

        public XmlCommand(MessageTranslator msgTrans,
                          string xmlFile)
            : base(msgTrans)
        {
            this.xmlFile = xmlFile;
            m_XmlDocument = new XmlDocument();
            m_XmlDocument.Load(xmlFile);
        }

        //public abstract string XmlFile { get; }
        public string XmlFile
        {
            get { return xmlFile; }
        }

        public XmlDocument XmlDocument
        {
            get { return m_XmlDocument; }
        }
    }
}