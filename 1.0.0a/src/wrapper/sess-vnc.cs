using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.IO;
using System.Windows.Forms;
using System.Collections;

namespace JADA
{
    public class VNC: IDisposable
    {
        // Funzioni native nella DLL




        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_VncNew(string szFile, int iLine, IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncDestroy(string szFile, int iLine, IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncGetRequestId(IntPtr vncSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncGetSessionId(IntPtr vncSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncIsConnected(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncGetConnection(IntPtr vncSelf, ref IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncOpenDesktop(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncAcceptDesktop(IntPtr vncSelf, int iAccepted);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncCloseDesktop(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncOpenWhiteboard(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncCleanWhiteboard(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncCloseWhiteboard(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncGetNextDrawFrame(IntPtr vncSelf, int iMilliSecs, ref IntPtr frmSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncSendDrawFrame(IntPtr vncSelf, IntPtr drwSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncEnqueueDrawFrame(IntPtr vncSelf, IntPtr drwSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncSetDrawPointer(IntPtr vncSelf, int iPointerType, int iPointX, int iPointY);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncResetDrawPointer(IntPtr vncSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_VncSendDesktopFrame(IntPtr vncSelf, byte[] szFrameData, int iLength);


        private IntPtr vncSelf;
        private bool disposed = false;

        //********************************************************************* 
        //
        //     S E R V I Z I     B A S E
        // 
        //********************************************************************* 
        public VNC(JADA.Connection connSelf)
        {
            vncSelf = JADA_VncNew(JADA.Common.__FILE__, JADA.Common.__LINE__, connSelf.Self);
        }

        ~VNC()
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
                if (disposing) {
                    iRet = JADA_VncDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, vncSelf);
                    vncSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        public int OpenDesktop()
        {
            int iRet;

            iRet = JADA_VncOpenDesktop(vncSelf);

            return (iRet);
        }

        public int AcceptDesktop(bool iAccepted)
        {
            int iRet;

            if (iAccepted == true)
                iRet = JADA_VncAcceptDesktop(vncSelf, JADA.Common.TRUE);
            else
                iRet = JADA_VncAcceptDesktop(vncSelf, JADA.Common.FALSE);

            return (iRet);
        }

        public int CloseDesktop()
        {
            int iRet;

            iRet = JADA_VncCloseDesktop(vncSelf);

            return (iRet);
        }

        public int OpenWhiteboard()
        {
            int iRet;

            iRet = JADA_VncOpenWhiteboard(vncSelf);
            return (iRet);
        }

        public int CleanWhiteboard()
        {
            int iRet;

            iRet = JADA_VncCleanWhiteboard(vncSelf);
            return (iRet);
        }

        public int CloseWhiteboard()
        {
            int iRet;

            iRet = JADA_VncCloseWhiteboard(vncSelf);
            return (iRet);
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public bool Connected
        {
            get {
                int iRet;

                iRet = JADA_VncIsConnected(vncSelf);
                if (iRet != 0) {
                    return (true);
                }
                else {
                    return(false);
                }
            }
        }

        #region Draw()
        //*********************************************************************
        //
        //      D R A W     M A N A G E M E N T
        //
        //*********************************************************************
        public void SendDrawFrame(JADA.Connection drwSelf)
        {
            int iRet;

            iRet = JADA_VncSendDrawFrame(vncSelf, drwSelf.Self);
        }

        public void SetDrawPointer(int iPointerType, int iPointX, int iPointY)
        {
            int iRet;

            iRet = JADA_VncSetDrawPointer(vncSelf, iPointerType, iPointX, iPointY);
        }

        public void ResetDrawPointer()
        {
            int iRet;

            iRet = JADA_VncResetDrawPointer(vncSelf);
        }
        #endregion
    }
}
