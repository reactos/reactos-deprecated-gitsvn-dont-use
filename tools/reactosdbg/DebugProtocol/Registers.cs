using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace DebugProtocol
{
    public class UlongToHexTypeConverter : TypeConverter
    {
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;
            else
                return base.CanConvertFrom(context, sourceType);
        }

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(string))
                return true;
            else
                return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
        {
            if (destinationType == typeof(string) && value.GetType() == typeof(ulong))
                return string.Format("0x{0:X8}", value);
            else
                return base.ConvertTo(context, culture, value, destinationType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value.GetType() == typeof(string))
            {
                string input = (string)value;

                if (input.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
                {
                    input = input.Substring(2);
                }

                return ulong.Parse(input, NumberStyles.HexNumber, culture);
            }
            else
            {
                return base.ConvertFrom(context, culture, value);
            }
        }
    }

    public class Registers
    {
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Eax { get { return RegisterSet[0]; } set { RegisterSet[0] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Ecx { get { return RegisterSet[1]; } set { RegisterSet[1] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Edx { get { return RegisterSet[2]; } set { RegisterSet[2] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Ebx { get { return RegisterSet[3]; } set { RegisterSet[3] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Esp { get { return RegisterSet[4]; } set { RegisterSet[4] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Ebp { get { return RegisterSet[5]; } set { RegisterSet[5] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Esi { get { return RegisterSet[6]; } set { RegisterSet[6] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Edi { get { return RegisterSet[7]; } set { RegisterSet[7] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Eip { get { return RegisterSet[8]; } set { RegisterSet[8] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Eflags { get { return RegisterSet[9]; } set { RegisterSet[9] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Cs { get { return RegisterSet[10]; } set { RegisterSet[10] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Ds { get { return RegisterSet[11]; } set { RegisterSet[11] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Es { get { return RegisterSet[12]; } set { RegisterSet[12] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Fs { get { return RegisterSet[13]; } set { RegisterSet[13] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Gs { get { return RegisterSet[14]; } set { RegisterSet[14] = value; } }
        [TypeConverter(typeof(UlongToHexTypeConverter))]
        public ulong Ss { get { return RegisterSet[15]; } set { RegisterSet[15] = value; } }
        public ulong[] RegisterSet = new ulong[32];
    }
}
