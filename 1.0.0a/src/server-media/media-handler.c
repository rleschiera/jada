#include "jada.h"
#include "jada_internal.h"
#include "server.h"

/******************************************************************************
 *
 *      H I G H     L E V E L     H A N D L E R S
 *
 *****************************************************************************/
/* Media Message Handler */
int ASRV_HandleMediaMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_OPEN_CALL:
        iRet = ASRV_HandleOpenCall(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CALL_OPENED:
        iRet = ASRV_HandleCallOpened(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CLOSE_CALL:
        iRet = ASRV_HandleCloseCall(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_ABORT_CALL:
        iRet = ASRV_HandleAbortCall(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    default:
        iFound = -1;
        //JADA_LogMessage("Messaggio sulla sessione media %d non valido (%s)\n",
        //        sesPtr->dbSess.iId, JADA_MsgOpcodeToString(msgHeader.iOpcode));
        break;
    }

    return(iFound);
}

/******************************************************************************
 *
 *      M E D I A     H A N D L E R S
 *
 *****************************************************************************/
int ASRV_HandleOpenCall(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogOpenCall(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenCallError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}


int ASRV_HandleCallOpened(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogCallOpened(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCallOpenedError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleCloseCall(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogCloseCall(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogCloseCallError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

int ASRV_HandleAbortCall(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
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
        ASRV_LogAbortCall(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    else {
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "Messaggio %s non inviato: la richiesta non e' in stato CONNECTED (%d)", 
                JADA_MsgOpcodeToString(msgHeader.iOpcode), sesPtr->dbReq.iStato);
        iRet = ASRV_TcpSendSessionMessageToPartner(sesPtr, iIdConnSource, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogAbortCallError(sesPtr, sesPtr->dbReq.iId, "stato non valido");
    }

    return(0);
}

