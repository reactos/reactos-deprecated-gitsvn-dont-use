using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace buildhelper
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 1) // updating Version Info
            {
                string infoPath = args[0];
                if (!infoPath.EndsWith("AssemblyInfo.cs"))
                {
                    Console.WriteLine("Please specify the 'AssemblyInfo.cs' you want to update.");
                    return;
                }

                if (!File.Exists(infoPath))
                {
                    Console.WriteLine(infoPath + " does not exist.");
                    return;
                }

                try
                {
                    StreamReader reader = File.OpenText(infoPath);
                    string contents = reader.ReadToEnd();
                    reader.Close();
                    Regex version = new Regex(@"(\d+\.\d+\.)(\d+)(\.)(\d+)");
                    Match versionMatch = version.Match(contents);
                    string oldVersion = versionMatch.Value;
#if DEBUG
                    string newVersion = versionMatch.Groups[1].Value +
                        versionMatch.Groups[2].Value +
                        versionMatch.Groups[3].Value +
                        (Convert.ToUInt32(versionMatch.Groups[4].Value) + 1).ToString();
#else
                    string newVersion = versionMatch.Groups[1].Value + 
                        (Convert.ToUInt32(versionMatch.Groups[2].Value) + 1).ToString() +
                        versionMatch.Groups[3].Value +
                        (Convert.ToUInt32(versionMatch.Groups[4].Value) + 1).ToString();
#endif
                    contents = contents.Replace(oldVersion, newVersion);
                    StreamWriter writer = File.CreateText(infoPath);
                    writer.Write(contents);
                    writer.Close();
                    Console.WriteLine(string.Format("New version is [{0}]", newVersion));
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
                return;
            }

            if (args.Length == 2) // updating WiX Version Info
            {
                FileVersionInfo version = FileVersionInfo.GetVersionInfo(args[0]);
                StreamWriter sw = new StreamWriter(args[1]);
                sw.WriteLine("<Include>");
                sw.WriteLine("  <?define version=\"" + version.ProductVersion + "\" ?>");
                sw.WriteLine("</Include>");
                sw.Close();
            }
        }
    }
}
