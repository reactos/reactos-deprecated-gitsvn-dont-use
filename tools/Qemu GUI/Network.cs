using System;
using System.Collections.Generic;
using System.Text;

namespace Qemu_GUI
{
    public enum NicModel
    {
        ne2k_pci,//rtl8029
        ne2k_isa,//rtl8029
        rtl8139,
        //smc91c11,
        pcnet
    };

    public abstract class VLan
    {
        public int vlan;

        public abstract override string ToString();
        public VLan() { }
    }

    public class VUser : VLan
    {
        public string hostname;

        public VUser() { }
        public VUser(string host, int vl)
        {
            hostname = host;
            vlan = vl;
        }

        public override string ToString()
        {
            //-net user[,vlan=n][,hostname=host]
            return "-net user,vlan=" + vlan + ",hostname=" + hostname + " ";
        }
    }

    public class VNic : VLan
    {
        public string macAddress;
        public NicModel _NicModel;

        public VNic() { }

        public VNic(string mac, NicModel mod)
        {
            macAddress = mac;
            _NicModel = mod;
        }

        public override string ToString()
        {
            //-net nic[,vlan=n][,macaddr=addr][,model=type]
            if(macAddress != "")
                return "-net nic,vlan=" + vlan + ",macaddr=" + macAddress + ",model=" + _NicModel + " ";
            else
                return "-net nic,vlan=" + vlan + ",model=" + _NicModel + " ";
        }
    }

    /* Unimplemented */
    //public class VTap : VLan
    //{
    //    public override string ToString()
    //    {
    //        return " ";
    //    }
    //}

    /* Unimplemented */
    //public class Socket : VLan
    //{
    //    public override string ToString()
    //    {
    //        return " ";
    //    }
    //}

}
