using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using WeifenLuo.WinFormsUI.Docking;
using AbstractPipe;
using DebugProtocol;
using KDBGProtocol;
using DbgHelpAPI;

namespace RosDBG
{
    public delegate void NoParamsDelegate();

    public partial class MainWindow : Form, IShell
    {
        private RegisterView m_RegView = new RegisterView();
        private BackTrace m_BackTrace = new BackTrace();
        private RawTraffic m_RawTraffic = new RawTraffic();
        private Locals m_Locals = new Locals();
        private MemoryWindow m_MemoryWindow = new MemoryWindow();

        private bool mRunning;
        private DebugConnection.Mode mConnectionMode;
        private ulong mCurrentEip;
        private string mSourceRoot = Settings.SourceDirectory, mCurrentFile;
        private int mCurrentLine;
        private DebugConnection mConnection = new DebugConnection();
        private SymbolContext mSymbolContext;
        Dictionary<uint, Module> mModules = new Dictionary<uint, Module>();
        private Dictionary<string, SourceView> mSourceFiles = new Dictionary<string, SourceView>();

        //public event CopyEventHandler CopyEvent;

        public MainWindow()
        {
            InitializeComponent();

            mSymbolContext = new SymbolContext();

            RegisterControl(m_RegView);
            RegisterControl(m_BackTrace);
            RegisterControl(m_RawTraffic);
            RegisterControl(m_Locals);
            RegisterControl(m_MemoryWindow);

            m_Locals.Show(dockPanel, DockState.DockRight);
            m_RegView.Show(dockPanel, DockState.DockRight);
            m_BackTrace.Show(dockPanel, DockState.DockBottom);
            m_RawTraffic.Show(dockPanel);
            m_MemoryWindow.Show(dockPanel);
            ReactOSWeb web = new ReactOSWeb();
            web.Show(dockPanel);
        }

        void ComposeTitleString()
        {
            FocusAddress(mCurrentEip);
            statusStrip.Text = "ConnectionMode: " + mConnectionMode + " - Running: " + mRunning + " - Source Location: " + mCurrentFile + ":" + mCurrentLine;
        }

