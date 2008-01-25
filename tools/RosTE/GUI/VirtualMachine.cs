using System;
using System.IO;
using System.Collections.Specialized;
using System.Data;
using System.Xml;
using System.Windows.Forms;
using System.Collections;
using System.Collections.Generic;

namespace RosTEGUI
{
    public struct VirtMachInfo
    {
        public int virtMachID;
        public string name;
        public string machType;
        public string defDir;
        public int memSize;
        public bool setClockToHost;
        public bool cdRomEnable;
        public bool cdRomUsePhys;
        public string cdRomPhysDrv;
        public bool cdRomUseIso;
        public string cdRomIsoImg;
        public bool floppyEnable;
        public bool floppyUsePhys;
        public string floppyPhysDrv;
        public bool floppyUseImg;
        public string floppyIsoImg;
    }


    public class VMHardDrive
    {
        private DataRow hdDataRow;
        private DataSet dataSet;

        #region properties

        public int DiskID
        {
            get { return (int)hdDataRow["DiskID"]; }
        }

        public string Name
        {
            get { return (string)hdDataRow["Name"]; }
            set { hdDataRow["Name"] = value; }
        }

        public string Drive
        {
            get { return (string)hdDataRow["Drive"]; }
        }

        public string Path
        {
            get { return (string)hdDataRow["Path"]; }
            set { hdDataRow["Path"] = value; }
        }

        public int Size
        {
            get { return (int)hdDataRow["Size"]; }
        }

        public bool BootImg
        {
            get { return (bool)hdDataRow["BootImg"]; }
            set { hdDataRow["BootImg"] = value; }
        }
        #endregion

        public override string ToString()
        {
            return Name;
        }

        public bool CreateHardDrive(string nameIn,
                                    string driveIn,
                                    string pathIn,
                                    int sizeIn,
                                    bool bootImgIn)
        {
            bool ret = false;

            try
            {
                DataTable hddt = dataSet.Tables["HardDisks"];
                hdDataRow = hddt.NewRow();
                hdDataRow["DiskID"] = hddt.Rows.Count;
                hdDataRow["Name"] = nameIn;
                hdDataRow["Drive"] = driveIn;
                hdDataRow["Path"] = pathIn;
                hdDataRow["Size"] = sizeIn;
                hdDataRow["BootImg"] = bootImgIn;
                hddt.Rows.Add(hdDataRow);

                ret = true;
            }
            catch (Exception e)
            {
                string message = "Failed to populate hard disk database";
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
            }

            return ret;
        }

        public void DeleteHardDrive(int diskID)
        {
            DataTable dt = dataSet.Tables["HardDisks"];
            //DataRow dr = dt.Rows.Find(diskID); <-- can't seem to apply a primary key??
            // workaround for the above
            foreach (DataRow dr in dt.Rows)
            {
                int id = (int)dr["DiskID"];
                if (id == diskID)
                {
                    dt.Rows.Remove(dr);
                    break;
                }
            }
        }

        public void LoadHardDrive(int index)
        {
            DataTable hddt = dataSet.Tables["HardDisks"];
            hdDataRow = hddt.Rows[index];
        }
    }

    public class VirtualMachine
    {
        private DataRow vmDataRow;
        private List<VirtMachInfo> virtMachInfo = null;
        private ArrayList hardDrives;
        private ArrayList netCards;
        private DataSet dataSet = null;

