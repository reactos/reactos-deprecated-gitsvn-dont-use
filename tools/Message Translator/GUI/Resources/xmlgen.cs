using System;
using System.IO;
using System.Text.RegularExpressions;

namespace bugcheck
{
    class Program
    {
        const string header = "<?xml version=\"1.0\" ?>\r\n<BugCheckList>";
        const string open = "\t<BugCheck text=\"";
        const string value = "\" value=\"";
        const string close = "\" />";
        const string footer = "</BugCheckList>";
        const string path = @"C:\WinDDK\6000\inc\api\bugcodes.h";
        const string outpath = @"C:\Users\Ged\MyFiles\ReactOS\Source\tools\Message Translator\GUI\Resources\bugcheck.xml";

        static void Main(string[] args)
        {
            try
            {
                StreamReader sr = new StreamReader(path);
                StreamWriter sw = new StreamWriter(outpath);

                string s = sr.ReadToEnd();

                Regex regex = new Regex(@"#define (?<def>[A-Z_\d]+)\s+\(\(ULONG\)0x(?<num>[\dA-F]+)L?\)");
                MatchCollection list = regex.Matches(s);

                if (list.Count > 0)
                {
                    sw.WriteLine(header);

                    foreach (Match match in list)
                    {
                        string line = open + match.Groups["def"].ToString() + value + match.Groups["num"].ToString() + close;
                        sw.WriteLine(line);

                        Console.WriteLine("{0} - {1}", match.Groups["def"].ToString(), match.Groups["num"].ToString());
                    }

                    sw.WriteLine(footer);
                    sw.Flush();
                }
            }
            catch (Exception)
            {
                Console.WriteLine("doh");
            }
        }
    }
}
