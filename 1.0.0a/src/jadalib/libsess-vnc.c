#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"


/******************************************************************************
 *
 *   V N C     S E R V I C E S
 *
 ******************************************************************************/
//typedef struct {
//    char        *szSession;
//    char        *szKey;
//    char        *szValue;
//    int         iIsPassword;
//} JADA_IniEntry;
//
//static JADA_IniEntry VNCServerIni[] = {
//    "UltraVNC",         "passwd",                   "EE5B0E48C8FE97716D",       0, // 1234
//    "UltraVNC",         "passwd2",                  "2F8DCA9F3DBC3B69C2",       0, // 5678 
//    "admin",            "UseRegistry",              "0",                        0,
//    "admin",            "MSLogonRequired",          "0",                        0,
//    "admin",            "NewMSLogon",               "0",                        0,
//    "admin",            "DebugMode",                "0",                        0,
//    "admin",            "Avilog",                   "0",                        0,
//    "admin",            "path",                     "C:\\jada\\bin",            0,
//    "admin",            "accept_reject_mesg",       "",                         0,
//    "admin",            "DebugLevel",               "8",                        0,
//    "admin",            "DisableTrayIcon",          "0",                        0,
//    "admin",            "LoopbackOnly",             "1",                        0,
//    "admin",            "UseDSMPlugin",             "0",                        0,
//    "admin",            "AllowLoopback",            "1",                        0,
//#ifdef VNC_PASSWORD
//    "admin",            "AuthRequired",             "1",                        0,
//#else
//    "admin",            "AuthRequired",             "1",                        0,
//#endif
//    "admin",            "ConnectPriority",          "0",                        0,
//    "admin",            "DSMPlugin",                "No Plugin Detected",       0,
//    "admin",            "AuthHosts",                "",                         0,
//    "admin",            "DSMPluginConfig",          "",                         0,
//    "admin",            "AllowShutdown",            "1",                        0,
//    "admin",            "AllowProperties",          "1",                        0,
//    "admin",            "AllowEditClients",         "1",                        0,
//    "admin",            "FileTransferEnabled",      "0",                        0,
//    "admin",            "FTUserImpersonation",      "1",                        0,
//    "admin",            "BlankMonitorEnabled",      "1",                        0,
//    "admin",            "BlankInputsOnly",          "0",                        0,
//    "admin",            "DefaultScale",             "1",                        0,
//    "admin",            "primary",                  "1",                        0,
//    "admin",            "secondary",                "0",                        0,
//    "admin",            "SocketConnect",            "1",                        0,
//    "admin",            "HTTPConnect",              "1",                        0,
//    "admin",            "AutoPortSelect",           "0",                        0,
//    "admin",            "PortNumber",               "45900",                    0,
//    "admin",            "HTTPPortNumber",           "45800",                    0,
//    "admin",            "IdleTimeout",              "0",                        0,
//    "admin",            "IdleInputTimeout",         "0",                        0,
//    "admin",            "RemoveWallpaper",          "0",                        0,
//    "admin",            "RemoveAero",               "0",                        0,
//#ifdef VNC_CONFIRM
//    "admin",            "QuerySetting",             "4",                        0,
//#else
//    "admin",            "QuerySetting",             "2",                        0,
//#endif
//    "admin",            "QueryTimeout",             "10",                       0,
//    "admin",            "QueryAccept",              "1",                        0,
//    "admin",            "QueryIfNoLogon",           "1",                        0,
//    "admin",            "InputsEnabled",            "1",                        0,
//    "admin",            "LockSetting",              "0",                        0,
//    "admin",            "LocalInputsDisabled",      "0",                        0,
//    "admin",            "EnableJapInput",           "0",                        0,
//    "admin",            "kickrdp",                  "0",                        0,
//    "admin",            "clearconsole",             "0",                        0,
//    "admin",            "service_commandline",      "",                         0,
//    "admin",            "FileTransferTimeout",      "1",                        0,
//    "admin",            "KeepAliveInterval",        "5",                        0,
//    "admin_auth",       "group1",                   "",                         0,
//    "admin_auth",       "group2",                   "",                         0,
//    "admin_auth",       "group3",                   "",                         0,
//    "admin_auth",       "locdom1",                  "0",                        0,
//    "admin_auth",       "locdom2",                  "0",                        0,
//    "admin_auth",       "locdom3",                  "0",                        0,
//    "poll",             "TurboMode",                "1",                        0,
//    "poll",             "PollUnderCursor",          "0",                        0,
//    "poll",             "PollForeground",           "0",                        0,
//    "poll",             "PollFullScreen",           "1",                        0,
//    "poll",             "OnlyPollConsole",          "0",                        0,
//    "poll",             "OnlyPollOnEvent",          "0",                        0,
//    "poll",             "MaxCpu",                   "40",                       0,
//    "poll",             "EnableDriver",             "0",                        0,
//    "poll",             "EnableHook",               "1",                        0,
//    "poll",             "EnableVirtual",            "0",                        0,
//    "poll",             "SingleWindow",             "0",                        0,
//    "poll",             "SingleWindowName",         "",                         0,
//    NULL,               NULL,                       NULL,                       0
//};
//
//int SetVNCServer(void)
//{
//    int     i;
//    DWORD   dwRet;
//
//    for (i = 0; VNCServerIni[i].szSession != NULL; i++) {
//        dwRet = WritePrivateProfileStringA(VNCServerIni[i].szSession, VNCServerIni[i].szKey, VNCServerIni[i].szValue, JADA_VNCSERVER_INI);
//        CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "errore nel settaggio del valore nel file INI");
//    }
//
//    return(0);
//}
//
//int SetVNCPassword(char *szNewPassword)
//{
//    int     i;
//    DWORD   dwRet;
//
//    for (i = 0; VNCServerIni[i].szSession != NULL; i++) {
//        if (VNCServerIni[i].iIsPassword != 0) {
//            if (szNewPassword != NULL) {
//                dwRet = WritePrivateProfileStringA(VNCServerIni[i].szSession, VNCServerIni[i].szKey, szNewPassword, JADA_VNCSERVER_INI);
//                CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "errore nel settaggio del valore nel file INI");
//            }
//            else {
//                dwRet = WritePrivateProfileStringA(VNCServerIni[i].szSession, VNCServerIni[i].szKey, VNCServerIni[i].szValue, JADA_VNCSERVER_INI);
//                CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "errore nel settaggio del valore nel file INI");
//            }
//        }
//    }
//
//    return(0);
//}
//
DllExport JADA_VNC __stdcall JADA_VncNew(char *szFile, int iLine, JADA_Connection connSelf)
{
    int             iRet;
    JADA_VncSelf    *vncPtr;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);

    vncPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_VncSelf));
    CORE_ReturnValIfFail(vncPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &vncPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &vncPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del support");
    vncPtr->iStatus = JADA_VNCSTATUS_AVAILABLE;
    vncPtr->connSelf = connSelf;
    time(&vncPtr->timLastActivity);
    vncPtr->iMagic = JADA_MAGIC_VNC;

    connPtr->vncSelf = (JADA_VNC) vncPtr;

    vncPtr->lstInDrawFrames = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Draw), NULL);
    CORE_ReturnValIfFail(vncPtr->lstInDrawFrames != NULL, NULL, ;, "Errore nella creazione della lista di frames");

    return((JADA_VNC) vncPtr);
}

