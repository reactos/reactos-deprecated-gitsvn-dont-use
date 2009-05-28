using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Configuration;
using System.Windows.Forms;
using System.Drawing.Design;
using System.IO;
using System.IO.Ports;

namespace RosDBG
{
    public partial class Settings : Form
    {
        public class SettingsPropertyValues : ApplicationSettingsBase
        {
            private SerialConnSettings _serialconnsettings;
            private PipeConnSettings _pipeconnsettings;

            [UserScopedSetting, DefaultSettingValue("0")]
            [Browsable(false)]
            public Connect.ConnectionType SelectedConnType
            {
                get { return (Connect.ConnectionType) this["SelectedConnType"]; }
                set { this["SelectedConnType"] = value; }
            }
            
            /* Hack to work around a crash (bug in .net?) 
             * using a TypeConverter in a class which is derived from
             * ApplicationSettingsBase results in a crash
             */
            [UserScopedSetting, DefaultSettingValue("COM1")]
            [Browsable(false)]
            public string Port
            {
                get { return this["Port"].ToString(); }
                set { this["Port"] = value; }
            }

            [UserScopedSetting, DefaultSettingValue("115200")]
            [Browsable(false)]
            public string Baudrate
            {
                get { return this["Baudrate"].ToString(); }
                set { this["Baudrate"] = value; }
            }

            [UserScopedSetting, DefaultSettingValue(@"RosDbg")]
            [Browsable(false)]
            public string Pipe
            {
                get { return this["Pipe"].ToString(); }
                set { this["Pipe"] = value; }
            }

            [UserScopedSetting, DefaultSettingValue(@"Client")]
            [Browsable(false)]
            public string Mode
            {
                get { return this["Mode"].ToString(); }
                set { this["Mode"] = value; }
            }

            /* end of hack */

