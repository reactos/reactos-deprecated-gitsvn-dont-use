using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Text;
using System.Security;
using System.Threading;
using System.Runtime.InteropServices;

namespace Qemu_GUI
{
    // This utility class exposes kernel32.dll methods for named pipes communication.
        [SuppressUnmanagedCodeSecurity]
        public class NamedPipeNative
        {
            #region Constants
            public const uint PIPE_ACCESS_OUTBOUND = 0x00000002;
            public const uint PIPE_ACCESS_DUPLEX = 0x00000003;
            public const uint PIPE_ACCESS_INBOUND = 0x00000001;
            public const uint PIPE_WAIT = 0x00000000;
            public const uint PIPE_NOWAIT = 0x00000001;
            public const uint PIPE_READMODE_BYTE = 0x00000000;
            public const uint PIPE_READMODE_MESSAGE = 0x00000002;
            public const uint PIPE_TYPE_BYTE = 0x00000000;
            public const uint PIPE_TYPE_MESSAGE = 0x00000004;
            public const uint PIPE_CLIENT_END = 0x00000000;
            public const uint PIPE_SERVER_END = 0x00000001;
            public const uint PIPE_UNLIMITED_INSTANCES = 255;
            public const uint NMPWAIT_WAIT_FOREVER = 0xffffffff;
            public const uint NMPWAIT_NOWAIT = 0x00000001;
            public const uint NMPWAIT_USE_DEFAULT_WAIT = 0x00000000;
            public const uint GENERIC_READ = (0x80000000);
            public const uint GENERIC_WRITE = (0x40000000);
            public const uint GENERIC_EXECUTE = (0x20000000);
            public const uint GENERIC_ALL = (0x10000000);
            public const uint CREATE_NEW = 1;
            public const uint CREATE_ALWAYS = 2;
            public const uint OPEN_EXISTING = 3;
            public const uint OPEN_ALWAYS = 4;
            public const uint TRUNCATE_EXISTING = 5;
            public const int INVALID_HANDLE_VALUE = -1;
            public const ulong ERROR_SUCCESS = 0;
            public const ulong ERROR_CANNOT_CONNECT_TO_PIPE = 2;
            public const ulong ERROR_PIPE_BUSY = 231;
            public const ulong ERROR_NO_DATA = 232;
            public const ulong ERROR_PIPE_NOT_CONNECTED = 233;
            public const ulong ERROR_MORE_DATA = 234;
            public const ulong ERROR_PIPE_CONNECTED = 535;
            public const ulong ERROR_PIPE_LISTENING = 536;
            #endregion

            #region Imports
            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern IntPtr CreateNamedPipe(
                String lpName,
                uint dwOpenMode,
                uint dwPipeMode,
                uint nMaxInstances,
                uint nOutBufferSize,
                uint nInBufferSize,
                uint nDefaultTimeOut,
                IntPtr pipeSecurityDescriptor
                );

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool GetNamedPipeInfo(
                IntPtr hNamedPipe,
                ref uint lpFlags,
                ref uint lpOutBufferSize,
                ref uint lpInBufferSize,
                ref uint lpMaxInstances
                );


            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool ConnectNamedPipe(
                IntPtr hHandle,
                Overlapped lpOverlapped
                );

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool CallNamedPipe(
                string lpNamedPipeName,
                byte[] lpInBuffer,
                uint nInBufferSize,
                byte[] lpOutBuffer,
                uint nOutBufferSize,
                byte[] lpBytesRead,
                int nTimeOut
                );

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern IntPtr CreateFile(
                String lpFileName,
                uint dwDesiredAccess,
                uint dwShareMode,
                SecurityAttributes attr,
                uint dwCreationDisposition,
                uint dwFlagsAndAttributes,
                uint hTemplateFile);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool ReadFile(
                IntPtr hHandle,	
                byte[] lpBuffer,
                uint nNumberOfBytesToRead,
                byte[] lpNumberOfBytesRead,
                uint lpOverlapped	
                );



            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool WriteFile(
                IntPtr hHandle,
                byte[] lpBuffer,
                uint nNumberOfBytesToWrite,
                byte[] lpNumberOfBytesWritten,
                uint lpOverlapped
                );

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool GetNamedPipeHandleState(
                IntPtr hHandle,
                IntPtr lpState,
                ref uint lpCurInstances,
                IntPtr lpMaxCollectionCount,
                IntPtr lpCollectDataTimeout,
                IntPtr lpUserName,
                IntPtr nMaxUserNameSize
                );

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool CancelIo(
                IntPtr hHandle
                );

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool WaitNamedPipe(
                String name,
                uint timeout);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern uint GetLastError();

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool FlushFileBuffers(
                IntPtr hHandle);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool DisconnectNamedPipe(
                IntPtr hHandle);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool SetNamedPipeHandleState(
                IntPtr hHandle,
                ref uint mode,
                IntPtr cc,
                IntPtr cd);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool CloseHandle(
                IntPtr hHandle);

