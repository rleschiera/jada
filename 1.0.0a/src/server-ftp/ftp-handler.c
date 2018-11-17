#include "jada.h"
#include "jada_internal.h"
#include "server.h"


/******************************************************************************
 *
 *      H I G H     L E V E L     H A N D L E R S
 *
 *****************************************************************************/
/* FTP Message Handler */
int ASRV_HandleFtpMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_OPEN_FTP:
        iRet = ASRV_HandleOpenFtp(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_FTP_OPENED:
        iRet = ASRV_HandleFtpOpened(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_LIST_DIRECTORY:
        iRet = ASRV_HandleListDirectory(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_DIRECTORY_LIST:
        iRet = ASRV_HandleDirectoryList(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_GET_FILE:
        iRet = ASRV_HandleGetFile(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CLOSE_FTP:
        iRet = ASRV_HandleCloseFtp(sesPtr, iIdConnSource, msgRecv);
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
 *      F T P     H A N D L E R S
 *
 *****************************************************************************/
int ASRV_HandleOpenFtp(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogOpenFtp(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenFtpError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleFtpOpened(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogFtpOpened(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogFtpOpenedError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleCloseFtp(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogCloseFtp(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCloseFtpError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleListDirectory(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogListDirectory(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogListDirectoryError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleDirectoryList(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogDirectoryList(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogDirectoryListError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleGetFile(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogGetFile(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogGetFileError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

