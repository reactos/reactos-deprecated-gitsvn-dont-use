namespace RosDBG
{
    partial class MemoryWindow
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
            this.MemoryAddress = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.MemoryView = new System.Windows.Forms.ListView();
            this.MemoryDataColumn = new System.Windows.Forms.ColumnHeader();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
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
            this.splitContainer1.Panel1.Controls.Add(this.MemoryAddress);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.MemoryView);
            this.splitContainer1.Size = new System.Drawing.Size(409, 341);
            this.splitContainer1.SplitterDistance = 25;
            this.splitContainer1.TabIndex = 0;
            // 
            // MemoryAddress
            // 
            this.MemoryAddress.Location = new System.Drawing.Point(54, 3);
            this.MemoryAddress.Name = "MemoryAddress";
            this.MemoryAddress.Size = new System.Drawing.Size(152, 20);
            this.MemoryAddress.TabIndex = 1;
            this.MemoryAddress.TextChanged += new System.EventHandler(this.MemoryAddress_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Address";
            // 
            // MemoryView
            // 
            this.MemoryView.AutoArrange = false;
            this.MemoryView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.MemoryDataColumn});
            this.MemoryView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MemoryView.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MemoryView.FullRowSelect = true;
            this.MemoryView.GridLines = true;
            this.MemoryView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.MemoryView.LabelEdit = true;
            this.MemoryView.LabelWrap = false;
            this.MemoryView.Location = new System.Drawing.Point(0, 0);
            this.MemoryView.MultiSelect = false;
            this.MemoryView.Name = "MemoryView";
            this.MemoryView.Size = new System.Drawing.Size(409, 312);
            this.MemoryView.TabIndex = 0;
            this.MemoryView.UseCompatibleStateImageBehavior = false;
            this.MemoryView.View = System.Windows.Forms.View.Details;
            this.MemoryView.VirtualListSize = 4096;
            this.MemoryView.VirtualMode = true;
            this.MemoryView.RetrieveVirtualItem += new System.Windows.Forms.RetrieveVirtualItemEventHandler(this.MemoryView_RetrieveVirtualItem);
            // 
            // MemoryDataColumn
            // 
            this.MemoryDataColumn.Text = "Memory Data";
            this.MemoryDataColumn.Width = 1000;
            // 
            // MemoryWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "MemoryWindow";
            this.Size = new System.Drawing.Size(409, 341);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TextBox MemoryAddress;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView MemoryView;
        private System.Windows.Forms.ColumnHeader MemoryDataColumn;
    }
}
