using System;
using System.Globalization;

namespace MsgTrans.Library
{
    public class NumberParser
    {
        //public bool Error = false;
        private const string SpecialHexCharacters = "ABCDEF";

        private long dec = 0;
        private string hex = string.Empty;

        public long Decimal
        {
            get { return dec; }
        }
        public string Hex
        {
            get { return hex; }
        }

        private static bool IsSpecialHexCharacter(char ch)
        {
            foreach (char specialChar in SpecialHexCharacters)
            {
                if (ch.ToString().ToUpper() == specialChar.ToString())
                    return true;
            }
            return false;
        }

        private static bool HasSpecialHexCharacters(string s)
        {
            foreach (char ch in s)
            {
                if (IsSpecialHexCharacter(ch))
                    return true;
            }
            return false;
        }
        
        public bool Parse(string s)
        {
            bool bRet = false;

            try
            {
                bool useHex = false;
                if (s.StartsWith("0x", StringComparison.InvariantCultureIgnoreCase))
                {
                    s = s.Substring(2);
                    useHex = true;
                }

                if (HasSpecialHexCharacters(s))
                    useHex = true;

                if (useHex)
                    dec = Int64.Parse(s,
                                      NumberStyles.HexNumber);
                else
                    dec = Int64.Parse(s);

                hex = dec.ToString("X");

                bRet = true;
            }
            catch (FormatException)
            {
                // Do nothing
            }
            catch (OverflowException)
            {
                // Do nothing
            }

            return bRet;
        }
    }
}
