using System.Data;
using System.IO;
using System.Xml;

namespace RosTEGUI
{
    public class MainConfig
    {
        private DataSet mainConfig;

        public MainConfig(MainForm mf)
        {
            string filename = "Config.xsd";
            mainConfig = new DataSet();
            if (File.Exists(filename))
            {
                FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                XmlTextReader xtr = new XmlTextReader(fs);
                mainConfig.ReadXmlSchema(xtr);
                xtr.Close();
            }

            // Add the data bindings
            DataTable dt = mainConfig.Tables["MainConfig"];

            //mf.im

        }
    }

    public class VMConfig
    {
        public VMConfig()
        {

        }

        public VMConfig(string name)
        {

        }
    }
}
