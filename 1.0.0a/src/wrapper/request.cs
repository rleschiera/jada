using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.IO;
using System.Windows.Forms;
using System.Collections;

namespace JADA
{
    public class Request: IDisposable
    {
        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_ReqNew(string szFile, int iLine, IntPtr sesSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqDestroy(string szFile, int iLine, IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSetId(IntPtr reqSelf, int iId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetId(IntPtr reqSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetSuspendedId(IntPtr reqSelf, ref int piId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSetSuspendedId(IntPtr reqSelf, int iId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqOpen(IntPtr reqSelf, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqConnect(IntPtr reqSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqAccept(IntPtr reqSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqLoad(IntPtr reqSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqResume(IntPtr reqSelf, int iRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSolicit(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqClose(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqPing(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqIsConnected(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSetStatus(IntPtr reqSelf, int iStatus);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetSession(IntPtr reqSelf, ref IntPtr sesSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqOpenCall(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCallReset(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCallMuteAudio(IntPtr reqSelf, int iMuteStatus);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCallMuteVideo(IntPtr reqSelf, int iMuteStatus);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCloseCall(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqOpenDesktop(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqAcceptDesktop(IntPtr reqSelf, int iAccepted);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCloseDesktop(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqOpenWhiteboard(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCleanWhiteboard(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCloseWhiteboard(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSendChat(IntPtr reqSelf, string szText);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetNextEvent(IntPtr reqSelf, int iMilliSecs, ref IntPtr evtSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetNextAudioFrame(IntPtr reqSelf, int iMilliSecs, ref IntPtr frmSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetNextVideoFrame(IntPtr reqSelf, int iMilliSecs, ref IntPtr frmSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetNextDrawFrame(IntPtr reqSelf, int iMilliSecs, ref IntPtr drwSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSendAudioFrame(IntPtr reqSelf, byte[] szPayload, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSendVideoFrame(IntPtr reqSelf, byte[] szPayload, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSetVideoFrameDelay(IntPtr reqSelf, int iDelay);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetVideoFrameDelay(IntPtr reqSelf, ref int piDelay);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSendDrawFrame(IntPtr reqSelf, IntPtr drwSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSetDrawPointer(IntPtr reqSelf, int iPointerType, int iPointX, int iPointY);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqResetDrawPointer(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqSetParameter(IntPtr reqSelf, string szSection, string szName, string szValue);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetParameter(IntPtr reqSelf, string szSection, string szName, StringBuilder szValue, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqListParameters(IntPtr reqSelf, string szFileName, int iWaitSeconds);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqListDirectory(IntPtr reqSelf, string szDirName);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqGetFile(IntPtr reqSelf, string szRemoteFile, string szLocalFile);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCancelGetFile(IntPtr reqSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqPutFile(IntPtr reqSelf, string szLocalFile, string szRemoteFile);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_ReqCancelPutFile(IntPtr reqSelf);

        public event EventHandler ChatTextReceived;
        public event EventHandler CallOpened;
        public event EventHandler CallClosed;
        public event EventHandler CallAborted;
        public event EventHandler DesktopOpened;
        public event EventHandler DesktopRequested;
        public event EventHandler DesktopClosed;
        public event EventHandler WhiteboardOpened;
        public event EventHandler WhiteboardCleaned;
        public event EventHandler WhiteboardClosed;
        public event EventHandler VideoFrameReceived;
        public event EventHandler DrawFrameReceived;
        public event EventHandler DrawPointerMoved;
        public event EventHandler DrawPointerReset;
        public event EventHandler ParameterListReceived;
        public event EventHandler DirectoryListReceived;
        public event EventHandler PingReply;

        private IntPtr reqSelf;
        private bool disposed = false;
        private System.Threading.Thread myEventThread = null;
        private System.Threading.Thread myAudioFrameThread = null;
        private System.Threading.Thread myVideoFrameThread = null;
        private System.Threading.Thread myDrawFrameThread = null;
        private System.Threading.Timer myAudioRestartTimer = null;
        private System.Threading.Timer myPingTimer = null;

        //********************************************************************* 
        //
        //     S E R V I Z I     B A S E
        // 
        //********************************************************************* 
        public Request(JADA.Session sesSelf)
        {
            reqSelf = JADA_ReqNew(JADA.Common.__FILE__, JADA.Common.__LINE__, sesSelf.Self);
            if (reqSelf != IntPtr.Zero) {
                myEventThread = new System.Threading.Thread(new ThreadStart(HandleEventThread));
                myAudioFrameThread = new System.Threading.Thread(new ThreadStart(HandleAudioFrameThread));
                myVideoFrameThread = new System.Threading.Thread(new ThreadStart(HandleVideoFrameThread));
                myDrawFrameThread = new System.Threading.Thread(new ThreadStart(HandleDrawFrameThread));

                myEventThread.IsBackground = true; myEventThread.Start();
                myAudioFrameThread.IsBackground = true; myAudioFrameThread.Start();
                myVideoFrameThread.IsBackground = true; myVideoFrameThread.Start();
                myDrawFrameThread.IsBackground = true; myDrawFrameThread.Start();
            }
        }

        ~Request()
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
                    iRet = JADA_ReqDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, reqSelf);
                    reqSelf = IntPtr.Zero;
                }

                if (myEventThread != null) {
                    myEventThread.Abort(); myEventThread = null;
                }
                if (myAudioFrameThread != null) {
                    myAudioFrameThread.Abort(); myAudioFrameThread = null;
                }
                if (myVideoFrameThread != null) {
                    myVideoFrameThread.Abort(); myVideoFrameThread = null;
                }
                if (myDrawFrameThread != null) {
                    myDrawFrameThread.Abort(); myDrawFrameThread = null;
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

            iRet = JADA_ReqPing(reqSelf);
        }

        public void HandleEventThread()
        {
            int iRet, iEventType;
            var evtPtr = new IntPtr();
            JADA.Event evtSelf = null;

            while (true)
            {
                iRet = JADA_ReqGetNextEvent(reqSelf, 15000, ref evtPtr);
                if (iRet == 0)
                {
                    try
                    {
                        evtSelf = new JADA.Event(evtPtr);
                        EventArgs e = new EventArgs(evtSelf);

                        iEventType = evtSelf.EventType;
                        switch (iEventType)
                        {
                            case JADA.Event.EVT_CHATTEXT_RECEIVED:
                                EventHandler ChatTextReceivedHandler = this.ChatTextReceived;
                                if (null != ChatTextReceivedHandler) ChatTextReceivedHandler(this, e);
                                break;
                            case JADA.Event.EVT_CALL_OPENED:
                                EventHandler CallOpenedHandler = this.CallOpened;
                                if (null != CallOpenedHandler) CallOpenedHandler(this, e);
                                StartAudio();
                                myAudioRestartTimer = new System.Threading.Timer(new TimerCallback(RestartAudioTimer), null, 0, 5 * 60 * 1000);
                                break;
                            case JADA.Event.EVT_CALL_CLOSED:
                                if (myAudioRestartTimer != null)
                                {
                                    myAudioRestartTimer.Dispose(); myAudioRestartTimer = null;
                                }
                                StopAudio();
                                EventHandler CallClosedHandler = this.CallClosed;
                                if (null != CallClosedHandler) CallClosedHandler(this, e);
                                break;
                            case JADA.Event.EVT_CALL_ABORTED:
                                if (myAudioRestartTimer != null)
                                {
                                    myAudioRestartTimer.Dispose(); myAudioRestartTimer = null;
                                }
                                StopAudio();
                                EventHandler CallAbortedHandler = this.CallAborted;
                                if (null != CallAbortedHandler) CallAbortedHandler(this, e);
                                break;
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
                            case JADA.Event.EVT_PARAMETERS_LIST:
                                EventHandler ParameterListReceivedHandler = this.ParameterListReceived;
                                if (null != ParameterListReceivedHandler) ParameterListReceivedHandler(this, e);
                                break;
                            case JADA.Event.EVT_DIRECTORY_LIST:
                                EventHandler DirectoryListReceivedHandler = this.DirectoryListReceived;
                                if (null != DirectoryListReceivedHandler) DirectoryListReceivedHandler(this, e);
                                break;
                            case JADA.Event.EVT_PING_REPLY:
                                EventHandler PingReplyHandler = this.PingReply;
                                if (null != PingReplyHandler) PingReplyHandler(this, e);
                                break;
                        }
                    }
                    catch (Exception e)
                    {
                        JADA.Common.LogMessage(String.Format("HandleEventThread {0}\n{1}", e.Message, e.StackTrace));
                        // MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "AREAS Customer Assistant");
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
        //     G E S T I O N E     F R A M E S
        // 
        //********************************************************************* 
        public void HandleAudioFrameThread()
        {
            int iRet;
            var frmPtr = new IntPtr();
            JADA.Frame frmSelf = null;
                
            while (true) {
                iRet = JADA_ReqGetNextAudioFrame(reqSelf, 15000, ref frmPtr);
                if (iRet == 0) {
                    try {
                        frmSelf = new JADA.Frame(frmPtr);
                        FrameEventArgs e = new FrameEventArgs(frmSelf);

                        int iFrameType = frmSelf.FrameType;
                        if (iFrameType == JADA.Frame.FRAMETYPE_AUDIO)
                            PlayAudio(frmSelf);

                        frmSelf.Dispose();
                    }
                    catch (Exception e)
                    {
                        JADA.Common.LogMessage(String.Format("HandleAudioFrameThread {0}\n{1}", e.Message, e.StackTrace));
                        // MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "AREAS Customer Assistant");
                    }
                    finally
                    {
                        if (frmSelf != null)
                        {
                            frmSelf.Dispose();
                        }
                    }
                }
            }
        }

        public void HandleVideoFrameThread()
        {
            int iRet;
            var frmPtr = new IntPtr();
            JADA.Frame frmSelf = null;

            while (true) {
                iRet = JADA_ReqGetNextVideoFrame(reqSelf, 15000, ref frmPtr);
                if (iRet == 0)
                {
                    try {
                        frmSelf = new JADA.Frame(frmPtr);
                        FrameEventArgs e = new FrameEventArgs(frmSelf);

                        int iFrameType = frmSelf.FrameType;
                        if (iFrameType == JADA.Frame.FRAMETYPE_VIDEO) {
                            EventHandler VideoFrameHandler = this.VideoFrameReceived;
                            if (null != VideoFrameHandler) VideoFrameHandler(this, e);
                        }

                        frmSelf.Dispose();
                    }
                    catch (Exception e)
                    {
                        JADA.Common.LogMessage(String.Format("HandleVideoFrameThread {0}\n{1}", e.Message, e.StackTrace));
                        // MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "AREAS Customer Assistant");
                    }
                    finally
                    {
                        if (frmSelf != null)
                        {
                            frmSelf.Dispose();
                        }
                    }
                }
            }
        }

        public void HandleDrawFrameThread()
        {
            int iRet;
            var drwPtr = new IntPtr();
            JADA.Draw drwSelf = null;

            while (true) {
                iRet = JADA_ReqGetNextDrawFrame(reqSelf, 15000, ref drwPtr);
                if (iRet == 0)
                {
                    try {
                        drwSelf = new JADA.Draw(drwPtr);
                        DrawEventArgs e = new DrawEventArgs(drwSelf);

                        EventHandler DrawFrameHandler = this.DrawFrameReceived;
                        if (null != DrawFrameHandler) DrawFrameHandler(this, e);

                        drwSelf.Dispose();
                    }
                    catch (Exception e)
                    {
                        JADA.Common.LogMessage(String.Format("HandleDrawFrameThread {0}\n{1}", e.Message, e.StackTrace));
                        // MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "AREAS Customer Assistant");
                    }
                    finally
                    {
                        if (drwSelf != null)
                        {
                            drwSelf.Dispose();
                        }
                    }
                }
            }
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

        public int Open(int iWaitSeconds)
        {
            int iRet;
            Cursor.Current = Cursors.WaitCursor;
            Application.DoEvents();

            iRet = JADA_ReqOpen(reqSelf, iWaitSeconds);
            Cursor.Current = Cursors.Default;

            return (iRet);
        }

        public int Connect(int iRequestId)
        {
            int iRet;

            iRet = JADA_ReqConnect(reqSelf, iRequestId);

            return (iRet);
        }

        public int Accept(int iRequestId)
        {
            int iRet;

            iRet = JADA_ReqAccept(reqSelf, iRequestId);

            return (iRet);
        }

        public int Load(int iRequestId)
        {
            int iRet;

            iRet = JADA_ReqLoad(reqSelf, iRequestId);

            return (iRet);
        }

        public int Resume(int iRequestId)
        {
            int iRet;

            iRet = JADA_ReqResume(reqSelf, iRequestId);

            return (iRet);
        }

        public int Solicit()
        {
            int iRet;

            iRet = JADA_ReqSolicit(reqSelf);

            return (iRet);
        }

        public int Close()
        {
            int iRet;

            iRet = JADA_ReqClose(reqSelf);
            if (iRet == 0)
            {
                StopPing();
            }

            return (iRet);
        }

        public int OpenCall()
        {
            int iRet;

            iRet = JADA_ReqOpenCall(reqSelf);

            return (iRet);
        }

        public int ResetCall()
        {
            int iRet;

            iRet = JADA_ReqCallReset(reqSelf);

            return (iRet);
        }

        public int MuteAudio(int iMuteStatus)
        {
            int iRet;

            iRet = JADA_ReqCallMuteAudio(reqSelf, iMuteStatus);

            return (iRet);
        }

        public int MuteVideo(int iMuteStatus)
        {
            int iRet;

            iRet = JADA_ReqCallMuteVideo(reqSelf, iMuteStatus);

            return (iRet);
        }

        public int CloseCall()
        {
            int iRet;

            iRet = JADA_ReqCloseCall(reqSelf);
            if (iRet == 0)
            {
                StopPing();
            }

            return (iRet);
        }

        public int OpenDesktop()
        {
            int iRet;

            iRet = JADA_ReqOpenDesktop(reqSelf);

            return (iRet);
        }

        public int AcceptDesktop(bool iAccepted)
        {
            int iRet;

            if (iAccepted == true)
                iRet = JADA_ReqAcceptDesktop(reqSelf, JADA.Common.TRUE);
            else
                iRet = JADA_ReqAcceptDesktop(reqSelf, JADA.Common.FALSE);

            return (iRet);
        }

        public int CloseDesktop()
        {
            int iRet;

            iRet = JADA_ReqCloseDesktop(reqSelf);

            return (iRet);
        }

        public int OpenWhiteboard()
        {
            int iRet;

            iRet = JADA_ReqOpenWhiteboard(reqSelf);
            return (iRet);
        }

        public int CleanWhiteboard()
        {
            int iRet;

            iRet = JADA_ReqCleanWhiteboard(reqSelf);
            return (iRet);
        }

        public int CloseWhiteboard()
        {
            int iRet;

            iRet = JADA_ReqCloseWhiteboard(reqSelf);
            return (iRet);
        }

        public int SendChat(string szText)
        {
            int iRet;

            iRet = JADA_ReqSendChat(reqSelf, szText);

            return (iRet);
        }

        public int ListParameters(string szFileName, int iWaitSeconds)
        {
            int iRet;

            Cursor.Current = Cursors.WaitCursor;
            Application.DoEvents();

            iRet = JADA_ReqListParameters(reqSelf, szFileName, iWaitSeconds);

            Cursor.Current = Cursors.Default;

            return (iRet);
        }

        public int ListDirectory(string szDirName)
        {
            int iRet;

            iRet = JADA_ReqListDirectory(reqSelf, szDirName);

            return (iRet);
        }

        public int GetFile(string szRemoteFile, string szLocalFile)
        {
            int iRet;

            iRet = JADA_ReqGetFile(reqSelf, szRemoteFile, szLocalFile);

            return (iRet);
        }

        public int CancelGetFile()
        {
            int iRet;

            iRet = JADA_ReqCancelGetFile(reqSelf);

            return (iRet);
        }

        public int PutFile(string szLocalFile, string szRemoteFile)
        {
            int iRet;

            iRet = JADA_ReqPutFile(reqSelf, szLocalFile, szRemoteFile);

            return (iRet);
        }

        public int CancelPutFile()
        {
            int iRet;

            iRet = JADA_ReqCancelPutFile(reqSelf);

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

            iRet = JADA_ReqGetParameter(reqSelf, mySection, myName, myValue, myValue.Capacity);
            if (iRet == 0)
                return (myValue.ToString());
            else
                return ("");
        }

        private void SetParameter(string mySection, string myName, string myValue)
        {
            int iRet;

            iRet = JADA_ReqSetParameter(reqSelf, mySection, myName, myValue);
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

        public string Area
        {
            get
            {
                return (GetParameter("richiesta", "area"));
            }

            set
            {
                SetParameter("richiesta", "area", value);
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

        public IntPtr Session
        {
            get
            {
                int iRet;
                var szMySession = new IntPtr();

                iRet = JADA_ReqGetSession(reqSelf, ref szMySession);
                if (iRet == 0)
                {
                    return (szMySession);
                }
                else
                {
                    return (IntPtr.Zero);
                }
            }
        }

        public int Id
        {
            get
            {
                int iRet;
                int iMyId = 0;

                iRet = JADA_ReqGetId(reqSelf, ref iMyId);
                if (iRet == 0)
                    return (iMyId);
                else
                    return (-1);
            }

            set
            {
                int iRet;

                iRet = JADA_ReqSetId(reqSelf, value);
            }
        }

        public int SuspendedId
        {
            get
            {
                int iRet;
                int iMyId = 0;

                iRet = JADA_ReqGetSuspendedId(reqSelf, ref iMyId);
                if (iRet == 0)
                    return (iMyId);
                else
                    return (iMyId);
            }
            set
            {
                int iRet;

                iRet = JADA_ReqSetSuspendedId(reqSelf, value);
            }
        }

        public bool Connected
        {
            get {
                int iRet;

                iRet = JADA_ReqIsConnected(reqSelf);
                if (iRet != 0) {
                    return (true);
                }
                else {
                    return(false);
                }
            }
        }

        public int VideoFrameDelay
        {
            get
            {
                int iRet;
                int iMyDelay = -1;

                iRet = JADA_ReqGetVideoFrameDelay(reqSelf, ref iMyDelay);
                if (iRet == 0)
                {
                    return (iMyDelay);
                }
                else
                {
                    return (-1);
                }
            }

            set
            {
                int iRet;

                iRet = JADA_ReqSetVideoFrameDelay(reqSelf, value);
            }
        }

        #region Audio()
        //*********************************************************************
        //
        //      A U D I O     M A N A G E M E N T
        //
        //*********************************************************************
        private WaveLib.WaveOutPlayer m_Player;
        private WaveLib.WaveInRecorder m_Recorder;
        private WaveLib.FifoStream m_Fifo = new WaveLib.FifoStream();
        private byte[] m_PlayBuffer;
        private byte[] m_RecBuffer;

        private void Filler(IntPtr data, int size)
        {
            if (m_PlayBuffer == null || m_PlayBuffer.Length < size)
                m_PlayBuffer = new byte[size];
            if (m_Fifo.Length >= size)
                m_Fifo.Read(m_PlayBuffer, 0, size);
            else
                for (int i = 0; i < m_PlayBuffer.Length; i++)
                    m_PlayBuffer[i] = 0;
            System.Runtime.InteropServices.Marshal.Copy(m_PlayBuffer, 0, data, size);
            // m_Fifo ==> m_PlayBuffer==> data ==> Speakers
        }

        public void RestartAudioTimer(object o)
        {
            try
            {
                // StartAudio();
            }
            catch (Exception e)
            {
                JADA.Common.LogMessage(String.Format("RestartAudioTimer {0}\n{1}", e.Message, e.StackTrace));
                // MessageBox.Show("Errore nell'applicazione: i dettagli sono stati salvati nel file di log", "AREAS Customer Assistant");
            }
        }

        private void StartAudio()
        {
            StopAudio();
            try
            {
                // Rates: 44100, 22050, 88200
                // Valore buono: 44100, 16, 1 / Buffer 8192, 2
                //WaveLib.WaveFormat fmt = new WaveLib.WaveFormat(44100, 16, 2);
                //m_Player = new WaveLib.WaveOutPlayer(-1, fmt, 8192, 3, new WaveLib.BufferFillEventHandler(Filler));
                //m_Recorder = new WaveLib.WaveInRecorder(-1, fmt, 8192, 3, new WaveLib.BufferDoneEventHandler(SendAudioFrame));
                WaveLib.WaveFormat fmt = new WaveLib.WaveFormat(22050, 16, 1);
                m_Player = new WaveLib.WaveOutPlayer(-1, fmt, 8192, 2, new WaveLib.BufferFillEventHandler(Filler));
                m_Recorder = new WaveLib.WaveInRecorder(-1, fmt, 8192, 2, new WaveLib.BufferDoneEventHandler(SendAudioFrame));
            }
            catch
            {
                StopAudio();
                throw;
            }
        }

        private void StopAudio()
        {
            if (m_Player != null)
                try
                {
                    m_Player.Dispose();
                }
                finally
                {
                    m_Player = null;
                }
            if (m_Recorder != null)
                try
                {
                    m_Recorder.Dispose();
                }
                finally
                {
                    m_Recorder = null;
                }
            m_Fifo.Flush(); // clear all pending data
        }

        private void PlayAudio(Frame myAudioFrame)
        {
            byte[] arrFrame = myAudioFrame.Data;

            m_Fifo.Write(arrFrame, 0, myAudioFrame.Length);
        }

        public void SetAudioVolume(int iVolumeLevel)
        {
            if (m_Player != null)
                m_Player.SetVolume(iVolumeLevel);
        }

        private void SendAudioFrame(IntPtr data, int size)
        {
            int iRet;

            //for Recorder
            if (m_RecBuffer == null || m_RecBuffer.Length < size)
                m_RecBuffer = new byte[size];

            try
            {
                // Leggi nel frame il blocco audio corrente dal microfono
                System.Runtime.InteropServices.Marshal.Copy(data, m_RecBuffer, 0, size);
                iRet = JADA_ReqSendAudioFrame(reqSelf, m_RecBuffer, size);
            }
            catch
            {
            }
        }
        #endregion

        #region Video()
        //*********************************************************************
        //
        //      V I D E O     M A N A G E M E N T
        //
        //*********************************************************************
        private byte[] m_VideoBuffer;

        public void SendVideoFrame(MemoryStream data)
        {
            int iRet;

            // Allarga il buffer di lavoro,se necessario
            if (m_VideoBuffer == null || m_VideoBuffer.Length < data.Length)
                m_VideoBuffer = new byte[data.Length];

            // Invia il frame video
            data.Position = 0;
            data.Read(m_VideoBuffer, 0, Convert.ToInt32(data.Length));
            iRet = JADA_ReqSendVideoFrame(reqSelf, m_VideoBuffer, Convert.ToInt32(data.Length));
        }
        #endregion

        #region Draw()
        //*********************************************************************
        //
        //      D R A W     M A N A G E M E N T
        //
        //*********************************************************************
        public void SendDrawFrame(JADA.Connection drwSelf)
        {
            int iRet;

            iRet = JADA_ReqSendDrawFrame(reqSelf, drwSelf.Self);
        }

        public void SetDrawPointer(int iPointerType, int iPointX, int iPointY)
        {
            int iRet;

            iRet = JADA_ReqSetDrawPointer(reqSelf, iPointerType, iPointX, iPointY);
        }

        public void ResetDrawPointer()
        {
            int iRet;

            iRet = JADA_ReqResetDrawPointer(reqSelf);
        }
        #endregion
    }
}
