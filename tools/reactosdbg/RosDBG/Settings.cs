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
using System.IO.Ports;

namespace RosDBG
{
    public partial class Settings : Form
    {
        public class SettingsPropertyValues : ApplicationSettingsBase
        {
            private SerialConnSettings _serialconnsettings;

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
            /* end of hack */

            [CategoryAttribute("Directories"), DescriptionAttribute("Directory settings")]
            [UserScopedSetting, DefaultSettingValue("."), Editor(typeof(DirectoryEditor), typeof(UITypeEditor))]
            public string SourceDirectory
            {
                get { return this["SourceDirectory"].ToString(); }
                set { this["SourceDirectory"] = value; }
            }

            [CategoryAttribute("Directories"), DescriptionAttribute("Directory settings")]
            [UserScopedSetting,DefaultSettingValue(".\\output-i386"),Editor(typeof(DirectoryEditor), typeof(UITypeEditor))]
            public string OutputDirectory
            {
                get { return this["OutputDirectory"].ToString(); }
                set { this["OutputDirectory"] = value; }
            }

            [CategoryAttribute("Connection"), DescriptionAttribute("Connection settings")]
            [UserScopedSetting, DefaultSettingValue(@"\\.\Pipe\RosDbg")]
            public string Pipe
            {
                get { return this["Pipe"].ToString(); }
                set { this["Pipe"] = value; }
            }
 
            [CategoryAttribute("Connection"), DescriptionAttribute("Connection settings")]
            public SerialConnSettings Serial
            {
                get { return new SerialConnSettings(Port, Baudrate, this); }
                set { _serialconnsettings = value; }
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

        static SettingsPropertyValues mProperties = new SettingsPropertyValues();

        public static string SourceDirectory { get { return mProperties.SourceDirectory; } }
        public static string OutputDirectory { get { return mProperties.OutputDirectory; } }
        public static string Pipe { get { return mProperties.Pipe;  } }
        public static string ComPort { get { return mProperties.Port; } }
        public static string Baudrate { get { return mProperties.Baudrate; } }

        Settings()
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
    }
}
