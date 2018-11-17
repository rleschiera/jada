using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.IO;
using System.Windows.Forms;
using System.Collections;

namespace JADA
{
    public class Support: IDisposable
    {
        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_SuppNew(string szFile, int iLine, IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppDestroy(string szFile, int iLine, IntPtr suppSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppGetRequestId(IntPtr suppSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppGetSessionId(IntPtr suppSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppOpen(IntPtr suppSelf, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppConnect(IntPtr suppSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppAccept(IntPtr suppSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppLoad(IntPtr suppSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppResume(IntPtr suppSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppSolicit(IntPtr suppSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppClose(IntPtr suppSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppIsConnected(IntPtr suppSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppSendChat(IntPtr suppSelf, string szText);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppSetStatus(IntPtr suppSelf, int iStatus);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppPing(IntPtr suppSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppGetConnection(IntPtr suppSelf, ref IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppListParameters(IntPtr suppSelf, ref int iStreamId, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppGetSuspendedRequestId(IntPtr suppSelf, ref int piSuspendedId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SuppSetSuspendedRequestId(IntPtr suppSelf, int iSuspendedId);


        private JADA.Connection myConnSelf;
        private IntPtr suppSelf;
        private bool disposed = false;
        private System.Threading.Timer myPingTimer = null;

        //********************************************************************* 
        //
        //     S E R V I Z I     B A S E
        // 
        //********************************************************************* 
        public Support(JADA.Connection currConnSelf)
        {
            myConnSelf = currConnSelf;
            suppSelf = JADA_SuppNew(JADA.Common.__FILE__, JADA.Common.__LINE__, myConnSelf.Self);
        }

        ~Support()
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
                    iRet = JADA_SuppDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, suppSelf);
                    suppSelf = IntPtr.Zero;
                }
                StopPing();

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     G E S T I O N E     E V E N T I
        // 
        //********************************************************************* 
        public void PingTimer(object o)
        {
            int iRet;

            iRet = JADA_SuppPing(suppSelf);
        }

        //********************************************************************* 
        //
        //     G E S T I O N E     R I C H I E S T A
        // 
        //********************************************************************* 
        public void StartPing()
        {
            if (myPingTimer == null)
            {
                myPingTimer = new System.Threading.Timer(new TimerCallback(PingTimer), null, 0, 10 * 1000);
            }
        }

        public void StopPing()
        {
            if (myPingTimer != null)
            {
                myPingTimer.Dispose(); myPingTimer = null;
            }
        }

        public int Open()
        {
            int iRet;
            Cursor.Current = Cursors.WaitCursor;
            Application.DoEvents();

            iRet = JADA_SuppOpen(suppSelf, JADA.Common.CALL_TIMEOUT);
            Cursor.Current = Cursors.Default;

            return (iRet);
        }

        public int Connect(int iRequestId)
        {
            int iRet;

            iRet = JADA_SuppConnect(suppSelf, iRequestId);

            return (iRet);
        }

        public int Accept(int iRequestId)
        {
            int iRet;

            iRet = JADA_SuppAccept(suppSelf, iRequestId);

            return (iRet);
        }

        public int Load(int iRequestId)
        {
            int iRet;

            iRet = JADA_SuppLoad(suppSelf, iRequestId);

            return (iRet);
        }

        public int Resume(int iRequestId)
        {
            int iRet;

            iRet = JADA_SuppResume(suppSelf, iRequestId);

            return (iRet);
        }

        public int Solicit()
        {
            int iRet;

            iRet = JADA_SuppSolicit(suppSelf);

            return (iRet);
        }

        public int Close()
        {
            int iRet;

            iRet = JADA_SuppClose(suppSelf);
            if (iRet == 0)
            {
                StopPing();
            }

            return (iRet);
        }

        public int SendChat(string szText)
        {
            int iRet;

            iRet = JADA_SuppSendChat(suppSelf, szText);

            return (iRet);
        }

        public JADA.Stream ListParameters()
        {
            int iRet, myStreamId = 0;

            Cursor.Current = Cursors.WaitCursor;
            Application.DoEvents();

            iRet = JADA_SuppListParameters(suppSelf, ref myStreamId, JADA.Common.CALL_TIMEOUT);
            if (iRet == 0)
            {
                JADA.Stream myStream = myConnSelf.GetStream(myStreamId);
                return (myStream);
            }

            Cursor.Current = Cursors.Default;

            return (null);
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        private string GetParameter(string szSection, string szName)
        {
            return (myConnSelf.GetParameter(szSection, szName));
        }

        private void SetParameter(string szSection, string szName, string szValue)
        {
            myConnSelf.SetParameter(szSection, szName, szValue);
        }

        public string YourName
        {
            get {
                return (GetParameter("richiesta", "partner"));
            }
            set
            {
                SetParameter("richiesta", "partner", value);
            }
        }

        public string Coda
        {
            get
            {
                return (GetParameter("richiesta", "coda"));
            }
            set
            {
                SetParameter("richiesta", "coda", value);
            }
        }

        public string Description
        {
            get
            {
                return (GetParameter("richiesta", "descrizione"));
            }
            set
            {
                SetParameter("richiesta", "descrizione", value);
            }
        }

        public string Password
        {
            get
            {
                return (GetParameter("richiesta", "password"));
            }
            set
            {
                SetParameter("richiesta", "password", value);
            }
        }

        public int RequestId
        {
            get
            {
                int iRet;
                int iMyId = 0;

                iRet = JADA_SuppGetRequestId(suppSelf, ref iMyId);
                if (iRet == 0)
                    return (iMyId);
                else
                    return (-1);
            }
        }

        public int SuspendedRequestId
        {
            get
            {
                int iRet;
                int iMyId = 0;

                iRet = JADA_SuppGetSuspendedRequestId(suppSelf, ref iMyId);
                if (iRet == 0)
                    return (iMyId);
                else
                    return (-1);
            }
            set
            {
                int iRet;

                iRet = JADA_SuppSetSuspendedRequestId(suppSelf, value);
            }
        }

        public bool Connected
        {
            get {
                int iRet;

                iRet = JADA_SuppIsConnected(suppSelf);
                if (iRet != 0) {
                    return (true);
                }
                else {
                    return(false);
                }
            }
        }
    }
}
