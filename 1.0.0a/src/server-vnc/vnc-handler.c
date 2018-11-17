#include "jada.h"
#include "jada_internal.h"
#include "server.h"


/******************************************************************************
 *
 *      H I G H     L E V E L     H A N D L E R S
 *
 *****************************************************************************/
/* VNC Message Handler */
int ASRV_HandleVncMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_OPEN_DESKTOP:
        iRet = ASRV_HandleOpenDesktop(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_DESKTOP_OPENED:
        iRet = ASRV_HandleDesktopOpened(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_OPEN_WHITEBOARD:
        iRet = ASRV_HandleOpenWhiteboard(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_WHITEBOARD_OPENED:
        iRet = ASRV_HandleWhiteboardOpened(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CLEAN_WHITEBOARD:
        iRet = ASRV_HandleCleanWhiteboard(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CLOSE_WHITEBOARD:
        iRet = ASRV_HandleCloseWhiteboard(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_WHITEBOARD_CLOSED:
        iRet = ASRV_HandleWhiteboardClosed(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_SET_DRAW_POINTER:
        iRet = ASRV_HandleSetDrawPointer(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_RESET_DRAW_POINTER:
        iRet = ASRV_HandleResetDrawPointer(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CLOSE_DESKTOP:
        iRet = ASRV_HandleCloseDesktop(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_DESKTOP_CLOSED:
        iRet = ASRV_HandleDesktopClosed(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    default:
        iFound = -1;
        //JADA_LogMessage("Messaggio sulla sessione VNC %d non valido (%s)\n",
        //        sesPtr->dbSess.iId, JADA_MsgOpcodeToString(msgHeader.iOpcode));
        break;
    }

    return(iFound);
}

/******************************************************************************
 *
 *      V N C     H A N D L E R S
 *
 *****************************************************************************/
int ASRV_HandleOpenDesktop(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_DbStartVnc(sesPtr->dbSess.iId);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenDesktop(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenDesktopError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleDesktopOpened(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogDesktopOpened(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogDesktopOpenedError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleCloseDesktop(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_DbStopVnc(sesPtr->dbSess.iId);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCloseDesktop(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCloseDesktopError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleDesktopClosed(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_DbStopVnc(sesPtr->dbSess.iId);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogDesktopClosed(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogDesktopClosedError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleOpenWhiteboard(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenWhiteboard(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenWhiteboardError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleWhiteboardOpened(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogWhiteboardOpened(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogWhiteboardOpenedError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleCleanWhiteboard(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCleanWhiteboard(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCleanWhiteboardError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleCloseWhiteboard(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCloseWhiteboard(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCloseWhiteboardError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleWhiteboardClosed(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogWhiteboardClosed(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogWhiteboardClosedError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleSetDrawPointer(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogSetDrawPointer(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogSetDrawPointerError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleResetDrawPointer(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgResult          msgErr;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Fai il forward del messaggio al partner di sessione */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        iRet = ASRV_TcpForwardSessionMessageToPartner(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogResetDrawPointer(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogResetDrawPointerError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}
