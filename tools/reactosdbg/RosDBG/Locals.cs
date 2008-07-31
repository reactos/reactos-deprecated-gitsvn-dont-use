using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Diagnostics;
using DebugProtocol;
using DbgHelpAPI;

namespace RosDBG
{
    [DebugControl, BuildAtStartup]
    public partial class Locals : UserControl, IUseDebugConnection, IUseSymbols, IUseShell
    {
        class DisplayValue
        {
            DebugConnection mConnection;
            Registers mRegisters;
            Variable mVariable;
            BinaryReader mReader;

            string mName;
            public string Name
            {
                get { return mName; }
            }

            ulong GetRegisterValue(Variable.Register reg)
            {
                ulong result;

                switch (reg)
                {
                    case Variable.Register.CV_REG_AL:
                    case Variable.Register.CV_REG_AH:
                    case Variable.Register.CV_REG_AX:
                    case Variable.Register.CV_REG_EAX:
                        result = mRegisters.Eax;
                        break;

                    case Variable.Register.CV_REG_CL:
                    case Variable.Register.CV_REG_CH:
                    case Variable.Register.CV_REG_CX:
                    case Variable.Register.CV_REG_ECX:
                        result = mRegisters.Ecx;
                        break;

                    case Variable.Register.CV_REG_DL:
                    case Variable.Register.CV_REG_DH:
                    case Variable.Register.CV_REG_DX:
                    case Variable.Register.CV_REG_EDX:
                        result = mRegisters.Edx;
                        break;

                    case Variable.Register.CV_REG_BL:
                    case Variable.Register.CV_REG_BH:
                    case Variable.Register.CV_REG_BX:
                    case Variable.Register.CV_REG_EBX:
                        result = mRegisters.Ebx;
                        break;

                    case Variable.Register.CV_REG_SP:
                    case Variable.Register.CV_REG_ESP:
                        result = mRegisters.Esp;
                        break;

                    case Variable.Register.CV_REG_BP:
                    case Variable.Register.CV_REG_EBP:
                        result = mRegisters.Ebp;
                        break;

                    case Variable.Register.CV_REG_SI:
                    case Variable.Register.CV_REG_ESI:
                        result = mRegisters.Esi;
                        break;

                    case Variable.Register.CV_REG_DI:
                    case Variable.Register.CV_REG_EDI:
                        result = mRegisters.Edi;
                        break;

                    case Variable.Register.CV_REG_ES:
                        result = mRegisters.Es;
                        break;

                    case Variable.Register.CV_REG_CS:
                        result = mRegisters.Cs;
                        break;

                    case Variable.Register.CV_REG_SS:
                        result = mRegisters.Ss;
                        break;

                    case Variable.Register.CV_REG_DS:
                        result = mRegisters.Ds;
                        break;

                    case Variable.Register.CV_REG_FS:
                        result = mRegisters.Fs;
                        break;

                    case Variable.Register.CV_REG_GS:
                        result = mRegisters.Gs;
                        break;

                    case Variable.Register.CV_REG_IP:
                    case Variable.Register.CV_REG_EIP:
                        result = mRegisters.Eip;
                        break;

                    case Variable.Register.CV_REG_FLAGS:
                    case Variable.Register.CV_REG_EFLAGS:
                        result = mRegisters.Eflags;
                        break;

                    default:
                        Trace.Assert(false);
                        return mRegisters.Eax;
                }

                switch (reg)
                {
                    case Variable.Register.CV_REG_AL:
                    case Variable.Register.CV_REG_CL:
                    case Variable.Register.CV_REG_DL:
                    case Variable.Register.CV_REG_BL:
                        return result & 0xff;

                    case Variable.Register.CV_REG_AH:
                    case Variable.Register.CV_REG_CH:
                    case Variable.Register.CV_REG_DH:
                    case Variable.Register.CV_REG_BH:
                        return (result >> 8) & 0xff;

                    case Variable.Register.CV_REG_AX:
                    case Variable.Register.CV_REG_CX:
                    case Variable.Register.CV_REG_DX:
                    case Variable.Register.CV_REG_BX:
                    case Variable.Register.CV_REG_SP:
                    case Variable.Register.CV_REG_BP:
                    case Variable.Register.CV_REG_SI:
                    case Variable.Register.CV_REG_DI:
                    case Variable.Register.CV_REG_ES:
                    case Variable.Register.CV_REG_CS:
                    case Variable.Register.CV_REG_SS:
                        return result & 0xffff;

                    default:
                        return result;
                }
            }

