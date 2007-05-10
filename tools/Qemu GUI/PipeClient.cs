using System;
using System.Windows.Forms;
using System.Threading;

namespace Qemu_GUI
{
    public class PipeClient
    {

        public void Listen(string name)
        {
            /*fix me: use pipes for debug to screen*/
            uint outbuff = 0 ,inbuff = 0;
            PipeHandle hpipe = NamedPipe.Create(name, outbuff, inbuff);
        }


    }
}
