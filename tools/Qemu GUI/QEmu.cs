using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Xml;
using System.Xml.Serialization;

namespace Qemu_GUI
{

    [XmlRoot("Settings")]
    public class QEmu : IDisposable 
    {

        string m_Path;
        Harddisk[] m_HardDisks = new Harddisk[4];

        public QEmu()
        {
            m_Path = ".";

            /* init array */
            for (int i = 0; i < 4; i++)
                m_HardDisks[i] = new Harddisk(); 
        }

        public void Dispose()
        {
          
        }

        public bool CreateImage(string FileName, long Size, string Format)
        {
            long d = Size * 1024;
            string argv = " create -f " + Format + " " + FileName + "." + Format + " " + d.ToString(); ;

            Process Qemu = new Process();
            Qemu.StartInfo.FileName = m_Path + "\\" + "qemu-img.exe";
            Qemu.StartInfo.WorkingDirectory = m_Path;
            Qemu.StartInfo.Arguments = argv;
            try
            {
                Qemu.Start();
            }
            catch
            {
                return false;
            }
            return true;
        }

        public bool Start()
        {
            Process Qemu = new Process();
            Qemu.StartInfo.FileName = m_Path + "\\" + "qemu.exe";
            Qemu.StartInfo.WorkingDirectory = m_Path;
            Qemu.StartInfo.Arguments = GetArgv();
            try
            {
                Qemu.Start();
            }
            catch 
            {
                return false;
            }
            return true;
        }

        [XmlElement("Harddisk")]
        public Harddisk[] Harddisks
        {
            get {return this.m_HardDisks;}
            set {this.m_HardDisks = value;}
        }

        [XmlElement("Path")]
        public string Path
        {
            get { return m_Path;}
            set { m_Path = value;}
        }
	
        private string GetArgv()
        {
            return "";
        }
        
    }

    public class Harddisk
    {
        [XmlAttribute("Path")] public string Path;
        [XmlAttribute("Use")] public bool Use;
        public Harddisk()
        {
        }
    }

}
