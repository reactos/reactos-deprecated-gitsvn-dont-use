using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DebugProtocol;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class RegisterView : UserControl, IUseDebugConnection
    {
        bool mGridEnabled;
        Registers mRegisters;
        DebugConnection mConnection;

        public RegisterView()
        {
            InitializeComponent();
            RegisterGrid.SelectedObject = new Registers();
            this.Tag = "Registers";
        }

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            mConnection.DebugRegisterChangeEvent += DebugRegisterChangeEvent;
            mConnection.DebugRunningChangeEvent += DebugRunningChangeEvent;
            if (!mConnection.Running)
                mConnection.Debugger.GetRegisterUpdate();
        }

        void UpdateGridEnabled()
        {
            RegisterGrid.Enabled = mGridEnabled;
        }

        void DebugRunningChangeEvent(object sender, DebugRunningChangeEventArgs args)
        {
            mGridEnabled = !args.Running;
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateGridEnabled"));
        }

        void UpdateGrid()
        {
            RegisterGrid.SelectedObject = null;
            RegisterGrid.SelectedObject = mRegisters;
            RegisterGrid.Refresh();
        }

        void DebugRegisterChangeEvent(object sender, DebugRegisterChangeEventArgs args)
        {
            mRegisters = args.Registers;
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateGrid"));
        }
    }
}
