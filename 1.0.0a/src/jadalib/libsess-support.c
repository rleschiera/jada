#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"


DllExport JADA_Support __stdcall JADA_SuppNew(char *szFile, int iLine, JADA_Connection connSelf)
{
    JADA_SuppSelf   *suppPtr;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);

    suppPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_SuppSelf));
    CORE_ReturnValIfFail(suppPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di supporto");

    JADA_ClearJadaId(&suppPtr->iSessionId);
    suppPtr->iStatus = JADA_SUPPSTATUS_AVAILABLE;
    suppPtr->connSelf = connSelf;
    time(&suppPtr->timLastActivity);
    suppPtr->iMagic = JADA_MAGIC_SUPPORT;

    connPtr->suppSelf = (JADA_Support) suppPtr;

    return((JADA_Support) suppPtr);
}

DllExport int __stdcall JADA_SuppDestroy(char *szFile, int iLine, JADA_Support suppSelf)
{
    int             iRet, iRetries = 100;
    JADA_SuppSelf   *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    switch (suppPtr->iStatus) {
    case JADA_SUPPSTATUS_CLOSING:
    case JADA_SUPPSTATUS_CLOSED:
        break;
    default:
        iRet = JADA_SuppClose(suppSelf);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella chiusura del supporto");
        break;
    }

    while (--iRetries > 0) {
        if (suppPtr->iStatus != JADA_SUPPSTATUS_CLOSED)
            JADA_Sleep(100);
        else
            break;
    }
    if (iRetries <= 0) {
        JADA_LogMessage("Timeout nella risposta alla chiusura del supporto");
    }

    JADA_Free(__FILE__, __LINE__, suppPtr, sizeof(*suppPtr));

    return(0);
}

DllExport int __stdcall JADA_SuppSetSessionId(JADA_Support suppSelf, JADA_SessId iId)
{
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    suppPtr->iSessionId = iId;

    return(0);
}

DllExport int __stdcall JADA_SuppGetRequestId(JADA_Support suppSelf, JADA_ReqId *piId)
{
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    *piId = suppPtr->iRequestId;

    return(0);
}

DllExport int __stdcall JADA_SuppGetSessionId(JADA_Support suppSelf, JADA_SessId *piId)
{
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    *piId = suppPtr->iSessionId;

    return(0);
}

