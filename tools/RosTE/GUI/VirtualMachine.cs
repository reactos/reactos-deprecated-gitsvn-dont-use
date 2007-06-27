using System;
using System.IO;
using System.Collections.Specialized;
using System.Data;
using System.Xml;
using System.Windows.Forms;
using System.Collections;

namespace RosTEGUI
{
    public class VirtualMachine
    {
        private Data data;
        private DataRow vmDataRow;
        private DataRow hdDataRow;
        private DataRow netDataRow;
        private ArrayList hardDrives;


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
                return (int)vmDataRow[key];
            }
            catch (ArgumentException e)
            {
                string message = "Failed to get " + key + " value";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
                err.ShowDialog();
                return 0;
            }
        }

        private bool GetBoolValue(string key)
        {
            try
            {
                return (bool)vmDataRow[key];
            }
            catch (ArgumentException e)
            {
                string message = "Failed to get " + key + " value";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
                err.ShowDialog();
                return false;
            }
        }

        private string GetStringValue(string key)
        {
            try
            {
                return (string)vmDataRow[key];
            }
            catch (ArgumentException e)
            {
                string message = "Failed to get " + key + " value";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
                err.ShowDialog();
                return string.Empty;
            }
        }

        private void SetIntValue(string key, int value)
        {
            try
            {
                vmDataRow[key] = value;
            }
            catch (ArgumentException e)
            {
                string message = "Failed to set " + key + " value";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
                err.ShowDialog();
            }
        }

        private void SetBoolValue(string key, bool value)
        {
            try
            {
                vmDataRow[key] = value;
            }
            catch (ArgumentException e)
            {
                string message = "Failed to set " + key + " value";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
                err.ShowDialog();
            }
        }

        private void SetStringValue(string key, string value)
        {
            try
            {
                vmDataRow[key] = value;
            }
            catch (ArgumentException e)
            {
                string message = "Failed to set " + key + " value";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
                err.ShowDialog();
            }
        }

        #endregion

        private bool PopulateVMDatabase(string name,
                                        string dir,
                                        float diskSize,
                                        string existImg,
                                        int memSize)
        {
            bool ret = false;

            try
            {
                DataTable vmdt = data.DataSet.Tables["VMConfig"];
                vmDataRow = vmdt.NewRow();
                vmDataRow["VirtMachID"] = vmdt.Rows.Count + 1;
                vmDataRow["Name"] = name;
                vmDataRow["MachType"] = "pc";
                vmDataRow["DefDir"] = dir;
                vmDataRow["MemSize"] = memSize;
                vmDataRow["SetClockToHost"] = true;
                vmDataRow["CdRomEnable"] = true;
                vmDataRow["CdRomUsePhys"] = true;
                vmDataRow["CdRomPhysDrv"] = string.Empty;
                vmDataRow["CdRomUseIso"] = false;
                vmDataRow["CdRomIsoImg"] = string.Empty;
                vmDataRow["FloppyEnable"] = true;
                vmDataRow["FloppyUsePhys"] = true;
                vmDataRow["FloppyPhysDrv"] = string.Empty;
                vmDataRow["FloppyUseImg"] = false;
                vmDataRow["FloppyIsoImg"] = string.Empty;
                vmdt.Rows.Add(vmDataRow);

                DataTable hddt = data.DataSet.Tables["HardDisks"];
                hdDataRow = hddt.NewRow();
                hdDataRow["DiskID"] = hddt.Rows.Count + 1;
                hdDataRow["VirtMachID"] = vmDataRow["VirtMachID"];
                hdDataRow["Name"] = "hda";
                hdDataRow["Path"] = string.Empty;
                hdDataRow["Size"] = 0;
                hddt.Rows.Add(hdDataRow);

                DataTable netdt = data.DataSet.Tables["NetCards"];
                netDataRow = netdt.NewRow();
                netDataRow["CardID"] = netdt.Rows.Count + 1;
                netDataRow["VirtMachID"] = vmDataRow["VirtMachID"];
                netDataRow["Option"] = "hda";
                netDataRow["Vlan"] = 0;
                netDataRow["MacAddr"] = string.Empty;
                netDataRow["Model"] = string.Empty;
                netDataRow["Hostname"] = string.Empty;
                netdt.Rows.Add(netDataRow);

                ret = true;
            }
            catch (Exception e)
            {
                string message = "Failed to populate database";
                ErrorForm err = new ErrorForm(message, e.Message, e.StackTrace);
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

                    DataTable vmdt = data.DataSet.Tables["VMConfig"];
                    vmDataRow = vmdt.Rows[0];

                    DataTable hddt = data.DataSet.Tables["HardDisks"];
                    hdDataRow = hddt.Rows[0];

                    ret = true;
                }
                catch (Exception e)
                {
                    MessageBox.Show("error loading the VM Config.xml: " + e.Message);
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