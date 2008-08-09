using System;
using System.IO;
using System.Collections.Generic;

namespace DbgHelpAPI
{
    public class FileMap
    {
        string []mDirectories = new string [] { "output-i386" };
        Dictionary<string, List<string>> mFileByShortName = new Dictionary<string, List<string>>();

        public string GetFilePathFromShortName(string shortname)
        {
            List<string> possibleMatches;
            if (mFileByShortName.TryGetValue(shortname.ToLower(), out possibleMatches) &&
                possibleMatches.Count > 0)
            {
                return possibleMatches[0];
            }
            return null;
        }

        public string []Directories
        {
            get
            {
                return mDirectories;
            }
            set
            {
                mDirectories = value;
                Scan();
            }
        }

        public FileMap()
        {
            Scan();
        }

        // Returns the directories that were scanned
        public string []Scan()
        {
            mFileByShortName.Clear();

            List<string> directoriesScanned = new List<string>();

            foreach (string dir in mDirectories)
                try
                {
                    if (Directory.Exists(dir)) 
                        Scan(dir);
                    directoriesScanned.Add(dir);
                }
                catch (Exception)
                {
                }

            return directoriesScanned.ToArray();
        }

        void RecordFileNamePossibility(string name, string fullpath)
        {
            List<string> possibilities;
            if (mFileByShortName.TryGetValue(name, out possibilities))
                possibilities.Add(fullpath);
            else
                mFileByShortName[name.ToLower()] = new List<string>(new string [] { fullpath });
        }

        void RecordFileName(string name)
        {
            string shortfname = Path.GetFileName(name);
            string shortnoext = Path.GetFileNameWithoutExtension(name);
            RecordFileNamePossibility(shortfname, name);
            RecordFileNamePossibility(shortnoext, name);
        }

        void Scan(string root)
        {
            foreach (string file in Directory.GetFiles(root))
                RecordFileName(file);

            foreach (string dir in Directory.GetDirectories(root))
            {
                try
                {
                    Scan(dir);
                }
                catch (Exception)
                {
                }
            }
        }
    }
}