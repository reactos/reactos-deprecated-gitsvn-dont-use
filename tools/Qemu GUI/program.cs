using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Qemu_GUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.Run(new MainForm());
        }
    }
}