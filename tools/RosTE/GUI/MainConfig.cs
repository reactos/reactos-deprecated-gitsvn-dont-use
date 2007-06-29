using System.Data;
using System.IO;
using System.Xml;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class MainConfig
    {
        private Data data = null;
        private DataRow drSettings;

        #region properties

        public string QemuPath
        {
            get { return (string)drSettings["QemuPath"]; }
            set { drSettings["QemuPath"] = value; }
        }

        public string VdkPath
        {
            get { return (string)drSettings["VdkPath"]; }
            set { drSettings["VdkPath"] = value; }
        }

        public string DefVmPath
        {
            get { return (string)drSettings["DefVmPath"]; }
            set { drSettings["DefVmPath"] = value; }
        }

        public int UpdateSched
        {
            get { return (int)drSettings["UpdateSched"]; }
            set { drSettings["UpdateSched"] = value; }
        }

        public bool AppDebug
        {
            get { return (bool)drSettings["AppDebug"]; }
            set { drSettings["AppDebug"] = value; }
        }

        #endregion

        #region VM Image functions

        public string GetExistingImage(int index)
        {
            DataTable dt = data.DataSet.Tables["VirtMach"];
            DataRow dr = dt.Rows[index];

            return (string)dr["Path"];
        }

        public int AddVirtMach(string Path)
        {
            int i;
            DataRow dr;
            DataTable dt = data.DataSet.Tables["VirtMach"];
            i = dt.Rows.Count + 1;
            dr = dt.NewRow();
            dr["VMConfigID"] = i;
            dr["Path"] = Path;
            dt.Rows.Add(dr);
            return i;
        }

        public void DeleteVirtMach(int index)
        {
            DataTable dt = data.DataSet.Tables["VirtMach"];
            dt.Rows.RemoveAt(index);
        }

        public int GetNumberOfVms()
        {
            DataTable dt = data.DataSet.Tables["VirtMach"];
            return dt.Rows.Count;
        }

        #endregion

        #region Settings functions

        public void CreateSettings()
        {
            DataTable dt = data.DataSet.Tables["Settings"];
            drSettings = dt.NewRow();
            // FIXME: these should be assigned by the installer
            drSettings["QemuPath"] = @"C:\Program Files\RosTE\QEmu";
            drSettings["VdkPath"] = @"C:\Program Files\RosTE\VDK";
            drSettings["DefVmPath"] = @"C:\Program Files\RosTE\Images";
            drSettings["UpdateSched"] = 3;
            drSettings["AppDebug"] = false;
            dt.Rows.Add(drSettings);
        }

        public void LoadSettings()
        {
            DataTable dt = data.DataSet.Tables["Settings"];
            drSettings = dt.Rows[0];
        }

        #endregion

        public MainConfig(Data dataIn)
        {
            data = dataIn;

            // FIXME: unfortunatley, .NET doesn't support binding of
            // listview controls, we'll need to implement this manually
            // and remove the need for LoadExistingImages / AddVirtMach / DeleteVirtMach
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

        public void SaveMainConfig()
        {
            string fileName = "Config.xml";
            FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            XmlTextWriter xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
            data.DataSet.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
            xtw.Close();
        }
    }
}