using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows.Forms;
using System.Drawing;

namespace RosDBG
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Mutex m = new Mutex(false, "ReactOS Remote Debugger");
            if (m.WaitOne(10, false))
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                MainWindow mw = new MainWindow();

                mw.WindowState = RosDBG.Properties.Settings.Default.WindowState;
                mw.StartPosition = FormStartPosition.Manual;
                mw.Location = RosDBG.Properties.Settings.Default.Location;
                mw.Size = RosDBG.Properties.Settings.Default.Size;
                Application.Run(mw);
            }
            else
            {
                /* TODO */
            }
        }
    }
}
