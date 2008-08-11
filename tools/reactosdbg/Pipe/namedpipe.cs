using System;
using System.IO;
using System.ComponentModel;
using System.Runtime.InteropServices;


namespace AbstractPipe
{
    class Kernel32
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr CreateNamedPipe(
            String lpName,
            OpenMode dwOpenMode,
            uint dwPipeMode,
            uint nMaxInstances,
            uint nOutBufferSize,
            uint nInBufferSize,
            DefaultTimeout nDefaultTimeOut,
            IntPtr pipeSecurityDescriptor
            );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool ConnectNamedPipe(
            IntPtr hHandle,
            IntPtr lpOverlapped
            );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool ReadFile(
            IntPtr hHandle,
            byte[] lpBuffer,
            uint nNumberOfBytesToRead,
            ref uint lpNumberOfBytesRead,
            IntPtr lpOverlapped
            );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool WriteFile(
            IntPtr hHandle,
            byte[] lpBuffer,
            uint nNumberOfBytesToWrite,
            ref uint lpNumberOfBytesWritten,
            IntPtr lpOverlapped
            );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool CloseHandle(
            IntPtr hHandle);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern uint GetLastError();

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool DisconnectNamedPipe(
            IntPtr hHandle);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool FlushFileBuffers(IntPtr handle);
    }

    [StructLayout(LayoutKind.Sequential)]
    public class Overlapped
    {
    }

    public enum OpenMode : uint
    {
        PIPE_ACCESS_INBOUND = 0x00000001,
        PIPE_ACCESS_OUTBOUND = 0x00000002,
        PIPE_ACCESS_DUPLEX = 0x00000003,
        FILE_FLAG_FIRST_PIPE_INSTANCE = 0x00080000,
        FILE_FLAG_OVERLAPPED = 0x40000000,
        FILE_FLAG_WRITE_THROUGH = 0x80000000
    }

    public enum DefaultTimeout : uint
    {
        NMPWAIT_USE_DEFAULT_WAIT = 0x00000000,
        NMPWAIT_NOWAIT = 0x00000001,
        NMPWAIT_WAIT_FOREVER = 0xffffffff
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SECURITY_ATTRIBUTES
    {
        public int nLength;
        public IntPtr lpSecurityDescriptor;
        public int bInheritHandle;
    }

    public class NamedPipe// : Pipe
    {
        private const Int32 INVALID_HANDLE_VALUE = -1;
        private const ulong ERROR_PIPE_CONNECTED = 535;

        private IntPtr handle;
        //FileAccess mode;

        public NamedPipe()
        {
            handle = IntPtr.Zero;
        }

        public IntPtr Create(string name)
        {
            handle = Kernel32.CreateNamedPipe(name,
                                              OpenMode.PIPE_ACCESS_DUPLEX | OpenMode.FILE_FLAG_OVERLAPPED,
                                              0,
                                              1,
                                              100,
                                              100,
                                              DefaultTimeout.NMPWAIT_WAIT_FOREVER,
                                              IntPtr.Zero);
            if (handle.ToInt32() == INVALID_HANDLE_VALUE)
            {
                throw new Win32Exception("Error creating named pipe " + name + " . Internal error: " + Marshal.GetLastWin32Error().ToString());
            }

            return handle;
        }

        public void Disconnect()
        {
            Kernel32.DisconnectNamedPipe(handle);
        }

        public void Close()
        {
            Kernel32.CloseHandle(handle);
            handle = IntPtr.Zero;
        }

        public void Flush()
        {
            if (handle == IntPtr.Zero)
                throw new ObjectDisposedException("NamedPipeStream", "The stream has already been closed");
            Kernel32.FlushFileBuffers(handle);
        }

        public bool Listen()
        {
            Disconnect();
            if (Kernel32.ConnectNamedPipe(handle, IntPtr.Zero) != true)
            {
                uint lastErr = (uint)Marshal.GetLastWin32Error();
                if (lastErr == ERROR_PIPE_CONNECTED)
                    return true;
                return false;
            }
            return true;
        }

        public int Read(byte[] buffer, int offset, int count)
        {
            if (buffer == null)
                throw new ArgumentNullException("buffer", "The buffer to read into cannot be null");
            if (buffer.Length < (offset + count))
                throw new ArgumentException("Buffer is not large enough to hold requested data", "buffer");
            if (offset < 0)
                throw new ArgumentOutOfRangeException("offset", offset, "Offset cannot be negative");
            if (count < 0)
                throw new ArgumentOutOfRangeException("count", count, "Count cannot be negative");
            if (handle == IntPtr.Zero)
                throw new ObjectDisposedException("NamedPipeStream", "The stream has already been closed");

            // first read the data into an internal buffer since ReadFile cannot read into a buf at
            // a specified offset
            uint read = 0;

            byte[] buf = buffer;
            if (offset != 0)
            {
                buf = new byte[count];
            }
            bool f = Kernel32.ReadFile(handle, buf, (uint)count, ref read, IntPtr.Zero);
            if (!f)
            {
                throw new Win32Exception(Marshal.GetLastWin32Error(), "ReadFile failed");
            }
            if (offset != 0)
            {
                for (int x = 0; x < read; x++)
                {
                    buffer[offset + x] = buf[x];
                }

            }
            return (int)read;
        }

        public void Write(byte[] buffer, int offset, int count)
        {
            if (buffer == null)
                throw new ArgumentNullException("buffer", "The buffer to write into cannot be null");
            if (buffer.Length < (offset + count))
                throw new ArgumentException("Buffer does not contain amount of requested data", "buffer");
            if (offset < 0)
                throw new ArgumentOutOfRangeException("offset", offset, "Offset cannot be negative");
            if (count < 0)
                throw new ArgumentOutOfRangeException("count", count, "Count cannot be negative");
            if (handle == IntPtr.Zero)
                throw new ObjectDisposedException("NamedPipeStream", "The stream has already been closed");

            // copy data to internal buffer to allow writing from a specified offset
            if (offset != 0)
            {
                byte[] buf = new Byte[count];
                for (int x = 0; x < count; x++)
                {
                    buf[x] = buffer[offset + x];
                }
                buffer = buf;
            }
            uint written = 0;
            bool result = Kernel32.WriteFile(handle, buffer, (uint)count, ref written, IntPtr.Zero);

            if (!result)
            {
                int err = (int)Marshal.GetLastWin32Error();
                throw new Win32Exception(err, "Writing to the stream failed");
            }
            if (written < count)
                throw new IOException("Unable to write entire buffer to stream");
        }
    }
}
