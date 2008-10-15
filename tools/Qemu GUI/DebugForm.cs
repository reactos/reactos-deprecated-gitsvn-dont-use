using System;
using System.IO;
using System.Diagnostics;
using System.ComponentModel;
using System.Windows.Forms;

namespace Qemu_GUI
{
    public partial class DebugForm : Form
    {
        public DebugForm()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        public void Listen()
        {
            this.Show();

            BackgroundWorker work = new BackgroundWorker();
            work.DoWork += new DoWorkEventHandler(work_DoWork);
            work.RunWorkerAsync();
        }

        void work_DoWork(object sender, DoWorkEventArgs e)
        {
            NamedPipe pipe = new NamedPipe();

            //it might take a while for qemu to launch and wait for a pipe
            System.Threading.Thread.Sleep(1000);

            pipe.CreateClientPipe("com_1");

            pipe.PipeReceiveEvent += new PipeReceiveEventHandler(PipeRecievedHandler);
            pipe.PipeErrorEvent += new PipeErrorEventHandler(PipeErrorHandler);

            pipe.ReadLoop();
        }

        private void PipeErrorHandler(object sender, PipeErrorEventArgs args)
        {
            ErrorForm err = new ErrorForm();
            err.txtError.Text += "PipeErrorHandler:" + Environment.NewLine;
            err.txtError.Text += args.ErrorDesc;
            err.Show();
        }

        private void PipeRecievedHandler(object sender, PipeReceiveEventArgs args)
        {
            WriteString(args.Received);
        }


        private delegate void WriteStringDelegate(string text);
        public void WriteString(string text)
        {
            if (txtDebug.InvokeRequired)
                txtDebug.Invoke(new WriteStringDelegate(WriteString), text);
            else
            {
                txtDebug.AppendText(text);
                txtDebug.ScrollToCaret();
            }
        }

         private void tsbuttonCopy_Click(object sender, EventArgs e)
         {
             if(txtDebug.Text != "")
                 Clipboard.SetText(txtDebug.Text);
         }

         private void tsButton_save_Click(object sender, EventArgs e)
         {
             StreamWriter log = null;
             ErrorForm error;
             FileDialog saveFileDialog = new System.Windows.Forms.SaveFileDialog();
             saveFileDialog.Title = "Save file";
             saveFileDialog.Filter = "Text file (*.txt)|*.txt";

             if (saveFileDialog.ShowDialog() == DialogResult.OK)
             {
                 try
                 {
                     log = new StreamWriter(saveFileDialog.FileName);
                     log.Write(txtDebug.Text);
                 }
                 catch (Exception ex)
                 {
                     error = new ErrorForm();
                     error.txtError.Text = "Exception while trying to save file!" + Environment.NewLine;
                     error.txtError.Text =  ex.Message ;
                     error.Show();
                 }
                 log.Close();
             }
         }
    }
}
