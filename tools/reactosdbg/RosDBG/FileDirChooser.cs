using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Drawing.Design;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace RosDBG
{
    public class DirectoryEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle  GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext typedesc, IServiceProvider provider, object value)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Set path for " + typedesc.PropertyDescriptor.DisplayName;
            if (fbd.ShowDialog() == DialogResult.OK)
                return fbd.SelectedPath;
            else
                return value;
        }
    }

    public class FileEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext typedesc, IServiceProvider provider, object value)
        {
            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            ofd.CheckFileExists = false;
            ofd.Filter = "log files (*.log)|*.log";
            ofd.DefaultExt = "log";
            ofd.AddExtension = true;
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == DialogResult.OK)
                return ofd.FileName;
            else
                return value;
        }
    }
}
