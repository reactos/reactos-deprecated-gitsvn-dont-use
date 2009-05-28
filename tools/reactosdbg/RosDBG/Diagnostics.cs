using System;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;

namespace RosDBG
{
    static class RosDiagnostics
    {
        public enum TraceType
        {
            Info, Error, Exception
        }

        static public void SetupLogger()
        {
            try
            {
                if (Convert.ToBoolean(Settings.AppLogging))
                {
                    File.Delete(Settings.AppLogFile);
                    FileStream traceLogFile = new FileStream(Settings.AppLogFile, FileMode.OpenOrCreate);
                    Trace.Listeners.Add(new TextWriterTraceListener(traceLogFile));
                    Trace.AutoFlush = true;
                }
            }
            catch (DirectoryNotFoundException)
            {
                MessageBox.Show(String.Format("Logging: {0} does not exist.\n" +
                                              "Please use the settings dialog to correct this",
                                              Settings.AppLogFile));
            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Format("Failed to setup logging. Unexpected error:\n {0}",
                                              ex.Message));
            }
        }

        static public void DebugTrace(TraceType type, string msg)
        {
            StackTrace st = new StackTrace(true);
            StackFrame sf = st.GetFrame(1);

            string typeStr = "";
            switch (type)
            {
                case TraceType.Info: typeStr = "Info"; break;
                case TraceType.Error: typeStr = "Err"; break;
                case TraceType.Exception: typeStr = "Exception"; break;
            }

            Trace.WriteLine(String.Format("{0, -9} : {1} {2}:{3} {4}",
                                          typeStr,
                                          msg,
                                          sf.GetMethod().Name,
                                          sf.GetFileName(),
                                          sf.GetFileLineNumber()));
        }
    }
}
