using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using System.Collections;
using Microsoft.Win32;


namespace JADA
{
    public class Connection
    {
        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_ConnNew(string szFile, int iLine, int iTestMode);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnDestroy(string szFile, int iLine, IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnConnect(IntPtr connSelf, int iSessionType, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnOpen(IntPtr connSelf, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnClose(IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnListRequests(IntPtr connSelf, string szFilterCoda, ref int iStreamId, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnListPendingRequests(IntPtr connSelf, string szFilterCoda, ref int iStreamId, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnGetListCounters(IntPtr connSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnRaiseEvent(IntPtr connSelf, IntPtr evtSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnGetNextEvent(IntPtr connSelf, int iMilliSecs, ref IntPtr evtSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnGetParameter(IntPtr connSelf, string szSection, string szName, StringBuilder szValue, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnGetParameterInt(IntPtr connSelf, string szSection, string szName, ref int piValue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnSetParameter(IntPtr connSelf, string szSection, string szName, string szValue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnSetParameterInt(IntPtr connSelf, string szSection, string szName, int iValue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnGetCombo(IntPtr connSelf, int iComboType, string szFilter, ref int iStreamId, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnGetSetting(IntPtr connSelf, string szKey, StringBuilder szValue, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_ConnGetStream(IntPtr connSelf, int iStreamId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ConnAddStream(IntPtr connSelf, IntPtr stmSelf);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        private class MEMORYSTATUSEX
        {
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
 
        private IntPtr connSelf;
        private bool disposed = false;

        public event EventHandler ConnectionConnected;
        public event EventHandler ConnectionOpened;
        public event EventHandler ConnectionClosed;
        public event EventHandler ChatTextReceived;
        public event EventHandler PingReply;
        public event EventHandler SupportOpened;
        public event EventHandler SupportAccepted;
        public event EventHandler SupportLoaded;
        public event EventHandler SupportConnected;
        public event EventHandler SupportSolicited;
        public event EventHandler SupportClosed;
        public event EventHandler SupportSuspended;
        public event EventHandler SupportResumed;
        public event EventHandler MsgResult;
        public event EventHandler SessionList;
        public event EventHandler ParameterList;
        public event EventHandler RequestsList;
        public event EventHandler CountersList;
        public event EventHandler ComboValues;
        public event EventHandler CallOpened;
        public event EventHandler CallClosed;
        public event EventHandler CallAborted;
        public event EventHandler DesktopRequested;
        public event EventHandler DesktopOpened;
        public event EventHandler DesktopClosed;
        public event EventHandler WhiteboardOpened;
        public event EventHandler WhiteboardCleaned;
        public event EventHandler WhiteboardClosed;
        public event EventHandler DrawPointerMoved;
        public event EventHandler DrawPointerReset;
        public event EventHandler FtpOpened;
        public event EventHandler DirectoryList;
        public event EventHandler FileSent;
        public event EventHandler FileReceived;
        public event EventHandler FtpClosed;

        private System.Threading.Thread myEventThread = null;

        public Connection()
        {
            connSelf = JADA_ConnNew(JADA.Common.__FILE__, JADA.Common.__LINE__, JADA.Common.FALSE); //UUU Da cambiare
            if (connSelf != IntPtr.Zero)
            {
                myEventThread = new System.Threading.Thread(new ThreadStart(HandleEventThread));
                myEventThread.IsBackground = true; myEventThread.Start();
            }
        }

        public Connection(IntPtr connPtr)
        {
            connSelf = connPtr;
        }

        ~Connection()
        {
            this.Dispose();
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
                    iRet = JADA_ConnDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, connSelf);
                    connSelf = IntPtr.Zero;
                }

                if (myEventThread != null)
                {
                    myEventThread.Abort(); myEventThread = null;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     M E T O D I
        // 
        //********************************************************************* 
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

            iRet = this.SetParameter("postazione", "computer", szComputerName.Trim());
            iRet = this.SetParameter("postazione", "domain user", szUserName.Trim());
            iRet = this.SetParameter("postazione", "BIOS", szBIOS.Trim());
            iRet = this.SetParameter("postazione", "CPU", szCPU.Trim());
            iRet = this.SetParameter("postazione", "RAM", szRAM.Trim());
            iRet = this.SetParameter("postazione", "versione OS", szOS.Trim());
            iRet = this.SetParameter("postazione", "schermo", szScreenSize.Trim());

            return (0);
        }

        public int Connect(int iSessionType)
        {
            int iRet;

            iRet = JADA_ConnConnect(connSelf, iSessionType, JADA.Common.CALL_TIMEOUT);

            return (iRet);
        }

        public int Open()
        {
            int iRet;

            iRet = JADA_ConnOpen(connSelf, JADA.Common.CALL_TIMEOUT);

            return (iRet);
        }

        public int Close()
        {
            int iRet;

            iRet = JADA_ConnClose(connSelf);

            return (iRet);
        }

        public JADA.Stream ListRequests(string szFilterCoda)
        {
            int iRet, myStreamId = 0;

            iRet = JADA_ConnListRequests(connSelf, szFilterCoda, ref myStreamId, JADA.Common.CALL_TIMEOUT);
            if (iRet == 0)
            {
                JADA.Stream myStream = this.GetStream(myStreamId);
                return (myStream);
            }

            return (null);
        }

        public JADA.Stream ListPendingRequests(string szFilterCoda)
        {
            int iRet, myStreamId = 0;

            iRet = JADA_ConnListPendingRequests(connSelf, szFilterCoda, ref myStreamId, JADA.Common.CALL_TIMEOUT);
            if (iRet == 0)
            {
                JADA.Stream myStream = this.GetStream(myStreamId);
                return (myStream);
            }

            return (null);
        }

        public void GetListCounters()
        {
            int iRet;

            iRet = JADA_ConnGetListCounters(connSelf);
        }

        public JADA.Stream GetStream(int iStreamId)
        {
            IntPtr stmPtr;

            stmPtr = JADA_ConnGetStream(connSelf, iStreamId);
            if (stmPtr != IntPtr.Zero)
            {
                JADA.Stream myStream = new JADA.Stream(stmPtr);
                return (myStream);
            }

            return (null);
        }

        public JADA.Stream  GetCombo(int iComboType, string szFilter)
        {
            int iRet, myStreamId = 0;

            iRet = JADA_ConnGetCombo(connSelf, iComboType, szFilter, ref myStreamId, JADA.Common.CALL_TIMEOUT);
            if (iRet == 0)
            {
                JADA.Stream myStream = this.GetStream(myStreamId);
                return(myStream);
            }

            return (null);
        }

        public String GetParameter(string szSection, string szName)
        {
            int iRet;
            var myValue = new StringBuilder(256);

            iRet = JADA_ConnGetParameter(connSelf, szSection, szName, myValue,   myValue.Capacity);
            if (iRet == 0)
                return (myValue.ToString());
            else
                return ("");
        }
            
        public int GetParameterInt(string szSection, string szName)
        {
            int iRet;
            int myValue = -1;

            iRet = JADA_ConnGetParameterInt(connSelf, szSection, szName, ref myValue);
            if (iRet == 0)
                return (myValue);
            else
                return (-1);
        }

        public int SetParameter(string szSection, string szName, string szValue)
        {
            int iRet;

            iRet = JADA_ConnSetParameter(connSelf, szSection, szName, szValue);
            return (iRet);
        }

        public int SetParameterInt(string szSection, string szName, int iValue)
        {
            int iRet;

            iRet = JADA_ConnSetParameterInt(connSelf, szSection, szName, iValue);
            return (iRet);
        }

        //********************************************************************* 
        //
        //     G E S T I O N E     E V E N T I
        // 
        //********************************************************************* 
        public void HandleEventThread()
        {
            int iRet, iEventType;
            var evtPtr = new IntPtr();
            JADA.Event evtSelf = null;

            while (true)
            {
                iRet = JADA_ConnGetNextEvent(connSelf, 15000, ref evtPtr);
                if (iRet == 0)
                {
                    try
                    {
                        evtSelf = new JADA.Event(evtPtr);
                        EventArgs e = new EventArgs(evtSelf);

                        iEventType = evtSelf.EventType;
                        switch (iEventType)
                        {
                        /* Opcode degli eventi: connessione */
                        case JADA.Event.EVT_CONNECTION_CONNECTED:
                                EventHandler ConnectionConnectedHandler = this.ConnectionConnected;
                                if (null != ConnectionConnectedHandler) ConnectionConnectedHandler(this, e);
                            break;
                        case JADA.Event.EVT_CONNECTION_OPENED:
                            EventHandler ConnectionOpenedHandler = this.ConnectionOpened;
                            if (null != ConnectionOpenedHandler) ConnectionOpenedHandler(this, e);
                            break;
                        case JADA.Event.EVT_CONNECTION_CLOSED:
                            EventHandler ConnectionClosedHandler = this.ConnectionClosed;
                            if (null != ConnectionClosedHandler) ConnectionClosedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SESSIONS_LIST:
                            EventHandler SessionListHandler = this.SessionList;
                            if (null != SessionListHandler) SessionListHandler(this, e);
                            break;
                        case JADA.Event.EVT_REQUESTS_LIST:
                            EventHandler RequestsListHandler = this.RequestsList;
                            if (null != RequestsListHandler) RequestsListHandler(this, e);
                            break;
                        case JADA.Event.EVT_LIST_COUNTERS:
                            EventHandler CountersListHandler = this.CountersList;
                            if (null != CountersListHandler) CountersListHandler(this, e);
                            break;
                        case JADA.Event.EVT_PARAMETERS_LIST:
                            EventHandler ParameterListHandler = this.ParameterList;
                            if (null != ParameterListHandler) ParameterListHandler(this, e);
                            break;
                        case JADA.Event.EVT_COMBO_VALUES:
                            EventHandler ComboValuesHandler = this.ComboValues;
                            if (null != ComboValuesHandler) ComboValuesHandler(this, e);
                            break;
                        case JADA.Event.EVT_MSG_RESULT:
                            EventHandler MsgResultHandler = this.MsgResult;
                            if (null != MsgResultHandler) MsgResultHandler(this, e);
                            break;
                        case JADA.Event.EVT_PING_REPLY:
                            EventHandler PingReplyHandler = this.PingReply;
                            if (null != PingReplyHandler) PingReplyHandler(this, e);
                            break;

                        /* Opcode degli eventi: supporto */
                        case JADA.Event.EVT_SUPPORT_OPENED:
                            EventHandler SupportOpenedHandler = this.SupportOpened;
                            if (null != SupportOpenedHandler) SupportOpenedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_ACCEPTED:
                            EventHandler SupportAcceptedHandler = this.SupportAccepted;
                            if (null != SupportAcceptedHandler) SupportAcceptedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_LOADED:
                            EventHandler SupportLoadedHandler = this.SupportLoaded;
                            if (null != SupportLoadedHandler) SupportLoadedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_CONNECTED:
                            EventHandler SupportConnectedHandler = this.SupportConnected;
                            if (null != SupportConnectedHandler) SupportConnectedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_SOLICITED:
                            EventHandler SupportSolicitedHandler = this.SupportSolicited;
                            if (null != SupportSolicitedHandler) SupportSolicitedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_CLOSED:
                            EventHandler SupportClosedHandler = this.SupportClosed;
                            if (null != SupportClosedHandler) SupportClosedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_SUSPENDED:
                            EventHandler SupportSuspendedHandler = this.SupportSuspended;
                            if (null != SupportSuspendedHandler) SupportSuspendedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SUPPORT_RESUMED:
                            EventHandler SupportResumedHandler = this.SupportResumed;
                            if (null != SupportResumedHandler) SupportResumedHandler(this, e);
                            break;

                        case JADA.Event.EVT_CHATTEXT_RECEIVED:
                            EventHandler ChatTextReceivedHandler = this.ChatTextReceived;
                            if (null != ChatTextReceivedHandler) ChatTextReceivedHandler(this, e);
                            break;

                        /* Opcode degli eventi: media */
                        case JADA.Event.EVT_CALL_OPENED:
                            EventHandler CallOpenedHandler = this.CallOpened;
                            if (null != CallOpenedHandler) CallOpenedHandler(this, e);
                            //StartAudio();
                            //myAudioRestartTimer = new System.Threading.Timer(new TimerCallback(RestartAudioTimer), null, 0, 5 * 60 * 1000);
                            break;
                        case JADA.Event.EVT_CALL_CLOSED:
                            //if (myAudioRestartTimer != null)
                            //{
                            //    myAudioRestartTimer.Dispose(); myAudioRestartTimer = null;
                            //}
                            //StopAudio();
                            EventHandler CallClosedHandler = this.CallClosed;
                            if (null != CallClosedHandler) CallClosedHandler(this, e);
                            break;
                        case JADA.Event.EVT_CALL_ABORTED:
                            //if (myAudioRestartTimer != null)
                            //{
                            //    myAudioRestartTimer.Dispose(); myAudioRestartTimer = null;
                            //}
                            //StopAudio();
                            EventHandler CallAbortedHandler = this.CallAborted;
                            if (null != CallAbortedHandler) CallAbortedHandler(this, e);
                            break;
    
                        /* Opcode degli eventi: VNC */
                        case JADA.Event.EVT_DESKTOP_OPENED:
                            EventHandler DesktopOpenedHandler = this.DesktopOpened;
                            if (null != DesktopOpenedHandler) DesktopOpenedHandler(this, e);
                            break;
                        case JADA.Event.EVT_DESKTOP_REQUESTED:
                            EventHandler DesktopRequestedHandler = this.DesktopRequested;
                            if (null != DesktopRequestedHandler) DesktopRequestedHandler(this, e);
                            break;
                        case JADA.Event.EVT_DESKTOP_CLOSED:
                            EventHandler DesktopClosedHandler = this.DesktopClosed;
                            if (null != DesktopClosedHandler) DesktopClosedHandler(this, e);
                            break;
                        case JADA.Event.EVT_WHITEBOARD_OPENED:
                            EventHandler WhiteboardOpenedHandler = this.WhiteboardOpened;
                            if (null != WhiteboardOpenedHandler) WhiteboardOpenedHandler(this, e);
                            break;
                        case JADA.Event.EVT_WHITEBOARD_CLEANED:
                            EventHandler WhiteboardCleanedHandler = this.WhiteboardCleaned;
                            if (null != WhiteboardCleanedHandler) WhiteboardCleanedHandler(this, e);
                            break;
                        case JADA.Event.EVT_WHITEBOARD_CLOSED:
                            EventHandler WhiteboardClosedHandler = this.WhiteboardClosed;
                            if (null != WhiteboardClosedHandler) WhiteboardClosedHandler(this, e);
                            break;
                        case JADA.Event.EVT_SET_DRAW_POINTER:
                            EventHandler DrawPointerMovedHandler = this.DrawPointerMoved;
                            if (null != DrawPointerMovedHandler) DrawPointerMovedHandler(this, e);
                            break;
                        case JADA.Event.EVT_RESET_DRAW_POINTER:
                            EventHandler DrawPointerResetHandler = this.DrawPointerReset;
                            if (null != DrawPointerResetHandler) DrawPointerResetHandler(this, e);
                            break;
                        
                        /* Opcode degli eventi: FTP */
                            case JADA.Event.EVT_FTP_OPENED:
                            EventHandler HandlerFtpOpened = this.FtpOpened;
                            if (null != HandlerFtpOpened) HandlerFtpOpened(this, e);
                            break;
                        case JADA.Event.EVT_DIRECTORY_LIST:
                            EventHandler DirectoryListHandler = this.DirectoryList;
                            if (null != DirectoryListHandler) DirectoryListHandler(this, e);
                            break;
                        case JADA.Event.EVT_FILE_SENT:
                            EventHandler FileSentHandler = this.FileSent;
                            if (null != FileSentHandler) FileSentHandler(this, e);
                            break;
                        case JADA.Event.EVT_FILE_RECEIVED:
                            EventHandler FileReceivedHandler = this.FileReceived;
                            if (null != FileReceivedHandler) FileReceivedHandler(this, e);
                            break;
                        case JADA.Event.EVT_FTP_CLOSED:
                            EventHandler FtpClosedHandler = this.FtpClosed;
                            if (null != FtpClosedHandler) FtpClosedHandler(this, e);
                            break;

                        default:
                            JADA.Common.LogMessage(String.Format("Tipo evento {0} non gestito da Connection", iEventType));
                            break;
                        }
                    }
                    catch (Exception e)
                    {
                        JADA.Common.LogMessage(String.Format("HandleEventThread {0}\n{1}", e.Message, e.StackTrace));
                        // MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "JADA");
                    }
                    finally
                    {
                        if (evtSelf != null)
                        {
                            evtSelf.Dispose();
                        }
                    }
                }
            }
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public IntPtr Self
        {
            get {
                return connSelf;
            }
        }

        public string Username
        {
            get
            {
                return(this.GetParameter("connessione", "username"));
            }
            set
            {
                this.SetParameter("connessione", "username", value);
            }
        }

        public string Password
        {
            get
            {
                return(this.GetParameter("connessione", "password"));
            }
            set
            {
                this.SetParameter("connessione", "password", value);
            }
        }

        public string Nome
        {
            get
            {
                return(this.GetParameter("connessione", "nome"));
            }
            set
            {
                this.SetParameter("connessione", "nome", value);
            }
        }

        public string Telefono
        {
            get
            {
                return(this.GetParameter("connessione", "telefono"));
            }
            set
            {
                this.SetParameter("connessione", "telefono", value);
            }
        }

        public string Cellulare
        {
            get
            {
                return(this.GetParameter("connessione", "cellulare"));
            }
            set
            {
                this.SetParameter("connessione", "cellulare", value);
            }
        }

        public string Mail
        {
            get
            {
                return(this.GetParameter("connessione", "mail"));
            }
            set
            {
                this.SetParameter("connessione", "mail", value);
            }
        }

        public string Regione
        {
            get
            {
                return(this.GetParameter("connessione", "regione"));
            }
            set
            {
                this.SetParameter("connessione", "regione", value);
            }
        }

        public string Cliente
        {
            get
            {
                return(this.GetParameter("connessione", "cliente"));
            }
            set
            {
                this.SetParameter("connessione", "cliente", value);
            }
        }

        public string Coda
        {
            get
            {
                return (this.GetParameter("connessione", "coda"));
            }
            //set
            //{
            //    this.SetParameter("connessione", "coda", value);
            //}
        }

        public bool UsaAudio
        {
            get
            {
                String szUsaAudio;

                szUsaAudio = this.GetParameter("connessione", "audio");
                if (szUsaAudio == "1")
                    return (true);
                else
                    return (false);
            }
            set
            {
                if (value == true)
                    this.SetParameter("connessione", "audio", "1");
                else
                    this.SetParameter("connessione", "audio", "0");
            }
        }

        public string Agreement
        {
            get
            {
                int iRet;
                var myValue = new StringBuilder(256);

                iRet = JADA_ConnGetSetting(connSelf, "agreement", myValue, myValue.Capacity);
                if (iRet == 0)
                    return (myValue.ToString());
                else
                    return ("");
            }
        }
    }
}
