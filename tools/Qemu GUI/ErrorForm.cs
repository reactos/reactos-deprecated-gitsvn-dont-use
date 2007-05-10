using System;
using System.Windows.Forms;

namespace Qemu_GUI
{
    public partial class ErrorForm : Form
    {
        
        public ErrorForm()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        
        private void error_copy_Click(object sender, EventArgs e)
        {
            string temp = txtError.Text;
            Clipboard.SetText(temp);
        }
    }
}