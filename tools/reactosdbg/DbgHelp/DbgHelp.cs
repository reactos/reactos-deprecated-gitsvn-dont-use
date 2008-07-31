using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace DbgHelpAPI
{
    public enum SymOptionsFlags
    {
        SYMOPT_ALLOW_ABSOLUTE_SYMBOLS = 0x80,
        SYMOPT_ALLOW_ZERO_ADDRESS = 0x1000000,
        SYMOPT_AUTO_PUBLICS = 0x10000,
        SYMOPT_CASE_INSENSITIVE = 1,
        SYMOPT_DEBUG = -2147483648,
        SYMOPT_DEFERRED_LOADS = 4,
        SYMOPT_DISABLE_SYMSRV_AUTODETECT = 0x2000000,
        SYMOPT_EXACT_SYMBOLS = 0x400,
        SYMOPT_FAIL_CRITICAL_ERRORS = 0x200,
        SYMOPT_FAVOR_COMPRESSED = 0x800000,
        SYMOPT_FLAT_DIRECTORY = 0x400000,
        SYMOPT_IGNORE_CVREC = 0x80,
        SYMOPT_IGNORE_IMAGEDIR = 0x200000,
        SYMOPT_IGNORE_NT_SYMPATH = 0x1000,
        SYMOPT_INCLUDE_32BIT_MODULES = 0x2000,
        SYMOPT_LOAD_ANYTHING = 0x40,
        SYMOPT_LOAD_LINES = 0x10,
        SYMOPT_NO_CPP = 8,
        SYMOPT_NO_IMAGE_SEARCH = 0x20000,
        SYMOPT_NO_PROMPTS = 0x80000,
        SYMOPT_NO_PUBLICS = 0x8000,
        SYMOPT_NO_UNQUALIFIED_LOADS = 0x100,
        SYMOPT_OVERWRITE = 0x100000,
        SYMOPT_PUBLICS_ONLY = 0x4000,
        SYMOPT_SECURE = 0x40000,
        SYMOPT_UNDNAME = 2
    }

    public enum SsigType
    {
        DBHHEADER_DEBUGDIRS = 1,
        DBHHEADER_CVMISC = 2
    }

    public struct MODLOAD_DATA
    {
        public UInt32 ssize;
        public SsigType ssig;
        public IntPtr data;
        public UInt32 size;
        public UInt32 flags;
    }

    public enum SymLoadModuleFlags 
    {
        SLMFLAG_NO_SYMBOLS = 4,
        SLMFLAG_VIRTUAL = 1
    }

    public struct IMAGEHLP_STACK_FRAME
    {
        public ulong InstructionOffset;
        ulong ReturnOffset;
        ulong FrameOffset;
        ulong StackOffset;
        ulong BackingStoreOffset;
        ulong FuncTableEntry;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst=4)]
        ulong[] Params;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst=5)]
        ulong[] Reserved;
        [MarshalAs(UnmanagedType.I1)]
        bool Virtual;
        uint Reserved2;
    }

    public enum SymInfoFlags
    {
        SYMFLAG_NONE = 0,
        SYMFLAG_CLR_TOKEN = 0x40000,
        SYMFLAG_CONSTANT = 0x100,
        SYMFLAG_EXPORT = 0x200,
        SYMFLAG_FORWARDER = 0x400,
        SYMFLAG_FRAMEREL = 0x20,
        SYMFLAG_FUNCTION = 0x800,
        SYMFLAG_ILREL = 0x10000,
        SYMFLAG_LOCAL = 0x80,
        SYMFLAG_METADATA = 0x20000,
        SYMFLAG_PARAMETER = 0x40,
        SYMFLAG_REGISTER = 8,
        SYMFLAG_REGREL = 0x10,
        SYMFLAG_SLOT = 0x8000,
        SYMFLAG_THUNK = 0x2000,
        SYMFLAG_TLSREL = 0x4000,
        SYMFLAG_VALUEPRESENT = 1,
        SYMFLAG_VIRTUAL = 0x1000
    }

    public struct SYMBOL_INFO
    {
        public uint SizeOfStruct;
        public uint TypeIndex;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst=2)]
        public ulong []Reserved;
        public uint Index;
        public uint Size;
        public ulong ModBase;
        public SymInfoFlags Flags;
        public ulong Value;
        public ulong Address;
        public uint Register;
        public uint Scope;
        public uint Tag;
        public uint NameLen;
        public uint MaxNameLen;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst=256)]
        public string Name;

        public void Initialize()
        {
            SizeOfStruct = (uint)Marshal.SizeOf(this);
            MaxNameLen = 256;
        }
    }

    public enum IMAGEHLP_SYMBOL_TYPE_INFO 
    {
        TI_GET_SYMTAG,
        TI_GET_SYMNAME,
        TI_GET_LENGTH,
        TI_GET_TYPE,
        TI_GET_TYPEID,
        TI_GET_BASETYPE,
        TI_GET_ARRAYINDEXTYPEID,
        TI_FINDCHILDREN,
        TI_GET_DATAKIND,
        TI_GET_ADDRESSOFFSET,
        TI_GET_OFFSET,
        TI_GET_VALUE,
        TI_GET_COUNT,
        TI_GET_CHILDRENCOUNT,
        TI_GET_BITPOSITION,
        TI_GET_VIRTUALBASECLASS,
        TI_GET_VIRTUALTABLESHAPEID,
        TI_GET_VIRTUALBASEPOINTEROFFSET,
        TI_GET_CLASSPARENTID,
        TI_GET_NESTED,
        TI_GET_SYMINDEX,
        TI_GET_LEXICALPARENT,
        TI_GET_ADDRESS,
        TI_GET_THISADJUST,
        TI_GET_UDTKIND,
        TI_IS_EQUIV_TO,
        TI_GET_CALLING_CONVENTION,
        TI_IS_CLOSE_EQUIV_TO,
        TI_GTIEX_REQS_VALID,
        TI_GET_VIRTUALBASEOFFSET,
        TI_GET_VIRTUALBASEDISPINDEX,
        TI_GET_IS_REFERENCE,
        TI_GET_INDIRECTVIRTUALBASECLASS
    }

    public struct IMAGEHLP_LINE64 
    {
        public uint SizeOfStruct;
        public IntPtr Key;
        public uint LineNumber;
        [MarshalAs(UnmanagedType.LPStr)]
        public string FileName;
        public ulong Address;
        public void Initialize() { SizeOfStruct = (uint)Marshal.SizeOf(this); }
    }

    public delegate bool SymEnumSymbolsProc(ref SYMBOL_INFO symInfo, uint SymbolSize, IntPtr contextZero);

    public enum DesiredAccess
    {
        GENERIC_READ = -2147483648
    }

    public enum ShareMode
    {
        FILE_SHARE_READ = 1
    }

    public enum CreateDisposition
    {
        OPEN_EXISTING = 3
    }

    public enum FlagsAndAttributes
    {
        FILE_ATTRIBUTE_NORMAL = 0x80
    }

    class Kernel32
    {
        public static IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
        [DllImport("kernel32")]
        public extern static IntPtr CreateFile(string FileName, DesiredAccess da, ShareMode sm, IntPtr SecurityAttributesZero, CreateDisposition cd, FlagsAndAttributes fa, IntPtr TemplateFileZero);
        [DllImport("kernel32")]
        public extern static void CloseHandle(IntPtr handle);
    }

    class DbgHelp
    {
        [DllImport("dbghelp")]
        public extern static SymOptionsFlags SymSetOptions(SymOptionsFlags flags);
        [DllImport("dbghelp")]
        public extern static SymOptionsFlags SymGetOptions();
        [DllImport("dbghelp")]
        public extern static bool SymInitialize(IntPtr hProcess, string UserSearchPath, bool fInvadeProcess);
        [DllImport("dbghelp")]
        public extern static bool SymSetSearchPath(IntPtr hProcess, string SearchPath);
        [DllImport("dbghelp")]
        public extern static bool SymGetSearchPath(IntPtr hProcess, StringBuilder SearchPathResult, int SearchPathLength);
        [DllImport("dbghelp")]
        public extern static long SymLoadModuleEx(IntPtr hProcess, IntPtr hFile, string ImageName, string ModuleName, ulong BaseOfDll, uint DllSize, IntPtr DataZero, SymLoadModuleFlags Flags);
        [DllImport("dbghelp")]
        public extern static bool SymUnloadModule64(IntPtr hProcess, ulong BaseOfDll);
        [DllImport("dbghelp")]
        public extern static bool SymSetContext(IntPtr hProcess, ref IMAGEHLP_STACK_FRAME StackFrame, IntPtr ContextIgnored);
        [DllImport("dbghelp")]
        public extern static bool SymSetScopeFromAddr(IntPtr hProcess, ulong Address);
        [DllImport("dbghelp")]
        public extern static bool SymFromAddr(IntPtr hProcess, ulong Address, IntPtr DisplacementUnused, ref SYMBOL_INFO Symbol);
        [DllImport("dbghelp")]
        public extern static bool SymFromName(IntPtr hProcess, string Name, ref SYMBOL_INFO Symbol);
        [DllImport("dbghelp")]
        public extern static bool SymGetScope(IntPtr hProcess, ulong BaseOfDll, uint Index, ref SYMBOL_INFO Symbol);
        [DllImport("dbghelp")]
        public extern static bool SymGetTypeInfo(IntPtr hProcess, ulong ModBase, uint TypeId, IMAGEHLP_SYMBOL_TYPE_INFO GetType, ref byte[] Data);
        [DllImport("dbghelp")]
        public extern static bool SymGetTypeFromName(IntPtr hProcess, ulong BaseOfDll, string Name, ref SYMBOL_INFO Symbol);
        [DllImport("dbghelp")]
        public extern static bool SymGetLineFromAddr64(IntPtr hProcess, ulong Addr, out IntPtr Displacement, ref IMAGEHLP_LINE64 Line);
        [DllImport("dbghelp")]
        public extern static bool SymEnumSymbols(IntPtr hProcess, ulong BaseOfDll, string Mask, SymEnumSymbolsProc Callback, IntPtr ContextZero);
    }

    public class Variable
    {
        public enum Register 
        {
            CV_REG_NONE = 0,
            CV_REG_AL = 1,
            CV_REG_CL = 2,
            CV_REG_DL = 3,
            CV_REG_BL = 4,
            CV_REG_AH = 5,
            CV_REG_CH = 6,
            CV_REG_DH = 7,
            CV_REG_BH = 8,
            CV_REG_AX = 9,
            CV_REG_CX = 10,
            CV_REG_DX = 11,
            CV_REG_BX = 12,
            CV_REG_SP = 13,
            CV_REG_BP = 14,
            CV_REG_SI = 15,
            CV_REG_DI = 16,
            CV_REG_EAX = 17,
            CV_REG_ECX = 18,
            CV_REG_EDX = 19,
            CV_REG_EBX = 20,
            CV_REG_ESP = 21,
            CV_REG_EBP = 22,
            CV_REG_ESI = 23,
            CV_REG_EDI = 24,
            CV_REG_ES = 25,
            CV_REG_CS = 26,
            CV_REG_SS = 27,
            CV_REG_DS = 28,
            CV_REG_FS = 29,
            CV_REG_GS = 30,
            CV_REG_IP = 31,
            CV_REG_FLAGS = 32,
            CV_REG_EIP = 33,
            CV_REG_EFLAGS = 34,

            /* <pcode> */
            CV_REG_TEMP = 40,
            CV_REG_TEMPH = 41,
            CV_REG_QUOTE = 42,
            CV_REG_PCDR3 = 43,   /* this includes PCDR4 to PCDR7 */
            CV_REG_CR0 = 80,   /* this includes CR1 to CR4 */
            CV_REG_DR0 = 90,   /* this includes DR1 to DR7 */
            /* </pcode> */

            CV_REG_GDTR = 110,
            CV_REG_GDTL = 111,
            CV_REG_IDTR = 112,
            CV_REG_IDTL = 113,
            CV_REG_LDTR = 114,
            CV_REG_TR = 115,

            CV_REG_PSEUDO1 = 116, /* this includes Pseudo02 to Pseuso09 */
            CV_REG_ST0 = 128, /* this includes ST1 to ST7 */
            CV_REG_CTRL = 136,
            CV_REG_STAT = 137,
            CV_REG_TAG = 138,
            CV_REG_FPIP = 139,
            CV_REG_FPCS = 140,
            CV_REG_FPDO = 141,
            CV_REG_FPDS = 142,
            CV_REG_ISEM = 143,
            CV_REG_FPEIP = 144,
            CV_REG_FPEDO = 145,
            CV_REG_MM0 = 146, /* this includes MM1 to MM7 */
            CV_REG_XMM0 = 154, /* this includes XMM1 to XMM7 */
            CV_REG_XMM00 = 162,
            CV_REG_XMM0L = 194, /* this includes XMM1L to XMM7L */
            CV_REG_XMM0H = 202, /* this includes XMM1H to XMM7H */
            CV_REG_MXCSR = 211,
            CV_REG_EDXEAX = 212,
            CV_REG_EMM0L = 220,
            CV_REG_EMM0H = 228,
            CV_REG_MM00 = 236,
            CV_REG_MM01 = 237,
            CV_REG_MM10 = 238,
            CV_REG_MM11 = 239,
            CV_REG_MM20 = 240,
            CV_REG_MM21 = 241,
            CV_REG_MM30 = 242,
            CV_REG_MM31 = 243,
            CV_REG_MM40 = 244,
            CV_REG_MM41 = 245,
            CV_REG_MM50 = 246,
            CV_REG_MM51 = 247,
            CV_REG_MM60 = 248,
            CV_REG_MM61 = 249,
            CV_REG_MM70 = 250,
            CV_REG_MM71 = 251
        };
        public string Name;
        public Register Reg;
        public int Offset, Size;
        public bool Local, Parameter, Regval, Regrel;
    }

    public class SymbolContext
    {
        IntPtr hProcess;
        static Random mRandom = new Random();
        static FileMap mFileMap = new FileMap();
        string mReactosOutputPath = "output-i386", mReactosSourcePath = ".";
        public string ReactosOutputPath
        {
            get { return mReactosOutputPath; }
            set
            {
                mReactosOutputPath = value;
                mFileMap.Directories = new string[] { mReactosOutputPath };
                mFileMap.Scan();
                LoadModules();
            }
        }
        public string ReactosSourcePath
        {
            get { return mReactosSourcePath; }
            set
            {
                mReactosSourcePath = value;
            }
        }
        Dictionary<string, ulong> mLoadedModules = new Dictionary<string,ulong>();

        static SymbolContext()
        {
            DbgHelp.SymSetOptions(SymOptionsFlags.SYMOPT_LOAD_LINES);
        }

        public SymbolContext() 
        { 
            hProcess = new IntPtr(mRandom.Next() + 1); 
        }
        public void Initialize()
        {
            DbgHelp.SymInitialize(hProcess, null, false);
            mFileMap.Directories = new string[] { mReactosOutputPath };
        }
        public void SetModuleAddress(string module, ulong addr)
        {
            if (mLoadedModules.ContainsKey(module))
            {
                DbgHelp.SymUnloadModule64(hProcess, mLoadedModules[module]);
            }
            mLoadedModules[module] = addr;
            LoadModule(module, addr);
        }
        public void LoadModule(string module, ulong addr)
        {
            string realPath = mFileMap.GetFilePathFromShortName((Path.GetFileNameWithoutExtension(module) + ".nostrip").ToUpper());
            if (realPath != null)
            {
                long loadResult = DbgHelp.SymLoadModuleEx
                    (hProcess,
                     IntPtr.Zero,
                     realPath,
                     null,
                     addr,
                     0,
                     IntPtr.Zero,
                     0);
            }
        }
        public void LoadModules()
        {
            foreach (string module in mLoadedModules.Keys)
                DbgHelp.SymUnloadModule64(hProcess, mLoadedModules[module]);
        }

        public KeyValuePair<string, int> GetFileAndLine(ulong addr)
        {
            IntPtr displacement = new IntPtr();
            IMAGEHLP_LINE64 lineNumberIdent = new IMAGEHLP_LINE64();
            lineNumberIdent.Initialize();
            if (DbgHelp.SymGetLineFromAddr64
                (hProcess, addr, out displacement, ref lineNumberIdent))
            {
                Console.Out.WriteLine("Got File and Line for addr " + addr + ": " + lineNumberIdent.FileName.ToString() + ":" + lineNumberIdent.LineNumber);
                return new KeyValuePair<string, int>(lineNumberIdent.FileName.ToString(), (int)lineNumberIdent.LineNumber);
            }
            return new KeyValuePair<string, int>("unknown", 0);
        }

        Variable MakeVarFromSym(SYMBOL_INFO symInfo)
        {
            Variable v = new Variable();
            v.Local = (symInfo.Flags & SymInfoFlags.SYMFLAG_LOCAL) != SymInfoFlags.SYMFLAG_NONE;
            v.Parameter = (symInfo.Flags & SymInfoFlags.SYMFLAG_PARAMETER) != SymInfoFlags.SYMFLAG_NONE;
            v.Regval = (symInfo.Flags & SymInfoFlags.SYMFLAG_REGISTER) != SymInfoFlags.SYMFLAG_NONE;
            v.Regrel = (symInfo.Flags & SymInfoFlags.SYMFLAG_REGREL) != SymInfoFlags.SYMFLAG_NONE;
            v.Reg = (Variable.Register)symInfo.Register;
            v.Offset = (int)symInfo.Address;
            v.Size = (int)symInfo.Size;
            v.Name = symInfo.Name;
            return v;
        }

        List<Variable> mVariables = new List<Variable>();
        bool EnumLocals(ref SYMBOL_INFO symInfo, uint symSize, IntPtr contextZero)
        {
            Variable var = MakeVarFromSym(symInfo);
            if (var != null)
                mVariables.Add(var);
            return true;
        }

        public List<Variable> GetLocals(ulong addr)
        {
            lock (mVariables)
            {
                mVariables.Clear();
                IMAGEHLP_STACK_FRAME stackFrame = new IMAGEHLP_STACK_FRAME();
                stackFrame.InstructionOffset = addr;
                // Always succeeds
                DbgHelp.SymSetContext(hProcess, ref stackFrame, IntPtr.Zero);
                DbgHelp.SymEnumSymbols(hProcess, 0, null, EnumLocals, IntPtr.Zero);
                return new List<Variable>(mVariables);
            }
        }

        public Variable GetNamedVar(string name, ulong addr)
        {
            SYMBOL_INFO symInfo = new SYMBOL_INFO();
            symInfo.Initialize();
            if (DbgHelp.SymFromName(hProcess, name, ref symInfo))
                return MakeVarFromSym(symInfo);
            else
                return null;
        }
    }
}
