using System.Data;
using System.IO;
using System.Xml;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class MainConfig
    {
        private DataSet data;

        private int GetNumberOfVms()
        {
            DataTable dt = data.Tables["MainConfig"];
            return dt.Rows.Count;
        }

        public MainConfig()
        {
            string filename = "Config.xsd";
            data = new DataSet();
            if (File.Exists(filename))
            {
                FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                XmlTextReader xtr = new XmlTextReader(fs);
                data.ReadXmlSchema(xtr);
                xtr.Close();

                // FIXME: unfortunatley, .NET doesn't support binding of
                // listview controls, we'll need to implement this manually
                // and remove the need for LoadNewImage / LoadExistingImages
            }
        }

        public void LoadExistingImages(ListView lv)
        {
            DataTable dt = data.Tables["MainConfig"];

            int num = GetNumberOfVms();
            for (int i = 0; i < num; i++)
            {
                DataRow dr = dt.Rows[i];
                lv.Items.Add((string)dr["Path"], 0);
            }
        }

        public void LoadNewImage(ListView lv)
        {
            DataTable dt = data.Tables["MainConfig"];

            int num = GetNumberOfVms();

            DataRow dr = dt.Rows[num - 1];
            lv.Items.Add((string)dr["Path"], 0);

        }

        public void AddVirtMach(string Path)
        {
            int i;
            DataRow dr;
            DataTable dt = data.Tables["MainConfig"];
            i = dt.Rows.Count + 1;
            dr = dt.NewRow();
            dr["ID"] = i;
            dr["Path"] = Path;
            dt.Rows.Add(dr);
        }

        public void SaveMainConfig()
        {
            string fileName = "Config.xml";
            FileStream fs = new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write);
            XmlTextWriter xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
            data.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
            xtw.Close();
        }

        public bool LoadMainConfig()
        {
            bool ret = false;
            string fileName = "Config.xml";

            if (File.Exists(fileName))
            {
                FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                XmlTextReader xtr = new XmlTextReader(fs);
                data.ReadXml(xtr, System.Data.XmlReadMode.ReadSchema);
                xtr.Close();
                ret = true;
            }

            return ret;
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
