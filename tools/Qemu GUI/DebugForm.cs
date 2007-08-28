using System;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;

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
            if (p.HasExited)
                return;

            proc = p;
            talker = Talker;

            start = new ThreadStart(ThreadProc);
            worker = new Thread(start);
            worker.Priority = ThreadPriority.Normal;
            this.Show();//show the dialog
            worker.Start();
        }
        private void ThreadProc()
        {
            FileStream log = null;
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
                        worker.Priority = ThreadPriority.Normal;
                        buffer += Convert.ToChar(data);

                        if (buffer.Contains("\n"))
                        {
                            temp = txtDebug.Text + buffer;
                            txtDebug.Text = "";
                            txtDebug.SelectedText = temp;
                            buffer = "";
                        }
                    }
                    else
                        worker.Priority = ThreadPriority.Lowest;
                        //Thread.Sleep(50);//wait a 1/4 of a second for more data
                }

                temp = txtDebug.Text + buffer;
                txtDebug.Text = "";
                txtDebug.SelectedText = temp;
                log.Close();
                DeleteTalker();
                //txtDebug.Text += "QEMU GUI: Exited listener!" + Environment.NewLine;
            }
            catch (Exception e)
            {
                /* fix me: writting to a form from a diferent thread, unsafe. */
                try
                {
                    log.Close();
                    txtDebug.Text += "QEMU GUI: Exited listener on exception!" + Environment.NewLine;
                    txtDebug.Text += e.Message;
                }
                catch
                {}
            }
            log.Close();
            DeleteTalker();
        }
         private void DeleteTalker()
         {
             /* sometimes it takes a while for qemu to free the handle to the file */

             while (File.Exists(talker))
             {
                 if (proc.HasExited)
                 {
                     worker.Priority = ThreadPriority.Lowest;

                     try
                     {
                         File.Delete(talker);
                         if (!File.Exists(talker))
                             break;
                     }
                     catch
                     { }
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
