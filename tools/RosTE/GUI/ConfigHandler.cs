using System;
using System.Data;
using System.IO;
using System.Xml;
using System.Windows.Forms;
using System.Collections.Generic;

namespace RosTEGUI
{
    public struct VirtMach
    {
        public int id;
        public string path;

        public VirtMach(int idIn, string pathIn)
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

        List<VirtMach> virtMachs = null;

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
        public string GetExistingImage(int index)
        {
            foreach (VirtMach vm in virtMachs)
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
                    virtMachs = new List<VirtMach>();
                }

                try
                {
                    foreach (DataRow dr in dataSet.Tables["VirtMach"].Rows)
                    {
                        virtMachs.Add(new VirtMach((int)dr["VMConfigID"], (string)dr["Path"]));
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

                    foreach (VirtMach vm in virtMachs)
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
                virtMachs.Add(new VirtMach(id, pathIn));
            }

            return virtMachs.Count;
        }

        public bool DeleteVirtMach(int index)
        {
            bool bRet = false;

            foreach (VirtMach vm in virtMachs)
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
}