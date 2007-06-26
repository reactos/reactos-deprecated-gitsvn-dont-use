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
            get { return (int)dataRow["VirtMachID"]; }
        }

        public string Name
        {
            get { return (string)dataRow["Name"]; }
            set { dataRow["Name"] = value; }
        }

        public string MachType
        {
            get { return (string)dataRow["MachType"]; }
            set { dataRow["MachType"] = value; }
        }

        public string DefDir
        {
            get { return (string)dataRow["DefDir"]; }
            set { dataRow["DefDir"] = value; }
        }

        public int MemSize
        {
            get { return (int)dataRow["MemSize"]; }
            set { dataRow["MemSize"] = value; }
        }

        public bool SetClockToHost
        {
            get { return (bool)dataRow["SetClockToHost"]; }
            set { dataRow["SetClockToHost"] = value; }
        }

        public bool CdRomEnable
        {
            get { return (bool)dataRow["CdRomEnable"]; }
            set { dataRow["CdRomEnable"] = value; }
        }

        public bool CdRomUsePhys
        {
            get { return (bool)dataRow["CdRomUsePhys"]; }
            set { dataRow["CdRomUsePhys"] = value; }
        }

        public string CdRomPhysDrv
        {
            get { return (string)dataRow["CdRomPhysDrv"]; }
            set { dataRow["CdRomPhysDrv"] = value; }
        }

        public bool CdRomUseIso
        {
            get { return (bool)dataRow["CdRomUseIso"]; }
            set { dataRow["CdRomUseIso"] = value; }
        }

        public string CdRomIsoImg
        {
            get { return (string)dataRow["CdRomIsoImg"]; }
            set { dataRow["CdRomIsoImg"] = value; }
        }

        public bool FloppyEnable
        {
            get { return (bool)dataRow["FloppyEnable"]; }
            set { dataRow["FloppyEnable"] = value; }
        }

        public bool FloppyUsePhys
        {
            get { return (bool)dataRow["FloppyUsePhys"]; }
            set { dataRow["FloppyUsePhys"] = value; }
        }

        public string FloppyPhysDrv
        {
            get { return (string)dataRow["FloppyPhysDrv"]; }
            set { dataRow["FloppyPhysDrv"] = value; }
        }

        public bool FloppyUseIso
        {
            get { return (bool)dataRow["FloppyUseIso"]; }
            set { dataRow["FloppyUseIso"] = value; }
        }

        public string FloppyIsoImg
        {
            get { return (string)dataRow["FloppyIsoImg"]; }
            set { dataRow["FloppyIsoImg"] = value; }
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
            bool ret = false;

            if (existImg != "")
            {
                DirectoryInfo di = Directory.GetParent(existImg);
                dir = di.FullName;
            }

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
            dataRow["CdRomUsePhys"] = false;
            dataRow["CdRomPhyDrv"] = "R:";
            dataRow["CdRomUseIso"] = true;
            dataRow["CdRomIsoImg"] = "err";
            dataRow["FloppyEnable"] = true;
            dataRow["FloppyUsePhys"] = true;
            dataRow["FloppyPhyDrive"] = "A:";
            dataRow["FloppyUseImg"] = false;
            dataRow["FloppyIsoImg"] = "err";

            dt.Rows.Add(dataRow);

            return ret;
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