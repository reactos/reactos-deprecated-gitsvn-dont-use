using System;
using System.Xml;
using System.Collections;
using System.Collections.Generic;

namespace MsgTrans.Library
{
    public class ErrorCommand : Command
    {
        private NtStatusCommand ntStatus;
        private WinerrorCommand winerror;
        private HResultCommand hresult;
        private List<Command> errorCommands = new List<Command>();

        public ErrorCommand(MessageTranslator msgTrans,
                            string ntstatusXml,
                            string winerrorXml,
                            string hresultXml)
            : base(msgTrans)
        {
            ntStatus = new NtStatusCommand(msgTrans, ntstatusXml);
            winerror = new WinerrorCommand(msgTrans, winerrorXml);
            hresult = new HResultCommand(msgTrans, hresultXml);
        }

        public override string[] AvailableCommands
        {
            get { return new string[] { "error" }; }
        }

        private static int GetSeverity(long error)
        {
            return (int)((error >> 30) & 0x3);
        }

        private static bool IsCustomer(long error)
        {
            return (error & 0x20000000) != 0;
        }
        
        private static bool IsReserved(long error)
        {
            return (error & 0x10000000) != 0;
        }

        private static int GetFacility(long error)
        {
            return (int)((error >> 16) & 0xFFF);
        }
        
        private static short GetCode(long error)
        {
            return (short)((error >> 0) & 0xFFFF);
        }

        private static string FormatSeverity(long error)
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

        private static string FormatFacility(long error)
        {
            int facility = GetFacility(error);
            return facility.ToString();
        }

        private static string FormatCode(long error)
        {
            int code = GetCode(error);
            return code.ToString();
        }

        private void AddErrorCommand(MessageType msgType,
                                     long dec,
                                     string hex,
                                     string code,
                                     string msg)
        {
            MsgType = msgType;
            Number = dec;
            Hex = hex;
            Code = code;
            Message = msg;
            MsgTrans.Messages.Add(this);
        }

        public override  bool Handle(MessageContext context,
                                     string commandName,
                                     string parameters)
        {
            string originalErrorText = parameters.Trim();
            if (originalErrorText.Equals(String.Empty))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          "Please provide an Error Code.");
                return false;
            }
            
            string errorText = originalErrorText;

        retry:
            NumberParser np = new NumberParser();
            if (!np.Parse(errorText))
            {
                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("{0} is not a valid Error Code.",
                                                        originalErrorText));
                return false;
            }
            
            ArrayList descriptions = new ArrayList();

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
                    AddErrorCommand(MessageType.Custom,
                                    np.Decimal,
                                    np.Hex,
                                    description,
                                    null);
                }
                // Reserved bit is set: HRESULT_FROM_NT(ntstatus)
                else if (IsReserved(np.Decimal))
                {
                    int status = (int)(np.Decimal & 0xCFFFFFFF);
                    string description = ntStatus.GetNtstatusDescription(status);
                    
                    if (description == null)
                        description = status.ToString("X");

                    description = String.Format("HRESULT_FROM_NT({0})", description);

                    AddErrorCommand(MessageType.Custom,
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
                        string description = winerror.GetWinerrorDescription(err);
                        
                        if (description == null)
                            description = err.ToString("D");
                        
                        description = String.Format("HRESULT_FROM_WIN32({0})", description);

                        AddErrorCommand(MessageType.Custom,
                                        np.Decimal,
                                        np.Hex,
                                        description,
                                        null);
                    }
                }
            }

            string winerrorDescription = winerror.GetWinerrorDescription(np.Decimal);
            if (winerrorDescription != null)
            {
                string message = new System.ComponentModel.Win32Exception(Convert.ToInt32(np.Decimal)).Message;

                AddErrorCommand(MessageType.WinError,
                                np.Decimal,
                                np.Hex,
                                winerrorDescription,
                                message);
            }

            string ntstatusDescription = ntStatus.GetNtstatusDescription(np.Decimal);
            if (ntstatusDescription != null)
            {
                AddErrorCommand(MessageType.NTStatus,
                                np.Decimal,
                                np.Hex,
                                ntstatusDescription,
                                null);
            }

            string hresultDescription = hresult.GetHresultDescription(np.Decimal);
            if (hresultDescription != null)
            {
                AddErrorCommand(MessageType.HResult,
                                np.Decimal,
                                np.Hex,
                                hresultDescription,
                                null);
            }

            if (MsgTrans.Messages.Count == 0)
            {
                // Last chance heuristics: attempt to parse a 8-digit decimal as hexadecimal
                if (errorText.Length == 8)
                {
                    errorText = "0x" + errorText;
                    goto retry;
                }

                MsgTrans.MsgOutput.MsgOut(context,
                                          String.Format("I don't know about Error Code {0}.",
                                                        originalErrorText));
                return false;
            }

            return true;
        }

        public override string Help()
        {
            return "!error <value>";
        }
    }
}
