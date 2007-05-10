using System;
using System.Collections.Generic;
using System.Text;

namespace Qemu_GUI
{
    public abstract class VLan
    {
        public int vlan;

        public abstract override string ToString();
        public VLan()
        {
            
        }
    }

    public class VUser : VLan
    {
        public string hostname;
        public override string ToString()
        {
            return "-net user,vlan=" + vlan + ",hostname=" + hostname + "  ";
        }
    }

    public class VNic : VLan
    {
        public string macAddress;
        public string NicModel;
        public override string ToString()
        {
            return " ";
        }
    }

    /* FIXME: Unimplemented */
    public class VTap : VLan
    {
        public override string ToString()
        {
            return " ";
        }
    }

    /* FIXME: Unimplemented */
    //public class Socket : VLan
    //{
    //    public override string ToString()
    //    {
    //        return " ";
    //    }
    //}

}
