using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class ReactOSWeb : UserControl
    {
        public ReactOSWeb()
        {
            InitializeComponent();
            this.Tag = "Web";

            BrowserView.CanGoBackChanged += BrowserView_CanGoBackChanged;
            BrowserView.CanGoForwardChanged += BrowserView_CanGoForwardChanged;
            
            BrowserView.GoHome(); 
        }
      
        private void BrowserView_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            AddressInput.Text = e.Url.ToString();
        }

        private void navPaste_Click(object sender, EventArgs e)
        {
            BrowserView.Navigate("http://www.reactos.org/paste");
        }

        private void navHome_Click(object sender, EventArgs e)
        {
            BrowserView.GoHome(); 
        }

        private void navBack_Click(object sender, EventArgs e)
        {
            BrowserView.GoBack();  
        }

        private void navForward_Click(object sender, EventArgs e)
        {
            BrowserView.GoForward();
        }

        private void navReload_Click(object sender, EventArgs e)
        {
            BrowserView.Refresh();  
        }

        private void BrowserView_CanGoBackChanged(object sender, EventArgs e)
        {
            navBack.Enabled = BrowserView.CanGoBack;   
        }

        private void BrowserView_CanGoForwardChanged(object sender, EventArgs e)
        {
            navForward.Enabled = BrowserView.CanGoForward;
        }


        private void AddressInput_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
                BrowserView.Navigate(((ToolStripComboBox)sender).Text);
        }

        private void toolStrip1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
                BrowserView.Navigate("http://www.reactos.org/bugzilla/show_bug.cgi?id=" + ((TextBox)sender).Text);
        }
                
    }
}