            [CategoryAttribute("Directories"), DescriptionAttribute("Directory settings")]
            [UserScopedSetting, DefaultSettingValue("."), Editor(typeof(DirectoryEditor), typeof(UITypeEditor))]
            public string SourceDirectory
            {
                get { return this["SourceDirectory"].ToString(); }
                set 
                {
                    if (!File.Exists(value + "\\ReactOS-generic.rbuild"))
                        MessageBox.Show("Can not find ReactOS sources in this directory!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    this["SourceDirectory"] = value;
                }
            }

            [CategoryAttribute("Directories"), DescriptionAttribute("Directory settings")]
            [UserScopedSetting,DefaultSettingValue(".\\output-i386"),Editor(typeof(DirectoryEditor), typeof(UITypeEditor))]
            public string OutputDirectory
            {
                get { return this["OutputDirectory"].ToString(); }
                set
                {
                    if (!File.Exists(value + "\\ntoskrnl\\ntoskrnl.nostrip.exe"))
                        MessageBox.Show("Can not find .nostrip files!\nThe Debugger will not work properly without them.\n\nPlease enable building of .nostrip files in RosBE options." , "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);  
                    this["OutputDirectory"] = value; 
                }
            }

            [CategoryAttribute("Connection"), DescriptionAttribute("Connection settings")]
            public SerialConnSettings Serial
            {
                get { return new SerialConnSettings(Port, Baudrate, this); }
                set { _serialconnsettings = value; }
            }

            [CategoryAttribute("Connection"), DescriptionAttribute("Connection settings")]
            public PipeConnSettings NamedPipe
            {
                get { return new PipeConnSettings(Mode, Pipe, this); }
                set { _pipeconnsettings = value; }
            }

            [Browsable(true)]
            [CategoryAttribute("Logging"), DescriptionAttribute("Turn application logging on or off")]
            [UserScopedSetting, DefaultSettingValue("true")]
            [TypeConverter(typeof(AppLoggingSelection))]
            public string AppLogging
            {
                get { return this["AppLogging"].ToString(); }
                set { this["AppLogging"] = value; }
            }

            [CategoryAttribute("Logging"), DescriptionAttribute("The log file in which to store the app log")]
            [UserScopedSetting, DefaultSettingValue(@".\rosdbg.log"), Editor(typeof(FileEditor), typeof(UITypeEditor))]
            public string AppLogFile
            {
                get { return this["AppLogFile"].ToString(); }
                set { this["AppLogFile"] = value; }
            }

            public SettingsPropertyValues()
            {
                Reload();
            }
        }

        #region TypeConverter
        public class ComPortConverter : StringConverter 
        {
            public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
            {
                return true;
            }

            public override System.ComponentModel.TypeConverter.StandardValuesCollection GetStandardValues(ITypeDescriptorContext context) 
            {
                return new StandardValuesCollection((string[]) SerialPort.GetPortNames());
            }
        }

        public class PipeConverter : StringConverter
        {
            public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
            {
                return true;
            }

            public override System.ComponentModel.TypeConverter.StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
            {
                return new StandardValuesCollection(new string[] {"Client", "Server"} );
            }
        }

        internal class AppLoggingSelection : StringConverter
        {
            public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
            {
                return true;
            }

            public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
            {
                return new StandardValuesCollection(new string[] { "true", "false" });
            }
        }
        #endregion

        [TypeConverterAttribute(typeof(ExpandableObjectConverter))]
        public class SerialConnSettings 
        {
            private string _Port;
            private string _Baudrate;
            private SettingsPropertyValues _Parent;

            public SerialConnSettings(string Port, string Baud, SettingsPropertyValues parent)
            {
                _Port = Port;
                _Baudrate = Baud;
                _Parent = parent;
            }

            public override string ToString()
            {
                return this.Port + ";" + this.Baudrate;
            }

            [TypeConverter(typeof(ComPortConverter))]
            public string Port
            {
                get { return _Port; }
                set { _Port = value; _Parent.Port = _Port; }
            }

            public string Baudrate
            {
                get { return _Baudrate; }
                set { _Baudrate = value; _Parent.Baudrate = _Baudrate; }
            }
        }

        [TypeConverterAttribute(typeof(ExpandableObjectConverter))]
        public class PipeConnSettings
        {
            private string _Pipe;
            private string _Mode;
            private SettingsPropertyValues _Parent;

            public PipeConnSettings(string Mode, string Pipe, SettingsPropertyValues parent)
            {
                _Pipe = Pipe;
                _Mode = Mode;
                _Parent = parent;
            }

            public override string ToString()
            {
                return this._Mode + ";" + this._Pipe;
            }

            public string Pipe
            {
                get { return _Pipe; }
                set { _Pipe = value; _Parent.Pipe = _Pipe; }
            }

            [TypeConverter(typeof(PipeConverter))]
            public string Mode
            {
                get { return _Mode; }
                set { _Mode = value; _Parent.Mode = _Mode; }
            }
        }

        static SettingsPropertyValues mProperties = new SettingsPropertyValues();

        public static string SourceDirectory { get { return mProperties.SourceDirectory; } }
        public static string OutputDirectory { get { return mProperties.OutputDirectory; } }
        public static string Mode { get { return mProperties.Mode;  } }
        public static string Pipe { get { return mProperties.Pipe; } }
        public static string ComPort { get { return mProperties.Port; } }
        public static string Baudrate { get { return mProperties.Baudrate; } }
        public static string AppLogging { get { return mProperties.AppLogging; } }
        public static string AppLogFile { get { return mProperties.AppLogFile; } }
        public static Connect.ConnectionType SelectedConnType { 
            get { return mProperties.SelectedConnType; }
            set {  mProperties.SelectedConnType = value; }
        }

        public Settings()
        {
            InitializeComponent();
            mProperties.Reload();
            SettingsGrid.SelectedObject = mProperties;
        }

        public static void ShowSettings()
        {
            Settings s = new Settings();
            if (s.ShowDialog() == DialogResult.OK)
            {
                mProperties.Save();
            }
        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        public static void Save()
        {
            mProperties.Save();
        }
    }
}