            string mValue;
            public string Value
            {
                get
                {
                    return mValue;
                }
            }

            public DisplayValue(DebugConnection conn, Registers reg, Variable var)
            {
                mName = var.Name;
                mConnection = conn;
                mRegisters = reg;
                mVariable = var;
                mReader = new BinaryReader(mConnection.NewMemoryStream());

                StringBuilder sb = new StringBuilder();
                ulong regVal;
                byte[] buf = new byte[mVariable.Size];

                if (mVariable.Regval)
                {
                    regVal = GetRegisterValue(mVariable.Reg);
                    sb.Append(string.Format("{0:X}", regVal));
                    mValue = sb.ToString();
                    return;
                }
                else if (mVariable.Regrel)
                {
                    regVal = GetRegisterValue(mVariable.Reg);
                    mReader.BaseStream.Seek((long)regVal + mVariable.Offset, SeekOrigin.Begin);
                    mReader.Read(buf, 0, buf.Length);
                }
                else if (mVariable.Local || mVariable.Parameter)
                {
                    mReader.BaseStream.Seek(mVariable.Offset, SeekOrigin.Begin);
                    mReader.Read(buf, 0, buf.Length);
                }
                if (buf.Length == 4)
                {
                    sb.Append(string.Format("{3:X2}{2:X2}{1:X2}{0:X2}",
                        buf[0], buf[1], buf[2], buf[3]));
                }
                else
                {
                    foreach (byte b in buf)
                        sb.Append(string.Format("{0:X2} ", (int)b));
                }
                mValue = sb.ToString();
            }
        }

        Dictionary<string, Variable> mVariables = new Dictionary<string, Variable>();

        public Locals()
        {
            InitializeComponent();
            LocalDataGrid.DataSource = mDisplaySet;
        }

        Registers mRegisters;
        DebugConnection mConnection;

        public void SetDebugConnection(DebugConnection conn)
        {
            mConnection = conn;
            mConnection.DebugRegisterChangeEvent += DebugRegisterChangeEvent;
            mConnection.DebugRunningChangeEvent += DebugRunningChangeEvent;
            mConnection.DebugModuleChangedEvent += DebugModuleChangedEvent;
        }

        Dictionary<string, DisplayValue> mDisplaySet = new Dictionary<string, DisplayValue>();

        void UpdateGrid()
        {
            LocalDataGrid.DataSource = new List<DisplayValue>(mDisplaySet.Values);
        }

        void ResolveVariable(object var)
        {
            Variable v = (Variable)var;
            DisplayValue dispVal = new DisplayValue(mConnection, mRegisters, v);
            lock (mDisplaySet)
            {
                mDisplaySet[dispVal.Name] = dispVal;
            }
            Invoke(Delegate.CreateDelegate(typeof(NoParamsDelegate), this, "UpdateGrid"));
        }

        void UpdateLocals()
        {
            if (mRegisters != null)
            {
                List<Variable> vars = mSymbols.GetLocals(mRegisters.Eip);
                foreach (Variable var in vars)
                    ThreadPool.QueueUserWorkItem(ResolveVariable, var);
            }
        }

        void DebugModuleChangedEvent(object sender, DebugModuleChangedEventArgs args)
        {
            UpdateLocals();
        }

        void DebugRunningChangeEvent(object sender, DebugRunningChangeEventArgs args)
        {
            UpdateLocals();
        }

        void  DebugRegisterChangeEvent(object sender, DebugRegisterChangeEventArgs args)
        {
            mRegisters = args.Registers;
            UpdateLocals();
        }

        IShell mShell;
        public void SetShell(IShell shell)
        {
            mShell = shell;
        }

        SymbolContext mSymbols;
        public void SetSymbolProvider(SymbolContext symbols)
        {
            mSymbols = symbols;
        }
    }
}
