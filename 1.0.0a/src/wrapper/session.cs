using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Microsoft.Win32;

namespace JADA
{
    public class Session: IDisposable
    {
        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_SesNew(string szFile, int iLine);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesDestroy(string szFile, int iLine, IntPtr sesSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesSetSystemInfo(IntPtr sesSelf, string szComputerName, string szUserName, 
                            string szOS, string szBIOS, string szCPU, string szRAM, string szScreenSize);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesGetRequest(IntPtr sesSelf, ref IntPtr reqPtr);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesOpen(IntPtr sesSelf, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesClose(IntPtr sesSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesListRequests(IntPtr sesSelf, string szFilterArea, string szFileName, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesListPendingRequests(IntPtr sesSelf, string szFilterArea, string szFileName, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesGetListCounters(IntPtr sesSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesGetNextEvent(IntPtr sesSelf, int iMilliSecs, ref IntPtr evtSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesSetParameter(IntPtr sesSelf, string szSection, string szName, string szValue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesSetParameterInt(IntPtr sesSelf, string szSection, string szName, int szValue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesGetParameter(IntPtr sesSelf, string szSection, string szName, StringBuilder szValue, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_SesGetParameterInt(IntPtr sesSelf, string szSection, string szName, ref int szValue);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        private class MEMORYSTATUSEX {
            public uint dwLength;
            public uint dwMemoryLoad;
            public ulong ullTotalPhys;
            public ulong ullAvailPhys;
            public ulong ullTotalPageFile;
            public ulong ullAvailPageFile;
            public ulong ullTotalVirtual;
            public ulong ullAvailVirtual;
            public ulong ullAvailExtendedVirtual;
            public MEMORYSTATUSEX()
            {
                this.dwLength = (uint)Marshal.SizeOf(typeof(MEMORYSTATUSEX));
            }
        }
        [return: MarshalAs(UnmanagedType.Bool)]
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool GlobalMemoryStatusEx([In, Out] MEMORYSTATUSEX lpBuffer);
        
        public event EventHandler SessionOpened;
        public event EventHandler SessionClosed;
        public event EventHandler RequestOpened;
        public event EventHandler RequestAccepted;
        public event EventHandler RequestLoaded;
        public event EventHandler RequestConnected;
        public event EventHandler RequestSolicited;
        public event EventHandler RequestClosed;
        public event EventHandler RequestSuspended;
        public event EventHandler RequestResumed;
        public event EventHandler ResultReceived;
        public event EventHandler SessionListReceived;
        public event EventHandler RequestListReceived;
        public event EventHandler ListCountersReceived;
        public event EventHandler FileSent;
        public event EventHandler FileReceived;

        private IntPtr sesSelf;
        private bool disposed = false;
        private System.Threading.Timer myEventTimer;

       
        //********************************************************************* 
        //
        //     S E R V I Z I     B A S E
        // 
        //********************************************************************* 
        public Session()
        {
            sesSelf = JADA_SesNew(JADA.Common.__FILE__, JADA.Common.__LINE__);
            if (sesSelf != IntPtr.Zero) {
                myEventTimer = new System.Threading.Timer(new TimerCallback(CheckEventTimer), null, 0, 100);
            }
        }

         ~Session()
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
                    iRet = JADA_SesDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, sesSelf);
                    sesSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     G E S T I O N E     E V E N T I
        // 
        //********************************************************************* 
        public void CheckEventTimer(object o)
        {
            int iRet, iEventType;
            var evtPtr = new IntPtr();

            iRet = JADA_SesGetNextEvent(sesSelf, 1, ref evtPtr); 
            if (iRet == 0) {
                // try {
                    JADA.Event evtSelf = new JADA.Event(evtPtr);
                    EventArgs e = new EventArgs(evtSelf);

                    iEventType = evtSelf.EventType;
                    switch (iEventType) {
                    case JADA.Event.EVT_SESSION_OPENED:
                        EventHandler SessionOpenedHandler = this.SessionOpened;
                        if (null != SessionOpenedHandler) SessionOpenedHandler(this, e);
                        break;
                    case JADA.Event.EVT_SESSION_CLOSED:
                        EventHandler SessionClosedHandler = this.SessionClosed;
                        if (null != SessionClosedHandler) SessionClosedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_OPENED:
                        EventHandler RequestOpenedHandler = this.RequestOpened;
                        if (null != RequestOpenedHandler) RequestOpenedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_ACCEPTED:
                        EventHandler RequestAcceptedHandler = this.RequestAccepted;
                        if (null != RequestAcceptedHandler) RequestAcceptedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_LOADED:
                        EventHandler RequestLoadedHandler = this.RequestLoaded;
                        if (null != RequestLoadedHandler) RequestLoadedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_CONNECTED:
                        EventHandler RequestConnectedHandler = this.RequestConnected;
                        if (null != RequestConnectedHandler) RequestConnectedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_SOLICITED:
                        EventHandler RequestSolicitedHandler = this.RequestSolicited;
                        if (null != RequestSolicitedHandler) RequestSolicitedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_CLOSED:
                        EventHandler RequestClosedHandler = this.RequestClosed;
                        if (null != RequestClosedHandler) RequestClosedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_SUSPENDED:
                        EventHandler RequestSuspendedHandler = this.RequestSuspended;
                        if (null != RequestSuspendedHandler) RequestSuspendedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUEST_RESUMED:
                        EventHandler RequestResumedHandler = this.RequestResumed;
                        if (null != RequestResumedHandler) RequestResumedHandler(this, e);
                        break;
                    case JADA.Event.EVT_MSG_RESULT:
                        EventHandler ResultReceivedHandler = this.ResultReceived;
                        if (null != ResultReceivedHandler) ResultReceivedHandler(this, e);
                        break;
                    case JADA.Event.EVT_SESSIONS_LIST:
                        EventHandler SessionListReceivedHandler = this.SessionListReceived;
                        if (null != SessionListReceivedHandler) SessionListReceivedHandler(this, e);
                        break;
                    case JADA.Event.EVT_REQUESTS_LIST:
                        EventHandler RequestListReceivedHandler = this.RequestListReceived;
                        if (null != RequestListReceivedHandler) RequestListReceivedHandler(this, e);
                        break;
                    case JADA.Event.EVT_LIST_COUNTERS:
                        EventHandler ListCountersReceivedHandler = this.ListCountersReceived;
                        if (null != ListCountersReceivedHandler) ListCountersReceivedHandler(this, e);
                        break;
                    case JADA.Event.EVT_FILE_SENT:
                        EventHandler FileSentHandler = this.FileSent;
                        if (null != FileSentHandler) FileSentHandler(this, e);
                        break;
                    case JADA.Event.EVT_FILE_RECEIVED:
                        EventHandler FileReceivedHandler = this.FileReceived;
                        if (null != FileReceivedHandler) FileReceivedHandler(this, e);
                        break;
                    }
                //}
                //catch (Exception e)
                //{
                //    JADA.Common.LogMessage(String.Format("CheckEventTimer {0}\n{1}", e.Message, e.StackTrace));
                //    MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "AREAS Customer Assistant");
                //}
            }
        }

        //********************************************************************* 
        //
        //     G E S T I O N E     S E S S I O N E
        // 
        //********************************************************************* 
        public int Open(int iWaitSeconds)
        {
            int iRet;

            do {
                Cursor.Current = Cursors.WaitCursor;
                Application.DoEvents();

                iRet = JADA_SesOpen(sesSelf, iWaitSeconds);
                Cursor.Current = Cursors.Default;
                if (iRet == JADA.Common.PROXY_AUTHENTICATE)
                {
                    FormProxyLogin frmProxyLogin = new FormProxyLogin ();
                    frmProxyLogin.ShowDialog(); 
                    if (frmProxyLogin.RetryConnection == false) {
                        break;
                    }
                }
            } while (iRet == JADA.Common.PROXY_AUTHENTICATE);

            return (iRet);
        }

        public int Close()
        {
            int iRet;

            iRet = JADA_SesClose(sesSelf);

            return (iRet);
        }

        public int SetSystemInfo()
        {
            int iRet;
            string szComputerName, szUserName, szOS, szBIOS, szCPU, szRAM, szScreenSize;

            szScreenSize = SystemInformation.PrimaryMonitorSize.ToString();
            szComputerName = SystemInformation.ComputerName.ToString();
            szOS = System.Environment.OSVersion.ToString();
            szUserName = System.Environment.UserName.ToString();
            RegistryKey Rkey1 = Registry.LocalMachine;
            Rkey1 = Rkey1.OpenSubKey("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
            szCPU = (string)Rkey1.GetValue("ProcessorNameString");
            RegistryKey Rkey2 = Registry.LocalMachine;
            Rkey2 = Rkey2.OpenSubKey("HARDWARE\\DESCRIPTION\\System");
            szBIOS = ((string[])Rkey2.GetValue("SystemBiosVersion"))[0];

            ulong lMemory;
            MEMORYSTATUSEX memStatus = new MEMORYSTATUSEX();
            if (GlobalMemoryStatusEx(memStatus))
            {
                lMemory = memStatus.ullTotalPhys / (1024 * 1024);
                szRAM = String.Format("{0} MB", lMemory);
            }
            else
            {
                szRAM = "Impossibile da rilevare";
            }

            iRet = JADA_SesSetParameter(sesSelf, "postazione", "computer", szComputerName.Trim());
            iRet = JADA_SesSetParameter(sesSelf, "postazione", "domain user", szUserName.Trim());
            iRet = JADA_SesSetParameter(sesSelf, "postazione", "BIOS", szBIOS.Trim());
            iRet = JADA_SesSetParameter(sesSelf, "postazione", "CPU", szCPU.Trim());
            iRet = JADA_SesSetParameter(sesSelf, "postazione", "RAM", szRAM.Trim());
            iRet = JADA_SesSetParameter(sesSelf, "postazione", "versione OS", szOS.Trim());
            iRet = JADA_SesSetParameter(sesSelf, "postazione", "schermo", szScreenSize.Trim());

            return (0);
        }


        public int GetListCounters()
        {
            int iRet;

            iRet = JADA_SesGetListCounters(sesSelf);

            return (iRet);
        }

        public int ListRequests(string szFilterArea, string szFileName, int iWaitSeconds)
        {
            int iRet;

            Cursor.Current = Cursors.WaitCursor;
            Application.DoEvents();

            iRet = JADA_SesListRequests(sesSelf, szFilterArea, szFileName, iWaitSeconds);

            Cursor.Current = Cursors.Default;

            return (iRet);
        }

        public int ListPendingRequests(string szFilterArea, string szFileName, int iWaitSeconds)
        {
            int iRet;

            Cursor.Current = Cursors.WaitCursor;
            Application.DoEvents();

            iRet = JADA_SesListPendingRequests(sesSelf, szFilterArea, szFileName, iWaitSeconds);

            Cursor.Current = Cursors.Default;

            return (iRet);
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        private string GetParameter(string mySection, string myName)
        {
            int iRet;
            var myValue = new StringBuilder(256);

            iRet = JADA_SesGetParameter(sesSelf, mySection, myName, myValue, myValue.Capacity);
            if (iRet == 0)
                return (myValue.ToString());
            else
                return ("");
        }

        private int GetParameterInt(string mySection, string myName)
        {
            int iRet;
            int myValue = -1;

            iRet = JADA_SesGetParameterInt(sesSelf, mySection, myName, ref myValue);
            if (iRet == 0)
                return (myValue);
            else
                return (-1);
        }

        private void SetParameter(string mySection, string myName, string myValue)
        {
            int iRet;

            iRet = JADA_SesSetParameter(sesSelf, mySection, myName, myValue);
        }

        private void SetParameterInt(string mySection, string myName, int myValue)
        {
            int iRet;

            iRet = JADA_SesSetParameterInt(sesSelf, mySection, myName, myValue);
        }

        public string Nome
        {
            get {
                return (GetParameter("sessione", "utente"));
            }

            set
            {
                SetParameter("sessione", "utente", value);
            }
        }

        public string Username
        {
            get
            {
                return (GetParameter("sessione", "username"));
            }

            set
            {
                SetParameter("sessione", "username", value);
            }
        }

        public string Password
        {
            get
            {
                return (GetParameter("sessione", "password"));
            }

            set
            {
                SetParameter("sessione", "password", value);
            }
        }

        public string Telefono
        {
            get
            {
                return (GetParameter("sessione", "telefono"));
            }

            set
            {
                SetParameter("sessione", "telefono", value);
            }
        }

        public string Cellulare
        {
            get
            {
                return (GetParameter("sessione", "cellulare"));
            }

            set
            {
                SetParameter("sessione", "cellulare", value);
            }
        }

        public string Mail
        {
            get
            {
                return (GetParameter("sessione", "mail"));
            }

            set
            {
                SetParameter("sessione", "mail", value);
            }
        }

        public string Regione
        {
            get
            {
                return (GetParameter("sessione", "regione"));
            }

            set
            {
                SetParameter("sessione", "regione", value);
            }
        }

        public string Cliente
        {
            get
            {
                return (GetParameter("sessione", "cliente"));
            }

            set
            {
                SetParameter("sessione", "cliente", value);
            }
        }

        public string Area
        {
            get
            {
                return (GetParameter("sessione", "area"));
            }

            set
            {
                SetParameter("sessione", "area", value);
            }
        }

        public bool UsaAudio
        {
            get
            {
                int myParameter = GetParameterInt("sessione", "audio");

                if (myParameter == 0)
                    return (false);
                else
                    return (true);
            }

            set
            {
                if (value == true)
                    SetParameterInt("sessione", "audio", 1);
                else
                    SetParameterInt("sessione", "audio", 0);
            }
        }

        public IntPtr Request
        {
            get
            {
                int iRet;
                var szMyRequest = new IntPtr();

                iRet = JADA_SesGetRequest(sesSelf, ref szMyRequest);
                if (iRet == 0)
                {
                    return (szMyRequest);
                }
                else
                {
                    return (IntPtr.Zero);
                }
            }
        }

        public IntPtr Self
        {
            get
            {
                return (sesSelf);
            }
        }

    }
}
