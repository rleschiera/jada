using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;


namespace JADA
{
    public class EventArgs : System.EventArgs
    {
        public int Type { get; private set; }
        public int Result { get; private set; }
        public string Area { get; private set; }
        public string Description { get; private set; }
        public int SessionId { get; private set; }
        public int StreamId { get; private set; }
        public string Time { get; private set; }
        public int RequestId { get; private set; }
        public string Utente { get; private set; }
        public int WhoAmI { get; private set; }
        public bool UsaAudio { get; private set; }
        public int ItemType { get; private set; }
        public int PointX { get; private set; }
        public int PointY { get; private set; }
        public string ChatText { get; private set; }
        public int AreaRequests { get; private set; }
        public int TotalRequests { get; private set; }
        public int AreaPendingRequests { get; private set; }
        public int TotalPendingRequests { get; private set; }
        public string Password { get; private set; }
        public string FileName { get; private set; }
        public int CurrFrame { get; private set; }
        public int Frames { get; private set; }

        public EventArgs(JADA.Event myEvent)
        {
            this.Type = myEvent.EventType;
            this.Result = myEvent.Result;
            this.SessionId = myEvent.SessionId;
            this.StreamId = myEvent.StreamId;
            this.RequestId = myEvent.RequestId;
            this.WhoAmI = myEvent.WhoAmI;
            this.UsaAudio = myEvent.UsaAudio;
            this.ItemType = myEvent.ItemType;
            this.PointX = myEvent.PointX;
            this.PointY = myEvent.PointY;
            this.Area = myEvent.Area;
            this.Description = myEvent.Description;
            this.ChatText = myEvent.ChatText;
            this.Utente = myEvent.Utente;
            this.Password = myEvent.Password;
            this.AreaRequests = myEvent.AreaRequests;
            this.TotalRequests = myEvent.TotalRequests;
            this.AreaPendingRequests = myEvent.AreaPendingRequests;
            this.TotalPendingRequests = myEvent.TotalPendingRequests;
            this.FileName = myEvent.FileName;
            this.Time = myEvent.Time;
            this.CurrFrame = myEvent.CurrFrame;
            this.Frames = myEvent.Frames;
        }
    }


    public class Event
    {
        /* Opcode degli eventi: connessione */
        public const int EVT_CONNECTION_CONNECTED   = 3000;
        public const int EVT_CONNECTION_OPENED      = 3001;
        public const int EVT_CONNECTION_CLOSED      = 3002;
        public const int EVT_SESSIONS_LIST          = 3007;
        public const int EVT_REQUESTS_LIST          = 3008;
        public const int EVT_LIST_COUNTERS          = 3009;
        public const int EVT_PARAMETERS_LIST        = 3010;
        public const int EVT_COMBO_VALUES           = 3011;
        public const int EVT_MSG_RESULT             = 3101;
        public const int EVT_PING_REPLY             = 3102;
        /* Opcode degli eventi: supporto */
        public const int EVT_SUPPORT_OPENED         = 3201;
        public const int EVT_SUPPORT_ACCEPTED       = 3202;
        public const int EVT_SUPPORT_LOADED         = 3203;
        public const int EVT_SUPPORT_CONNECTED      = 3204;
        public const int EVT_SUPPORT_SOLICITED      = 3205;
        public const int EVT_SUPPORT_CLOSED         = 3206;
        public const int EVT_SUPPORT_SUSPENDED      = 3207;
        public const int EVT_SUPPORT_RESUMED        = 3208;
        public const int EVT_CHATTEXT_RECEIVED      = 3209;

        /* Opcode degli eventi: media */
        public const int EVT_CALL_OPENED            = 3301;
        public const int EVT_CALL_CLOSED            = 3302;
        public const int EVT_CALL_ABORTED           = 3303;
        /* Opcode degli eventi: VNC */
        public const int EVT_DESKTOP_OPENED         = 3401;
        public const int EVT_DESKTOP_REQUESTED      = 3402;
        public const int EVT_DESKTOP_CLOSED         = 3403;
        public const int EVT_WHITEBOARD_OPENED      = 3404;
        public const int EVT_WHITEBOARD_CLEANED     = 3405;
        public const int EVT_WHITEBOARD_CLOSED      = 3406;
        public const int EVT_SET_DRAW_POINTER       = 3407;
        public const int EVT_RESET_DRAW_POINTER     = 3408;
        /* Opcode degli eventi: FTP */
        public const int EVT_FTP_OPENED             = 3501;
        public const int EVT_DIRECTORY_LIST         = 3502;
        public const int EVT_FILE_SENT              = 3503;
        public const int EVT_FILE_RECEIVED          = 3504;
        public const int EVT_FTP_CLOSED             = 3505;

