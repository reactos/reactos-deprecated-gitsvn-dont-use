using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

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
                LinearGradientBrush lgb = new LinearGradientBrush(e.Bounds,
                                                                  Color.LightSteelBlue,
                                                                  Color.SteelBlue,
                                                                  65f,
                                                                  true);
                GraphicsPath gp = BuildPath(e);
                e.Graphics.FillPath(lgb, gp);
                e.Graphics.DrawPath(new Pen(Color.Black, 1), gp);
                DrawGlow(e, gp);

                gp.Dispose();
                lgb.Dispose();
            }
            else
            {
                e.Graphics.FillRectangle(Brushes.White, e.Bounds);
            }

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

        }

        protected GraphicsPath BuildPath(DrawItemEventArgs e)
        {
            Rectangle rct = e.Bounds;
            GraphicsPath gp = new GraphicsPath();
            int ArcWidth = 10;

            rct.Width -= 1;
            rct.Inflate(-1, -1);

            Rectangle arcRct = new Rectangle(rct.X, rct.Y, ArcWidth, ArcWidth);
            Point pt1 = new Point(rct.X + ArcWidth, rct.Y);
            Point pt2 = new Point(rct.X + rct.Width - ArcWidth, rct.Y);


            gp.AddArc(arcRct, 180, 90);
            gp.AddLine(pt1, pt2);

            arcRct.Location = pt2;
            gp.AddArc(arcRct, 270, 90);

            pt1 = new Point(rct.X + rct.Width, rct.Y + ArcWidth);
            pt2 = new Point(rct.X + rct.Width, rct.Y + rct.Height - ArcWidth);
            gp.AddLine(pt1, pt2);

            arcRct.Y = pt2.Y;
            gp.AddArc(arcRct, 0, 90);

            pt1 = new Point(rct.X + rct.Width - ArcWidth, rct.Y + rct.Height);
            pt2 = new Point(rct.X + ArcWidth, rct.Y + rct.Height);
            gp.AddLine(pt1, pt2);

            arcRct.X = rct.X;
            gp.AddArc(arcRct, 90, 90);

            gp.CloseFigure();

            return gp;
        }

        private void DrawGlow(DrawItemEventArgs e, GraphicsPath gp)
        {
            RectangleF rctf = gp.GetBounds();

            LinearGradientBrush lgBrush = new LinearGradientBrush(rctf,
                                                                  Color.FromArgb(150, Color.White),
                                                                  Color.FromArgb(0, Color.White),
                                                                  90f,
                                                                  true);
            e.Graphics.FillPath(lgBrush, gp);
            lgBrush.Dispose();
        }
    }
}