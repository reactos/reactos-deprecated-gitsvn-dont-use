namespace RosDBG
{
    partial class RegisterView
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RegisterView));
            this.RegisterGrid = new System.Windows.Forms.PropertyGrid();
            this.SuspendLayout();
            // 
            // RegisterGrid
            // 
            this.RegisterGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.RegisterGrid.Location = new System.Drawing.Point(0, 0);
            this.RegisterGrid.Name = "RegisterGrid";
            this.RegisterGrid.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.RegisterGrid.Size = new System.Drawing.Size(291, 238);
            this.RegisterGrid.TabIndex = 0;
            this.RegisterGrid.ToolbarVisible = false;
            // 
            // RegisterView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(291, 238);
            this.Controls.Add(this.RegisterGrid);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HideOnClose = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "RegisterView";
            this.Text = "Register";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid RegisterGrid;
    }
}
