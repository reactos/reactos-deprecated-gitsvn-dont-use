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
        
        private ArrayList hardDrives;
        private ArrayList netCards;

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
            bool ret = false;

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
    }
}