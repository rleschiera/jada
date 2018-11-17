using System;
using System.Drawing;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;

namespace winagent
{
    public partial class AgtFormDraw : Form
    {
        private Color color = Color.Black;
        private List<JADA.Draw> myCurves = new List<JADA.Draw>();

        static readonly IntPtr HWND_TOPMOST = new IntPtr(-1);
        const UInt32 SWP_NOSIZE = 0x0001;
        const UInt32 SWP_NOMOVE = 0x0002;
        const UInt32 TOPMOST_FLAGS = SWP_NOMOVE | SWP_NOSIZE;

        [DllImport("user32.dll")] [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);
        [DllImport("user32.dll")]
        static extern IntPtr GetDC(IntPtr hWnd);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern IntPtr FindWindow(string strClassName, string strWindowName);

        public AgtFormDraw()
        {
            InitializeComponent();
        }

        private void FormDraw_Load(object sender, EventArgs e)
        {
            SetWindowPos(this.Handle, HWND_TOPMOST, 0, 0, 0, 0, TOPMOST_FLAGS);
        }

        private void FormDraw_Activate(object sender, EventArgs e)
        {
            this.Left = Screen.PrimaryScreen.Bounds.Left;
            this.Top = Screen.PrimaryScreen.Bounds.Top;
            this.Width = Screen.PrimaryScreen.Bounds.Width;
            this.Height = Screen.PrimaryScreen.Bounds.Height;
        }

        public void DrawOpened()
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(delegate
                {
                    this.Clean();
                    this.Show();
                }));
            }
            else
            {
                this.Clean();
                this.Show();
            }
        }

        public void DrawClosed()
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(delegate
                {
                    this.Hide();
                    this.Clean();
                }));
            }
            else
            {
                this.Hide();
                this.Clean();
            }
        }

        public void SetPointer(int iPointerType, int iPointX, int iPointY)
        {
            int iOffsetX, iOffsetY;

            if (this.picPointer.InvokeRequired) {
                this.picPointer.Invoke(new MethodInvoker(delegate
                {
                    picPointer.Visible = false;

                    switch (iPointerType)
                    {
                        case JADA.Common.POINTER_HAND:
                            picPointer.Image = winagent.Properties.Resources.hand;
                            iOffsetX = (this.picPointer.Width); iOffsetY = (this.picPointer.Height / 2);
                            break;
                        case JADA.Common.POINTER_MOUSE_HAND:
                            picPointer.Image = winagent.Properties.Resources.mouse_hand;
                            iOffsetX = (0); iOffsetY = (0);
                            break;
                        case JADA.Common.POINTER_ARROW:
                            picPointer.Image = winagent.Properties.Resources.arrow;
                            iOffsetX = (this.picPointer.Width); iOffsetY = (this.picPointer.Height / 2);
                            break;
                        case JADA.Common.POINTER_CURVED_ARROW:
                            picPointer.Image = winagent.Properties.Resources.curved_arrow;
                            iOffsetX = (this.picPointer.Width); iOffsetY = (this.picPointer.Height / 2);
                            break;
                        case JADA.Common.POINTER_ROUND_ARROW:
                            picPointer.Image = winagent.Properties.Resources.round_arrow;
                            iOffsetX = (this.picPointer.Width); iOffsetY = (this.picPointer.Height / 2);
                            break;
                        default:
                            return;
                    }
                    this.picPointer.Left = iPointX - iOffsetX;
                    this.picPointer.Top = iPointY - iOffsetY - SystemInformation.CaptionHeight;

                    this.picPointer.Invalidate();
                    this.picPointer.Visible = true;
                }));
            }
        }

        public void ResetPointer()
        {
            if (this.picPointer.InvokeRequired)
            {
                this.picPointer.Invoke(new MethodInvoker(delegate
                {
                    picPointer.Visible = false;
                }));
            }
            else
            {
                picPointer.Visible = false;
            }
        }

        public void Draw(JADA.Draw drwSelf)
        {
            int c, p;

            drwSelf.Duplicate();
            myCurves.Add(drwSelf);
 
            this.Invoke(new MethodInvoker(delegate
            {
                using (Graphics gr = Graphics.FromHdc(GetDC(this.Handle)))
                {
                    for (c = 0; c < drwSelf.CurveCount; c++)
                    {
                        JADA.Curve crvSelf = drwSelf.GetCurve(c);
                        Color myColor = Color.FromArgb(crvSelf.Alpha, crvSelf.Red, crvSelf.Green, crvSelf.Blue);

                        using (Pen myPen = new Pen(myColor, crvSelf.Width))
                        {
                            if (crvSelf.CoordinatesCount > 1)
                            {
                                List<Point> points = new List<Point>();

                                for (p = 0; p <= crvSelf.CoordinatesCount - 1; p++)
                                {
                                    int X = 0, Y = 0;

                                    crvSelf.GetCoordinate(p, ref X, ref Y);
                                    points.Add(new Point(X, Y - SystemInformation.CaptionHeight));
                                    // JADA.Common.LogMessage(string.Format("DrawCurve: point {0} is {1},{2}\n", p, X, Y));
                                    // winagent.Globals.jadaSelf.LogMessage(string.Format("DrawCurve: point {0} is {1},{2}\n", p, X, Y));
                                }

                                gr.DrawCurve(myPen, points.ToArray(), 0.5f);
                            }
                        }
                    }
                }
            }));
        }

        public void Clean()
        {
            this.picPointer.Visible = false;
            myCurves.Clear();
            this.Invalidate();
        }
    }
}