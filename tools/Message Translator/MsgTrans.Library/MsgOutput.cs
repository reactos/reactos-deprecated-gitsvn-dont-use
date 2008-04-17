using System;

namespace MsgTrans.Library
{
    public interface IMsgOutput
    {
        void MsgOut(MessageContext context,
                       string message);
    }
}
