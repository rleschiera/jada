#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"

#define JADA_FORCE_FTP_SEND_AFTER       6

static int myAbortCallAfter = 30;

/******************************************************************************
 *
 *   I N T E R N A L     M E S S A G E     M A N A G E M E N T
 *
 ******************************************************************************/
static int JADA_HandleConnectionMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_CONNECTION_CONNECTED:      
        iRet = JADA_HandleConnectionConnected(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_CONNECTION_OPENED:      
        iRet = JADA_HandleConnectionOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    //case JADA_MSG_SESSION_LOADED:
    //    iRet = JADA_HandleSessionLoaded(connPtr, msgRecv);
    //    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    //    break;
    case JADA_MSG_REQUESTS_LIST:
        iRet = JADA_HandleRequestsList(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_PENDING_REQUESTS_LIST:
        iRet = JADA_HandlePendingRequestsList(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_LIST_COUNTERS:
        iRet = JADA_HandleListCounters(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_CLOSED:
        iRet = JADA_HandleSupportClosed(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_COMBO_VALUES:
        iRet = JADA_HandleComboValues(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
     default:
        JADA_LogMessage ("Comando ricevuto non valido (%d)\n", msgHeader.iOpcode);
    }

    return(0);
}

static int JADA_HandleSupportMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;
    

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_PING:
        iRet = JADA_HandlePing(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_PING_REPLY:
        iRet = JADA_HandlePingReply(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_RESULT:
        iRet = JADA_HandleResult(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_OPENED:
        iRet = JADA_HandleSupportOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_ACCEPTED:
        iRet = JADA_HandleSupportAccepted(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_CONNECTED:
        iRet = JADA_HandleSupportConnected(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_LOADED:
        iRet = JADA_HandleSupportLoaded(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_SOLICITED:
        iRet = JADA_HandleSupportSolicited(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_SUSPENDED:
        iRet = JADA_HandleSupportSuspended(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SUPPORT_RESUMED:
        iRet = JADA_HandleSupportResumed(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SEND_CHAT_TEXT:
        iRet = JADA_HandleTextChatReceived(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_PARAMETERS_LIST:
        iRet = JADA_HandleParametersList(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    default:
        iFound = -1;
        //JADA_LogMessage ("Comando ricevuto (support) non valido (%d)\n", msgHeader.iOpcode);
    }

    return(iFound);
}

static int JADA_HandleCallSessionMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_PING:
        iRet = JADA_HandlePing(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_PING_REPLY:
        iRet = JADA_HandlePingReply(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_RESULT:
        iRet = JADA_HandleResult(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_OPEN_CALL:
        iRet = JADA_HandleOpenCall(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_CALL_OPENED:
        iRet = JADA_HandleCallOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_CLOSE_CALL:
        iRet = JADA_HandleCloseCall(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_ABORT_CALL:
        iRet = JADA_HandleAbortCall(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    default:
        iFound = -1;
        //JADA_LogMessage ("Comando ricevuto (media) non valido (%d)\n", msgHeader.iOpcode);
    }

    return(iFound);
}

static int JADA_HandleVNCSessionMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_PING:
        iRet = JADA_HandlePing(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_PING_REPLY:
        iRet = JADA_HandlePingReply(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_RESULT:
        iRet = JADA_HandleResult(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_OPEN_DESKTOP:
        iRet = JADA_HandleOpenDesktop(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
   case JADA_MSG_DESKTOP_OPENED:
        iRet = JADA_HandleDesktopOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_CLOSE_DESKTOP:
        iRet = JADA_HandleCloseDesktop(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_OPEN_WHITEBOARD:
        iRet = JADA_HandleOpenWhiteboard(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_WHITEBOARD_OPENED:
        iRet = JADA_HandleWhiteboardOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_CLEAN_WHITEBOARD:
        iRet = JADA_HandleCleanWhiteboard(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_CLOSE_WHITEBOARD:
        iRet = JADA_HandleCloseWhiteboard(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_WHITEBOARD_CLOSED:
        iRet = JADA_HandleWhiteboardOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_SET_DRAW_POINTER:
        iRet = JADA_HandleSetDrawPointer(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_RESET_DRAW_POINTER:
        iRet = JADA_HandleResetDrawPointer(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    default:
        iFound = -1;
        //JADA_LogMessage ("Comando ricevuto (VNC) non valido (%d)\n", msgHeader.iOpcode);
    }

    return(iFound);
}

static int JADA_HandleFTPSessionMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;


    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_PING:
        iRet = JADA_HandlePing(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_PING_REPLY:
        iRet = JADA_HandlePingReply(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_FTP_OPENED:
        iRet = JADA_HandleFtpOpened(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_RESULT:
        iRet = JADA_HandleResult(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_LIST_DIRECTORY:
        iRet = JADA_HandleListDirectory(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_DIRECTORY_LIST:
        iRet = JADA_HandleDirectoryList(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    case JADA_MSG_GET_FILE:
        iRet = JADA_HandleGetFile(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;
    default:
        iFound = -1;
        // JADA_LogMessage ("Comando ricevuto (FTP) non valido (%d)\n", msgHeader.iOpcode);
    }

    return(iFound);
}

static int JADA_HandleSessionMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_SessionMsgHeader   msgHeader;
    
    iRet = JADA_HandleSupportMessage(connPtr, msgRecv);
    if (iRet != 0) {
        iRet = JADA_HandleCallSessionMessage(connPtr, msgRecv);
        if (iRet != 0) {
            iRet = JADA_HandleVNCSessionMessage(connPtr, msgRecv);
            if (iRet != 0) {
                iRet = JADA_HandleFTPSessionMessage(connPtr, msgRecv);
                if (iRet != 0) {
                    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
                    JADA_LogMessage ("Comando ricevuto di sessione non valido (%d)\n", msgHeader.iOpcode);
                }
            }
        }
    }

    return(iRet);
}

static int JADA_HandleCallMessage(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int              iRet;
    JADA_MsgHeader   msgHeader;


    iRet = JADA_MsgGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

	switch (msgHeader.iType) {
    case JADA_MSGTYPE_AUDIOFRAME:
        iRet = JADA_HandleAudioFrame(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;

    case JADA_MSGTYPE_VIDEOFRAME:
        iRet = JADA_HandleVideoFrame(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        break;

    default:
        JADA_LogMessage ("Tipo messaggio ricevuto non valido (%d)\n", msgHeader.iType);
    }

    return(0);
}

/******************************************************************************/
/*                                                                            */
/*   T H R E A D S                                                            */
/*                                                                            */
/******************************************************************************/
void *JADA_ConnectionReceiverThread(void *FuncArg)
{
    int                         iRet;
    fd_set                      fdSet;
    struct timeval              tv = {1, 0};
    JADA_MsgHeader              msgHeader;
    JADA_SessionMsgHeader       msgSesHeader;
    JADA_Message                msgIn;
    JADA_MsgConnectConnection   msgConnectConnection;
    JADA_Connection             connSelf = (JADA_Connection) FuncArg;
    JADA_ConnSelf               *connPtr = (JADA_ConnSelf *) FuncArg;
    struct sockaddr_in          myAddress;
    int                         iLength;
    int                         iSocketCount; //, iSocket, iSocketVNC = -1;
    ShortString                 szHostIP;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);  

    iLength = sizeof(myAddress);  
    iRet = getsockname(connPtr->sokConn.iSocket, (struct sockaddr *)&myAddress, &iLength);  
    if (iRet >= 0) {
        strcpy(szHostIP, inet_ntoa(myAddress.sin_addr));
        iRet = JADA_ConnSetParameter(connSelf, "postazione", "IP", szHostIP);
    }

    memset(&msgConnectConnection, 0, sizeof(msgConnectConnection));
    msgConnectConnection.iConnectionType = connPtr->iType;
    iRet = JADA_ConnGetParameter(connSelf, "postazione", "IP", msgConnectConnection.szHostIP, sizeof(msgConnectConnection.szHostIP));
    iRet = JADA_SendConnectionMessage((JADA_Connection) connPtr, JADA_MSG_CONNECT_CONNECTION, &msgConnectConnection, sizeof(msgConnectConnection));
    CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore nella definizione del messaggio CONNECT_CONNECTION");
    connPtr->iStatus = JADA_CONNSTATUS_CONNECTING;

    iRet = JADA_IniGetInt("call", "abortafter", &myAbortCallAfter);
    CORE_GotoIfFail(iRet == 0, lblError, ;, "errore nella lettura del parametro call/abortafter");

    while (connPtr->iReceiverThreadStatus == 1) {
        FD_ZERO(&fdSet); iSocketCount = 1;
        //if (iSocketVNC > 0) {
        //    FD_SET(iSocketVNC, &fdSet); iSocketCount++;
        //}
        FD_SET(connPtr->sokConn.iSocket, &fdSet);
        //if (connPtr->sokVNC.iSocket > 0) {
        //    FD_SET(connPtr->sokVNC.iSocket, &fdSet); iSocketCount++;
        //}

        iRet = select(iSocketCount, &fdSet, NULL, NULL, &tv);
        // JADA_LogMessage("ConnectionReceiverThread: select = %d\n", iRet);
        CORE_SilentGotoIfFail(iRet >= 0, lblError, ;);

        CORE_GotoIfFail(connPtr->iSenderThreadStatus == 1, lblError, ;, "Il sender thread e' stato terminato");

	  //  if (FD_ISSET(iSocketVNC, &fdSet)) {
   //         JADA_LogMessage("Nuova connessione dal listen socket VNC (%d).\n", iSocketVNC);
			//iSocket = accept(iSocketVNC, NULL, 0);
			//CORE_GotoIfFail(iSocket > 0, lblError, ;, "Errore generico");
   //         iRet = JADA_TcpCreateSocket(&connPtr->sokVNC, iSocket);
			//CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore generico");

   //         memset(&msgOpenDesktop, 0, sizeof(msgOpenDesktop));
   //         iRet = JADA_VncGetRequestId(connPtr->vncSelf, &msgOpenDesktop.iRequestId);
   //         CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore nell'invio del messaggio OPEN_DESKTOP");
   //         iRet = JADA_SendVncMessage(connPtr->vncSelf, JADA_MSG_OPEN_DESKTOP, &msgOpenDesktop, sizeof(msgOpenDesktop));
   //         CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore nell'invio del messaggio OPEN_DESKTOP");
   //     }
   //     if (FD_ISSET(connPtr->sokVNC.iSocket, &fdSet)) {
   //         iBytesRecv = JADA_VncReceiveDataFromSocket(connPtr->sokVNC.iSocket, szVncBuffer, sizeof(szVncBuffer));
   //         // iBytesRecv = recv(sesPtr->sokVNC.iSocket, szVncBuffer, sizeof(szVncBuffer), 0);
   //         if (iBytesRecv > 0) {
   //             // JADA_LogMessage("%d bytes disponibili dal socket VNC (%d).\n", iBytesRecv, sesPtr->sokVNC.iSocket);
   //             iRet = JADA_VncSendDesktopFrame(connPtr->vncSelf, szVncBuffer, iBytesRecv);
   //             CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
   //         }
   //         else if (iBytesRecv <= 0) {
   //             memset(&msgCloseDesktop, 0, sizeof(msgCloseDesktop));
   //             iRet = JADA_VncGetRequestId(connPtr->vncSelf, &msgCloseDesktop.iRequestId);
   //             CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore nell'invio del messaggio CLOSE_DESKTOP");
   //             iRet = JADA_SendVncMessage(connPtr->vncSelf, JADA_MSG_CLOSE_DESKTOP, &msgCloseDesktop, sizeof(msgCloseDesktop));
   //             CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore nell'invio del messaggio CLOSE_DESKTOP");
   //             iRet = JADA_TcpDestroySocket(&connPtr->sokVNC);
   //             CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
   //             JADA_LogMessage("Socket con VNC chiuso.\n");
   //         }
   //     }
	    if (FD_ISSET(connPtr->sokConn.iSocket, &fdSet)) {
            // JADA_LogMessage("Nuovo messaggio disponibile dal socket di sessione (%d).\n", connPtr->sokConn.iSocket);
            msgIn = JADA_TcpReceiveMessage(__FILE__, __LINE__, &connPtr->sokConn);
            CORE_GotoIfFail(msgIn != NULL, lblError, ;, "Errore generico");

            iRet = JADA_MsgGetHeader(msgIn, &msgHeader);
            CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
            iRet = JADA_MsgSesGetHeader(msgIn, &msgSesHeader);
            CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");

	        switch (msgHeader.iType) {
            case JADA_MSGTYPE_CONNECTION:
                iRet = JADA_HandleConnectionMessage(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            case JADA_MSGTYPE_SUPPORT:
                iRet = JADA_HandleSessionMessage(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            case JADA_MSGTYPE_AUDIOFRAME:
                iRet = JADA_HandleAudioFrame(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            case JADA_MSGTYPE_VIDEOFRAME:
                iRet = JADA_HandleVideoFrame(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            case JADA_MSGTYPE_DRAWFRAME:
                iRet = JADA_HandleDrawFrame(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            case JADA_MSGTYPE_DESKTOPFRAME:
                iRet = JADA_HandleDesktopFrame(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            case JADA_MSGTYPE_FTPFRAME:
                iRet = JADA_HandleFtpFrame(connPtr, msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, JADA_MsgDestroy(__FILE__, __LINE__, msgIn);, "Errore generico");
                break;

            default:
                JADA_LogMessage ("Tipo messaggio ricevuto non valido (%d)\n", msgHeader.iType);
            }

            iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgIn);
            CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
        } 
lblNext:
        ;
    }

lblError:
    /* Chiudi il thread di invio, ed aspetta che sia terminato */
    connPtr->iSenderThreadStatus++;
    while (connPtr->iSenderThreadStatus < 3) {
        JADA_Sleep(1000);
    }

    //if (iSocketVNC > 0) {
    //    closesocket(iSocketVNC);
    //}
    JADA_TcpDestroySocket(&connPtr->sokConn);

    iRet = JADA_RaiseEventConnectionClosed(connPtr, connPtr->iId, time(0));
    connPtr->iReceiverThreadStatus++;
    JADA_LogMessage("ConnectionReceiverThread e' stato chiuso.\n");

    return(NULL);
}

/******************************************************************************
 *
 *      S E S S I O N     S E N D E R
 *
 *****************************************************************************/
static int ProcessCallMgs(JADA_ConnSelf *connPtr)
{
    int             iRet, iResult = 0, iSendMessage = 1;
    JADA_Message    msgOut, msgTmp;
    JADA_MsgHeader  msgHeader;
    int             iCount, iRetries = 0;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstWait(connPtr->sokConn.lstOutCallMsgs, &msgOut, sizeof(msgOut), 0);
    if (iRet == 0) {

        if (connPtr->sokConn.iSocket > 0) {
            if (connPtr->iClosingCall == 0) {
                iRet = JADA_MsgGetHeader(msgOut, &msgHeader);
			    CORE_LogIfFail(iRet == 0, "Errore generico");

                switch (msgHeader.iType) {
                case JADA_MSGTYPE_AUDIOFRAME:
                    // JADA_LogMessage("AUDIO frame\n");
                    if (JADA_CallIsAudioMuted(connPtr->callSelf) == TRUE) {
                        iSendMessage = 0;
                    }
                    break;
                case JADA_MSGTYPE_VIDEOFRAME:
                    // JADA_LogMessage("VIDEO frame\n");
                    if (JADA_CallIsVideoMuted(connPtr->callSelf) == TRUE) {
                        iSendMessage = 0;
                    }
                    break;
                }

                if (iSendMessage != 0) {
                    iRet = JADA_TcpSendMessage(&connPtr->sokConn, msgOut);
			        CORE_LogIfFail(iRet == 0, "Errore generico");
                    iResult = 1;
                }
            }
            else {
                iRet = JADA_LstWait(connPtr->sokConn.lstOutCallMsgs, &msgTmp, sizeof(msgTmp), 0);
                while (iRet == 0) {
                    iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgTmp);
		            CORE_LogIfFail(iRet == 0, "Errore generico");
                    iRet = JADA_LstWait(connPtr->sokConn.lstOutCallMsgs, &msgTmp, sizeof(msgTmp), 0);
                }
                connPtr->iClosingCall = 0;
            }
        }

        iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
		CORE_LogIfFail(iRet == 0, "Errore generico");

        iCount = JADA_LstGetCount(connPtr->sokConn.lstOutCallMsgs);
        if ((iCount >= myAbortCallAfter) || (JADA_CallIsReset(connPtr->callSelf) != FALSE)) {
            iRet = JADA_LstWait(connPtr->sokConn.lstOutCallMsgs, &msgTmp, sizeof(msgTmp), 0);
            while (iRet == 0) {
                iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgTmp);
		        CORE_LogIfFail(iRet == 0, "Errore generico");
                iRet = JADA_LstWait(connPtr->sokConn.lstOutCallMsgs, &msgTmp, sizeof(msgTmp), 0);
            }

            iRetries++;
            JADA_CallReset(connPtr->callSelf, FALSE);
            if (iCount >= myAbortCallAfter) {
                iRet = JADA_CallAbort(connPtr->callSelf, iRetries);
			    CORE_LogIfFail(iRet == 0, "Errore nell'abort della chiamata.");
            }
        }
    } 

    return(iResult);
}

static int ProcessDesktopMgs(JADA_ConnSelf *connPtr)
{
    int             iRet, iResult = 0;
    JADA_Message    msgOut, msgTmp;
    int             iCount;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstWait(connPtr->sokConn.lstOutVncMsgs, &msgOut, sizeof(msgOut), 0);
    if (iRet == 0) {

        if (connPtr->sokConn.iSocket > 0) {
            if (connPtr->iClosingVnc == 0) {
                iRet = JADA_TcpSendMessage(&connPtr->sokConn, msgOut);
			    CORE_LogIfFail(iRet == 0, "Errore generico");
                iResult = 1;
            }
            else {
                iRet = JADA_LstWait(connPtr->sokConn.lstOutVncMsgs, &msgTmp, sizeof(msgTmp), 0);
                while (iRet == 0) {
                    iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgTmp);
		            CORE_LogIfFail(iRet == 0, "Errore generico");
                    iRet = JADA_LstWait(connPtr->sokConn.lstOutVncMsgs, &msgTmp, sizeof(msgTmp), 0);
                }
                connPtr->iClosingVnc;
            }
        }

        iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
		CORE_LogIfFail(iRet == 0, "Errore generico");

        iCount = JADA_LstGetCount(connPtr->sokConn.lstOutVncMsgs);
        //if (iCount >= 5)
        //    JADA_LogMessage("Messaggi DESKTOP in coda: %d\n", iCount);
    } 

    return(iResult);
}

static int ProcessFtpMgs(JADA_ConnSelf *connPtr)
{
    int             iRet, iResult = 0;
    JADA_Message    msgOut, msgTmp;
    int             iCount;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstWait(connPtr->sokConn.lstOutFtpMsgs, &msgOut, sizeof(msgOut), 0);
    if (iRet == 0) {

        if (connPtr->sokConn.iSocket > 0) {
            if (connPtr->iClosingFtp == 0) {
                iRet = JADA_TcpSendMessage(&connPtr->sokConn, msgOut);
			    CORE_LogIfFail(iRet == 0, "Errore generico");
                iResult = 1;
            }
            else {
                iRet = JADA_LstWait(connPtr->sokConn.lstOutFtpMsgs, &msgTmp, sizeof(msgTmp), 0);
                while (iRet == 0) {
                    iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgTmp);
		            CORE_LogIfFail(iRet == 0, "Errore generico");
                    iRet = JADA_LstWait(connPtr->sokConn.lstOutFtpMsgs, &msgTmp, sizeof(msgTmp), 0);
                }
                connPtr->iClosingFtp = 0;
            }
        }

        iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
		CORE_LogIfFail(iRet == 0, "Errore generico");

        iCount = JADA_LstGetCount(connPtr->sokConn.lstOutFtpMsgs);
        //if (iCount >= 20)
        //    JADA_LogMessage("Messaggi FTP in coda: %d\n", iCount);
    } 

    return(iResult);
}

static int ProcessSessionMgs(JADA_ConnSelf *connPtr)
{
    int             iRet, iResult = 0;
    JADA_Message    msgOut;
    int             iCount;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstWait(connPtr->sokConn.lstOutSupportMsgs, &msgOut, sizeof(msgOut), 0);
    if (iRet == 0) {
        if (connPtr->sokConn.iSocket > 0) {
            iRet = JADA_TcpSendMessage(&connPtr->sokConn, msgOut);
			CORE_LogIfFail(iRet == 0, "Errore generico");
            iResult = 1;
        }

        iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
		CORE_LogIfFail(iRet == 0, "Errore generico");

        iCount = JADA_LstGetCount(connPtr->sokConn.lstOutSupportMsgs);
        //if (iCount >= 1)
        //    JADA_LogMessage("Messaggi SESSION in coda: %d\n", iCount);
    } 

    return(iResult);
}

void *JADA_ConnectionSenderThread(void *FuncArg)
{
    int             iRet, iCallSent, iDesktopSent, iFtpSent, iSessionSent, iForceFtpSend = JADA_FORCE_FTP_SEND_AFTER;
    int             iCallMsgs = 0, iDesktopMsgs = 0, iFtpMsgs = 0, iSessionMsgs = 0;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) FuncArg;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);  

    while (connPtr->iSenderThreadStatus == 1) {
        iRet = JADA_TcpWaitEnqueuedMessage(&connPtr->sokConn, 5000);
        if (iRet > 0) {
            iSessionSent = ProcessSessionMgs(connPtr);
            if (iSessionSent > 0) {
                iSessionMsgs++;
            }

            iDesktopSent = ProcessDesktopMgs(connPtr);
            if (iDesktopSent > 0) {
                iDesktopMsgs++;
            }

            iCallSent = ProcessCallMgs(connPtr);
            if (iCallSent > 0) {
                iCallMsgs++;
            }

            if ((iCallSent == 0) && (iDesktopSent == 0) && (iSessionSent == 0)) {
                iForceFtpSend = JADA_FORCE_FTP_SEND_AFTER;
                iFtpSent = ProcessFtpMgs(connPtr);
                if (iFtpSent > 0) {
                    iFtpMsgs++;
                }
            }
            else {
                iForceFtpSend--;
                if (iForceFtpSend == 0) {
                    iForceFtpSend = JADA_FORCE_FTP_SEND_AFTER;
                    iFtpSent = ProcessFtpMgs(connPtr);
                    if (iFtpSent > 0) {
                        iFtpMsgs++;
                    }
                }
            }
        }
        else {
            // JADA_LogMessage("ConnectionSenderThread: JADA_TcpWaitEnqueuedMessage = %d\n", iRet);
        }
    }

    connPtr->iSenderThreadStatus++;
    JADA_LogMessage("ConnectionSenderThread e' stato chiuso.\n");

    return(NULL);
}

/******************************************************************************
 *
 *      F T P     S E N D E R
 *
 *****************************************************************************/
static int JADA_RaiseEventFileSent(JADA_Connection connSelf, char *szFileName, int iCurrFrame, int iFrames)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_FILE_SENT);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    strcpy(evtOut.szFileName, szFileName);
    evtOut.iCurrFrame = iCurrFrame;
    evtOut.iFrames = iFrames;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent(connSelf, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(FILE_SENT)");

    return(0);
}

void *JADA_FtpSenderThread(void *FuncArg)
{
    int                 iRet, iSize, i;
    JADA_MsgFtpFrame    msgPayload;
    JADA_Connection     connSelf = (JADA_Connection) FuncArg;
    JADA_ConnSelf       *connPtr = (JADA_ConnSelf *) FuncArg;
    JADA_FtpSelf        *ftpPtr;
    JADA_FileTransfer   xmitSelf;


    JADA_ReturnValIfInvalidConnection(connPtr, NULL);  
    ftpPtr = (JADA_FtpSelf *) connPtr->ftpSelf;
    if (JADA_IsValidFTP(ftpPtr)) {  

        while (connPtr->iSenderThreadStatus == 1) {
            iRet = JADA_LstWait(ftpPtr->lstTransfer, &xmitSelf, sizeof(xmitSelf), 10000);
            if (iRet == 0) {

                /* Apri il file da spedire */
                xmitSelf.fp = fopen(xmitSelf.szFileFrom, "rb");
			    CORE_GotoIfFail(xmitSelf.fp != NULL, lblNext, ;, "Il file da inviare non e' stato trovato.");

                /* Calcola quanti frame devono essere inviati */
                iRet = fseek(xmitSelf.fp, 0L, SEEK_END);
                iSize = ftell(xmitSelf.fp);
                xmitSelf.iFrames = (iSize / JADA_FTP_FRAME_LENGTH) + 1;
                iRet = fseek(xmitSelf.fp, 0L, SEEK_SET);

                /* Prepara la struttura frame */
                memset(&msgPayload, 0, sizeof(msgPayload));
                strcpy(msgPayload.ftpInfo.szFileName, xmitSelf.szFileTo);
                msgPayload.ftpInfo.iFrames = xmitSelf.iFrames;

                /* Per ciascuno dei frames previsti in invio... */
                for (i = 0; i < xmitSelf.iFrames; i++) {

                    /* Prepara il frame da inviare */
                    xmitSelf.iCurrFrame = i + 1;
                    msgPayload.ftpInfo.iCurrFrame = xmitSelf.iCurrFrame;
                    msgPayload.ftpInfo.iLength = fread(msgPayload.szPayload, 1, sizeof(msgPayload.szPayload), xmitSelf.fp);
			        CORE_GotoIfFail(msgPayload.ftpInfo.iLength > 0, lblNext, ;, "Errore nella lettura del frame FTP.");

                    /* Accoda il frame in invio */
                    iRet = JADA_FtpSendFtpFrame(connPtr->ftpSelf, &msgPayload, sizeof(JADA_MsgFtpInfo) + msgPayload.ftpInfo.iLength);
			        CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore nell'invio del frame FTP.");

                    /* Attendi 200ms per non saturare il canale di invio */
                    iRet = JADA_RaiseEventFileSent(connSelf, msgPayload.ftpInfo.szFileName, 
                                    msgPayload.ftpInfo.iCurrFrame, msgPayload.ftpInfo.iFrames);
			        CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore nell'invio del frame FTP.");
                    JADA_Sleep(200);
                }
            }

    lblNext:
            ;
        }
    }

    connPtr->iSenderThreadStatus++;
    JADA_LogMessage("FtpSenderThread e' stato chiuso.\n");

    return(NULL);
}
