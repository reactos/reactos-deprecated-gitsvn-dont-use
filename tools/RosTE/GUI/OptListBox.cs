using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RosTEGUI
{
    public class OptListBox : ListBox
    {
        private IContainer components;
        private ImageList myImageList;

        public ImageList ImageList
        {
            get { return myImageList; }
            set { myImageList = value; }
        }

        public OptListBox()
        {
            this.BorderStyle = BorderStyle.FixedSingle;
            this.Cursor = Cursors.Hand;
            this.DrawMode = DrawMode.OwnerDrawFixed;
            this.FormattingEnabled = true;
        }

        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            e.DrawFocusRectangle();

            OptListBoxItem item =(OptListBoxItem)Items[e.Index];
            Size imageSize = myImageList.ImageSize;

            if ((e.State & DrawItemState.Selected) == DrawItemState.Selected)
            {
                e.Graphics.FillRectangle(Brushes.Silver, e.Bounds);
            }
            else
            {
                e.Graphics.FillRectangle(Brushes.White, e.Bounds);
            }

            if (item.ImageIndex != -1)
            {
                ImageList.Draw(e.Graphics,
                               e.Bounds.Right - (e.Bounds.Right / 2) - (imageSize.Width / 2),
                               e.Bounds.Top + 4,
                               item.ImageIndex);
            }

            StringFormat strfmt = new StringFormat();
            strfmt.Alignment = StringAlignment.Center;
            strfmt.LineAlignment = StringAlignment.Center;

            e.Graphics.DrawString(item.Text,
                                  this.Font,
                                  Brushes.Black,
                                  e.Bounds.X + e.Bounds.Width / 2,
                                  e.Bounds.Y + e.Bounds.Height - Font.Height,
                                  strfmt);

            base.OnDrawItem(e);
        }
    }


    public class OptListBoxItem
    {
        private string itemText;
        private int imgId;

        public string Text
        {
            get { return itemText; }
            set { itemText = value; }
        }

        public int ImageIndex
        {
            get { return imgId; }
            set { imgId = value; }
        }

        public OptListBoxItem(string text, int index)
        {
            itemText = text;
            imgId = index;
        }

        public override string ToString()
        {
            return itemText;
        }
    }
}