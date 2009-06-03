using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.Windows.Forms;

namespace RosDBG
{

    public partial class ExtTools : Form
    {
        private ExternalToolList mExternalToolsList;
        private bool mUpdatingList = false;

        public ExtTools()
        {
            InitializeComponent();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Settings.ExternalTools = mExternalToolsList; 
            Close();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            ExternalTool t = new ExternalTool();
            mExternalToolsList.Add(t);
            ToolsListBox.Items.Add(t);
            ToolsListBox.SelectedIndex = ToolsListBox.Items.Count - 1;
            btnRemove.Enabled = true;
        }

        private void ExtTools_Load(object sender, EventArgs e)
        {
            mExternalToolsList = Settings.ExternalTools;
            if (mExternalToolsList == null)
                mExternalToolsList = new ExternalToolList(); 

            foreach (object o in mExternalToolsList)
                ToolsListBox.Items.Add(o);
        }

        private void btnRemove_Click(object sender, EventArgs e)
        {
            if ((ToolsListBox.Items.Count > 0) && (ToolsListBox.SelectedIndex != -1))
            {
                int Idx = ToolsListBox.SelectedIndex;
                ToolsListBox.Items.RemoveAt(Idx);
                mExternalToolsList.RemoveAt(Idx);  
                if ((ToolsListBox.Items.Count > 0) && (Idx > 0))
                    ToolsListBox.SelectedIndex = Idx - 1;
                else
                    btnRemove.Enabled = false;
            }
        }

        private void SaveItem(ExternalTool item, int Idx)
        {
            mUpdatingList = true;
            item.Title = txtTitle.Text;
            item.Path = txtPath.Text;
            ToolsListBox.Items[Idx] = item;
            mUpdatingList = false;
        }

        private void ToolsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!mUpdatingList)
            {
                ExternalTool et = (ExternalTool)ToolsListBox.SelectedItem;
                if (et != null)
                {
                    txtTitle.Enabled = true;
                    txtPath.Enabled = true;
                    btnBrowse.Enabled = true;
                    txtTitle.Text = et.Title;
                    txtPath.Text = et.Path;
                    btnUp.Enabled = (ToolsListBox.SelectedIndex != 0);
                    btnDown.Enabled = (ToolsListBox.SelectedIndex != ToolsListBox.Items.Count - 1);
                }
                else
                {
                    txtTitle.Enabled = false;
                    txtPath.Enabled = false;
                    btnBrowse.Enabled = false;
                    txtTitle.Text = "";
                    txtPath.Text = "";
                }
            }
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog();
            fd.Filter = "Executable files (*.exe)|*.exe";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                txtPath.Text = fd.FileName; 
            }
        }

        private void txtTitle_TextChanged(object sender, EventArgs e)
        {
            if (ToolsListBox.SelectedItem != null) 
                SaveItem((ExternalTool)ToolsListBox.SelectedItem, ToolsListBox.SelectedIndex);
        }

        private void txtPath_TextChanged(object sender, EventArgs e)
        {
            if (ToolsListBox.SelectedItem != null)
                SaveItem((ExternalTool)ToolsListBox.SelectedItem, ToolsListBox.SelectedIndex);
        }
    }

    [Serializable]
    public class ExternalToolList : ArrayList, IXmlSerializable 
    { 
        public System.Xml.Schema.XmlSchema GetSchema() 
        { 
            return null;
        } 

        public void ReadXml(System.Xml.XmlReader reader)
        { 
            this.Clear();
            XmlSerializer x = new XmlSerializer(typeof(ExternalTool));
            while (reader.ReadToFollowing("ExternalTool"))
            { 
                object o = x.Deserialize(reader);
                if (o is ExternalTool) this.Add(o);
            }
        }

        public void WriteXml(System.Xml.XmlWriter writer)
        {
            XmlSerializer x = new XmlSerializer(typeof(ExternalTool));
            foreach (ExternalTool et in this)
                x.Serialize(writer, et); 
        }
    }

    public class ExternalTool : IXmlSerializable 
    {
        private string mTitle;
        private string mPath;

        public ExternalTool()
        {
            mTitle = "[New Tool]";
            mPath = "";
        }

        public ExternalTool(string Title, string Path)
        {
            mTitle = Title;
            mPath = Path;
        }

        public string Title
        {
            get { return mTitle; }
            set { mTitle = value; }
        }

        public string Path
        {
            get { return mPath; }
            set { mPath = value; }
        }

        public override string ToString()
        {
            return mTitle;
        }

        public System.Xml.Schema.XmlSchema GetSchema()
        { 
            return null;
        }

        public void ReadXml(System.Xml.XmlReader reader)
        {
            if (reader.MoveToAttribute("Title"))
                mTitle = reader.Value;
            else
                throw new Exception();
            if (reader.MoveToAttribute("Path"))
                mPath = reader.Value;
            else
                throw new Exception();
        }

        public void WriteXml(System.Xml.XmlWriter writer)
        {
            writer.WriteAttributeString("Title", mTitle);
            writer.WriteAttributeString("Path", mPath);
        } 
    }

}
