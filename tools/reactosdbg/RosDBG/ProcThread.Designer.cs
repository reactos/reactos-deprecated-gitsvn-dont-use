namespace RosDBG
{
    partial class ProcThread
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.Processes = new System.Windows.Forms.DataGridView();
            this.Threads = new System.Windows.Forms.DataGridView();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Processes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Threads)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.Processes);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.Threads);
            this.splitContainer1.Size = new System.Drawing.Size(419, 349);
            this.splitContainer1.SplitterDistance = 139;
            this.splitContainer1.TabIndex = 0;
            // 
            // Processes
            // 
            this.Processes.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.Processes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Processes.Location = new System.Drawing.Point(0, 0);
            this.Processes.Name = "Processes";
            this.Processes.Size = new System.Drawing.Size(419, 139);
            this.Processes.TabIndex = 0;
            this.Processes.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.Processes_CellDoubleClick);
            // 
            // Threads
            // 
            this.Threads.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.Threads.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Threads.Location = new System.Drawing.Point(0, 0);
            this.Threads.Name = "Threads";
            this.Threads.Size = new System.Drawing.Size(419, 206);
            this.Threads.TabIndex = 0;
            this.Threads.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.Threads_CellDoubleClick);
            // 
            // ProcThread
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "ProcThread";
            this.Size = new System.Drawing.Size(419, 349);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.Processes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Threads)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView Processes;
        private System.Windows.Forms.DataGridView Threads;
    }
}
