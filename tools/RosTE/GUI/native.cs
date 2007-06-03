using System.Runtime.InteropServices;

namespace Native
{
    public class Memory
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct MEMORYSTATUSEX
        {
            public uint dwLength;
            public uint dwMemoryLoad;
            public ulong ullTotalPhys;
            public ulong ullAvailPhys;
            public ulong ullTotalPageFile;
            public ulong ullAvailPageFile;
            public ulong ullTotalVirtual;
            public ulong ullAvailVirtual;
            public ulong ullAvailExtendedVirtual;
        }
        [DllImport("kernel32.dll")]
        public static extern bool GlobalMemoryStatusEx(ref MEMORYSTATUSEX ms);

        public static ulong GetTotalMemory()
        {
            MEMORYSTATUSEX memStat = new MEMORYSTATUSEX();
            memStat.dwLength = (uint)Marshal.SizeOf(typeof(MEMORYSTATUSEX));
            GlobalMemoryStatusEx(ref memStat);

            return memStat.ullTotalPhys;
        }
    }
}