DllExport int __stdcall JADA_SuppOpen(JADA_Support suppSelf, int iWaitSeconds)
{
    int                 iRet, iResult = -1;
    JADA_SuppSelf       *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf       *connPtr;
    JADA_MsgOpenRequest msgPayload;
    JADA_Event          evtSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;
    /* TODO: controllo di stato */

    memset(&msgPayload, 0, sizeof(msgPayload));
    iRet = JADA_ConnGetParameter(suppPtr->connSelf, "richiesta", "coda", msgPayload.szCoda, sizeof(msgPayload.szCoda));
    iRet = JADA_ConnGetParameter(suppPtr->connSelf, "richiesta", "descrizione", msgPayload.szDescription, sizeof(msgPayload.szDescription));

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnSubscribeEvent(suppPtr->connSelf, JADA_EVT_SUPPORT_OPENED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_OPEN_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio OPEN_SUPPORT");
    suppPtr->iStatus = JADA_SUPPSTATUS_OPENING;

    iRet = JADA_ConnCopyParameterValue((JADA_Connection)connPtr, "connessione", "cliente", "richiesta", "cliente");
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_ConnCopyParameterValue((JADA_Connection)connPtr, "connessione", "telefono", "richiesta", "telefono");
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_ConnCopyParameterValue((JADA_Connection)connPtr, "connessione", "cellulare", "richiesta", "cellulare");
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_ConnCopyParameterValue((JADA_Connection)connPtr, "connessione", "mail", "richiesta", "mail");
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_ConnCopyParameterValue((JADA_Connection)connPtr, "connessione", "audio", "richiesta", "audio");
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_SendParametersToServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(suppPtr->connSelf, JADA_EVT_SUPPORT_OPENED, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetResult(evtSelf, &iResult);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(suppPtr->connSelf, JADA_EVT_SUPPORT_OPENED);, "Errore generico");
        }

        iRet = JADA_ConnUnsubscribeEvent(suppPtr->connSelf, JADA_EVT_SUPPORT_OPENED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    return(iResult);
}

DllExport int __stdcall JADA_SuppConnect(JADA_Support suppSelf, JADA_ReqId iRequestId)
{
    int                     iRet;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf           *connPtr;
    JADA_MsgConnectRequest  msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    /* TODO: controllo di stato */

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = iRequestId;

    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_CONNECT_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio CONNECT_SUPPORT");
    suppPtr->iStatus = JADA_SUPPSTATUS_CONNECTING;

    return(0);
}

DllExport int __stdcall JADA_SuppAccept(JADA_Support suppSelf, JADA_ReqId iRequestId)
{
    int                     iRet;
    JADA_SuppSelf            *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf            *connPtr;
    JADA_MsgAcceptRequest   msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    /* TODO: controllo di stato */

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = iRequestId;

    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_ACCEPT_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio ACCEPT_SUPPORT");
    suppPtr->iStatus = JADA_SUPPSTATUS_OPENING;

    return(0);
}

DllExport int __stdcall JADA_SuppLoad(JADA_Support suppSelf, JADA_ReqId iRequestId)
{
    int                 iRet;
    JADA_SuppSelf        *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf        *connPtr;
    JADA_MsgLoadRequest msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    /* TODO: controllo di stato */

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = iRequestId;

    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_LOAD_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio LOAD_SUPPORT");
    suppPtr->iStatus = JADA_SUPPSTATUS_LOADING;

    return(0);
}

DllExport int __stdcall JADA_SuppResume(JADA_Support suppSelf, JADA_ReqId iRequestId)
{
    int                   iRet;
    JADA_SuppSelf          *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf          *connPtr;
    JADA_MsgResumeRequest msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    /* TODO: controllo di stato */

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = iRequestId;

    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_RESUME_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio RESUME_SUPPORT");
    suppPtr->iStatus = JADA_SUPPSTATUS_RESUMING;

    return(0);
}

DllExport int __stdcall JADA_SuppSolicit(JADA_Support suppSelf)
{
    int                     iRet;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf           *connPtr;
    JADA_MsgSolicitRequest  msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    /* TODO: controllo di stato */

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = suppPtr->iRequestId;

    iRet = JADA_SendSupportMessage(suppSelf, JADA_MSG_SOLICIT_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio SOLICIT_SUPPORT");

    return(0);
}

DllExport int __stdcall JADA_SuppClose(JADA_Support suppSelf)
{
    int                   iRet;
    JADA_SuppSelf         *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf         *connPtr;
    JADA_MsgCloseRequest  msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    /* TODO: controllo di stato */

    iRet = JADA_IniSetInt("richiesta", "id", -1);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scruttura del parametro richiesta/id nel file .ini");

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = suppPtr->iRequestId;

    iRet = JADA_SendSupportMessage(suppSelf, JADA_MSG_CLOSE_SUPPORT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio CLOSE_SUPPORT");
    suppPtr->iStatus = JADA_SUPPSTATUS_CLOSING;

    return(0);
}

DllExport int __stdcall JADA_SuppIsConnected(JADA_Support suppSelf)
{
    int             iIsConnected;
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    switch (suppPtr->iStatus) {
    case JADA_SUPPSTATUS_OPENED:
    case JADA_SUPPSTATUS_CONNECTED:
    case JADA_SUPPSTATUS_LOADED:
        iIsConnected = TRUE;
        break;
    default:
        iIsConnected = FALSE;
        break;
    }

    return(iIsConnected);
}

DllExport int __stdcall JADA_SuppSetStatus(JADA_Support suppSelf, int iStatus)
{
    int             iRet;
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    switch (iStatus) {
    case JADA_SUPPSTATUS_OPENED:
    case JADA_SUPPSTATUS_REQUESTED:
    case JADA_SUPPSTATUS_CONNECTED:
    case JADA_SUPPSTATUS_LOADED:
    case JADA_SUPPSTATUS_SUSPENDED:
        suppPtr->iStatus = iStatus;
        iRet = JADA_IniSetJadaId("richiesta", "id", &suppPtr->iRequestId);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del parametro richiesta/id nel file .ini");
        break;
    case JADA_SUPPSTATUS_CLOSED:
        suppPtr->iStatus = iStatus;
        iRet = JADA_IniSetJadaId("richiesta", "id", NULL);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del parametro richiesta/id nel file .ini");
        break;
    default:
        CORE_ReturnValIfFail(0, -1, ;, "Stato non valido nella ReqSetStatus");
    }

    return(0);
}

DllExport int __stdcall JADA_SuppPing(JADA_Support suppSelf)
{
    int             iRet;
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf    *connPtr;
    JADA_MsgPing    msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = suppPtr->iRequestId;
    _ftime(&msgPayload.timPingTime);

    iRet = JADA_SendSupportMessage(suppSelf, JADA_MSG_PING, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio PING");

    return(0);
}

DllExport int __stdcall JADA_SuppGetConnection(JADA_Support suppSelf, JADA_Connection *connSelf)
{
    JADA_SuppSelf    *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    *connSelf = (JADA_Connection) &connPtr;

    return(0);
}

DllExport int __stdcall JADA_SuppSendChat(JADA_Support suppSelf, char *szText)
{
    int                 iRet;
    JADA_SuppSelf        *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf        *connPtr;
    JADA_MsgChatText    msgPayload;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = suppPtr->iRequestId;
    msgPayload.iWhoAmI = JADA_GetWhoAmI();
    strcpy(msgPayload.szText, szText);

    iRet = JADA_SendSupportMessage(suppSelf, JADA_MSG_SEND_CHAT_TEXT, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio SEND_CHAT");

    return(0);
}

DllExport int __stdcall JADA_SuppListParameters(JADA_Support suppSelf, int *piStreamId, int iWaitSeconds)
{
    int                     iRet, iResult = -1;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf           *connPtr;
    JADA_MsgListParameters  outMsg;
    JADA_Event              evtSelf;
    JADA_EventData          evtParams;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;
    /* TODO: controllo di stato */

    memset(&outMsg, 0, sizeof(outMsg));
    outMsg.iRequestId = suppPtr->iRequestId; 

    iRet = JADA_ConnSubscribeEvent(suppPtr->connSelf, JADA_EVT_PARAMETERS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_SendSupportMessage(suppSelf, JADA_MSG_LIST_PARAMETERS, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(suppPtr->connSelf, JADA_EVT_PARAMETERS_LIST);, "Errore nell'invio del messaggio LIST_PARAMETERS");

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(suppPtr->connSelf, JADA_EVT_PARAMETERS_LIST, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetData(evtSelf, &evtParams);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(suppPtr->connSelf, JADA_EVT_PARAMETERS_LIST);, "Errore generico");
            *piStreamId = evtParams.iStreamId; 
            iResult = 0;
        }
    }

    iRet = JADA_ConnUnsubscribeEvent(suppPtr->connSelf, JADA_EVT_PARAMETERS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iResult);
}

DllExport int __stdcall JADA_SuppGetSuspendedRequestId(JADA_Support suppSelf, int *piId)
{
    int             iRet, iSuspendedId;
    JADA_SuppSelf   *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    iRet = JADA_IniGetInt("richiesta", "id", &iSuspendedId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del parametro richiesta/id dal file .ini");
    *piId = iSuspendedId;

    return(0);
}

DllExport int __stdcall JADA_SuppSetSuspendedRequestId(JADA_Support suppSelf, int iSuspendedId)
{
    int             iRet;
    JADA_SuppSelf   *suppPtr = (JADA_SuppSelf *) suppSelf;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  

    iRet = JADA_IniSetInt("richiesta", "id", iSuspendedId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scruttura del parametro richiesta/id nel file .ini");

    return(0);
}


//DllExport int __stdcall JADA_SuppRequestService(JADA_Support suppSelf, int iType)
//{
//    int                     iRet, iResult = -1;
//    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) suppSelf;
//    JADA_ConnSelf           *connPtr;
//    JADA_MsgRequestService  msgPayload;
//
//    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
//    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
//    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
//    /* TODO: controllo di stato */
//
//    memset(&msgPayload, 0, sizeof(msgPayload));
//    msgPayload.iType = iType;
//    msgPayload.iRequestId = suppPtr->iRequestId;
//    time(&msgPayload.timRequestTime);
//
//    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_REQUEST_SERVICE, &msgPayload, sizeof(msgPayload));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio REQUEST_SERVICE");
//
//    return(0);
//}
//
//DllExport int __stdcall JADA_SuppAcceptService(JADA_Support suppSelf, int iType, int iAccepted)
//{
//    int                     iRet, iResult = -1;
//    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) suppSelf;
//    JADA_ConnSelf           *connPtr;
//    JADA_MsgAcceptService   msgPayload;
//
//    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
//    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
//    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
//    /* TODO: controllo di stato */
//
//    memset(&msgPayload, 0, sizeof(msgPayload));
//    msgPayload.iType = iType;
//    msgPayload.iRequestId = suppPtr->iRequestId;
//    msgPayload.iAccepted  = iAccepted;
//    time(&msgPayload.timAcceptTime);
//
//    iRet = JADA_SendConnectionMessage(suppPtr->connSelf, JADA_MSG_ACCEPT_SERVICE, &msgPayload, sizeof(msgPayload));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio ACCEPT_SERVICE");
//
//    return(0);
//}
//
int JADA_SendSupportMessage(JADA_Support suppSelf, int iOpcode, void *pPayload, int iSize)
{
    int             iRet;
    JADA_SuppSelf   *suppPtr = (JADA_SuppSelf *) suppSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut;

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    connPtr = (JADA_ConnSelf *) suppPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    /* Crea il messaggio in output */
    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), suppPtr->iSessionId, iOpcode, iSize);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iSize);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Invia il messagio in output */
    CORE_ReturnValIfFail(connPtr->sokConn.iSocket > 0, -1, ;, "Errore generico");
    iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_SESSION);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

