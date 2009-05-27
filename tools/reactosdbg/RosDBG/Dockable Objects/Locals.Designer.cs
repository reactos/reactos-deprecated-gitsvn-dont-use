namespace RosDBG
{
    partial class Locals
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Locals));
            this.LocalDataGrid = new System.Windows.Forms.DataGridView();
            ((System.ComponentModel.ISupportInitialize)(this.LocalDataGrid)).BeginInit();
            this.SuspendLayout();
            // 
            // LocalDataGrid
            // 
            this.LocalDataGrid.AllowUserToAddRows = false;
            this.LocalDataGrid.AllowUserToDeleteRows = false;
            this.LocalDataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.LocalDataGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LocalDataGrid.Location = new System.Drawing.Point(0, 0);
            this.LocalDataGrid.Name = "LocalDataGrid";
            this.LocalDataGrid.ReadOnly = true;
            this.LocalDataGrid.Size = new System.Drawing.Size(412, 221);
            this.LocalDataGrid.TabIndex = 0;
            // 
            // Locals
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(412, 221);
            this.Controls.Add(this.LocalDataGrid);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HideOnClose = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Locals";
            this.Text = "Locals";
            ((System.ComponentModel.ISupportInitialize)(this.LocalDataGrid)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView LocalDataGrid;
    }
}
