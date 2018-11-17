using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;


namespace JADA
{
    public class Stream
    {
        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_StmNew(string szLine, int iLine, IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_StmDestroy(string szLine, int iLine, IntPtr stmSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_StmGetId(IntPtr stmSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_StmAddLine(IntPtr stmSelf, string szLine);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_StmGetFirstLine(IntPtr stmSelf, StringBuilder szLine, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_StmGetNextLine(IntPtr stmSelf, StringBuilder szLine, int iLength);

        private IntPtr stmSelf;
        private bool disposed = false;

        public Stream(IntPtr stmPtr)
        {
            stmSelf = stmPtr;
        }

        ~Stream()
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
                    iRet = JADA_StmDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, stmSelf);
                    stmSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public string GetFirstLine()
        {
            int iRet;
            var myLine = new StringBuilder(256);

            iRet = JADA_StmGetFirstLine(stmSelf, myLine, myLine.Capacity);
            if (iRet == 0)
            {
                return (myLine.ToString());
            }
            else
            {
                return ("");
            }
        }

        public string GetNextLine()
        {
            int iRet;
            var myLine = new StringBuilder(256);

            iRet = JADA_StmGetNextLine(stmSelf, myLine, myLine.Capacity);
            if (iRet == 0)
            {
                return (myLine.ToString());
            }
            else
            {
                return ("");
            }
        }
    }
}
