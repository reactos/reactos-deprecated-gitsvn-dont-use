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
            set { dataRow["Name"] = value; }
        }

        public string MachType
        {
            get { return GetStringValue("MachType"); }
            set { dataRow["MachType"] = value; }
        }

        public string DefDir
        {
            get { return GetStringValue("DefDir"); }
            set { dataRow["DefDir"] = value; }
        }

        public int MemSize
        {
            get { return GetIntValue("MemSize"); }
            set { dataRow["MemSize"] = value; }
        }

        public bool SetClockToHost
        {
            get { return GetBoolValue("SetClockToHost"); }
            set { dataRow["SetClockToHost"] = value; }
        }

        public bool CdRomEnable
        {
            get { return GetBoolValue("CdRomEnable"); }
            set { dataRow["CdRomEnable"] = value; }
        }

        public bool CdRomUsePhys
        {
            get { return GetBoolValue("CdRomUsePhys"); }
            set { dataRow["CdRomUsePhys"] = value; }
        }

        public string CdRomPhysDrv
        {
            get { return GetStringValue("CdRomPhysDrv"); }
            set { dataRow["CdRomPhysDrv"] = value; }
        }

        public bool CdRomUseIso
        {
            get { return GetBoolValue("CdRomUseIso"); }
            set { dataRow["CdRomUseIso"] = value; }
        }

        public string CdRomIsoImg
        {
            get { return GetStringValue("CdRomIsoImg"); }
            set { dataRow["CdRomIsoImg"] = value; }
        }

        public bool FloppyEnable
        {
            get { return GetBoolValue("FloppyEnable"); }
            set { dataRow["FloppyEnable"] = value; }
        }

        public bool FloppyUsePhys
        {
            get { return GetBoolValue("FloppyUsePhys"); }
            set { dataRow["FloppyUsePhys"] = value; }
        }

        public string FloppyPhysDrv
        {
            get { return GetStringValue("FloppyPhysDrv"); }
            set { dataRow["FloppyPhysDrv"] = value; }
        }

        public bool FloppyUseIso
        {
            get { return GetBoolValue("FloppyUseIso"); }
            set { dataRow["FloppyUseIso"] = value; }
        }

        public string FloppyIsoImg
        {
            get { return GetStringValue("FloppyIsoImg"); }
            set { dataRow["FloppyIsoImg"] = value; }
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
                dataRow["CdRomPhysDrv"] = "R:";
                dataRow["CdRomUseIso"] = false;
                dataRow["CdRomIsoImg"] = string.Empty;
                dataRow["FloppyEnable"] = true;
                dataRow["FloppyUsePhys"] = true;
                dataRow["FloppyPhyDrive"] = "A:";
                dataRow["FloppyUseImg"] = false;
                dataRow["FloppyIsoImg"] = string.Empty;

                dt.Rows.Add(dataRow);

                ret = true;
            }
            catch (Exception e)
            {
                MessageBox.Show("error populating VM database: " + e.Message);
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