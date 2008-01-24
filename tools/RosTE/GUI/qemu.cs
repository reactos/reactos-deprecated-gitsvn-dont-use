using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Management;
using System.IO;
using System.Diagnostics;


namespace RosTEGUI
{
    public class Qemu
    {
        public enum Platforms
        {
            x86 = 0,
            x86_ISA = 1,
            x64 = 2,
            x64_ISA = 3,
            ARM_integratorcp926 = 4,
            ARM_integratorcp1026 = 5,
            ARM_versatilepb = 6,
            ARM_versatileab = 7,
            PPC_g3bw = 8,
            PPC_mac99 = 9,
            PPC_prep = 10,
            Sparc_sun4m = 11
        }

        public Qemu()
        {
        }

    }

    public class QemuImg
    {

    }


}