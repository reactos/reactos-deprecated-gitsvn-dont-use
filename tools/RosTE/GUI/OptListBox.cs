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
        private ImageList myImageList;
        private StringFormat strFmt;

        /// <summary>
        /// The image list accociated with the listbox.
        /// </summary>
        [Description("The image list accociated with the listbox")]
        public ImageList ImageList
        {
            get { return myImageList; }
            set { myImageList = value; }
        }

        public OptListBox()
        {
            strFmt = new StringFormat();
            strFmt.Alignment = StringAlignment.Center;
            strFmt.LineAlignment = StringAlignment.Center;
            this.BorderStyle = BorderStyle.FixedSingle;
            this.Cursor = Cursors.Hand;
            this.DrawMode = DrawMode.OwnerDrawFixed;
            this.FormattingEnabled = true;
        }


        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            // prevent from error Visual Designer
            if (this.Items.Count > 0)
            {                
                OptListBoxItem item = (OptListBoxItem)this.Items[e.Index];
                item.DrawItem(e, this.Margin, Font, strFmt, myImageList);
            }
        }

        protected override void OnMouseEnter(EventArgs e)
        {
            base.OnMouseEnter(e);
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

        public void DrawItem(DrawItemEventArgs e,
                             Padding margin,
                             Font textFont,
                             StringFormat aligment,
                             ImageList imgLst)
        {
            if ((e.State & DrawItemState.Selected) == DrawItemState.Selected)
            {
                e.Graphics.FillRectangle(Brushes.CornflowerBlue, e.Bounds);
            }
            else
            {
                e.Graphics.FillRectangle(Brushes.White, e.Bounds);
            }

            /*
            e.Graphics.DrawLine(Pens.DarkGray,
                                e.Bounds.X,
                                e.Bounds.Y,
                                e.Bounds.X + e.Bounds.Width,
                                e.Bounds.Y);*/

            imgLst.Draw(e.Graphics,
                        e.Bounds.Right - (e.Bounds.Right / 2) - (imgLst.ImageSize.Width / 2),
                        e.Bounds.Top + margin.Top,
                        imgId);

            Rectangle titleBounds = new Rectangle(e.Bounds.X + margin.Horizontal,
                                                  e.Bounds.Y + margin.Top + imgLst.ImageSize.Height,
                                                  e.Bounds.Width - margin.Right - margin.Horizontal,
                                                  (int)textFont.GetHeight() + 2);

            e.Graphics.DrawString(itemText,
                                  textFont,
                                  Brushes.Black,
                                  titleBounds,
                                  aligment);

            e.DrawFocusRectangle();
        }
    }
}