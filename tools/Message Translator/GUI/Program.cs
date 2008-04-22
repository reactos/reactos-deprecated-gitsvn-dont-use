using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Microsoft.Win32;

namespace MsgTranslator
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            MsgTransApplicationContext appContext = new MsgTransApplicationContext();
            
            Application.Run(appContext);
        }
    }

    public class MsgTransApplicationContext : ApplicationContext
    {
        private System.ComponentModel.IContainer components;
        private System.Windows.Forms.NotifyIcon notifyIcon;
        private System.Windows.Forms.ContextMenuStrip contextMenu;
        private System.Windows.Forms.ToolStripMenuItem restoreToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeApplicationToolStripMenuItem;
        private System.Windows.Forms.Form mainForm;

        public MsgTransApplicationContext() 
        {
            // create the notify icon and it's associated context menu
            InitializeContext();

            if (!HideOnMinimize())
                ShowForm();
        }

        private bool HideOnMinimize()
        {
            string regPath = @"Software\ReactOS\MsgTrans";
            string keyName = "HideOnMin";

            try
            {
                RegistryKey rk = Registry.CurrentUser.CreateSubKey(regPath);
                string hideOnMin = rk.GetValue(keyName).ToString();

                return Convert.ToBoolean(hideOnMin);
            }
            catch (Exception)
            {
                return false;
            }
        }

        private void InitializeContext()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.restoreToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeApplicationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();

            // 
            // notifyIcon
            // 
            this.notifyIcon.ContextMenuStrip = this.contextMenu;
            this.notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.notifyIcon.Text = "Message Translator";
            this.notifyIcon.Visible = true;
            this.notifyIcon.DoubleClick += new System.EventHandler(this.notifyIcon_DoubleClick);
            // 
            // contextMenu
            // 
            this.contextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.restoreToolStripMenuItem,
            this.closeApplicationToolStripMenuItem});
            this.contextMenu.Name = "contextMenu";
            this.contextMenu.Size = new System.Drawing.Size(168, 48);
            // 
            // restoreToolStripMenuItem
            // 
            this.restoreToolStripMenuItem.Name = "restoreToolStripMenuItem";
            this.restoreToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.restoreToolStripMenuItem.Text = "Restore";
            this.restoreToolStripMenuItem.Click += new System.EventHandler(this.restoreContextMenuItem_Click);
            // 
            // closeApplicationToolStripMenuItem
            // 
            this.closeApplicationToolStripMenuItem.Name = "closeApplicationToolStripMenuItem";
            this.closeApplicationToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.closeApplicationToolStripMenuItem.Text = "Close Application";
            this.closeApplicationToolStripMenuItem.Click += new EventHandler(exitContextMenuItem_Click);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
        }

        private void ShowForm()
        {
            if (mainForm == null)
            {
                // create a fresh new form and show it.
                mainForm = new MainForm();
                mainForm.Show();

                // hook onto the closed event so we can null out the main form
                // this avoids reshowing a disposed form.
                mainForm.Closed += new EventHandler(mainForm_Closed);
            }
            else
            {
                // the form is currently visible, go ahead and bring it to the front so the user can interact
                mainForm.Activate();
            }
        }

        private void notifyIcon_DoubleClick(object sender, EventArgs e)
        {
            ShowForm();
        }

        private void restoreContextMenuItem_Click(object sender, EventArgs e)
        {
            ShowForm();
        }

        private void exitContextMenuItem_Click(object sender, EventArgs e)
        {
            ExitThread();
        }

        private void mainForm_Closed(object sender, EventArgs e)
        {
            // null out the main form so we know to create a new one.
            this.mainForm = null;

            if (!HideOnMinimize())
                ExitThread();
        }

        protected override void ExitThreadCore()
        {
            if (mainForm != null)
            {
                // before we exit, give the main form a chance to clean itself up.
                mainForm.Close();
            }
            base.ExitThreadCore();
        }
    }
}