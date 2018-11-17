#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"


//static int JADA_StartDesktopServer(JADA_Request reqSelf)
//{
//    int                 iRet;
//    JADA_ReqSelf        *reqPtr = (JADA_ReqSelf *) reqSelf;
//
//    reqPtr->dskSelf = JADA_DskNew(__FILE__, __LINE__, reqSelf);
//    CORE_ReturnValIfFail(reqPtr->dskSelf != NULL, -1, ;, "Errore generico");
//
//    iRet = JADA_DskAccept(reqPtr->dskSelf);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    return(0);
//}
//
static int JADA_StopDesktopServer(JADA_ConnSelf *connPtr)
{
    int     iRet;


    iRet = JADA_VncCloseDesktop(connPtr->vncSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

static int JADA_RaiseEventResult(JADA_ConnSelf *connPtr, int iResult, char *szDescription)
{
    int                 iRet;
    JADA_Event          evtSelf;
    JADA_EventData      evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_MSG_RESULT);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iResult = iResult;
    strcpy(evtOut.szDescription, szDescription);
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(MSG_RESULT)");

    return(0);
}

static int JADA_RaiseEventConnectionConnected(JADA_ConnSelf *connPtr, int iResult, JADA_ConnId iId, JADA_Timet timTime)
{
    int                     iRet;
    JADA_Event              evtSelf;
    JADA_EventData          evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CONNECTION_CONNECTED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iResult = iResult;
    evtOut.iConnectionId = iId;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CONNECTION_CONNECTED)");

    return(0);
}

static int JADA_RaiseEventConnectionOpened(JADA_ConnSelf *connPtr, int iResult, JADA_ConnId iId, JADA_Timet timTime)
{
    int                     iRet;
    JADA_Event              evtSelf;
    JADA_EventData          evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CONNECTION_OPENED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iResult = iResult;
    evtOut.iConnectionId = iId;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CONNECTION_OPENED)");

    return(0);
}

int JADA_RaiseEventConnectionClosed(JADA_ConnSelf *connPtr, JADA_ConnId iId, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CONNECTION_CLOSED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iConnectionId = iId;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CONNECTION_CLOSED)");

    return(0);
}

static int JADA_RaiseEventSupportOpened(JADA_ConnSelf *connPtr, JADA_ReqId iId, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_OPENED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iId;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_OPENED)");

    return(0);
}

static int JADA_RaiseEventSupportAccepted(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, char *szUtente, char *szTelefono, 
                char *szMail, char *szCliente, int iUsaAudio, char *szArea, char *szDescription, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;
    ShortString     szUsaAudio;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_ACCEPTED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    strcpy(evtOut.szUtente, szUtente);
    evtOut.iUsaAudio = iUsaAudio;
    strcpy(evtOut.szArea, szArea);
    strcpy(evtOut.szDescription, szDescription);
    strcpy(szUsaAudio, CORE_Evaluate(iUsaAudio != 0, "usa", "NON usa"));
    sprintf(evtOut.szChatText, "(telefono %s, mail %s, cliente %s, %s audio)", szTelefono, szMail, szCliente, szUsaAudio);
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_ACCEPTED)");

    return(0);
}

static int JADA_RaiseEventSupportResumed(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, int iStatus, char *szUtente, char *szTelefono, 
                char *szMail, char *szCliente, int iUsaAudio, char *szArea, char *szDescription, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;
    ShortString     szUsaAudio;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_RESUMED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    evtOut.iResult = CORE_Evaluate(iStatus == JADA_SUPPSTATUS_CONNECTED, 1, 0);
    strcpy(evtOut.szUtente, szUtente);
    evtOut.iUsaAudio = iUsaAudio;
    strcpy(evtOut.szArea, szArea);
    strcpy(evtOut.szDescription, szDescription);
    strcpy(szUsaAudio, CORE_Evaluate(iUsaAudio != 0, "usa", "NON usa"));
    if (iStatus == JADA_SUPPSTATUS_CONNECTED) {
        sprintf(evtOut.szChatText, "(telefono %s, mail %s, cliente %s, %s audio)", szTelefono, szMail, szCliente, szUsaAudio);
    }
    else {
        sprintf(evtOut.szChatText, "(telefono %s, mail %s, cliente %s)", szTelefono, szMail, szCliente);
    }
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_RESUMED)");

    return(0);
}

static int JADA_RaiseEventSupportConnected(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, 
                char *szUtente, int iUsaAudio, char *szArea, char *szDescription, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_CONNECTED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    strcpy(evtOut.szUtente, szUtente);
    evtOut.iUsaAudio = iUsaAudio;
    strcpy(evtOut.szArea, szArea);
    strcpy(evtOut.szDescription, szDescription);
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_CONNECTED)");

    return(0);
}

static int JADA_RaiseEventSupportLoaded(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, char *szUtente, char *szArea, char *szDescription, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_LOADED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    strcpy(evtOut.szUtente, szUtente);
    strcpy(evtOut.szArea, szArea);
    strcpy(evtOut.szDescription, szDescription);
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_LOADED)");

    return(0);
}

static int JADA_RaiseEventSupportSolicited(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_SOLICITED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_SOLICITED)");

    return(0);
}

static int JADA_RaiseEventSupportClosed(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_CLOSED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_CLOSED)");

    return(0);
}

static int JADA_RaiseEventSupportSuspended(JADA_ConnSelf *connPtr, JADA_ReqId iRequestId, char *szUtente, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SUPPORT_SUSPENDED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iRequestId;
    strcpy(evtOut.szUtente, szUtente);
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUEST_SUSPENDED)");

    return(0);
}

static int JADA_RaiseEventChatTextReceived(JADA_ConnSelf *connPtr, int iWhoAmI, char *szText)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CHATTEXT_RECEIVED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iWhoAmI = iWhoAmI;
    strcpy(evtOut.szChatText, szText);
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CHATTEXT_RECEIVED)");

    return(0);
}

