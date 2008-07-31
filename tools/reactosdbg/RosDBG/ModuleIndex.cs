using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RosDBG
{
    class ModuleIndex
    {
        string mReactOSBuild;
        Dictionary<string, string> mModcache = new Dictionary<string, string>();

        public string ReactOSBuild
        {
            get { return mReactOSBuild; }
            set 
            { 
                mReactOSBuild = value;
                mModcache.Clear();
                if (mReactOSBuild != null)
                    ReadDirs(mReactOSBuild);
            }
        }

        void ReadDirs(string dir)
        {
            foreach (string subdir in Directory.GetDirectories(dir))
                ReadDirs(Path.Combine(dir, subdir));

            foreach (string file in Directory.GetFiles(dir))
                mModcache[Path.GetFileNameWithoutExtension(file).ToLowerInvariant()] =
                    Path.Combine(dir, file);
        }

        public ModuleIndex()
        {
        }

        public string GetModuleByName(string name)
        {
            string result;
            if (mModcache.TryGetValue(name.ToLowerInvariant(), out result))
                return result;
            else
                return null;
        }
    }
}
