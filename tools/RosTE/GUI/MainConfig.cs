using System.Data;
using System.IO;
using System.Xml;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class MainConfig
    {
        private Data data = null;

        private static void PrintRows(DataTable dt)
        {
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                string str = "row: " + i + ", VMConfigID: " + dt.Rows[i]["VMConfigID"] + ", Path " + dt.Rows[i]["Path"];
                MessageBox.Show(str);
            }
        }

        public MainConfig(Data dataIn)
        {
            data = dataIn;

            // FIXME: unfortunatley, .NET doesn't support binding of
            // listview controls, we'll need to implement this manually
            // and remove the need for LoadExistingImages / AddVirtMach / DeleteVirtMach
        }

        public int GetNumberOfVms()
        {
            DataTable dt = data.DataSet.Tables["MainConfig"];
            return dt.Rows.Count;
        }

        public VirtualMachine GetExistingImage(int index)
        {
            DataTable dt = data.DataSet.Tables["MainConfig"];
            DataRow dr = dt.Rows[index];

            VirtualMachine vm = new VirtualMachine(data);
            vm.LoadVirtMach((string)dr["Path"]);
            return vm;
        }

        public int AddVirtMach(string Path)
        {
            int i;
            DataRow dr;
            DataTable dt = data.DataSet.Tables["MainConfig"];
            i = dt.Rows.Count + 1;
            dr = dt.NewRow();
            dr["VMConfigID"] = i;
            dr["Path"] = Path;
            dt.Rows.Add(dr);
            return i;
        }

        public void DeleteVirtMach(int index)
        {
            DataTable dt = data.DataSet.Tables["MainConfig"];
            dt.Rows.RemoveAt(index);
        }

        public void SaveMainConfig()
        {
            string fileName = "Config.xml";
            FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            XmlTextWriter xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
            data.DataSet.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
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
                data.DataSet.ReadXml(xtr, System.Data.XmlReadMode.ReadSchema);
                xtr.Close();
                ret = true;
            }

            return ret;
        }
    }
}