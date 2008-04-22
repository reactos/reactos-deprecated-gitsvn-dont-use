using System;

namespace MsgTrans.Library
{
    public class CustomCommand : Command
    {
        public CustomCommand(MessageTranslator msgTrans)
            : base(msgTrans)
        {
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "custom" }; }
        }

        private int GetSeverity(long error)
        {
            return (int)((error >> 30) & 0x3);
        }

        private bool IsCustomer(long error)
        {
            return (error & 0x20000000) != 0;
        }

        private bool IsReserved(long error)
        {
            return (error & 0x10000000) != 0;
        }

        private int GetFacility(long error)
        {
            return (int)((error >> 16) & 0xFFF);
        }

        private short GetCode(long error)
        {
            return (short)((error >> 0) & 0xFFFF);
        }

        private string FormatSeverity(long error)
        {
            int severity = GetSeverity(error);
            switch (severity)
            {
                case 0: return "SUCCESS";
                case 1: return "INFORMATIONAL";
                case 2: return "WARNING";
                case 3: return "ERROR";
            }
            return null;
        }

        private string FormatFacility(long error)
        {
            int facility = GetFacility(error);
            return facility.ToString();
        }

        private string FormatCode(long error)
        {
            int code = GetCode(error);
            return code.ToString();
        }

        public override bool Handle(MessageContext context,
                                    string commandName,
                                    string parameters)
        {
            string originalErrorText = parameters.Trim();
            if (originalErrorText.Equals(String.Empty))
            {
                return false;
            }

            string errorText = originalErrorText;

            NumberParser np = new NumberParser();
            if (!np.Parse(errorText))
            {
                return false;
            }

            // Error is out of bounds
            if ((ulong)np.Decimal > uint.MaxValue)
            {
                // Do nothing
            }
            // Error is outside of the range [0, 65535]: it cannot be a plain Win32 error code
            else if ((ulong)np.Decimal > ushort.MaxValue)
            {
                // Customer bit is set: custom error code
                if (IsCustomer(np.Decimal))
                {
                    string description = String.Format("[custom, severity {0}, facility {1}, code {2}]",
                                                       FormatSeverity(np.Decimal),
                                                       FormatFacility(np.Decimal),
                                                       FormatCode(np.Decimal));
                    AddMessage(MessageType.Custom,
                                    np.Decimal,
                                    np.Hex,
                                    description,
                                    null);
                }
                // Reserved bit is set: HRESULT_FROM_NT(ntstatus)
                else if (IsReserved(np.Decimal))
                {
                    int status = (int)(np.Decimal & 0xCFFFFFFF);

                    string description;// = ntStatus.GetNtstatusDescription(status);

                    //if (description == null)
                        description = status.ToString("X");

                    description = String.Format("HRESULT_FROM_NT({0})", description);

                    AddMessage(MessageType.Custom,
                                    np.Decimal,
                                    np.Hex,
                                    description,
                                    null);
                }
                // Win32 facility: HRESULT_FROM_WIN32(winerror)
                else if (GetFacility(np.Decimal) == 7)
                {
                    // Must be an error code
                    if (GetSeverity(np.Decimal) == 2)
                    {
                        short err = GetCode(np.Decimal);
                        string description;// = winerror.GetWinerrorDescription(err);

                        //if (description == null)
                            description = err.ToString("D");

                        description = String.Format("HRESULT_FROM_WIN32({0})", description);

                        AddMessage(MessageType.Custom,
                                        np.Decimal,
                                        np.Hex,
                                        description,
                                        null);
                    }
                }
            }

            return true;
        }
    }
}
