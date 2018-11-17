using System;
using System.Drawing;
using System.Windows.Forms;
using Mousemove;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;

namespace winmanager
{
    public partial class MgrFormDraw : Form
    {
        private IntPtr myParentHandle = IntPtr.Zero;
        private bool isdrawing = false;
        private int penWidth = 2;
        private Color color = Color.Black;
        private Point prev = new Point();
        private JADA.Curve cv;
        private JADA.Draw draw = new JADA.Draw();
        private Rect WindowRect = new Rect();
        private Rect ClientRect = new Rect();

        [DllImport("user32.dll")]
        static extern IntPtr GetDC(IntPtr hWnd);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern IntPtr FindWindow(string strClassName, string strWindowName);
        [DllImport("user32.dll")]
        static extern bool GetWindowRect(IntPtr hwnd, ref Rect rectangle);
        [DllImport("user32.dll")]
        static extern bool GetClientRect(IntPtr hwnd, ref Rect rectangle);

        public struct Rect {
           public int Left { get; set; }
           public int Top { get; set; }
           public int Right { get; set; }
           public int Bottom { get; set; }
        }

        public MgrFormDraw(IntPtr ParentHandle)
        {
            InitializeComponent();

            this.myParentHandle = ParentHandle;
        }

        ~MgrFormDraw()
        {
            this.myParentHandle = IntPtr.Zero;
            winmanager.Globals.reqSelf.CloseWhiteboard();

            this.Dispose(false);
        }

        private void FormDraw_Activate(object sender, EventArgs e)
        {
            try
            {
                int myYOffset = SystemInformation.CaptionHeight + 24; // Altezza del controllo winmanager.Globals.frmViewer.menuStrip1 

                GetWindowRect(this.myParentHandle, ref WindowRect);
                GetClientRect(this.myParentHandle, ref ClientRect);
                this.Left = WindowRect.Left;
                this.Top = WindowRect.Top + myYOffset;
                this.Width = WindowRect.Right - WindowRect.Left;
                this.Height = WindowRect.Bottom - (WindowRect.Top + myYOffset);
            }
            catch
            {
                MessageBox.Show("AZZ...");
            }
        }

        public void Clear()
        {
            winmanager.Globals.reqSelf.CleanWhiteboard();

            draw = new JADA.Draw();
            prev = new Point();
            this.Invalidate();
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == (Keys.Alt | Keys.C)) {
                winmanager.Globals.reqSelf.CleanWhiteboard();

                draw = new JADA.Draw();
                prev = new Point();
                this.Invalidate();
                return(true);
            }

            if (keyData == (Keys.Alt | Keys.P))
            {
                if (colorDialog.ShowDialog() == DialogResult.OK)
                    color = colorDialog.Color;
                return(true);
            }

            if (keyData == (Keys.Alt | Keys.Down))
            {
                this.WindowState = FormWindowState.Minimized;
                return (true);
            }

            if (keyData == (Keys.Alt | Keys.W))
            {
                MgrFormPenWidth myFormPenWidth = new MgrFormPenWidth(penWidth);
                myFormPenWidth.ShowDialog();
                penWidth = myFormPenWidth.PenWidth;
                return (true);
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            prev = e.Location;
            isdrawing = true;
            cv = new JADA.Curve();
            cv.SetColor(color.A, color.R, color.G, color.B);
            cv.Width = penWidth;
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            Pen p = new Pen(color, penWidth);
            if (isdrawing)
            {
                using (Graphics gr = this.CreateGraphics())
                {
                    // gr.DrawLine(p, prev, e.Location);
                    cv.AddCoordinate(prev.X, prev.Y);
                    prev = e.Location;

                    // Test di invio ripetuto
                    //cv.AddCoordinate(prev.X, prev.Y);
                    //draw.AddCurve(cv);
                    //winmanager.Globals.reqSelf.SendDrawFrame(draw);
                    //cv = new JADA.Curve();
                    //cv.SetColor(color.R, color.G, color.B);
                    //cv.Width = iWidth;
                }
            }
            p.Dispose();
        }

        private void Form1_MouseUp(object sender, MouseEventArgs e)
        {
            isdrawing = false;
            cv.AddCoordinate(prev.X, prev.Y);
            draw.AddCurve(cv);

            winmanager.Globals.reqSelf.SendDrawFrame(draw);
        }

        private void RePaint()
        {
            using (Graphics gr = this.CreateGraphics())
            {
                for (int i = 0; i < draw.CurveCount; i++)
                {
                    JADA.Curve tempcv = draw.GetCurve(i);
                    Pen myPen = new Pen(Color.FromArgb(tempcv.Red,tempcv.Green, tempcv.Blue), tempcv.Width);

                    for (int j = 0; j < tempcv.CoordinatesCount - 1; j++)
                    {
                        int X1 = new int();
                        int Y1 = new int();
                        int X2 = new int();
                        int Y2 = new int();

                        tempcv.GetCoordinate(j, ref X1, ref Y1);
                        tempcv.GetCoordinate(j + 1, ref X2, ref Y2);

                        Point pt1 = new Point(X1, Y1);
                        Point pt2 = new Point(X2, Y2);

                        gr.DrawLine(myPen, pt1, pt2);
                    }
                    myPen.Dispose();
                }
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            // RePaint();
        }
    }
}