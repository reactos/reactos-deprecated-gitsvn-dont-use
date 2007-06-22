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

    public class MainConfig
    {
        private Data data = null;

        private int GetNumberOfVms()
        {
            DataTable dt = data.DataSet.Tables["MainConfig"];
            return dt.Rows.Count;
        }

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

        public void LoadExistingImages(ListView lv)
        {
            DataTable dt = data.DataSet.Tables["MainConfig"];

            int num = GetNumberOfVms();
            for (int i = 0; i < num; i++)
            {
                DataRow dr = dt.Rows[i];

                VirtualMachine vm = new VirtualMachine(data);
                vm.LoadVirtMach((string)dr["Path"]);

                ListViewItem lvi = lv.Items.Add((string)dr["Path"], 0);
                lvi.Tag = vm;
            }
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
            DataRow dr;
            DataTable dt = data.DataSet.Tables["MainConfig"];
            //dt.Rows[index].Delete();
            dt.Rows.RemoveAt(index);
        }

        public void SaveMainConfig()
        {
            string fileName = "Config.xml";
            FileStream fs = new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write);
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


    public class VMConfig
    {
        private Data data = null;

        // default
        public VMConfig(Data dataIn,
                        string name)
        {
            data = dataIn;
            //DataRow dr = MainConfig.DataSet;
        }

        // existing
        public VMConfig(Data dataIn,
                        string name,
                        string existImg,
                        int memSize)
        {
            data = dataIn;
            MessageBox.Show(existImg + " " + memSize);
        }

        // new
        public VMConfig(Data dataIn,
                        string name,
                        string dir,
                        int diskSize,
                        string existImg,
                        int memSize)
        {
            data = dataIn;
            MessageBox.Show(name + " " + dir + " " + diskSize + " " + existImg + " " + memSize);
        }
    }
}
