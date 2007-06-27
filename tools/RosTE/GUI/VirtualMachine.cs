using System;
using System.IO;
using System.Collections.Specialized;
using System.Data;
using System.Xml;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class VirtualMachine
    {
        private Data data;
        private DataRow dataRow;

        #region Virtual machine properties

        public int VirtMachID
        {
            get { return GetIntValue("VirtMachID"); }
        }

        public string Name
        {
            get { return GetStringValue("Name"); }
            set { SetStringValue("Name", value); }
        }

        public string MachType
        {
            get { return GetStringValue("MachType"); }
            set { SetStringValue("MachType", value); }
        }

        public string DefDir
        {
            get { return GetStringValue("DefDir"); }
            set { SetStringValue("DefDir", value); }
        }

        public int MemSize
        {
            get { return GetIntValue("MemSize"); }
            set { SetIntValue("MemSize", value); }
        }

        public bool SetClockToHost
        {
            get { return GetBoolValue("SetClockToHost"); }
            set { SetBoolValue("SetClockToHost", value); }
        }

        public bool CdRomEnable
        {
            get { return GetBoolValue("CdRomEnable"); }
            set { SetBoolValue("CdRomEnable", value); }
        }

        public bool CdRomUsePhys
        {
            get { return GetBoolValue("CdRomUsePhys"); }
            set { SetBoolValue("CdRomUsePhys", value); }
        }

        public string CdRomPhysDrv
        {
            get { return GetStringValue("CdRomPhysDrv"); }
            set { SetStringValue("CdRomPhysDrv", value); }
        }

        public bool CdRomUseIso
        {
            get { return GetBoolValue("CdRomUseIso"); }
            set { SetBoolValue("CdRomUseIso", value); }
        }

        public string CdRomIsoImg
        {
            get { return GetStringValue("CdRomIsoImg"); }
            set { SetStringValue("CdRomIsoImg", value); }
        }

        public bool FloppyEnable
        {
            get { return GetBoolValue("FloppyEnable"); }
            set { SetBoolValue("FloppyEnable", value); }
        }

        public bool FloppyUsePhys
        {
            get { return GetBoolValue("FloppyUsePhys"); }
            set { SetBoolValue("FloppyUsePhys", value); }
        }

        public string FloppyPhysDrv
        {
            get { return GetStringValue("FloppyPhysDrv"); }
            set { SetStringValue("FloppyPhysDrv", value); }
        }

        public bool FloppyUseIso
        {
            get { return GetBoolValue("FloppyUseIso"); }
            set { SetBoolValue("FloppyUseIso", value); }
        }

        public string FloppyIsoImg
        {
            get { return GetStringValue("FloppyIsoImg"); }
            set { SetStringValue("FloppyIsoImg", value); }
        }

        #endregion

        #region property helper functions

        private int GetIntValue(string key)
        {
            try
            {
                return (int)dataRow[key];
            }
            catch (ArgumentException e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
                return 0;
            }
        }

        private bool GetBoolValue(string key)
        {
            try
            {
                return (bool)dataRow[key];
            }
            catch (ArgumentException e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
                return false;
            }
        }

        private string GetStringValue(string key)
        {
            try
            {
                return (string)dataRow[key];
            }
            catch (ArgumentException e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
                return string.Empty;
            }
        }

        private void SetIntValue(string key, int value)
        {
            try
            {
                dataRow[key] = value;
            }
            catch (ArgumentException e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
            }
        }

        private void SetBoolValue(string key, bool value)
        {
            try
            {
                dataRow[key] = value;
            }
            catch (ArgumentException e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
            }
        }

        private void SetStringValue(string key, string value)
        {
            try
            {
                dataRow[key] = value;
            }
            catch (ArgumentException e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
            }
        }

        #endregion

        private static void PrintRows(DataTable dt)
        {
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                string str = "row: " + i + ", Name: " + dt.Rows[i]["Name"] + ", DefDir " + dt.Rows[i]["DefDir"];
                MessageBox.Show(str);
            }
        }

        private bool PopulateVMDatabase(string name,
                                        string dir,
                                        float diskSize,
                                        string existImg,
                                        int memSize)
        {
            bool ret = false;

            try
            {
                DataTable dt = data.DataSet.Tables["VMConfig"];
                int i = dt.Rows.Count + 1;
                dataRow = dt.NewRow();
                dataRow["VirtMachID"] = i;
                dataRow["Name"] = name;
                dataRow["MachType"] = "pc";
                dataRow["DefDir"] = dir;
                dataRow["MemSize"] = memSize;
                dataRow["SetClockToHost"] = true;
                dataRow["CdRomEnable"] = true;
                dataRow["CdRomUsePhys"] = true;
                dataRow["CdRomPhysDrv"] = string.Empty;
                dataRow["CdRomUseIso"] = false;
                dataRow["CdRomIsoImg"] = string.Empty;
                dataRow["FloppyEnable"] = true;
                dataRow["FloppyUsePhys"] = true;
                dataRow["FloppyPhysDrv"] = string.Empty;
                dataRow["FloppyUseImg"] = false;
                dataRow["FloppyIsoImg"] = string.Empty;

                dt.Rows.Add(dataRow);

                ret = true;
            }
            catch (Exception e)
            {
                ErrorForm err = new ErrorForm(e.Message);
                err.ShowDialog();
            }

            return ret;
        }

        public override string ToString()
        {
            return Name;
        }

        public VirtualMachine()
        {
            data = new Data();
            if (!data.LoadVirtMachData())
                MessageBox.Show("Failed to load VM Schema");
        }

        // default
        public bool CreateVMConfig(string name)
        { 
            return CreateVMConfig(name,
                                  Directory.GetCurrentDirectory() + "\\Images\\" + name,
                                  0.2f,
                                  "",
                                  256);
        }

        // existing
        public bool CreateVMConfig(string name, string existImg, int memSize)
        {
            return CreateVMConfig(name,
                                  null,
                                  0.0f,
                                  existImg,
                                  memSize);
        }

        // new
        public bool CreateVMConfig(string name,
                                   string dir,
                                   float diskSize,
                                   string existImg,
                                   int memSize)
        {
            if (existImg != "")
            {
                DirectoryInfo di = Directory.GetParent(existImg);
                dir = di.FullName;
            }

            return PopulateVMDatabase(name,
                                      dir,
                                      diskSize,
                                      existImg,
                                      memSize);
        }

        public bool LoadVMConfig(string path)
        {
            bool ret = false;
            string fileName = path + "\\Config.xml";

            if (File.Exists(fileName))
            {
                try
                {
                    FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                    XmlTextReader xtr = new XmlTextReader(fs);
                    data.DataSet.ReadXml(xtr, System.Data.XmlReadMode.ReadSchema);
                    xtr.Close();

                    DataTable dt = data.DataSet.Tables["VMConfig"];
                    dataRow = dt.Rows[0];

                    ret = true;
                }
                catch (Exception e)
                {
                    MessageBox.Show("error loading VM Config.xml: " + e.Message);
                }
            }

            return ret;
        }

        public void SaveVMConfig()
        {
            try
            {
                string fileName = DefDir + "\\Config.xml";
                Directory.CreateDirectory(DefDir);
                FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
                XmlTextWriter xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
                data.DataSet.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
                xtw.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show("error loading VM Config.xml: " + e.Message);
            }
        }
    }
}