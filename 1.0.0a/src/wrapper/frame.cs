using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;


namespace JADA
{
    public class FrameEventArgs : System.EventArgs
    {
        public int Type { get; private set; }
        public Frame Frame { get; private set; }

        public FrameEventArgs(JADA.Frame myFrame)
        {
            this.Type = myFrame.FrameType;
            this.Frame = myFrame;
        }
    }


    public class Frame
    {
        // Costanti di utilizzo
        public const int FRAMETYPE_AUDIO = 12341;
        public const int FRAMETYPE_DESKTOP = 12342;
        public const int FRAMETYPE_DRAW = 12343;
        public const int FRAMETYPE_VIDEO = 12344;
        public const int FRAMETYPE_FTP = 12345;
        public const int FRAME_LENGTH = 16384;

        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_FrmNew(string szFile, int iLine, int iType);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_FrmNewFromMessage(string szFile, int iLine, IntPtr msgSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_FrmDestroy(string szFile, int iLine, IntPtr evtSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_FrmGetType(IntPtr evtSelf, ref int iType);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_FrmGetLength(IntPtr evtSelf, ref int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_FrmSetData(IntPtr evtSelf, byte[] szPayload, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_FrmGetData(IntPtr evtSelf, byte[] szPayload, int iLength);

        private IntPtr frmSelf;
        private bool disposed = false;

        public Frame(IntPtr frmPtr)
        {
            frmSelf = frmPtr;
        }

        ~Frame()
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
                    iRet = JADA_FrmDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, frmSelf);
                    frmSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public int FrameType
        {
            get
            {
                int iRet;
                int iMyType = -1;

                iRet = JADA_FrmGetType(frmSelf, ref iMyType);
                if (iRet == 0)
                {
                    return (iMyType);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int Length
        {
            get
            {
                int iRet;
                int iMyLength = -1;

                iRet = JADA_FrmGetLength(frmSelf, ref iMyLength);
                if (iRet == 0)
                {
                    return (iMyLength);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public byte[] Data
        {
            get
            {
                int iRet;
                byte[] szMyBuffer = new byte[this.Length];
 
                iRet = JADA_FrmGetData(frmSelf, szMyBuffer, szMyBuffer.Length);
                if (iRet == 0)
                {
                    return (szMyBuffer);
                }
                else
                {
                    return (null);
                }
            }
        }

        public MemoryStream Stream
        {
            get
            {
                int iRet;
                byte[] szMyBuffer = new byte[this.Length];
                MemoryStream szMyStream;
 
                iRet = JADA_FrmGetData(frmSelf, szMyBuffer, szMyBuffer.Length);
                if (iRet == 0)
                {
                    szMyStream = new MemoryStream(szMyBuffer);
                    return (szMyStream);
                }
                else
                {
                    return (null);
                }
            }
        }
    }
}