        public DataSet DataSet
        {
            get { return dataSet; }
        }

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
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
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
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
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
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
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
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
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
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
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
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
            }
        }

        #endregion

        #region database functions

        private bool PopulateVMDatabase(string name,
                                        string dir,
                                        float diskSize,
                                        string existImg,
                                        int memSize)
        {
            DataRow netDataRow;
            bool ret = false;

            try
            {
                DataTable vmdt = dataSet.Tables["VMConfig"];
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

                VMHardDrive vmhd = new VMHardDrive();
                vmhd.CreateHardDrive("Main Drive", "hda", dir, 768, true);
                hardDrives.Add(vmhd);

                DataTable netdt = dataSet.Tables["NetCards"];
                netDataRow = netdt.NewRow();
                netDataRow["CardID"] = netdt.Rows.Count + 1;
                netDataRow["VirtMachID"] = vmDataRow["VirtMachID"];
                netDataRow["Option"] = "user";
                netDataRow["Vlan"] = 0;
                netDataRow["MacAddr"] = string.Empty;
                netDataRow["Model"] = string.Empty;
                netDataRow["Hostname"] = string.Empty;
                netdt.Rows.Add(netDataRow);
                netCards.Add(netDataRow);

                ret = true;
            }
            catch (Exception e)
            {
                string message = "Failed to populate database";
                Debug.LogMessage(message, e.Message, e.StackTrace, true);
            }

            return ret;
        }



        #endregion

        public VirtualMachine()
        {/*
            vmConfig = new VMConfig();
            if (!vmConfig.LoadVirtMachData())
                MessageBox.Show("Failed to load VM Schema");

            hardDrives = new ArrayList(3);
            netCards = new ArrayList();*/
        }

        public override string ToString()
        {
            return Name;
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
            else
            {
                ;//FIXME: create a vm image 'qemu-img.exe create'
            }

            return PopulateVMDatabase(name,
                                      dir,
                                      diskSize,
                                      existImg,
                                      memSize);
        }

        public VMHardDrive AddHardDisk(string nameIn,
                                       string driveIn,
                                       string pathIn,
                                       int sizeIn,
                                       bool bootImgIn)
        {
            VMHardDrive vmhd = new VMHardDrive();
            if (vmhd.CreateHardDrive(nameIn, driveIn, pathIn, sizeIn, bootImgIn))
            {
                hardDrives.Add(vmhd);
                return vmhd;
            }

            return null;
        }

        public void DeleteHardDisk(VMHardDrive vmhd)
        {
            hardDrives.Remove(vmhd);
            vmhd.DeleteHardDrive(vmhd.DiskID);
        }

        public ArrayList GetHardDisks()
        {
            return hardDrives;
        }


        public bool LoadVmSettings()
        {
            bool bRet = false;

            if (dataSet != null)
            {
                if (virtMachInfo == null)
                {
                    virtMachInfo = new List<VirtMachInfo>();
                }

                try
                {
                    foreach (DataRow vmRow in dataSet.Tables["VMConfig"].Rows)
                    {
                        VirtMachInfo vmi = new VirtMachInfo();
                        vmi.virtMachID = (int)vmRow["VirtMachID"];
                        vmi.name = (string)vmRow["Name"];
                        vmi.machType = (string)vmRow["MachType"];
                        vmi.defDir = (string)vmRow["DefDir"];
                        vmi.memSize = (int)vmRow["MemSize"];
                        vmi.setClockToHost = (bool)vmRow["SetClockToHost"];
                        vmi.cdRomEnable = (bool)vmRow["CdRomEnable"];
                        vmi.cdRomUsePhys = (bool)vmRow["CdRomUsePhys"];
                        vmi.cdRomPhysDrv = (string)vmRow["CdRomPhysDrv"];
                        vmi.cdRomUseIso = (bool)vmRow["CdRomUseIso"];
                        vmi.cdRomIsoImg = (string)vmRow["CdRomIsoImg"];
                        vmi.floppyEnable = (bool)vmRow["FloppyEnable"];
                        vmi.floppyUsePhys = (bool)vmRow["FloppyUsePhys"];
                        vmi.floppyPhysDrv = (string)vmRow["FloppyPhysDrv"];
                        vmi.floppyUseImg = (bool)vmRow["FloppyUseImg"];
                        vmi.floppyIsoImg = (string)vmRow["FloppyIsoImg"];

                        virtMachInfo.Add(vmi);
                    }

                    bRet = true;
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("error loading VM config", ex.Message, ex.StackTrace, true);
                }
            }

            return bRet;
        }

        public void SaveVmSettings()
        {
            if (dataSet != null)
            {
                dataSet.Tables["VirtMach"].Rows.Clear();

                try
                {
                    foreach (VirtMachInfo vmi in virtMachInfo)
                    {
                        DataRow vmRow = dataSet.Tables["VirtMach"].NewRow();

                        vmRow["VirtMachID"] = vmi.virtMachID;
                        vmRow["Name"] = vmi.name;
                        vmRow["MachType"] = vmi.machType;
                        vmRow["DefDir"] = vmi.defDir;
                        vmRow["MemSize"] = vmi.memSize;
                        vmRow["SetClockToHost"] = vmi.setClockToHost;
                        vmRow["CdRomEnable"] = vmi.cdRomEnable;
                        vmRow["CdRomUsePhys"] = vmi.cdRomUsePhys;
                        vmRow["CdRomPhysDrv"] = vmi.cdRomPhysDrv;
                        vmRow["CdRomUseIso"] = vmi.cdRomUseIso;
                        vmRow["CdRomIsoImg"] = vmi.cdRomIsoImg;
                        vmRow["FloppyEnable"] = vmi.floppyEnable;
                        vmRow["FloppyUsePhys"] = vmi.floppyUsePhys;
                        vmRow["FloppyPhysDrv"] = vmi.floppyPhysDrv;
                        vmRow["FloppyUseImg"] = vmi.floppyUseImg;
                        vmRow["FloppyIsoImg"] = vmi.floppyIsoImg;
                    }
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("error loading VM config", ex.Message, ex.StackTrace, true);
                }
            }
        }


        public bool LoadVMConfig(string path)
        {
            XmlTextReader xtr = null;
            string fileName = path + "\\Config.xml";
            bool ret = false;

            if (LoadVirtMachSchema())
            {
                if (File.Exists(fileName))
                {
                    try
                    {
                        FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                        xtr = new XmlTextReader(fs);
                        dataSet.ReadXml(xtr, System.Data.XmlReadMode.ReadSchema);
                        xtr.Close();
                        ret = true;
                    }
                    catch (Exception ex)
                    {
                        Debug.LogMessage("error loading VM config", ex.Message, ex.StackTrace, true);
                    }
                    finally
                    {
                        if (xtr != null)
                            xtr.Close();
                    }
                }
            }

            return ret;
        }

        public void SaveVMConfig()
        {
            XmlTextWriter xtw = null;
            string fileName = DefDir + "\\Config.xml";

            if (!Directory.Exists(DefDir))
                Directory.CreateDirectory(DefDir);

            if (dataSet == null)
            {
                if (!LoadVirtMachSchema())
                    return;
            }

            try
            {
                FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
                xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
                dataSet.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
            }
            catch (Exception ex)
            {
                Debug.LogMessage("error saving VM config", ex.Message, ex.StackTrace, true);
            }
            finally
            {
                if (xtw != null)
                    xtw.Close();
            }
        }

        private bool LoadVirtMachSchema()
        {
            XmlTextReader xtr = null;
            string filename = "VMConfig.xsd";
            bool ret = false;

            dataSet = new DataSet();
            if (File.Exists(filename))
            {
                try
                {
                    FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                    xtr = new XmlTextReader(fs);
                    dataSet.ReadXmlSchema(xtr);
                    ret = true;
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("error loading VM config schema", ex.Message, ex.StackTrace, true);
                }
                finally
                {
                    if (xtr != null)
                        xtr.Close();
                }
            }

            return ret;
        }
    }
}