//static int JADA_RaiseEventServiceRequested(JADA_ConnSelf *connPtr, int iType, JADA_Timet timTime)
//{
//    int             iRet;
//    JADA_Event      evtSelf;
//    JADA_EventData  evtOut;
//
//    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SERVICE_REQUESTED);
//    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");
//
//    memset(&evtOut, 0, sizeof(evtOut));
//    evtOut.iItemType = iType;
//    evtOut.timTime = timTime;
//    iRet = JADA_EvtSetData(evtSelf, &evtOut);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");
//
//    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(SERVICE_REQUESTED)");
//
//    return(0);
//}
//
//static int JADA_RaiseEventServiceAccepted(JADA_ConnSelf *connPtr, int iType, int iAccepted, JADA_Timet timTime)
//{
//    int             iRet;
//    JADA_Event      evtSelf;
//    JADA_EventData  evtOut;
//
//    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SERVICE_ACCEPTED);
//    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");
//
//    memset(&evtOut, 0, sizeof(evtOut));
//    evtOut.iItemType = iType;
//    evtOut.iResult = iAccepted;
//    evtOut.timTime = timTime;
//    iRet = JADA_EvtSetData(evtSelf, &evtOut);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");
//
//    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(SERVICE_ACCEPTED)");
//
//    return(0);
//}
//
static int JADA_RaiseEventCallOpened(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    iRet = JADA_CallMuteAudio(connPtr->callSelf, FALSE);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "non posso togliere il MUTE all'audio");
    iRet = JADA_CallMuteVideo(connPtr->callSelf, FALSE);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "non posso togliere il MUTE al video");

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CALL_OPENED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CALL_OPENED)");

    return(0);
}

static int JADA_RaiseEventCallClosed(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CALL_CLOSED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CALL_CLOSED)");

    return(0);
}

static int JADA_RaiseEventCallAborted(JADA_ConnSelf *connPtr, int iRetries, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_CALL_ABORTED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iTotalRequests = iRetries;
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(CALL_ABORTED)");

    return(0);
}

static int JADA_RaiseEventDesktopOpened(JADA_ConnSelf *connPtr, char *szPassword, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_DESKTOP_OPENED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    strcpy(evtOut.szPassword, szPassword);
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(DESKTOP_OPENED)");

    return(0);
}

static int JADA_RaiseEventDesktopRequested(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_DESKTOP_REQUESTED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(DESKTOP_REQUESTED)");

    return(0);
}

static int JADA_RaiseEventDesktopClosed(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_DESKTOP_CLOSED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(DESKTOP_CLOSED)");

    return(0);
}

static int JADA_RaiseEventWhiteboardOpened(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_WHITEBOARD_OPENED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(WHITEBOARD_OPENED)");

    return(0);
}

static int JADA_RaiseEventWhiteboardCleaned(JADA_ConnSelf *connPtr)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_WHITEBOARD_CLEANED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(WHITEBOARD_CLEANED)");

    return(0);
}

static int JADA_RaiseEventWhiteboardClosed(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_WHITEBOARD_CLOSED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(WHITEBOARD_CLOSED)");

    return(0);
}

static int JADA_RaiseEventSetDrawPointer(JADA_ConnSelf *connPtr, int iPointerType, int iPointX, int iPointY)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_SET_DRAW_POINTER);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iItemType = iPointerType;
    evtOut.iPointX = iPointX;
    evtOut.iPointY = iPointY;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(WHITEBOARD_CLOSED)");

    return(0);
}

static int JADA_RaiseEventResetDrawPointer(JADA_ConnSelf *connPtr)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_RESET_DRAW_POINTER);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(RESET_DRAW_POINTER)");

    return(0);
}

static int JADA_RaiseEventRequestsList(JADA_ConnSelf *connPtr, int iListType, JADA_Stream stmSelf)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));

    iRet = JADA_StmGetId(stmSelf, &evtOut.iStreamId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(REQUESTS_LIST)");

    return(0);
}

static int JADA_RaiseEventParametersList(JADA_ConnSelf *connPtr, JADA_Stream stmSelf)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_PARAMETERS_LIST);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    iRet = JADA_StmGetId(stmSelf, &evtOut.iStreamId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(PARAMETERS_LIST)");

    return(0);
}

static int JADA_RaiseEventComboValues(JADA_ConnSelf *connPtr, JADA_Stream stmSelf)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_COMBO_VALUES);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    iRet = JADA_StmGetId(stmSelf, &evtOut.iStreamId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(COMBO_VALUES)");

    return(0);
}

static int JADA_RaiseEventDirectoryList(JADA_ConnSelf *connPtr, char *szFileName)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_DIRECTORY_LIST);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    strcpy(evtOut.szFileName, szFileName);
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(DIRECTORY_LIST)");

    return(0);
}

static int JADA_RaiseEventListCounters(JADA_ConnSelf *connPtr, int iAreaRequests, int iTotalRequests, 
                                       int iAreaPendingRequests, int iTotalPendingRequests)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_LIST_COUNTERS);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iAreaRequests = iAreaRequests;
    evtOut.iTotalRequests = iTotalRequests;
    evtOut.iAreaPendingRequests = iAreaPendingRequests;
    evtOut.iTotalPendingRequests = iTotalPendingRequests;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(LIST_COUNTERS)");

    return(0);
}

static int JADA_RaiseEventFtpOpened(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_FTP_OPENED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(FTP_OPENED)");

    return(0);
}

static int JADA_RaiseEventFtpClosed(JADA_ConnSelf *connPtr, JADA_Timet timTime)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_FTP_CLOSED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.timTime = timTime;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(FTP_CLOSED)");

    return(0);
}

static int JADA_RaiseEventFileReceived(JADA_ConnSelf *connPtr, char *szFileName, int iCurrFrame, int iFrames)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_FILE_RECEIVED);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    strcpy(evtOut.szFileName, szFileName);
    evtOut.iCurrFrame = iCurrFrame;
    evtOut.iFrames = iFrames;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(FILE_RECEIVED)");

    return(0);
}

