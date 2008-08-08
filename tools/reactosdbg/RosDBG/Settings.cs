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

namespace RosDBG
{
    public partial class Settings : Form
    {
        public class SettingsPropertyValues : ApplicationSettingsBase
        {
            [UserScopedSetting, DefaultSettingValue("."), Editor(typeof(DirectoryEditor), typeof(UITypeEditor))]
            public string SourceDirectory
            {
                get { return this["SourceDirectory"].ToString(); }
                set { this["SourceDirectory"] = value; }
            }

            [UserScopedSetting,DefaultSettingValue(".\\output-i386"),Editor(typeof(DirectoryEditor), typeof(UITypeEditor))]
            public string OutputDirectory
            {
                get { return this["OutputDirectory"].ToString(); }
                set { this["OutputDirectory"] = value; }
            }

            public SettingsPropertyValues()
            {
                Reload();
            }
        }

        static SettingsPropertyValues mProperties = new SettingsPropertyValues();

        public static string SourceDirectory { get { return mProperties.SourceDirectory; } }
        public static string OutputDirectory { get { return mProperties.OutputDirectory; } }

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
