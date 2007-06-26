using System;
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
                try
                {
                    FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                    XmlTextReader xtr = new XmlTextReader(fs);
                    data.ReadXmlSchema(xtr);
                    xtr.Close();
                    ret = true;
                }
                catch (Exception e)
                {
                    MessageBox.Show("error loading main config schema: " + e.Message);
                }
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
                try
                {
                    FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                    XmlTextReader xtr = new XmlTextReader(fs);
                    data.ReadXmlSchema(xtr);
                    xtr.Close();
                    ret = true;
                }
                catch (Exception e)
                {
                    MessageBox.Show("error loading VM config schema: " + e.Message);
                }
            }

            return ret;
        }
    }
}