static int JADA_RaiseEventPingReply(JADA_ConnSelf *connPtr, JADA_ReqId iId, int iDelay, int iOffset)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_EventData  evtOut;

    evtSelf = JADA_EvtNew(__FILE__, __LINE__, JADA_EVT_PING_REPLY);
    CORE_ReturnValIfFail(evtSelf != NULL, -1, ;, "errore nella lettura del payload");

    memset(&evtOut, 0, sizeof(evtOut));
    evtOut.iRequestId = iId;
    evtOut.iPointX = iDelay;
    evtOut.iPointY = iOffset;
    iRet = JADA_EvtSetData(evtSelf, &evtOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del payload dell'evento");

    iRet = JADA_ConnRaiseEvent((JADA_Connection) connPtr, evtSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_ConnRaiseEvent(PING_REPLY)");

    return(0);
}

/******************************************************************************
 *
 *     O P E N     C O N N E C T I O N
 *
 *****************************************************************************/
int JADA_SendParametersToServer(JADA_ConnSelf *connPtr)
{
    int                         iRet, i;
    JADA_MsgParametersList      msgOut;
    JADA_Parameter              parSelf;
    CORE_Currency               curP;

    memset(&msgOut, 0, sizeof(msgOut));
    iRet = JADA_LstGetFirst(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    while (iRet == 0) {
        if (parSelf.iUpdated == TRUE) {
            i = msgOut.iParameters;
            strcpy(msgOut.lstParameters[i].szSection, parSelf.szSection); 
            strcpy(msgOut.lstParameters[i].szName, parSelf.szName); 
            strcpy(msgOut.lstParameters[i].szValue, parSelf.szValue); 
            parSelf.iUpdated = FALSE;
            iRet = JADA_LstUpdate(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);

            msgOut.iParameters++;
            if (msgOut.iParameters >= JADA_MAX_PARAMETERS_IN_LIST) {
                break;
            }
        }

        iRet = JADA_LstGetNext(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    }

    iRet = JADA_SendConnectionMessage((JADA_Connection) connPtr, JADA_MSG_CONN_PARAMETERS_LIST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int JADA_HandleConnectionConnected(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                         iRet;
    JADA_MsgConnectionConnected msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (msgIn.iResult == 0) {
        connPtr->iId = msgIn.iConnectionId;
        connPtr->iStatus = JADA_CONNSTATUS_CONNECTED;
    }

    iRet = JADA_RaiseEventConnectionConnected(connPtr, msgIn.iResult, connPtr->iId, msgIn.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleConnectionOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                         iRet;
    JADA_MsgConnectionOpened    msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (msgIn.iResult == JADA_CREDENTIAL_OK) {
        connPtr->iId = msgIn.iConnectionId;
        connPtr->iStatus = JADA_CONNSTATUS_OPENED;
        if (JADA_GetWhoAmI() == JADA_WHOAMI_MANAGER) {
            iRet = JADA_ConnSetParameter((JADA_Connection) connPtr, "connessione", "coda", msgIn.szCoda); 
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
        }

        iRet = JADA_SendParametersToServer(connPtr);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }

    iRet = JADA_RaiseEventConnectionOpened(connPtr, msgIn.iResult, connPtr->iId, msgIn.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     O P E N     S E S S I O N
 *
 *****************************************************************************/
//int JADA_HandleSessionOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
//{
//    int                         iRet, i;
//    JADA_MsgSessionOpened       msgIn;
//    JADA_MsgParametersList      msgOut;
//    JADA_Parameter              parSelf;
//    CORE_Currency               curP;
//    JADA_SesSelf                *sesPtr; 
//
//    sesPtr = (JADA_SesSelf *) connPtr->sesSelf;
//    JADA_ReturnValIfInvalidConnection(connPtr, -1);
//
//    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//
//    if (msgIn.iResult == 0) {
//        sesPtr->iId = msgIn.iSessionId;
//        sesPtr->iStatus = JADA_CONNSTATUS_SESS_CONNECTED;
//
//        memset(&msgOut, 0, sizeof(msgOut));
//        iRet = JADA_LstGetFirst(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
//        while (iRet == 0) {
//            i = msgOut.iParameters;
//            strcpy(msgOut.lstParameters[i].szSection, parSelf.szSection); 
//            strcpy(msgOut.lstParameters[i].szName, parSelf.szName); 
//            strcpy(msgOut.lstParameters[i].szValue, parSelf.szValue); 
//            msgOut.iParameters++;
//            if (msgOut.iParameters >= JADA_MAX_PARAMETERS_IN_LIST) {
//                break;
//            }
//            iRet = JADA_LstGetNext(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
//        }
//
//        iRet = JADA_SendSessionMessage(connPtr, JADA_MSG_SESS_PARAMETERS_LIST, &msgOut, sizeof(msgOut));
//        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//    }
//
//    iRet = JADA_RaiseEventSessionOpened(connPtr, msgIn.iResult, msgIn.iSessionId, msgIn.timOpenTime);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//
//    return(0);
//}

/******************************************************************************
 *
 *     L O A D     S E S S I O N
 *
 *****************************************************************************/
//int JADA_HandleSessionLoaded(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
//{
//    int                         iRet, i;
//    JADA_MsgSessionLoaded       msgIn;
//    JADA_MsgParametersList      msgOut;
//    JADA_Parameter              parSelf;
//    CORE_Currency               curP;
//    JADA_SesSelf                *sesPtr; 
//
//    sesPtr = (JADA_SesSelf *) connPtr->sesSelf;
//    JADA_ReturnValIfInvalidConnection(connPtr, -1);
//
//    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//
//    if (msgIn.iResult == 0) {
//        sesPtr->iId = msgIn.iSessionId;
//        sesPtr->iStatus = JADA_CONNSTATUS_SESS_CONNECTED;
//
//        memset(&msgOut, 0, sizeof(msgOut));
//        iRet = JADA_LstGetFirst(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
//        while (iRet == 0) {
//            i = msgOut.iParameters;
//            strcpy(msgOut.lstParameters[i].szSection, parSelf.szSection); 
//            strcpy(msgOut.lstParameters[i].szName, parSelf.szName); 
//            strcpy(msgOut.lstParameters[i].szValue, parSelf.szValue); 
//            msgOut.iParameters++;
//            if (msgOut.iParameters >= JADA_MAX_PARAMETERS_IN_LIST) {
//                break;
//            }
//            iRet = JADA_LstGetNext(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
//        }
//
//        iRet = JADA_SendSessionMessage(connPtr, JADA_MSG_SESS_PARAMETERS_LIST, &msgOut, sizeof(msgOut));
//        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//    }
//
//    iRet = JADA_RaiseEventSessionLoaded(connPtr, msgIn.iResult, msgIn.iSessionId, msgIn.timLoadTime);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//
//    return(0);
//}

/******************************************************************************
 *
 *     P I N G
 *
 *****************************************************************************/
int JADA_HandlePing(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet;
    JADA_MsgPing    msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    _ftime(&msgIn.timReplyTime);

    iRet = JADA_SendSupportMessage(connPtr->suppSelf, JADA_MSG_PING_REPLY, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int JADA_HandlePingReply(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet;
    int             iDelay, iOffset;
    JADA_MsgPing    msgIn;
    struct _timeb   timNow;

    _ftime(&timNow);
    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Calcola il tempo di attraversamento del pacchetto */
    iDelay = JADA_GetTimeDelay(msgIn.timPingTime.time, msgIn.timPingTime.millitm, timNow.time, timNow.millitm);
    iDelay = iDelay / 2;

    /* Calcola l'offset di orario fra le due postazioni */
    iOffset = JADA_GetTimeDelay(msgIn.timPingTime.time, msgIn.timPingTime.millitm, msgIn.timReplyTime.time, msgIn.timReplyTime.millitm);
    iOffset = iOffset - iDelay;

    JADA_LogMessage("PING: %d msecs\n", iDelay);
    iRet = JADA_RaiseEventPingReply(connPtr, msgIn.iRequestId, iDelay, iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     O P E N     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgRequestOpened   msgIn;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) connPtr->suppSelf;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    suppPtr->iRequestId = msgIn.iRequestId;
    suppPtr->iSessionId = msgIn.iSessionId;
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "password", msgIn.szPassword_DaButtare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento della password della richiesta");
    iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_OPENED);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");

    iRet = JADA_SendParametersToServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventSupportOpened(connPtr, msgIn.iRequestId, msgIn.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     A C C E P T     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportAccepted(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgRequestAccepted msgIn;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) connPtr->suppSelf;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    suppPtr->iRequestId = msgIn.iRequestId;
    suppPtr->iSessionId = msgIn.iSessionId;
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "password", msgIn.szPassword_DaButtare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione dell'ID di richiesta");
    iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_OPENED);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cliente", msgIn.szCliente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "telefono", msgIn.szTelefono);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cellulare", msgIn.szCellulare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "mail", msgIn.szMail);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameterInt((JADA_Connection)connPtr, "richiesta", "audio", msgIn.iUsaAudio);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo uso dell'audio");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "coda", msgIn.szCoda);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "descrizione", msgIn.szDescription);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");

    iRet = JADA_SendParametersToServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventSupportAccepted(connPtr, msgIn.iRequestId, msgIn.szUtente, msgIn.szTelefono, msgIn.szMail, 
                            msgIn.szCliente, msgIn.iUsaAudio, msgIn.szCoda, msgIn.szDescription, msgIn.timAcceptTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     C O N N E C T     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportConnected(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                      iRet;
    JADA_MsgRequestConnected msgIn;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) connPtr->suppSelf;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    suppPtr->iRequestId = msgIn.iRequestId;
    suppPtr->iSessionId = msgIn.iSessionId;
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "password", msgIn.szPassword_DaButtare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione della password di richiesta");
    iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_CONNECTED);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cliente", msgIn.szCliente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "telefono", msgIn.szTelefono);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cellulare", msgIn.szCellulare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "mail", msgIn.szMail);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameterInt((JADA_Connection)connPtr, "richiesta", "audio", msgIn.iUsaAudio);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo uso dell'audio");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "coda", msgIn.szCoda);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "descrizione", msgIn.szDescription);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");

    iRet = JADA_SendParametersToServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventSupportConnected(connPtr, msgIn.iRequestId, msgIn.szUtente, 
                                msgIn.iUsaAudio, msgIn.szCoda, msgIn.szDescription, msgIn.timConnectTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     S U S P E N D     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportSuspended(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                         iRet;
    JADA_ReqId                  iCurrId;
    JADA_MsgRequestSuspended    msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    CORE_ReturnValIfFail(JADA_IsValidSupport(connPtr->suppSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &iCurrId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione dell'ID di richiesta");
    if (JADA_CompareJadaId(&iCurrId, &msgIn.iRequestId)) {
        switch (JADA_GetWhoAmI()) {
        case JADA_WHOAMI_AGENT:
            iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_OPENED);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
            break;
        case JADA_WHOAMI_MANAGER:
            iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_LOADED);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
            break;
        }
    }

    iRet = JADA_RaiseEventSupportSuspended(connPtr, msgIn.iRequestId, msgIn.szUtente, msgIn.timSuspendTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleSupportResumed(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgRequestResumed  msgIn;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) connPtr->suppSelf;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    suppPtr->iRequestId = msgIn.iRequestId;
    suppPtr->iSessionId = msgIn.iSessionId;
 
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "password", msgIn.szPassword_DaButtare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione della password di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_SuppSetStatus(connPtr->suppSelf, msgIn.iStatus);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cliente", msgIn.szCliente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "telefono", msgIn.szTelefono);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cellulare", msgIn.szCellulare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "mail", msgIn.szMail);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameterInt((JADA_Connection)connPtr, "richiesta", "audio", msgIn.iUsaAudio);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo uso dell'audio");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "coda", msgIn.szCoda);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "descrizione", msgIn.szDescription);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");

    iRet = JADA_SendParametersToServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventSupportResumed(connPtr, msgIn.iRequestId, msgIn.iStatus, msgIn.szUtente, msgIn.szTelefono, msgIn.szMail, 
                            msgIn.szCliente, msgIn.iUsaAudio, msgIn.szCoda, msgIn.szDescription, msgIn.timResumeTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     L O A D     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportLoaded(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgRequestLoaded   msgIn;
    JADA_SuppSelf           *suppPtr = (JADA_SuppSelf *) connPtr->suppSelf;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    JADA_ReturnValIfInvalidSupport(suppPtr, -1);  
    suppPtr->iRequestId = msgIn.iRequestId;
    suppPtr->iSessionId = msgIn.iSessionId;
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "password", msgIn.szPassword_DaButtare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione della password di richiesta");
    iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_LOADED);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "partner", msgIn.szUtente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cliente", msgIn.szCliente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "telefono", msgIn.szTelefono);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "cellulare", msgIn.szCellulare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "mail", msgIn.szMail);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameterInt((JADA_Connection)connPtr, "richiesta", "audio", msgIn.iUsaAudio);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo uso dell'audio");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "coda", msgIn.szCoda);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");
    iRet = JADA_ConnSetParameter((JADA_Connection)connPtr, "richiesta", "descrizione", msgIn.szDescription);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione del tuo nome di richiesta");

    iRet = JADA_SendParametersToServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventSupportLoaded(connPtr, msgIn.iRequestId, msgIn.szUtente, 
                        msgIn.szCoda, msgIn.szDescription, msgIn.timLoadTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     S O L I C I T     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportSolicited(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                      iRet;
    JADA_ReqId               iCurrId;
    JADA_MsgRequestSolicited msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    CORE_ReturnValIfFail(JADA_IsValidSupport(connPtr->suppSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &iCurrId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione dell'ID di richiesta");

    iRet = JADA_RaiseEventSupportSolicited(connPtr, msgIn.iRequestId, msgIn.timSolicitTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     C L O S E     S U P P O R T
 *
 *****************************************************************************/
int JADA_HandleSupportClosed(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_ReqId              iCurrId;
    JADA_MsgRequestClosed   msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    CORE_ReturnValIfFail(JADA_IsValidSupport(connPtr->suppSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &iCurrId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella definizione dell'ID di richiesta");
    if (JADA_CompareJadaId(&iCurrId, &msgIn.iRequestId)) {
        iRet = JADA_SuppSetStatus(connPtr->suppSelf, JADA_SUPPSTATUS_CLOSED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento di stato della richiesta");
    }

    iRet = JADA_IniSetInt("richiesta", "id", -1);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del parametro richiesta/id nel file .ini");
    iRet = JADA_RaiseEventSupportClosed(connPtr, msgIn.iRequestId, msgIn.timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     G E N E R I C     R E S U L T
 *
 *****************************************************************************/
int JADA_HandleResult(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet;
    JADA_MsgResult  msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventResult(connPtr, msgIn.iResult, msgIn.szDescription);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     T E X T     C H A T
 *
 *****************************************************************************/
int JADA_HandleTextChatReceived(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_MsgChatText    msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventChatTextReceived(connPtr, msgIn.iWhoAmI, msgIn.szText);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     R E Q U E S T     S E R V I C E
 *
 *****************************************************************************/
//int JADA_HandleRequestService(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
//{
//    int                     iRet;
//    JADA_ReqId              iRequestId;
//    JADA_MsgRequestService  msgIn;
//
//    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//    CORE_ReturnValIfFail(JADA_IsValidSupport(connPtr->suppSelf), -1, ;, "errore nella lettura del payload");
//    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &iRequestId);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");
//
//    iRet = JADA_RaiseEventServiceRequested(connPtr, msgIn.iType, msgIn.timRequestTime);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//
//    return(0);
//}
//
//int JADA_HandleAcceptService(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
//{
//    int                     iRet;
//    JADA_ReqId              iRequestId;
//    JADA_MsgAcceptService   msgIn;
//
//    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//    CORE_ReturnValIfFail(JADA_IsValidSupport(connPtr->suppSelf), -1, ;, "errore nella lettura del payload");
//    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &iRequestId);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");
//
//    iRet = JADA_RaiseEventServiceAccepted(connPtr, msgIn.iType, msgIn.iAccepted, msgIn.timAcceptTime);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
//
//    return(0);
//}

/******************************************************************************
 *
 *     O P E N     C A L L
 *
 *****************************************************************************/
int JADA_HandleOpenCall(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_ReqId          iRequestId;
    JADA_MsgOpenCall    msgIn;
    JADA_MsgCallOpened  msgOut;
    FixString           szParams;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidCall(connPtr->callSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_CallGetRequestId(connPtr->callSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    iRet = JADA_ProcExists(JADA_PROC_CALLAGENT);
    if (iRet == 0) {
        iRet = JADA_ProcKill(JADA_PROC_CALLAGENT);
        JADA_Sleep(1000);
    }

    sprintf(szParams, "-audio 1 -video 0 -id %d", connPtr->iId);
    iRet = JADA_ProcExecute(JADA_PROC_CALLAGENT, szParams);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    JADA_LogMessage("Ho lanciato il comando %s %s\n", JADA_PROC_CALLAGENT, szParams);
    JADA_Sleep(1000);

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iResult = 0;
    iRet = JADA_SendCallMessage(connPtr->callSelf, JADA_MSG_CALL_OPENED, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_RaiseEventCallOpened(connPtr, msgIn.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleCallOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_MsgCallOpened  msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (msgIn.iResult == 0) {
        iRet = JADA_RaiseEventCallOpened(connPtr, msgIn.timOpenTime);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }
    else {
        iRet = JADA_ProcExists(JADA_PROC_CALLMANAGER);
        if (iRet == 0) {
            iRet = JADA_ProcKill(JADA_PROC_CALLMANAGER);
        }

        JADA_LogMessage ("Chiamata audiovideo non accettata.\n");
    }

    return(0);
}

/******************************************************************************
 *
 *     C L O S E     C A L L
 *
 *****************************************************************************/
int JADA_HandleCloseCall(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_ReqId          iRequestId;
    JADA_MsgCloseCall   msgIn;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_ProcExists(JADA_PROC_CALLAGENT);
    if (iRet == 0) {
        iRet = JADA_ProcKill(JADA_PROC_CALLAGENT);
    }

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidCall(connPtr->callSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_CallGetRequestId(connPtr->callSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    iRet = JADA_RaiseEventCallClosed(connPtr, msgIn.timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleCallClosed(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int     iRet;
    JADA_Timet  timCloseTime;

    timCloseTime = time(0);

    iRet = JADA_RaiseEventCallClosed(connPtr, timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     A B O R T     C A L L
 *
 *****************************************************************************/
int JADA_HandleAbortCall(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_ReqId          iRequestId;
    JADA_MsgAbortCall   msgIn;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidCall(connPtr->callSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_CallGetRequestId(connPtr->callSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    iRet = JADA_RaiseEventCallAborted(connPtr, msgIn.iRetries, msgIn.timAbortTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleCallAborted(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_MsgAbortCall   msgIn;
    //JADA_Timet          timAbortTime;

    //timAbortTime = time(0);
    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_RaiseEventCallAborted(connPtr, msgIn.iRetries, msgIn.timAbortTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     O P E N     R E M O T E     D E S K T O P
 *
 *****************************************************************************/
int JADA_HandleOpenDesktop(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_MsgOpenDesktop msgIn;
    FixString           szPassword;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Cambia la password di accesso a VNC */
    iRet = JADA_ConnGetParameter((JADA_Connection)connPtr, "richiesta", "password", szPassword, sizeof(szPassword));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione al VNC Server");
    //iRet = SetVNCPassword(szPassword);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'aggiornamento della password temporanea");

    // L'accettazione non e' fatta piu' dal VNC Server, ma richiesta alla GUI tramite
    // evento e confermata tramite il servizio JADA_ReqAcceptDesktop()
    //iRet = JADA_StartDesktopServer(connPtr->vncSelf);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione al VNC Server");
    iRet = JADA_RaiseEventDesktopRequested(connPtr, msgIn.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleDesktopOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgDesktopOpened   msgIn;
    FixString               szPassword;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (msgIn.iAccepted == TRUE) {
        iRet = JADA_ConnGetParameter((JADA_Connection)connPtr, "richiesta", "password", szPassword, sizeof(szPassword));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione al VNC Server");
        iRet = JADA_RaiseEventDesktopOpened(connPtr, szPassword, msgIn.timOpenTime);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }
    else {
        iRet = JADA_ProcExists(JADA_PROC_VNCVIEWER);
        if (iRet == 0) {
            iRet = JADA_ProcKill(JADA_PROC_VNCVIEWER);
        }

        JADA_LogMessage("Apertura remote desktop non accettata.\n");
        //iRet = JADA_TcpDestroySocket(&connPtr->sokVNC);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }

    return(0);
}

/******************************************************************************
 *
 *     C L O S E     R E M O T E     D E S K T O P
 *
 *****************************************************************************/
int JADA_HandleCloseDesktop(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                  iRet;
    JADA_MsgCloseDesktop msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_StopDesktopServer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione al VNC Server");

    iRet = JADA_RaiseEventDesktopClosed(connPtr, msgIn.timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     O P E N     W H I T E B O A R D
 *
 *****************************************************************************/
int JADA_HandleOpenWhiteboard(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                         iRet;
    JADA_ReqId                  iRequestId;
    JADA_MsgOpenWhiteboard      msgIn;
    JADA_MsgWhiteboardOpened    msgOut;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidVNC(connPtr->vncSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_VncGetRequestId(connPtr->vncSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iResult = 0;
    msgOut.iRequestId = iRequestId;
    //msgOut.iWidth = sesPtr->iWhiteboardWidth;
    //msgOut.iHeight = sesPtr->iWhiteboardHeight;
    msgOut.timOpenTime = time(0);
    iRet = JADA_SendVncMessage(connPtr->vncSelf, JADA_MSG_WHITEBOARD_OPENED, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_RaiseEventWhiteboardOpened(connPtr, msgOut.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleWhiteboardOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                         iRet;
    JADA_MsgWhiteboardOpened    msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (msgIn.iResult == 0) {

        iRet = JADA_RaiseEventWhiteboardOpened(connPtr, msgIn.timOpenTime);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }
    else {
        // JADA_LogMessage ("Apertuta whiteboard non accettata.\n");
    }

    return(0);
}

/******************************************************************************
 *
 *     C L E A N     W H I T E B O A R D
 *
 *****************************************************************************/
int JADA_HandleCleanWhiteboard(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_ReqId              iRequestId;
    JADA_MsgCleanWhiteboard msgIn;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidVNC(connPtr->vncSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_VncGetRequestId(connPtr->vncSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    iRet = JADA_RaiseEventWhiteboardCleaned(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     C L O S E     W H I T E B O A R D
 *
 *****************************************************************************/
int JADA_HandleCloseWhiteboard(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_ReqId              iRequestId;
    JADA_MsgCloseWhiteboard msgIn;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidVNC(connPtr->vncSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_VncGetRequestId(connPtr->vncSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    iRet = JADA_RaiseEventWhiteboardClosed(connPtr, msgIn.timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     S E T     D R A W     P O I N T E R
 *
 *****************************************************************************/
int JADA_HandleSetDrawPointer(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgSetDrawPointer  msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventSetDrawPointer(connPtr, msgIn.iPointerType, msgIn.iPointX, msgIn.iPointY);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleResetDrawPointer(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;

    iRet = JADA_RaiseEventResetDrawPointer(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     O P E N     F T P 
 *
 *****************************************************************************/
int JADA_HandleOpenFtp(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_ReqId          iRequestId;
    JADA_MsgOpenCall    msgIn;
    JADA_MsgCallOpened  msgOut;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidFTP(connPtr->ftpSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_FtpGetRequestId(connPtr->ftpSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iResult = 0;
    iRet = JADA_SendFtpMessage(connPtr->ftpSelf, JADA_MSG_FTP_OPENED, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_RaiseEventFtpOpened(connPtr, msgIn.timOpenTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleFtpOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_MsgCallOpened  msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    if (msgIn.iResult == 0) {
        iRet = JADA_RaiseEventFtpOpened(connPtr, msgIn.timOpenTime);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }
    else {
        // JADA_LogMessage ("Chiamata audiovideo non accettata.\n");
    }

    return(0);
}

/******************************************************************************
 *
 *     C L O S E     F T P
 *
 *****************************************************************************/
int JADA_HandleCloseFtp(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet;
    JADA_ReqId          iRequestId;
    JADA_MsgCloseCall   msgIn;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_IsValidFTP(connPtr->ftpSelf), -1, ;, "errore nella lettura del payload");
    iRet = JADA_FtpGetRequestId(connPtr->ftpSelf, &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&iRequestId, &msgIn.iRequestId), -1, ;, "request id non corrispondente");

    iRet = JADA_RaiseEventFtpClosed(connPtr, msgIn.timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleFtpClosed(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int     iRet;
    JADA_Timet  timCloseTime;

    timCloseTime = time(0);

    iRet = JADA_RaiseEventFtpClosed(connPtr, timCloseTime);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     D A T A     Q U E R Y
 *
 *****************************************************************************/
int JADA_HandleRequestsList(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet, i;
    JADA_MsgRequestList msgIn;
    JADA_Stream         stmSelf;
    FixString           szLine;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    stmSelf = JADA_StmNew(__FILE__, __LINE__, (JADA_Connection) connPtr);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore nella lettura del payload");

    for (i = 0; i < msgIn.iRequests; i++) {
        if (msgIn.lstRequests[i].iStatus != 0) {
            JADA_ChangeStringChar(msgIn.lstRequests[i].szDescrizione, '\n', ' ');
            JADA_ChangeStringChar(msgIn.lstRequests[i].szDescrizione, '\r', ' ');
            sprintf(szLine, "%s; %s; %s; %s; %s; %s; %s",
                    JADA_JadaIdToString(&msgIn.lstRequests[i].iRequestId, NULL),
                    msgIn.lstRequests[i].szUtente,
                    msgIn.lstRequests[i].szCliente,
                    msgIn.lstRequests[i].szTelefono,
                    msgIn.lstRequests[i].szCellulare,
                    msgIn.lstRequests[i].szMail,
                    msgIn.lstRequests[i].szDescrizione);
            iRet = JADA_StmAddLine(stmSelf, szLine);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
        }
    }

    iRet = JADA_RaiseEventRequestsList(connPtr, JADA_LISTTYPE_REQUESTS, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleComboValues(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet, i;
    JADA_MsgComboValues msgIn;
    JADA_Stream         stmSelf;
    FixString           szLine;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    stmSelf = JADA_StmNew(__FILE__, __LINE__, (JADA_Connection) connPtr);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore nella lettura del payload");

    for (i = 0; i < msgIn.iValues; i++) {
        sprintf(szLine, "%s", msgIn.lstValues[i]);
        iRet = JADA_StmAddLine(stmSelf, szLine);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }

    iRet = JADA_RaiseEventComboValues(connPtr, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandlePendingRequestsList(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet, i;
    JADA_MsgRequestList msgIn;
    JADA_Stream         stmSelf;
    FixString           szLine;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    stmSelf = JADA_StmNew(__FILE__, __LINE__, (JADA_Connection) connPtr);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore nella lettura del payload");

    for (i = 0; i < msgIn.iRequests; i++) {
        if (msgIn.lstRequests[i].iStatus != 0) {
            sprintf(szLine, "%s; %s; %s; %s; %s; %s; %s",
                    JADA_JadaIdToString(&msgIn.lstRequests[i].iRequestId, NULL),
                    msgIn.lstRequests[i].szUtente,
                    msgIn.lstRequests[i].szCliente,
                    msgIn.lstRequests[i].szTelefono,
                    msgIn.lstRequests[i].szCellulare,
                    msgIn.lstRequests[i].szMail,
                    msgIn.lstRequests[i].szDescrizione);
            iRet = JADA_StmAddLine(stmSelf, szLine);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
        }
    }

    iRet = JADA_RaiseEventRequestsList(connPtr, JADA_LISTTYPE_PENDING_REQUESTS, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleParametersList(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet, i;
    JADA_MsgParametersList  msgIn;
    JADA_Stream         stmSelf;
    FixString           szLine;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    stmSelf = JADA_StmNew(__FILE__, __LINE__, (JADA_Connection) connPtr);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore nella lettura del payload");

    for (i = 0; i < msgIn.iParameters; i++) {
        sprintf(szLine, "%s; %s; %s", msgIn.lstParameters[i].szSection, 
                msgIn.lstParameters[i].szName, msgIn.lstParameters[i].szValue);
        iRet = JADA_StmAddLine(stmSelf, szLine);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }

    iRet = JADA_RaiseEventParametersList(connPtr, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleListDirectory(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    wchar_t                 szFileName[1024];
    FixString               currFileName;
    WIN32_FIND_DATA         FindFileData;
    HANDLE                  hFind;
    BOOL                    boolRet;
    JADA_MsgListDirectory   msgIn;
    JADA_MsgDirectoryList   msgOut;


    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
 
    memset(&msgOut, 0, sizeof(msgOut));

    memset(szFileName, 0, sizeof(szFileName));
    mbstowcs(szFileName, msgIn.szDirName, strlen(msgIn.szDirName));
    wcscat(szFileName, L"*");

    /* Mando prima le directory... */
    hFind = FindFirstFile(szFileName, &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            WideCharToMultiByte(CP_ACP, 0, FindFileData.cFileName, sizeof(FindFileData.cFileName), currFileName, sizeof(currFileName), NULL, NULL); 
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if ((strcmp(currFileName, ".") != 0) && (strcmp(currFileName, "..") != 0)) {
                    sprintf(msgOut.lstFiles[msgOut.iFiles].szFileName, "[%s]", currFileName);
                    msgOut.iFiles++;
                    if (msgOut.iFiles > JADA_MAX_FILES_IN_LIST) {
                        iRet = JADA_SendFtpMessage(connPtr->ftpSelf, JADA_MSG_DIRECTORY_LIST, &msgOut, sizeof(msgOut));
                        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
                        memset(&msgOut, 0, sizeof(msgOut));
                    }
                }
            }

            boolRet = FindNextFile(hFind, &FindFileData);
        } while (boolRet != 0);

        FindClose(hFind);
    }

    /* ... poi mando i files */
    hFind = FindFirstFile(szFileName, &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            WideCharToMultiByte(CP_ACP, 0, FindFileData.cFileName, sizeof(FindFileData.cFileName), currFileName, sizeof(currFileName), NULL, NULL); 
            if (! (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                strcpy(msgOut.lstFiles[msgOut.iFiles].szFileName, currFileName);
                msgOut.iFiles++;
                if (msgOut.iFiles > JADA_MAX_FILES_IN_LIST) {
                    iRet = JADA_SendFtpMessage(connPtr->ftpSelf, JADA_MSG_DIRECTORY_LIST, &msgOut, sizeof(msgOut));
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
                    memset(&msgOut, 0, sizeof(msgOut));
                }
            } 

            boolRet = FindNextFile(hFind, &FindFileData);
        } while (boolRet != 0);

        FindClose(hFind);
    }

    /* ... infine mando l'ultimo messaggio */
    if (msgOut.iFiles > 0) {
        iRet = JADA_SendFtpMessage(connPtr->ftpSelf, JADA_MSG_DIRECTORY_LIST, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        memset(&msgOut, 0, sizeof(msgOut));
    }

    return(0);
}

int JADA_HandleDirectoryList(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet, i;
    JADA_MsgDirectoryList   msgIn;
    FixString               szFileName;
    FILE                    *fpOut;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_MkTemp(szFileName, sizeof(szFileName));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    fpOut = fopen(szFileName, "w+");
    CORE_ReturnValIfFail(fpOut != NULL, -1, ;, "errore nella lettura del payload");

    for (i = 0; i < msgIn.iFiles; i++) {
        fprintf(fpOut, "%s\n", msgIn.lstFiles[i].szFileName);
    }

    fclose(fpOut);

    iRet = JADA_RaiseEventDirectoryList(connPtr, szFileName);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

int JADA_HandleGetFile(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgFileTransfer    msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_FtpEnqueueFtp(connPtr->ftpSelf, msgIn.szFileFrom, msgIn.szFileTo);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiunta in lista del descrittore FTP");

    return(0);
}

int JADA_HandleListCounters(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                  iRet;
    JADA_MsgListCounters msgIn;


    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    iRet = JADA_RaiseEventListCounters(connPtr, msgIn.iAreaRequests, msgIn.iTotalRequests, 
                                       msgIn.iAreaPendingRequests, msgIn.iTotalPendingRequests);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

/******************************************************************************
 *
 *     F R A M E S
 *
 *****************************************************************************/
int JADA_HandleAudioFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet, iType;
    JADA_Frame      frmSelf;
    int             iTimeDelay;
    JADA_MsgHeader  msgHeader;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgGetHeader(msgRecv, &msgHeader);
	CORE_LogIfFail(iRet == 0, "Errore generico");
    iRet = JADA_MsgGetTimeDelay(msgRecv, &iTimeDelay);
	//JADA_LogMessage("Time delay sul messaggio %s (IN): %d\n", 
    //                JADA_MsgTypeToString(msgHeader.iType), iTimeDelay);

    frmSelf = JADA_FrmNewFromMessage(__FILE__, __LINE__, msgRecv);
    CORE_ReturnValIfFail(frmSelf != NULL, -1, ;, "errore nella creazione del frame audio");
    iRet = JADA_FrmGetType(frmSelf, &iType);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del tipo di frame");
    CORE_ReturnValIfFail(iType == JADA_FRAMETYPE_AUDIO, -1, ;, "il frame non e' di tipo audio");
    
    iRet = JADA_CallEnqueueAudioFrame(connPtr->callSelf, &frmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del frame audio");

    return(0);
}

int JADA_HandleVideoFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet, iType;
    JADA_Frame      frmSelf;
    int             iTimeDelay;
    JADA_MsgHeader  msgHeader;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgGetHeader(msgRecv, &msgHeader);
	CORE_LogIfFail(iRet == 0, "Errore generico");
    iRet = JADA_MsgGetTimeDelay(msgRecv, &iTimeDelay);
	//JADA_LogMessage("Time delay sul messaggio %s (IN): %d\n", 
    //                JADA_MsgTypeToString(msgHeader.iType), iTimeDelay);

    frmSelf = JADA_FrmNewFromMessage(__FILE__, __LINE__, msgRecv);
    CORE_ReturnValIfFail(frmSelf != NULL, -1, ;, "errore nella creazione del frame video");
    iRet = JADA_FrmGetType(frmSelf, &iType);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del tipo di frame");
    CORE_ReturnValIfFail(iType == JADA_FRAMETYPE_VIDEO, -1, ;, "il frame non e' di tipo video");

    iRet = JADA_CallEnqueueVideoFrame(connPtr->callSelf, &frmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del frame video");

    return(0);
}

int JADA_HandleDrawFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet;
    JADA_Draw       drwSelf;
    char            szDrawData[JADA_DRAW_FRAME_LENGTH];

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgGetPayload(msgRecv, szDrawData, sizeof(szDrawData));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del tipo di frame");
    drwSelf = JADA_DrwDeserialize(szDrawData, sizeof(szDrawData));
    CORE_ReturnValIfFail(drwSelf != NULL, -1, ;, "errore nella lettura del tipo di frame");
 
    iRet = JADA_VncEnqueueDrawFrame(connPtr->vncSelf, &drwSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del draw frame");

    return(0);
}

int JADA_HandleDesktopFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int             iRet, iType;
    JADA_Frame      frmSelf;
    char            szFrameData[JADA_DESKTOP_FRAME_LENGTH];
    int             iLength;//, iBytesSent;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    frmSelf = JADA_FrmNewFromMessage(__FILE__, __LINE__, msgRecv);
    CORE_ReturnValIfFail(frmSelf != NULL, -1, ;, "errore nella creazione del frame desktop");
    iRet = JADA_FrmGetType(frmSelf, &iType);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del tipo di frame");
    CORE_ReturnValIfFail(iType == JADA_FRAMETYPE_DESKTOP, -1, ;, "il frame non e' di tipo desktop");

    iRet = JADA_FrmGetLength(frmSelf, &iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del tipo di frame");
    iRet = JADA_FrmGetData(frmSelf, szFrameData, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura dei dati del frame desktop");

    //if (connPtr->sokVNC.iSocket > 0) {
    //    // JADA_LogToFile(myWhoAmI, "C:/DesktopFrame.recv", szFrameData, iLength);
    //    iBytesSent = JADA_VncSendDataToSocket(connPtr->sokVNC.iSocket, szFrameData, iLength);
    //    // iBytesSent = send(sesPtr->sokVNC.iSocket, szFrameData, iLength, 0);
    //    // JADA_LogMessage("Nuovi dati inviati verso VNC (%d)\n", iBytesSent);
    //    CORE_ReturnValIfFail(iBytesSent == iLength, -1, ;, "errore nella lettura dei dati del frame desktop");
    //}

    iRet = JADA_FrmDestroy(__FILE__, __LINE__, frmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella distruzione del frame desktop");

    return(0);
}

int JADA_HandleFtpFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv)
{
    int                 iRet; //, iSize, iCurrFrame;
    JADA_MsgFtpFrame    msgIn;
    FILE                *fp;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_MsgGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura dei dati del frame FTP");

    if (msgIn.ftpInfo.iCurrFrame == 1) {
        iRet = JADA_FileRemove(msgIn.ftpInfo.szFileName);
    }
    fp = fopen(msgIn.ftpInfo.szFileName, "a+b");
    CORE_ReturnValIfFail(fp != NULL, -1, ;, "errore nell'apertura del file in ricezione FTP");

    //fseek(fp, 0L, SEEK_END);
    //iSize = ftell(fp);
    //iCurrFrame = (iSize / JADA_FTP_FRAME_LENGTH);
    //CORE_ReturnValIfFail(iCurrFrame == msgIn.ftpInfo.iCurrFrame, -1, ;, "errore nella sequenza di frames FTP");

    iRet = fwrite(msgIn.szPayload, 1, msgIn.ftpInfo.iLength, fp);
    CORE_ReturnValIfFail(iRet == msgIn.ftpInfo.iLength, -1, ;, "errore nella scrittura su file del frame FTP");

    fflush(fp); 
    fclose(fp); 

    iRet = JADA_RaiseEventFileReceived(connPtr, msgIn.ftpInfo.szFileName, msgIn.ftpInfo.iCurrFrame, msgIn.ftpInfo.iFrames);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella raise dell'evento");

    return(0);
}
