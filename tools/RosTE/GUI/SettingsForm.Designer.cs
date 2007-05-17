namespace RosTEGUI
{
    partial class SettingsForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.listboxSelection = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // listboxSelection
            // 
            this.listboxSelection.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listboxSelection.Cursor = System.Windows.Forms.Cursors.Hand;
            this.listboxSelection.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.listboxSelection.FormattingEnabled = true;
            this.listboxSelection.ItemHeight = 45;
            this.listboxSelection.Items.AddRange(new object[] {
            "test1",
            "test2",
            "test3",
            "test4",
            "test5",
            "test6",
            "test7",
            "test8"});
            this.listboxSelection.Location = new System.Drawing.Point(12, 12);
            this.listboxSelection.Name = "listboxSelection";
            this.listboxSelection.Size = new System.Drawing.Size(74, 362);
            this.listboxSelection.TabIndex = 0;
            this.listboxSelection.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.listboxSelection_DrawItem);
            // 
            // SettingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(443, 391);
            this.Controls.Add(this.listboxSelection);
            this.Name = "SettingsForm";
            this.Text = "SettingsForm";
            this.Load += new System.EventHandler(this.SettingsForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox listboxSelection;
    }
}