DllExport int __stdcall JADA_VncDestroy(char *szFile, int iLine, JADA_VNC vncSelf)
{
    int             iRet, iRetries = 100;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_Draw       drwSelf;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  

    if (JADA_VncIsConnected(vncSelf) == TRUE) {
        iRet = JADA_VncCloseDesktop(vncSelf);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella chiusura della richiesta");

        while (iRetries-- > 0) {
            if (vncPtr->iStatus != JADA_VNCSTATUS_CLOSED) {
                JADA_Sleep(100);
            }
        }
        CORE_ReturnValIfFail(iRetries > 0, -1, ;, "Timeout nella risposta alla chiusura della richiesta");
    }

    iRet = JADA_LstGetFirst(vncPtr->lstInDrawFrames, &drwSelf, sizeof(drwSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_DrwDestroy(__FILE__, __LINE__, drwSelf);
        iRet = JADA_LstGetNext(vncPtr->lstInDrawFrames, &drwSelf, sizeof(drwSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, vncPtr->lstInDrawFrames);

    JADA_Free(__FILE__, __LINE__, vncPtr, sizeof(*vncPtr));

    return(0);
}

DllExport int __stdcall JADA_VncGetRequestId(JADA_VNC vncSelf, JADA_ReqId *piId)
{
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  

    *piId = vncPtr->iRequestId;

    return(0);
}

DllExport int __stdcall JADA_VncGetSessionId(JADA_VNC vncSelf, JADA_SessId *piId)
{
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  

    *piId = vncPtr->iSessionId;

    return(0);
}

DllExport int __stdcall JADA_VncIsConnected(JADA_VNC vncSelf)
{
    int             iIsConnected;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  

    switch (vncPtr->iStatus) {
    case JADA_VNCSTATUS_CONNECTED:
        iIsConnected = TRUE;
        break;
    default:
        iIsConnected = FALSE;
        break;
    }

    return(iIsConnected);
}

DllExport int __stdcall JADA_VncGetConnection(JADA_VNC vncSelf, JADA_Connection *connSelf)
{
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    *connSelf = (JADA_Connection) &connPtr;

    return(0);
}

DllExport int __stdcall JADA_VncOpenDesktop(JADA_VNC vncSelf)
{
    JADA_VncSelf        *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf       *connPtr;
    JADA_MsgOpenDesktop msgOpenDesktop;
    int                 iRet;
    FixString           szParams;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &vncPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &vncPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");

    iRet = JADA_ProcExists(JADA_PROC_VNCVIEWER);
    if (iRet == 0) {
        iRet = JADA_ProcKill(JADA_PROC_VNCVIEWER);
    }

    sprintf(szParams, "-proxy %s:5901 ID:%d", connPtr->szServerName, vncPtr->iRequestId);
    iRet = JADA_ProcExecute(JADA_PROC_VNCVIEWER, szParams);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    JADA_LogMessage("Ho lanciato il comando %s %s\n", JADA_PROC_VNCVIEWER, szParams);

    memset(&msgOpenDesktop, 0, sizeof(msgOpenDesktop));
    iRet = JADA_VncGetRequestId(connPtr->vncSelf, &msgOpenDesktop.iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio OPEN_DESKTOP");
    iRet = JADA_SendVncMessage(connPtr->vncSelf, JADA_MSG_OPEN_DESKTOP, &msgOpenDesktop, sizeof(msgOpenDesktop));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio OPEN_DESKTOP");

    vncPtr->iStatus = JADA_VNCSTATUS_REQUESTED;
    time(&vncPtr->timLastActivity);

    return(0);
}

DllExport int __stdcall JADA_VncAcceptDesktop(JADA_VNC vncSelf, int iAccepted)
{
    int                     iRet;
    JADA_VncSelf            *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf           *connPtr;
    JADA_MsgDesktopOpened   msgOpened;
    FixString               szParams;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &vncPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &vncPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");

    if (iAccepted == TRUE) {
        iRet = JADA_ProcExists(JADA_PROC_VNCSERVER);
        if (iRet == 0) {
            iRet = JADA_ProcKill(JADA_PROC_VNCSERVER);
            JADA_Sleep(1000);
        }

        sprintf(szParams, "-multi -id:%d -connect %s:5500 -run", vncPtr->iRequestId, connPtr->szServerName);
        iRet = JADA_ProcExecute(JADA_PROC_VNCSERVER, szParams);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        JADA_LogMessage("Ho lanciato il comando %s %s\n", JADA_PROC_VNCSERVER, szParams);
        JADA_Sleep(1000);

        //iRet = JADA_TcpConnectToPort("127.0.0.1", JADA_VNCSERVER_PORT, &connPtr->sokVNC);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        //JADA_LogMessage("Nuova connessione verso VNC, porta %d\n", JADA_VNCSERVER_PORT);

        vncPtr->iStatus = JADA_VNCSTATUS_CONNECTED;

        //pthread_attr_init(&attr);
        //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        //iRet = pthread_create(&dskPtr->pThreadId, &attr, JADA_AgentDesktopThread, dskSelf);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }
    else {
        iRet = JADA_TcpDestroySocket(&vncPtr->sokVNC);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        JADA_LogMessage("Connessione verso VNC chiusa per richiesta del manager\n");

        iRet = JADA_ProcExists(JADA_PROC_VNCSERVER);
        if (iRet == 0) {
            iRet = JADA_ProcKill(JADA_PROC_VNCSERVER);
        }
    }

    memset(&msgOpened, 0, sizeof(msgOpened));
    msgOpened.iAccepted = iAccepted;
    iRet = JADA_VncGetRequestId(vncSelf, &msgOpened.iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    time(&msgOpened.timOpenTime);
    iRet = JADA_SendVncMessage(vncSelf, JADA_MSG_DESKTOP_OPENED, &msgOpened, sizeof(msgOpened));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_VncCloseDesktop(JADA_VNC vncSelf)
{
    int                 iRet;
    JADA_VncSelf        *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf        *connPtr;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    //iRet = JADA_TcpDestroySocket(&vncPtr->sokVNC);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    //JADA_LogMessage("Connessione verso VNC chiusa per richiesta del manager\n");

    iRet = JADA_ProcExists(JADA_PROC_VNCSERVER);
    if (iRet == 0) {
        iRet = JADA_ProcKill(JADA_PROC_VNCSERVER);
    }

    return(0);
}

DllExport int __stdcall JADA_VncOpenWhiteboard(JADA_VNC vncSelf)
{
    int                     iRet;
    JADA_VncSelf            *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf            *connPtr;
    JADA_MsgOpenWhiteboard  msgPayload;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = vncPtr->iRequestId;

    iRet = JADA_SendVncMessage(vncSelf, JADA_MSG_OPEN_WHITEBOARD, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio OPEN_WHITEBOARD");

    return(0);
}

DllExport int __stdcall JADA_VncCleanWhiteboard(JADA_VNC vncSelf)
{
    int                     iRet;
    JADA_VncSelf            *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf            *connPtr;
    JADA_MsgCleanWhiteboard msgPayload;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = vncPtr->iRequestId;

    iRet = JADA_SendVncMessage(vncSelf, JADA_MSG_CLEAN_WHITEBOARD, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio CLEAN_WHITEBOARD");

    return(0);
}

DllExport int __stdcall JADA_VncCloseWhiteboard(JADA_VNC vncSelf)
{
    int                     iRet;
    JADA_VncSelf            *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf            *connPtr;
    JADA_MsgCloseWhiteboard msgPayload;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = vncPtr->iRequestId;
    msgPayload.timCloseTime = time(0);

    iRet = JADA_SendVncMessage(vncSelf, JADA_MSG_CLOSE_WHITEBOARD, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio CLOSE_WHITEBOARD");

    return(0);
}

DllExport int __stdcall JADA_VncGetNextDrawFrame(JADA_VNC vncSelf, int iMilliSecs, JADA_Frame *frmSelf)
{
    int             iRet;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_LstWait(vncPtr->lstInDrawFrames, frmSelf, sizeof(*frmSelf), iMilliSecs);
    if (iRet == 0) {
        iRet = 0;
    }

    return(iRet);
}

DllExport int __stdcall JADA_VncSendDrawFrame(JADA_VNC vncSelf, JADA_Draw drwSelf)
{
    int             iRet;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut = NULL;
    char            szFrameData[JADA_DRAW_FRAME_LENGTH];
    int             iLength;
    JADA_MsgId      iMsgId;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (connPtr->sokConn.iSocket > 0) {
        iRet = JADA_DrwSerialize(drwSelf, szFrameData, sizeof(szFrameData), &iLength);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione del draw frame message");
        JADA_ReqIdToMsgId(&vncPtr->iRequestId, &iMsgId);
        msgOut = JADA_MsgNew(__FILE__, __LINE__, JADA_MSGTYPE_DRAWFRAME, iMsgId, iLength);
        CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "errore nella creazione del draw frame message");
        iRet = JADA_MsgSetPayload(msgOut, szFrameData, iLength);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione del draw frame message");
        iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_DESKTOP);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "Errore nell'invio del draw frame message");
    }

    return(0);
}

DllExport int __stdcall JADA_VncEnqueueDrawFrame(JADA_VNC vncSelf, JADA_Draw *drwSelf)
{
    int             iRet;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  

    iRet = JADA_LstAdd(vncPtr->lstInDrawFrames, drwSelf, sizeof(*drwSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del frame draw");

    return(0);
}


DllExport int __stdcall JADA_VncSetDrawPointer(JADA_VNC vncSelf, int iPointerType, int iPointX, int iPointY)
{
    int                     iRet;
    JADA_VncSelf            *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf            *connPtr;
    JADA_MsgSetDrawPointer  msgPayload;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = vncPtr->iRequestId;
    msgPayload.iPointerType = iPointerType;
    msgPayload.iPointX = iPointX;
    msgPayload.iPointY = iPointY;

    iRet = JADA_SendVncMessage(vncSelf, JADA_MSG_SET_DRAW_POINTER, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio JADA_MSG_SET_DRAW_POINTER");

    return(0);
}

DllExport int __stdcall JADA_VncResetDrawPointer(JADA_VNC vncSelf)
{
    int                     iRet;
    JADA_VncSelf            *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf            *connPtr;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_SendVncMessage(vncSelf, JADA_MSG_RESET_DRAW_POINTER, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio JADA_MSG_RESET_DRAW_POINTER");

    return(0);
}

DllExport int __stdcall JADA_VncSendDesktopFrame(JADA_VNC vncSelf, void *szFrameData, int iLength)
{
    int             iRet;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut = NULL;
    JADA_MsgId      iMsgId;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (connPtr->sokConn.iSocket > 0) {
        // JADA_LogToFile(myWhoAmI, "C:/DesktopFrame.send", szFrameData, iLength);
        JADA_ReqIdToMsgId(&vncPtr->iRequestId, &iMsgId);
        msgOut = JADA_MsgNew(__FILE__, __LINE__, JADA_MSGTYPE_DESKTOPFRAME, iMsgId, iLength);
        CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "errore nella creazione del desktop frame message");
        iRet = JADA_MsgSetPayload(msgOut, szFrameData, iLength);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione del desktop frame message");
        iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_DESKTOP);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "Errore nell'invio del desktop frame message");
    }

    return(0);
}

int JADA_SendVncMessage(JADA_VNC vncSelf, int iOpcode, void *pPayload, int iSize)
{
    int             iRet;
    JADA_VncSelf    *vncPtr = (JADA_VncSelf *) vncSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut;

    JADA_ReturnValIfInvalidVNC(vncPtr, -1);  
    connPtr = (JADA_ConnSelf *) vncPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    /* Crea il messaggio in output */
    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), vncPtr->iSessionId, iOpcode, iSize);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iSize);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Invia il messagio in output */
    CORE_ReturnValIfFail(connPtr->sokConn.iSocket > 0, -1, ;, "Errore generico");
    iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_SESSION);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Cancella il messaggio creato */
    //iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}


