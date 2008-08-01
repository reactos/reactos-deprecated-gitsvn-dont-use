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
    public partial class DirectoryChooser : Form
    {
        Dictionary<string, TreeNode> mDirectories = new Dictionary<string, TreeNode>();

        public string Value
        {
            get
            {
                TreeNode selected = DirectoryTree.SelectedNode;
                if (selected == null)
                    return Path.GetDirectoryName(Application.ExecutablePath);
                else
                    return (string)selected.Tag;
            }
            set
            {
                TreeNode tn = ShowNode(value);
                DirectoryTree.SelectedNode = tn;
                tn.EnsureVisible();
            }
        }

        public DirectoryChooser()
        {
            InitializeComponent();
            DirectoryTree.Nodes.Clear();
            foreach (DriveInfo drive in DriveInfo.GetDrives())
            {
                ShowNode(drive.RootDirectory.FullName);
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

        void ExpandNode(TreeNode node, string dir)
        {
            try
            {
                if (node.Nodes.Count == 1 && node.Nodes[0].Text == "...")
                {
                    node.Nodes.Clear();

                    foreach (string subdir in Directory.GetDirectories(dir))
                    {
                        TreeNode newNode = new TreeNode(Path.GetFileName(subdir));
                        newNode.Tag = subdir;
                        newNode.Nodes.Add("...");
                        mDirectories[subdir.ToLower()] = newNode;
                        node.Nodes.Add(newNode);
                    }
                }
            }
            catch (IOException)
            {
            }
        }

        TreeNode ShowNode(string dir)
        {
            string parentDir = Path.GetDirectoryName(dir);
            TreeNode theNode;
            // We reached a root
            if (mDirectories.TryGetValue(dir.ToLower(), out theNode))
            {
                return theNode;
            }
            else if (parentDir == null)
            {
                theNode = new TreeNode(dir);
                theNode.Tag = dir;
                theNode.Nodes.Add("...");
                mDirectories[dir.ToLower()] = theNode;
                DirectoryTree.Nodes.Add(theNode);
                return theNode;
            }
            else
            {
                theNode = ShowNode(parentDir);
                ExpandNode(theNode, parentDir);
            }
            return ShowNode(dir);
        }

        private void DirectoryTree_AfterExpand(object sender, TreeViewEventArgs e)
        {
            string dir = (string)e.Node.Tag;
            ExpandNode(e.Node, dir);
        }

        private void DirectoryTree_AfterCollapse(object sender, TreeViewEventArgs e)
        {
            e.Node.Nodes.Clear();
            e.Node.Nodes.Add("...");
        }
    }

    public class DirectoryEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle  GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext typedesc, IServiceProvider provider, object value)
        {
            DirectoryChooser chooser = new DirectoryChooser();
            chooser.Value = (string)value;
            if (chooser.ShowDialog() == DialogResult.OK)
                return chooser.Value;
            else
                return value;
        }
    }

}
