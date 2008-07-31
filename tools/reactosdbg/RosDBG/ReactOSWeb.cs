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
        }

        private void AddressInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                e.Handled = true;
                BrowserView.Navigate(((TextBox)sender).Text);
            }
        }

        private void BrowserView_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            AddressInput.Text = e.Url.ToString();
        }

        private void BugzillaNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                e.Handled = true;
                BrowserView.Navigate("http://www.reactos.org/bugzilla/show_bug.cgi?id=" + ((TextBox)sender).Text);
            }
        }

        private void Paste_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            BrowserView.Navigate("http://www.reactos.org/paste");
        }
    }
}
