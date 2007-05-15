using System;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace Qemu_GUI
{
    public partial class DebugForm : Form
    {
        string talker;
        Process proc;
        ThreadStart start;
        Thread worker;

        public DebugForm()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        public void Listen(Process p, string Talker)
        {
            proc = p;
            talker = Talker;

            start = new ThreadStart(ThreadProc);
            worker = new Thread(start);
            worker.Priority = ThreadPriority.BelowNormal;//let other apps like qemu run before us.
            this.Show();//show the dialog
            worker.Start();
        }
        private void ThreadProc()
        {
            FileStream log;
            txtDebug.Text = "";
            string buffer = "";
            string temp = "";

            /* fix me: writting to a form from a diferent thread, unsafe. */
            try
            {
                log = new FileStream(talker, FileMode.OpenOrCreate, FileAccess.Read, FileShare.ReadWrite);
                while (!proc.HasExited)
                {

                    int data = log.ReadByte();
                    if (data != -1)
                    {
                        buffer += Convert.ToChar(data);
                    }
                    else
                        Thread.Sleep(250);//wait a 1/4 of a second for more data

                    if (buffer.Contains("\n"))
                    {
                        temp = txtDebug.Text + buffer;
                        txtDebug.Text = "";
                        txtDebug.SelectedText = temp;
                        buffer = "";
                    }
                }

                temp = txtDebug.Text + buffer;
                txtDebug.Text = "";
                txtDebug.SelectedText = temp;

                DeleteTalker();
                log.Close();
                //txtDebug.Text += "QEMU GUI: Exited listener!" + Environment.NewLine;
            }
            catch (Exception e)
            {
                /* fix me: writting to a form from a diferent thread, unsafe. */
                try
                {
                    txtDebug.Text += "QEMU GUI: Exited listener on exception!" + Environment.NewLine;
                    txtDebug.Text += e.Message;
                }
                catch
                {}
                DeleteTalker();
            }
            DeleteTalker();
        }
         private void DeleteTalker()
         {
             int attempts = 0;
             /* sometimes it takes a while for qemu to free the handle to the file */
             
             while (File.Exists(talker))
             {
                 try
                 {
                     File.Delete(talker);
                 }
                 catch
                 {
                     // if (File.Exists(data.Debug.SerialPort.FileName))
                     // MessageBox.Show("Warning temporary file still exists!");
                 }
                 attempts++;
                 Thread.Sleep(150);
                 if (attempts > 15)
                 {
                     break;//we've spent 2.25minutes here.. just give up.
                 }
             }
             //MessageBox.Show("Temp File deleted!");
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
                     error.txtError.Text += ex.Message;
                     error.Show();
                 }
                 log.Close();
             }
         }
    }
}