        public const int LISTTYPE_REQUESTS          = 1001;
        public const int LISTTYPE_PENDING_REQUESTS  = 1002;

        // Funzioni native nella DLL
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr JADA_EvtNew(string szFile, int iLine);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtDestroy(string szFile, int iLine, IntPtr evtSelf);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetType(IntPtr evtSelf, ref int iEventType);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetResult(IntPtr evtSelf, ref int piResult);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetSessionId(IntPtr evtSelf, ref int piSessionId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetStreamId(IntPtr evtSelf, ref int piStreamId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetRequestId(IntPtr evtSelf, ref int piRequestId);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetWhoAmI(IntPtr evtSelf, ref int piWhoAmI);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetUsaAudio(IntPtr evtSelf, ref int piUsaAudio);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetItemType(IntPtr evtSelf, ref int piItemType);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetPointX(IntPtr evtSelf, ref int piPointX);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetPointY(IntPtr evtSelf, ref int piPointY);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetUtente(IntPtr evtSelf, StringBuilder szUtente, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetPassword(IntPtr evtSelf, StringBuilder szPassword, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetArea(IntPtr evtSelf, StringBuilder szArea, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetDescription(IntPtr evtSelf, StringBuilder szDescription, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetFileName(IntPtr evtSelf, StringBuilder szFileName, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetChatText(IntPtr evtSelf, StringBuilder szChatText, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetTime(IntPtr evtSelf, StringBuilder szTime, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetAreaRequests(IntPtr evtSelf, ref int piRequests);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetTotalRequests(IntPtr evtSelf, ref int piRequests);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetAreaPendingRequests(IntPtr evtSelf, ref int piPendingRequests);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetTotalPendingRequests(IntPtr evtSelf, ref int piPendingRequests);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetCurrFrame(IntPtr evtSelf, ref int piCurrFrame);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_EvtGetFrames(IntPtr evtSelf, ref int piFrames);

        private IntPtr evtSelf;
        private bool disposed = false;
        
        public Event(IntPtr evtPtr)
        {
            evtSelf = evtPtr;
        }

        ~Event()
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
                    iRet = JADA_EvtDestroy(JADA.Common.__FILE__, JADA.Common.__LINE__, evtSelf);
                    evtSelf = IntPtr.Zero;
                }

                disposed = true;
            }
        }

        //********************************************************************* 
        //
        //     A T T R I B U T I
        // 
        //********************************************************************* 
        public int EventType
        {
            get
            {
                int iRet;
                int iMyType = -1;

                iRet = JADA_EvtGetType(evtSelf, ref iMyType);
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

        public int Result
        {
            get
            {
                int iRet;
                int iMyResult = -1;

                iRet = JADA_EvtGetResult(evtSelf, ref iMyResult);
                if (iRet == 0)
                {
                    return (iMyResult);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int SessionId
        {
            get
            {
                int iRet;
                int iMySessionId = -1;

                iRet = JADA_EvtGetSessionId(evtSelf, ref iMySessionId);
                if (iRet == 0)
                {
                    return (iMySessionId);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int StreamId
        {
            get
            {
                int iRet;
                int iMyStreamId = -1;

                iRet = JADA_EvtGetStreamId(evtSelf, ref iMyStreamId);
                if (iRet == 0)
                {
                    return (iMyStreamId);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int RequestId
        {
            get
            {
                int iRet;
                int iMyRequestId = -1;

                iRet = JADA_EvtGetRequestId(evtSelf, ref iMyRequestId);
                if (iRet == 0)
                {
                    return (iMyRequestId);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int WhoAmI
        {
            get
            {
                int iRet;
                int iMyWhoAmI = -1;

                iRet = JADA_EvtGetWhoAmI(evtSelf, ref iMyWhoAmI);
                if (iRet == 0)
                {
                    return (iMyWhoAmI);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public bool UsaAudio
        {
            get
            {
                int iRet;
                int iMyUsaAudio = -1;

                iRet = JADA_EvtGetUsaAudio(evtSelf, ref iMyUsaAudio);
                if (iRet == 0)
                {
                    return (Convert.ToBoolean(iMyUsaAudio));
                }
                else
                {
                    return (false);
                }
            }
        }

        public int ItemType
        {
            get
            {
                int iRet;
                int iMyItemType = -1;

                iRet = JADA_EvtGetItemType(evtSelf, ref iMyItemType);
                if (iRet == 0)
                {
                    return (iMyItemType);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int PointX
        {
            get
            {
                int iRet;
                int iMyPointX = -1;

                iRet = JADA_EvtGetPointX(evtSelf, ref iMyPointX);
                if (iRet == 0)
                {
                    return (iMyPointX);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int PointY
        {
            get
            {
                int iRet;
                int iMyPointY = -1;

                iRet = JADA_EvtGetPointY(evtSelf, ref iMyPointY);
                if (iRet == 0)
                {
                    return (iMyPointY);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int CurrFrame
        {
            get
            {
                int iRet;
                int iMyCurrFrame = -1;

                iRet = JADA_EvtGetCurrFrame(evtSelf, ref iMyCurrFrame);
                if (iRet == 0)
                    return (iMyCurrFrame);
                else
                    return (-1);
            }
        }

        public int Frames
        {
            get
            {
                int iRet;
                int iMyFrames = -1;

                iRet = JADA_EvtGetFrames(evtSelf, ref iMyFrames);
                if (iRet == 0)
                    return (iMyFrames);
                else
                    return (-1);
            }
        }

        public string Utente
        {
            get
            {
                int iRet;
                StringBuilder szMyUtente = new StringBuilder(256);

                iRet = JADA_EvtGetUtente(evtSelf, szMyUtente, szMyUtente.Capacity);
                if (iRet == 0)
                {
                    return (szMyUtente.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }

        public string Password
        {
            get
            {
                int iRet;
                StringBuilder szMyPassword = new StringBuilder(256);

                iRet = JADA_EvtGetPassword(evtSelf, szMyPassword, szMyPassword.Capacity);
                if (iRet == 0)
                {
                    return (szMyPassword.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }

        public int AreaRequests
        {
            get
            {
                int iRet;
                int iMyRequests = -1;

                iRet = JADA_EvtGetAreaRequests(evtSelf, ref iMyRequests);
                if (iRet == 0)
                {
                    return (iMyRequests);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int TotalRequests
        {
            get
            {
                int iRet;
                int iMyRequests = -1;

                iRet = JADA_EvtGetTotalRequests(evtSelf, ref iMyRequests);
                if (iRet == 0)
                {
                    return (iMyRequests);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int AreaPendingRequests
        {
            get
            {
                int iRet;
                int iMyPendingRequests = -1;

                iRet = JADA_EvtGetAreaPendingRequests(evtSelf, ref iMyPendingRequests);
                if (iRet == 0)
                {
                    return (iMyPendingRequests);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public int TotalPendingRequests
        {
            get
            {
                int iRet;
                int iMyPendingRequests = -1;

                iRet = JADA_EvtGetTotalPendingRequests(evtSelf, ref iMyPendingRequests);
                if (iRet == 0)
                {
                    return (iMyPendingRequests);
                }
                else
                {
                    return (-1);
                }
            }
        }

        public string Area
        {
            get
            {
                int iRet;
                StringBuilder szMyArea = new StringBuilder(256);

                iRet = JADA_EvtGetArea(evtSelf, szMyArea, szMyArea.Capacity);
                if (iRet == 0)
                {
                    return (szMyArea.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }

        public string Description
        {
            get
            {
                int iRet;
                StringBuilder szMyDescription = new StringBuilder(256);

                iRet = JADA_EvtGetDescription(evtSelf, szMyDescription, szMyDescription.Capacity);
                if (iRet == 0)
                {
                    return (szMyDescription.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }

        public string FileName
        {
            get
            {
                int iRet;
                StringBuilder szMyFileName = new StringBuilder(256);

                iRet = JADA_EvtGetFileName(evtSelf, szMyFileName, szMyFileName.Capacity);
                if (iRet == 0)
                {
                    return (szMyFileName.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }

        public string ChatText
        {
            get
            {
                int iRet;
                StringBuilder szMyChatText = new StringBuilder(1024);

                iRet = JADA_EvtGetChatText(evtSelf, szMyChatText, szMyChatText.Capacity);
                if (iRet == 0)
                {
                    return (szMyChatText.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }

        public string Time {
            get
            {
                int iRet;
                StringBuilder szMyTime = new StringBuilder(256);

                iRet = JADA_EvtGetTime(evtSelf, szMyTime, szMyTime.Capacity);
                if (iRet == 0)
                {
                    return (szMyTime.ToString());
                }
                else
                {
                    return ("");
                }
            }
        }
    }
}
