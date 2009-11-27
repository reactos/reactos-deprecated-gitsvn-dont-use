using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MsgTrans.Library;

namespace MsgTrans.Library.Tests
{
    [TestClass]
    public class MsgTrans
    {
        private MessageTranslator msgTran;


        public MsgTrans()
        {
            msgTran = new MessageTranslator(new MessageOutput(),
                                            Properties.Settings.Default.NtStatusXml,
                                            Properties.Settings.Default.WinerrorXml,
                                            Properties.Settings.Default.HresultXml,
                                            Properties.Settings.Default.BugcheckXml,
                                            Properties.Settings.Default.WmXml,
                                            Properties.Settings.Default.BugUrl);
            Assert.IsNotNull(msgTran);
        }

        [TestMethod]
        public void TestGetErrorMsg()
        {
            int ErrNum = 18;
            string message = "error " + ErrNum.ToString();
            Assert.IsTrue(msgTran.ParseCommandMessage(null, message));

            Command cmd = msgTran.Messages[0];
            Assert.IsTrue(cmd.MsgType == MessageType.WinError);

            Assert.AreEqual(cmd.MsgType.ToString(), "WinError");
            Assert.AreEqual(cmd.Number, ErrNum);
            Assert.AreEqual(cmd.Hex, ErrNum.ToString("X"));
            Assert.AreEqual(cmd.Code, "ERROR_NO_MORE_FILES");
            string ErrMessage = new System.ComponentModel.Win32Exception(ErrNum).Message;
            Assert.AreEqual(cmd.Message, ErrMessage);
        }

        [TestMethod]
        public void TestNtStatusMsg()
        {
            string ErrNum = "C0000005";
            string message = "error " + ErrNum;
            Assert.IsTrue(msgTran.ParseCommandMessage(null, message));

            Command cmd = null;
            foreach (Command c in msgTran.Messages)
            {
                if (c.MsgType == MessageType.NTSTATUS)
                    cmd = c;
            }
            Assert.IsNotNull(cmd);
            Assert.IsTrue(cmd.MsgType == MessageType.NTSTATUS);
            Assert.AreEqual(cmd.MsgType.ToString(), "NTSTATUS");
            long num = long.Parse(ErrNum, System.Globalization.NumberStyles.HexNumber);
            Assert.AreEqual(cmd.Number, num);
            Assert.AreEqual(cmd.Hex, ErrNum);
            Assert.AreEqual(cmd.Code, "STATUS_ACCESS_VIOLATION");
        }

        [TestMethod]
        public void TestHResultMsg()
        {
            string ErrNum = "80000003";
            string message = "error " + ErrNum;
            Assert.IsTrue(msgTran.ParseCommandMessage(null, message));

            Command cmd = null;
            foreach (Command c in msgTran.Messages)
            {
                if (c.MsgType == MessageType.HRESULT)
                    cmd = c;
            }
            Assert.IsNotNull(cmd);
            Assert.IsTrue(cmd.MsgType == MessageType.HRESULT);
            Assert.AreEqual(cmd.MsgType.ToString(), "HRESULT");
            long num = long.Parse(ErrNum, System.Globalization.NumberStyles.HexNumber);
            Assert.AreEqual(cmd.Number, num);
            Assert.AreEqual(cmd.Hex, ErrNum);
            Assert.AreEqual(cmd.Code, "E_INVALIDARG");
        }

        [TestMethod]
        public void TestBugCheckMsg()
        {
            string ErrNum = "A";
            string message = "error " + ErrNum;
            Assert.IsTrue(msgTran.ParseCommandMessage(null, message));

            Command cmd = null;
            foreach (Command c in msgTran.Messages)
            {
                if (c.MsgType == MessageType.BugCheck)
                    cmd = c;
            }
            Assert.IsNotNull(cmd);
            Assert.IsTrue(cmd.MsgType == MessageType.BugCheck);
            Assert.AreEqual(cmd.MsgType.ToString(), "BugCheck");
            long num = long.Parse(ErrNum, System.Globalization.NumberStyles.HexNumber);
            Assert.AreEqual(cmd.Number, num);
            Assert.AreEqual(cmd.Hex, ErrNum);
            Assert.AreEqual(cmd.Code, "IRQL_NOT_LESS_OR_EQUAL");
        }

        [TestMethod]
        public void TestWindowsMsg()
        {
            int WmNum = 16;
            string message = "wm " + WmNum.ToString();
            Assert.IsTrue(msgTran.ParseCommandMessage(null, message));

            Command cmd = null;
            foreach (Command c in msgTran.Messages)
            {
                if (c.MsgType == MessageType.WinMsg)
                    cmd = c;
            }
            Assert.IsNotNull(cmd);
            Assert.IsTrue(cmd.MsgType == MessageType.WinMsg);

            Assert.AreEqual(cmd.MsgType.ToString(), "WinMsg");
            Assert.AreEqual(cmd.Number, WmNum);
            Assert.AreEqual(cmd.Hex, WmNum.ToString("X"));
            Assert.AreEqual(cmd.Code, "WM_CLOSE");
        }

        [TestMethod]
        public void TestBugUrlMsg()
        {
            int BugNum = 256;
            string message = "bug " + BugNum.ToString();
            Assert.IsTrue(msgTran.ParseCommandMessage(null, message));

            Command cmd = null;
            foreach (Command c in msgTran.Messages)
            {
                if (c.MsgType == MessageType.BugUrl)
                    cmd = c;
            }
            Assert.IsNotNull(cmd);
            Assert.IsTrue(cmd.MsgType == MessageType.BugUrl);
            Assert.AreEqual(cmd.Number, BugNum);
            Assert.AreEqual(cmd.Hex, BugNum.ToString("X"));
            string url = String.Format(Properties.Settings.Default.BugUrl, BugNum.ToString());
            Assert.AreEqual(cmd.Code, url);
            
        }
    }

    public class MessageOutput : IMsgOutput
    {
        public void MsgOut(MessageContext context,
                           string message)
        {
            Console.WriteLine(message);
        }
    }
}
