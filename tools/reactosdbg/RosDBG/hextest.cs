using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Globalization;

namespace Gdb
{
    public class GdbSocketTest
    {
        public static void Main(string []args)
        {
		foreach (string arg in args)
		{
			Console.WriteLine(long.Parse(arg, NumberStyles.HexNumber));
		}
        }
    }
}
