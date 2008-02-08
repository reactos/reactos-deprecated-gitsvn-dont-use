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

        public List<HardDriveInfo> hardDrives;
        public List<NetCardInfo> netCards;
    }

    public struct HardDriveInfo
    {
        public int diskID;
        public string name;
        public string drive;
        public string path;
        public int size;
        public bool bootImg;
    }

    public struct NetCardInfo
    {
        public int cardID;
        public int virtMachID;
        public string option;
        public int vlan;
        public string macAddr;
        public string model;
        public string hostname;
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

    public class VMNetCard
    {
    }

    public class VirtualMachine
    {
        private VirtMachInfo vmInfo;

        private ArrayList netCards;
        private ArrayList hardDrives;

        #region Virtual machine properties
        public int VirtMachID
        {
            get { return vmInfo.virtMachID; }
        }
        public string Name
        {
            get { return vmInfo.name; }
            set { vmInfo.name = value; }
        }
        public string MachType
        {
            get { return vmInfo.machType; }
            set { vmInfo.machType = value; }
        }
        public string DefDir
        {
            get { return vmInfo.defDir; }
            set { vmInfo.defDir = value; }
        }
        public int MemSize
        {
            get { return vmInfo.memSize; }
            set { vmInfo.memSize = value; }
        }
        public bool SetClockToHost
        {
            get { return vmInfo.setClockToHost; }
            set { vmInfo.setClockToHost = value; }
        }
        public bool CdRomEnable
        {
            get { return vmInfo.cdRomEnable; }
            set { vmInfo.cdRomEnable = value; }
        }
        public bool CdRomUsePhys
        {
            get { return vmInfo.cdRomUsePhys; }
            set { vmInfo.cdRomUsePhys = value; }
        }
        public string CdRomPhysDrv
        {
            get { return vmInfo.cdRomPhysDrv; }
            set { vmInfo.cdRomPhysDrv = value; }
        }
        public bool CdRomUseIso
        {
            get { return vmInfo.cdRomUseIso; }
            set { vmInfo.cdRomUseIso = value; }
        }
        public string CdRomIsoImg
        {
            get { return vmInfo.cdRomIsoImg; }
            set { vmInfo.cdRomIsoImg = value; }
        }
        public bool FloppyEnable
        {
            get { return vmInfo.floppyEnable; }
            set { vmInfo.floppyEnable = value; }
        }
        public bool FloppyUsePhys
        {
            get { return vmInfo.floppyUsePhys; }
            set { vmInfo.floppyUsePhys = value; }
        }
        public string FloppyPhysDrv
        {
            get { return vmInfo.floppyPhysDrv; }
            set { vmInfo.floppyPhysDrv = value; }
        }
        public bool FloppyUseImg
        {
            get { return vmInfo.floppyUseImg; }
            set { vmInfo.floppyUseImg = value; }
        }
        public string FloppyIsoImg
        {
            get { return vmInfo.floppyIsoImg; }
            set { vmInfo.floppyIsoImg = value; }
        }
        public VirtMachInfo VMInfo
        {
            get { return vmInfo; }
        }
        public List<HardDriveInfo> HardDrives
        {
            get { return vmInfo.hardDrives; }
        }
        public List<NetCardInfo> NetCards
        {
            get { return vmInfo.netCards; }
        }
        #endregion

        public VirtualMachine()
        {
            vmInfo = new VirtMachInfo();
            CreateDefaultConfig();
        }

        public VirtualMachine(VirtMachInfo vmInfoIn)
        {
            vmInfo = vmInfoIn;
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

            return false;
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

        private void CreateDefaultConfig()
        {
            vmInfo.virtMachID = 0;
            vmInfo.name = "New VM " + vmInfo.virtMachID.ToString();
            vmInfo.machType = string.Empty;
            vmInfo.defDir = string.Empty;
            vmInfo.memSize = 0;
            vmInfo.setClockToHost = true;
            vmInfo.cdRomEnable = true;
            vmInfo.cdRomUsePhys = true;
            vmInfo.cdRomPhysDrv = string.Empty;
            vmInfo.cdRomUseIso = false;
            vmInfo.cdRomIsoImg = string.Empty;
            vmInfo.floppyEnable = false;
            vmInfo.floppyUsePhys = false;
            vmInfo.floppyPhysDrv = string.Empty;
            vmInfo.floppyUseImg = false;
            vmInfo.floppyIsoImg = string.Empty;

            HardDriveInfo hdi = new HardDriveInfo();
            hdi.bootImg = true;
            hdi.diskID = 1;
            hdi.drive = string.Empty;
            hdi.name = "root";
            hdi.path = string.Empty;
            hdi.size = 0;

            vmInfo.hardDrives = new List<HardDriveInfo>();
            vmInfo.hardDrives.Add(hdi);

            NetCardInfo nci = new NetCardInfo();
            nci.cardID = 1;
            nci.hostname = string.Empty;
            nci.macAddr = string.Empty;
            nci.model = string.Empty;
            nci.option = string.Empty;
            nci.virtMachID = 0;
            nci.vlan = 0;

            vmInfo.netCards = new List<NetCardInfo>();
            vmInfo.netCards.Add(nci);
        }
    }
}