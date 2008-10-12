namespace RosDBG
{
    partial class Modules
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
            this.ModulesList = new System.Windows.Forms.ListView();
            this.MListAddress = new System.Windows.Forms.ColumnHeader();
            this.MListShortName = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // ModulesList
            // 
            this.ModulesList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.MListAddress,
            this.MListShortName});
            this.ModulesList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ModulesList.FullRowSelect = true;
            this.ModulesList.GridLines = true;
            this.ModulesList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.ModulesList.Location = new System.Drawing.Point(0, 0);
            this.ModulesList.Name = "ModulesList";
            this.ModulesList.Size = new System.Drawing.Size(244, 218);
            this.ModulesList.TabIndex = 0;
            this.ModulesList.UseCompatibleStateImageBehavior = false;
            this.ModulesList.View = System.Windows.Forms.View.Details;
            this.ModulesList.VirtualMode = true;
            this.ModulesList.RetrieveVirtualItem += new System.Windows.Forms.RetrieveVirtualItemEventHandler(this.ModulesList_RetrieveVirtualItem);
            // 
            // MListAddress
            // 
            this.MListAddress.Text = "Address";
            this.MListAddress.Width = 80;
            // 
            // MListShortName
            // 
            this.MListShortName.Text = "ShortName";
            this.MListShortName.Width = 120;
            // 
            // Modules
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ModulesList);
            this.Name = "Modules";
            this.Size = new System.Drawing.Size(244, 218);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView ModulesList;
        private System.Windows.Forms.ColumnHeader MListAddress;
        private System.Windows.Forms.ColumnHeader MListShortName;
    }
}