            [DllImport("Advapi32.dll", SetLastError = true)]
            public static extern bool SetSecurityDescriptorDacl(ref SECURITY_DESCRIPTOR sd, bool bDaclPresent, IntPtr Dacl, bool bDaclDefaulted);

            [DllImport("Advapi32.dll", SetLastError = true)]
            public static extern bool InitializeSecurityDescriptor(out SECURITY_DESCRIPTOR sd, int dwRevision);
            #endregion
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_DESCRIPTOR
        {
            private byte Revision;
            private byte Sbz1;
            private ushort Control;
            private IntPtr Owner;
            private IntPtr Group;
            private IntPtr Sacl;
            private IntPtr Dacl;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES
        {
            public int nLength;
            public IntPtr lpSecurityDescriptor;
            public bool bInheritHandle;
        }

        [StructLayout(LayoutKind.Sequential)]
        public class Overlapped
        {
        }

        [StructLayout(LayoutKind.Sequential)]
        public class SecurityAttributes
        {
        }

    public enum PipeState
    {
        NotSet = 0,
        Error = 1,
        Creating = 2,
        Created = 3,
        WaitingForClient = 4,
        ConnectedToClient = 5,
        ConnectingToServer = 6,
        ConnectedToServer = 7,
        Reading = 8,
        ReadData = 9,
        Writing = 10,
        WroteData = 11,
        Flushing = 12,
        FlushedData = 13,
        Disconnecting = 14,
        Disconnected = 15,
        Closing = 16,
        Closed = 17,
    }

    public class PipeHandle
    {
        public IntPtr Handle;
        public PipeState State;

        public PipeHandle()
        {
            this.Handle = new IntPtr(NamedPipeNative.INVALID_HANDLE_VALUE);
            this.State = PipeState.NotSet;
        }

        public PipeHandle(int hnd)
        {
            this.Handle = new IntPtr(hnd);
            this.State = PipeState.NotSet;
        }

    }
    public class NamedPipe
    {

        private const int ATTEMPTS = 2; //retries when creating a pipe or connecting to a pipe
        //private const int WAIT_TIME = 1000;

        public static bool SetNamedPipeHandleState(PipeHandle pipe, ref uint mode)
        {
            return NamedPipeNative.SetNamedPipeHandleState(pipe.Handle, ref mode, IntPtr.Zero, IntPtr.Zero);
        }

        public static string Read(PipeHandle handle, int maxBytes)
        {
            string returnVal = "";
            byte[] bytes = ReadBytes(handle, maxBytes);
            if (bytes != null)
            {
                returnVal = System.Text.Encoding.UTF8.GetString(bytes);
            }
            return returnVal;
        }

		public static byte[] ReadBytes(PipeHandle handle, int maxBytes)
        {
			byte[] numReadWritten = new byte[4];
			byte[] intBytes = new byte[4];
			byte[] msgBytes = null;
			int len = 0;
			
			// Set the Handle state to Reading
			handle.State = PipeState.Reading;
			// Read the first four bytes and convert them to integer
			if (NamedPipeNative.ReadFile(handle.Handle, intBytes, 4, numReadWritten, 0))
            {
				len = BitConverter.ToInt32(intBytes, 0);
				msgBytes = new byte[len];
				// Read the rest of the data
				if (!NamedPipeNative.ReadFile(handle.Handle, msgBytes, (uint)len, numReadWritten, 0))
                {
					handle.State = PipeState.Error;
					MessageBox.Show("Error reading from pipe. Internal error: " + NamedPipeNative.GetLastError().ToString());
				}
			}
			else 
            {
				handle.State = PipeState.Error;
				MessageBox.Show("Error reading from pipe. Internal error: " + NamedPipeNative.GetLastError().ToString());
			}
			handle.State = PipeState.ReadData;
			if (len > maxBytes)
            {
				return null;
			}
			return msgBytes;
		}

        public static PipeHandle ConnectToPipe(string pipeName)
        {
			PipeHandle handle = new PipeHandle();
			// Build the name of the pipe.
			string name = "\\\\" + "." + "\\pipe\\" + pipeName;

            NamedPipeNative.WaitNamedPipe(name, NamedPipeNative.NMPWAIT_USE_DEFAULT_WAIT);

			for (int i = 1; i<=ATTEMPTS; i++)
            {
				handle.State = PipeState.ConnectingToServer;
				// Try to connect to the server
				handle.Handle = NamedPipeNative.CreateFile(name, NamedPipeNative.GENERIC_READ | NamedPipeNative.GENERIC_WRITE, 0, null, NamedPipeNative.OPEN_EXISTING, 0, 0);
				if (handle.Handle.ToInt32() != NamedPipeNative.INVALID_HANDLE_VALUE)
                {
					// The client managed to connect to the server pipe
                    handle.State = PipeState.ConnectedToServer;
					// Set the read mode of the pipe channel
					uint mode = NamedPipeNative.PIPE_READMODE_MESSAGE;
					if (NamedPipeNative.SetNamedPipeHandleState(handle.Handle, ref mode, IntPtr.Zero, IntPtr.Zero)) 
                    {
						break;
					}	
					if (i >= ATTEMPTS) 
                    {
                        handle.State = PipeState.Error;
                        //MessageBox.Show("Error setting read mode on pipe " + name + " . Internal error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
                    }
                }
                if (i >= ATTEMPTS)
                {
                    if (NamedPipeNative.GetLastError() != NamedPipeNative.ERROR_PIPE_BUSY)
                    {
                        handle.State = PipeState.Error;
                        // After a certain number of unsuccessful attempts
                        //MessageBox.Show("Error connecting to pipe " + name + " . Winerror error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
                    }
                    else
                    {
                        handle.State = PipeState.Error;
                       // MessageBox.Show("Pipe " + name + " is too busy. Internal error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
                    }
                }
                else
                {
                    // The pipe is busy so lets wait for some time and try again
                    if (NamedPipeNative.GetLastError() == NamedPipeNative.ERROR_PIPE_BUSY)
                        NamedPipeNative.WaitNamedPipe(name, NamedPipeNative.NMPWAIT_USE_DEFAULT_WAIT);
                }
            }

            return handle;
        }

        public static PipeHandle Create(string name, uint outBuffer, uint inBuffer)
        {
            bool secure = true;

            name = @"\\.\pipe\" + name;
            PipeHandle handle = new PipeHandle();
            IntPtr secAttr = IntPtr.Zero;
            SECURITY_ATTRIBUTES sa = new SECURITY_ATTRIBUTES();

            if (!secure)
            {
                SECURITY_DESCRIPTOR sd;
                GetNullDaclSecurityDescriptor(out sd);
                sa.lpSecurityDescriptor = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SECURITY_DESCRIPTOR)));
                Marshal.StructureToPtr(sd, sa.lpSecurityDescriptor, false);
                sa.bInheritHandle = false;
                sa.nLength = Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES));
                secAttr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES)));
                Marshal.StructureToPtr(sa, secAttr, false);
            }

            try
            {
                for (int i = 1; i <= ATTEMPTS; i++)
                {
                    handle.State = PipeState.Creating;
                    handle.Handle = NamedPipeNative.CreateNamedPipe(
                        name,
                        NamedPipeNative.PIPE_ACCESS_DUPLEX,
                        NamedPipeNative.PIPE_TYPE_MESSAGE | NamedPipeNative.PIPE_READMODE_MESSAGE | NamedPipeNative.PIPE_WAIT,
                        1,
                        outBuffer,
                        inBuffer,
                        NamedPipeNative.NMPWAIT_WAIT_FOREVER,
                        secAttr);
                    if (handle.Handle.ToInt32() != NamedPipeNative.INVALID_HANDLE_VALUE)
                    {
                        handle.State = PipeState.Created;
                        break;
                    }
                    if (i >= ATTEMPTS)
                    {
                        handle.State = PipeState.Error;
                        MessageBox.Show("Error creating named pipe " + name + " . Internal error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
                    }
                }
            }
            finally
            {
                if (!secure)
                {
                    Marshal.FreeHGlobal(sa.lpSecurityDescriptor);
                    Marshal.FreeHGlobal(secAttr);
                }
            }
            return handle;
        }

        public static void GetNullDaclSecurityDescriptor(out SECURITY_DESCRIPTOR sd)
        {
            if (NamedPipeNative.InitializeSecurityDescriptor(out sd, 1))
            {
                if (!NamedPipeNative.SetSecurityDescriptorDacl(ref sd, true, IntPtr.Zero, false))
                {
                    MessageBox.Show("Error setting SECURITY_DESCRIPTOR attributes. Internal error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
                }
            }
            else
            {
                MessageBox.Show("Error setting SECURITY_DESCRIPTOR attributes. Internal error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
            }
        }

        public static uint NumberPipeInstances(PipeHandle handle)
        {
            uint curInstances = 0;

            if (NamedPipeNative.GetNamedPipeHandleState(handle.Handle, IntPtr.Zero, ref curInstances, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero))
            {
                return curInstances;
            }
            else
            {
                MessageBox.Show("Error getting the pipe state. Internal error: " + NamedPipeNative.GetLastError().ToString(), NamedPipeNative.GetLastError().ToString());
                return 0;
            }
        }

        public static void Close(PipeHandle handle)
        {
            handle.State = PipeState.Closing;
            NamedPipeNative.CloseHandle(handle.Handle);
            handle.Handle = IntPtr.Zero;
            handle.State = PipeState.Closed;
        }

        public static void Flush(PipeHandle handle)
        {
            handle.State = PipeState.Flushing;
            NamedPipeNative.FlushFileBuffers(handle.Handle);
            handle.State = PipeState.FlushedData;
        }

        public static bool Disconnect(PipeHandle handle)
        {
            bool state;
            handle.State = PipeState.Disconnecting;
            state = NamedPipeNative.DisconnectNamedPipe(handle.Handle);
            handle.State = PipeState.Disconnected;
            return state;
        }

    }


    
}

