using System.Data;
using System.IO;
using System.Xml;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class Data
    {
        private DataSet data = null;

        public Data()
        {
        }

        public DataSet DataSet
        {
            get { return data; }
        }

        public bool LoadMainData()
        {
            string filename = "MainConfig.xsd";
            bool ret = false;

            data = new DataSet();
            if (File.Exists(filename))
            {
                FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                XmlTextReader xtr = new XmlTextReader(fs);
                data.ReadXmlSchema(xtr);
                xtr.Close();
                ret = true;
            }

            return ret;
        }

        public bool LoadVirtMachData()
        {
            string filename = "VMConfig.xsd";
            bool ret = false;

            data = new DataSet();
            if (File.Exists(filename))
            {
                FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                XmlTextReader xtr = new XmlTextReader(fs);
                data.ReadXmlSchema(xtr);
                xtr.Close();
                ret = true;
            }

            return ret;
        }
    }
}