        void DebugModuleChangedEvent(object sender, DebugModuleChangedEventArgs args)
        {
            Module themod;
            if (!mModules.TryGetValue(args.ModuleAddr, out themod) ||
                themod.ShortName != args.ModuleName.ToLower())
            {
                mModules[args.ModuleAddr] = new Module(args.ModuleAddr, args.ModuleName);
                mSymbolContext.LoadModule(args.ModuleName, args.ModuleAddr);
            }
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "ComposeTitleString"));
        }

        void DebugRunningChangeEvent(object sender, DebugRunningChangeEventArgs args)
        {
            mRunning = args.Running;
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "ComposeTitleString"));
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateDebuggerMenu"));
        }

        void DebugConnectionModeChangedEvent(object sender, DebugConnectionModeChangedEventArgs args)
        {
            mConnectionMode = args.Mode;
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "ComposeTitleString"));
        }

        void DebugRegisterChangeEvent(object sender, DebugRegisterChangeEventArgs args)
        {
            mCurrentEip = args.Registers.Eip;
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "ComposeTitleString"));
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            mSymbolContext.Initialize();
            mConnection.DebugConnectionModeChangedEvent += DebugConnectionModeChangedEvent;
            mConnection.DebugRunningChangeEvent += DebugRunningChangeEvent;
            mConnection.DebugRegisterChangeEvent += DebugRegisterChangeEvent;
            mConnection.DebugModuleChangedEvent += DebugModuleChangedEvent;
            ComposeTitleString();
            mSymbolContext.ReactosOutputPath = Settings.OutputDirectory;
        }

        public void RegisterControl(Control ctrl)
        {
            IUseDebugConnection usedbg = ctrl as IUseDebugConnection;
            if (usedbg != null)
                usedbg.SetDebugConnection(mConnection);
            IUseSymbols usesym = ctrl as IUseSymbols;
            if (usesym != null)
                usesym.SetSymbolProvider(mSymbolContext);
            IUseShell useshell = ctrl as IUseShell;
            if (useshell != null)
                useshell.SetShell(this);
        }

        private void OpenFile(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Sourcefiles (*.c;*.cpp)|*.c;*.cpp";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                OpenSourceFile(fileDialog.FileName);
            }
        }

        private void SaveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            saveFileDialog.Filter = "Textdateien (*.txt)|*.txt|Alle Dateien (*.*)|*.*";
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string FileName = saveFileDialog.FileName;
            }
        }

        private void ExitToolsStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void CutToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void CopyToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void PasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void ToolBarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStrip.Visible = toolBarToolStripMenuItem.Checked;
        }

        private void StatusBarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            statusStrip.Visible = statusBarToolStripMenuItem.Checked;
        }

        private void MainWindowMDI_FormClosing(object sender, FormClosingEventArgs e)
        {
            mConnection.Close(true);
        }

        void UpdateDebuggerMenu()
        {
            if (mConnection.ConnectionMode == DebugConnection.Mode.ClosedMode)
            {
                breakToolStripMenuItem.Enabled = false;
                nextToolStripMenuItem.Enabled = false;
                stepToolStripMenuItem.Enabled = false;
                continueToolStripMenuItem.Enabled = false;
            }
            else
            {
                breakToolStripMenuItem.Enabled = mRunning;
                nextToolStripMenuItem.Enabled = !mRunning;
                stepToolStripMenuItem.Enabled = !mRunning;
                continueToolStripMenuItem.Enabled = !mRunning;
            }
        }

        public void FocusAddress(ulong eipToFocus)
        {
            KeyValuePair<string, int> fileline = mSymbolContext.GetFileAndLine(eipToFocus);
            mCurrentFile = fileline.Key;
            mCurrentLine = fileline.Value;
            TryToDisplaySource();
        }

        void Rehighlight(SourceView vw)
        {
            vw.ClearHighlight();
            vw.AddHighlight(mCurrentLine, Color.SteelBlue, Color.White);
            vw.ScrollTo(mCurrentLine);
        }

        void TryToDisplaySource()
        {
            if (mCurrentFile == null || mCurrentFile == "unknown") return;
            OpenSourceFile(Path.Combine(mSourceRoot, mCurrentFile));
        }

        private void OpenSourceFile(string FileName)
        {
            SourceView theSourceView;
            if (File.Exists(FileName))
            {
                if (mSourceFiles.TryGetValue(FileName, out theSourceView))
                    Rehighlight(theSourceView);
                else
                {
                    theSourceView = new SourceView(Path.GetFileName(FileName));
                    mSourceFiles[FileName] = theSourceView;
                    theSourceView.SourceFile = FileName;
                    Rehighlight(theSourceView);
                    theSourceView.Show(dockPanel);  
                }
            }
        }

        public DebugConnection DebugConnection
        {
            get { return mConnection; }
        }

        private void breakToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mConnection.Break();
        }

        private void continueToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mConnection.Go();
        }

        private void stepToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mConnection.Step();
        }

        private void nextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mConnection.Next();
        }

        private void contentsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ReactOSWeb Help = new ReactOSWeb("Help", "http://www.reactos.org/wiki/index.php/ReactOSDbg");
            Help.Show(dockPanel);  
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Settings.ShowSettings();
            mSourceRoot = Settings.SourceDirectory;
            mSymbolContext.ReactosOutputPath = Settings.OutputDirectory;
        }

        private void consoleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_RawTraffic.Show(dockPanel);
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mConnection.ConnectionMode == DebugConnection.Mode.ClosedMode)
            {
                Connect newConnection = new Connect();
                if (newConnection.ShowDialog() == DialogResult.OK)
                {
                    mConnection.Close();
                    switch (newConnection.Type)
                    {
                        case Connect.ConnectionType.Serial:
                            mConnection.StartSerial(newConnection.ComPort, newConnection.Baudrate);
                            break;
                        case Connect.ConnectionType.Pipe:
                            mConnection.StartPipe(newConnection.PipeName, newConnection.PipeMode);
                            break;
                        case Connect.ConnectionType.Socket:
                            mConnection.StartTCP(newConnection.Host, newConnection.Port);
                            break;
                    }
                }
                connectToolStripMenuItem.Text = "&Disconect";
            }
            else
            {
                mConnection.Close();
                UpdateDebuggerMenu();
                connectToolStripMenuItem.Text = "&Connect";
            }
        }

        private void memoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_MemoryWindow.Show(dockPanel);
        }

        private void webbrowserToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ReactOSWeb web = new ReactOSWeb();
            web.Show(dockPanel); 
        }

    }

}
