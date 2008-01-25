using System;
using System.Data;
using System.IO;
using System.Xml;
using System.Windows.Forms;
using System.Collections.Generic;

namespace RosTEGUI
{
    public struct MainVmInfo
    {
        public int id;
        public string path;

        public MainVmInfo(int idIn, string pathIn)
        {
            id = idIn;
            path = pathIn;
        }
    }

    public class MainConfig
    {
        private DataSet dataSet = null;

        private string qemuPath;
        private string vdkPath;
        private string defVmPath;
        private int updateSched;
        private bool appDebug;

        List<MainVmInfo> virtMachs = null;

        #region properties
        public string QemuPath
        {
            get { return qemuPath; }
            set { qemuPath = value; }
        }

        public string VdkPath
        {
            get { return vdkPath; }
            set { vdkPath = value; }
        }

        public string DefVmPath
        {
            get { return defVmPath; }
            set { defVmPath = value; }
        }

        public int UpdateSched
        {
            get { return updateSched; }
            set { updateSched = value; }
        }

        public bool AppDebug
        {
            get { return appDebug; }
            set { appDebug = value; }
        }
        #endregion

        #region virtmach element
        public string GetImagePath(int index)
        {
            foreach (MainVmInfo vm in virtMachs)
            {
                if (vm.id == index)
                    return vm.path;
            }

            return null;
        }

        private bool LoadVirtMachs()
        {
            bool bRet = false;

            if (dataSet != null)
            {
                if (virtMachs == null)
                {
                    virtMachs = new List<MainVmInfo>();
                }

                try
                {
                    foreach (DataRow dr in dataSet.Tables["VirtMach"].Rows)
                    {
                        virtMachs.Add(new MainVmInfo((int)dr["VMConfigID"], (string)dr["Path"]));
                    }

                    bRet = true;
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("Failed loading virtual machines", ex.Message, ex.StackTrace, true);
                }
            }

            return bRet;
        }

        private void SaveVirtMachs()
        {
            if (dataSet != null && virtMachs != null)
            {
                try
                {
                    DataTable virtMachTable = dataSet.Tables["VirtMach"];
                    virtMachTable.Clear();

                    foreach (MainVmInfo vm in virtMachs)
                    {
                        DataRow dr = virtMachTable.NewRow();
                        dr["VMConfigID"] = vm.id;
                        dr["Path"] = vm.path;

                        virtMachTable.Rows.Add(dr);
                    }
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("Failed to save virtual machine table", ex.Message, ex.StackTrace, true);
                }
            }
        }

        public int AddVirtMach(string pathIn)
        {
            if (dataSet != null && virtMachs != null)
            {
                int id = virtMachs.Count;
                virtMachs.Add(new MainVmInfo(id, pathIn));
            }

            return virtMachs.Count;
        }

        public bool DeleteVirtMach(int index)
        {
            bool bRet = false;

            foreach (MainVmInfo vm in virtMachs)
            {
                if (vm.id == index)
                {
                    virtMachs.Remove(vm);
                    bRet = true;
                }
            }

            return bRet;
        }

        public int GetNumberOfVms()
        {
            return virtMachs.Count;
        }
        #endregion

        #region settings element
        public void CreateSettings()
        {
            if (dataSet != null)
            {
                DataTable settingsTable;
                DataRow settingsRow;

                try
                {
                    settingsTable = dataSet.Tables["Settings"];

                    if (settingsTable.Rows.Count == 0)
                    {
                        settingsRow = settingsTable.NewRow();
                    }
                    else
                    {
                        settingsRow = settingsTable.Rows[0];
                    }

                    // FIXME: these should be assigned by the installer
                    settingsRow["QemuPath"] = @"C:\Program Files\RosTE\QEmu";
                    settingsRow["VdkPath"] = @"C:\Program Files\RosTE\VDK";
                    settingsRow["DefVmPath"] = @"C:\Program Files\RosTE\Images";
                    settingsRow["UpdateSched"] = 3;
                    settingsRow["AppDebug"] = false;

                    settingsTable.Rows.Add(settingsRow);
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("failed to create new settings", ex.Message, ex.StackTrace, true);
                }
            }
        }
        
        public bool LoadSettings()
        {
            bool bRet = false;

            if (dataSet != null)
            {
                try
                {
                    DataRow settingsRow = dataSet.Tables["Settings"].Rows[0];

                    qemuPath = (string)settingsRow["QemuPath"];
                    vdkPath = (string)settingsRow["VdkPath"];
                    defVmPath = (string)settingsRow["DefVmPath"];
                    updateSched = (int)settingsRow["UpdateSched"];
                    appDebug = (bool)settingsRow["AppDebug"];

                    bRet = LoadVirtMachs();
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("Failed to read settings from mainConf dataset", ex.Message);
                }
            }

            return bRet;
        }

        public void SaveSettings()
        {
            if (dataSet != null)
            {
                try
                {
                    DataRow settingsRow = dataSet.Tables["Settings"].Rows[0];
                    settingsRow["QemuPath"] = qemuPath;
                    settingsRow["VdkPath"] = vdkPath;
                    settingsRow["DefVmPath"] = defVmPath;
                    settingsRow["UpdateSched"] = updateSched;
                    settingsRow["AppDebug"] = appDebug;
                }
                catch (Exception ex)
                {
                    Debug.LogMessage("Failed to read settings from mainConf dataset", ex.Message);
                }
            }
        }
        #endregion

        public bool LoadMainConfig()
        {
            XmlTextReader xtr = null;
            string fileName = "Config.xml";
            bool ret = false;

            if (LoadMainSchema())
            {
                if (File.Exists(fileName))
                {
                    try
                    {
                        FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                        xtr = new XmlTextReader(fs);
                        dataSet.ReadXml(xtr, System.Data.XmlReadMode.ReadSchema);
                        ret = true;
                    }
                    catch (Exception ex)
                    {
                        Debug.LogMessage("Failed to load main config file", ex.Message, ex.StackTrace, true);
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

        public void SaveMainConfig()
        {
            XmlTextWriter xtw = null;
            string fileName = "Config.xml";

            if (dataSet == null)
            {
                if (!LoadMainSchema())
                    return;
            }

            SaveVirtMachs();

            try
            {
                FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
                xtw = new XmlTextWriter(fs, System.Text.Encoding.Unicode);
                dataSet.WriteXml(xtw, System.Data.XmlWriteMode.WriteSchema);
            }
            catch (Exception ex)
            {
                Debug.LogMessage("Failed to save main config file", ex.Message, ex.StackTrace, true);
            }
            finally
            {
                if (xtw != null)
                    xtw.Close();
            }
        }

        #region private methods
        private bool LoadMainSchema()
        {
            XmlTextReader xtr = null;
            string filename = "MainConfig.xsd";
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
                    Debug.LogMessage("error loading main config schema", ex.Message, ex.StackTrace, true);
                }
                finally
                {
                    if (xtr != null)
                        xtr.Close();
                }
            }

            return ret;
        }
        #endregion
    }


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

    public class VirtMachConfig
    {
        private DataSet dataSet = null;
        private List<VirtMachInfo> virtMachInfo = null;

        public List<VirtMachInfo> VMInfo
        {
            get { return virtMachInfo; }
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

        public void SaveVMConfig(string path)
        {
            XmlTextWriter xtw = null;
            string fileName = path + "\\Config.xml";

            if (!Directory.Exists(fileName))
                Directory.CreateDirectory(fileName);

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