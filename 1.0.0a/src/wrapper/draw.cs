using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;


namespace JADA
{
    public class DrawEventArgs : System.EventArgs
    {
        public Draw Frame { get; private set; }

        public DrawEventArgs(JADA.Draw myFrame)
        {
            this.Frame = myFrame;
        }
    }

    public class Draw
    {

        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_DrwNew(string szFile, int iLine);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_DrwDestroy(string szFile, int iLine, IntPtr drwSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_DrwGetCurveCount(IntPtr drwSelf, ref int piCount);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_DrwGetCurve(IntPtr drwSelf, int iIndex, ref IntPtr crvSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_DrwAddCurve(IntPtr drwSelf, IntPtr crvSelf);

        private int myInstances = 1;
        private IntPtr drwSelf;
        private bool disposed = false;

        public Draw()
        {
            drwSelf = JADA_DrwNew(JADA.Common.__FILE__, JADA.Common.__LINE__);
        }

        public Draw(IntPtr drwPtr)
        {
            drwSelf = drwPtr;
        }

        ~Draw()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            this.myInstances--;
            if (this.myInstances <= 0) {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            int iRet;

            if (!disposed)
            {
                if (disposing) {
                    iRet = JADA_DrwDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, drwSelf);
                    drwSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     M E T O D I
        // 
        //********************************************************************* 
        public void Duplicate()
        {
            this.myInstances++;
        }

        public JADA.Curve GetCurve(int iIndex)
        {
            int iRet;
            IntPtr crvPtr = new IntPtr();

            iRet = JADA_DrwGetCurve(drwSelf, iIndex, ref crvPtr);
            if (iRet == 0)
            {
                JADA.Curve crvSelf = new JADA.Curve(crvPtr);
                return (crvSelf);
            }
            else
            {
                return (null);
            }
        }

        public int AddCurve(JADA.Curve crvSelf)
        {
            int iRet;

            iRet = JADA_DrwAddCurve(drwSelf, crvSelf.Self);
            return (iRet);
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public IntPtr Self
        {
            get {
                return drwSelf;
            }
        }

        public int CurveCount
        {
            get
            {
                int iRet;
                int iMyCount = -1;

                iRet = JADA_DrwGetCurveCount(drwSelf, ref iMyCount);
                if (iRet == 0)
                {
                    return (iMyCount);
                }
                else
                {
                    return (-1);
                }
            }
        }

    }

    public class Curve
    {
        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_CrvNew(string szFile, int iLine);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvDestroy(string szFile, int iLine, IntPtr crvSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvSetColor(IntPtr crvSelf, int iAlpha, int iRed, int iGreen, int iBlue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvGetColor(IntPtr crvSelf, ref int piAlpha, ref int piRed, ref int piGreen, ref int piBlue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvSetWidth(IntPtr crvSelf, int iWidth);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvGetWidth(IntPtr crvSelf, ref int piWidth);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvGetCoordinatesCount(IntPtr crvSelf, ref int piCount);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvGetCoordinate(IntPtr crvSelf, int iIndex, ref int pX, ref int pY);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_CrvAddCoordinate(IntPtr crvSelf, int X, int Y);

        private IntPtr crvSelf;
        private bool disposed = false;

        public Curve()
        {
            crvSelf = JADA_CrvNew(JADA.Common.__FILE__, JADA.Common.__LINE__);
        }

        public Curve(IntPtr crvPtr)
        {
            crvSelf = crvPtr;
        }

        ~Curve()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            int iRet;

            if (!disposed)
            {
                if (disposing)
                {
                    iRet = JADA_CrvDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, crvSelf);
                    crvSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     M E T O D I
        // 
        //********************************************************************* 
        public int SetColor(int iAlpha, int iRed, int iGreen, int iBlue)
        {
            int iRet;

            iRet = JADA_CrvSetColor(crvSelf, iAlpha, iRed, iGreen, iBlue);
            return (iRet);
        }

        public int GetCoordinate(int iIndex, ref int X, ref int Y)
        {
            int iRet;

            iRet = JADA_CrvGetCoordinate(crvSelf, iIndex, ref X, ref Y);
            return (iRet);
        }

        public int AddCoordinate(int X, int Y)
        {
            int iRet;

            iRet = JADA_CrvAddCoordinate(crvSelf, X, Y);
            return (iRet);
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public IntPtr Self
        {
            get
            {
                return crvSelf;
            }
        }

        public int Alpha
        {
            get
            {
                int iRet;
                int iMyAlpha = new int(), iMyRed = new int(), iMyGreen = new int(), iMyBlue = new int();

                iRet = JADA_CrvGetColor(crvSelf, ref iMyAlpha, ref iMyRed, ref iMyGreen, ref iMyBlue);
                if (iRet == 0)
                {
                    return (iMyAlpha);
                }
                else
                {
                    return (128);
                }
            }
        }

        public int Red
        {
            get
            {
                int iRet;
                int iMyAlpha = new int(), iMyRed = new int(), iMyGreen = new int(), iMyBlue = new int();

                iRet = JADA_CrvGetColor(crvSelf, ref iMyAlpha, ref iMyRed, ref iMyGreen, ref iMyBlue);
                if (iRet == 0)
                {
                    return (iMyRed);
                }
                else
                {
                    return (128);
                }
            }
        }

        public int Green
        {
            get
            {
                int iRet;
                int iMyAlpha = new int(), iMyRed = new int(), iMyGreen = new int(), iMyBlue = new int();

                iRet = JADA_CrvGetColor(crvSelf, ref iMyAlpha, ref iMyRed, ref iMyGreen, ref iMyBlue);
                if (iRet == 0)
                {
                    return (iMyGreen);
                }
                else
                {
                    return (128);
                }
            }
        }

        public int Blue
        {
            get
            {
                int iRet;
                int iMyAlpha = new int(), iMyRed = new int(), iMyGreen = new int(), iMyBlue = new int();

                iRet = JADA_CrvGetColor(crvSelf, ref iMyAlpha, ref iMyRed, ref iMyGreen, ref iMyBlue);
                if (iRet == 0)
                {
                    return (iMyBlue);
                }
                else
                {
                    return (128);
                }
            }
        }

        public int Width
        {
            get
            {
                int iRet;
                int iMyWidth = new int();

                iRet = JADA_CrvGetWidth(crvSelf, ref iMyWidth);
                if (iRet == 0)
                {
                    return (iMyWidth);
                }
                else
                {
                    return (-1);
                }
            }

            set
            {
                int iRet;

                iRet = JADA_CrvSetWidth(crvSelf, value);
            }
        }

        public int CoordinatesCount
        {
            get
            {
                int iRet;
                int iMyCount = new int();

                iRet = JADA_CrvGetCoordinatesCount(crvSelf, ref iMyCount);
                if (iRet == 0)
                {
                    return (iMyCount);
                }
                else
                {
                    return (-1);
                }
            }
        }
    }
}
