#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"

static JADA_ParameterEntry SessionParams[] = {
    "sessione",     "coda",             1,          0,
    "sessione",     "descrizione",      1,          0,
    //"sessione",     "username",         1,          0,
    //"sessione",     "password",         1,          0,
    //"sessione",     "nome",             1,          0,
    //"sessione",     "telefono",         1,          0,
    //"sessione",     "cellulare",        1,          0,
    //"sessione",     "mail",             1,          0,
    //"sessione",     "regione",          1,          0,
    //"sessione",     "cliente",          1,          0,
    //"sessione",     "area",             1,          0,
    //"sessione",     "audio",            1,          0,
    //"postazione",   "host",             0,          0,
    //"postazione",   "IP",               0,          0,
    //"postazione",   "tipo OS",          0,          0,
    //"postazione",   "computer",         0,          0,
    //"postazione",   "domain user",      0,          0,
    //"postazione",   "BIOS",             0,          0,
    //"postazione",   "CPU",              0,          0,
    //"postazione",   "RAM",              0,          0,
    //"postazione",   "versione OS",      0,          0,
    //"postazione",   "schermo",          0,          0,
    NULL,           NULL,               -1,         -1
};


int JADA_SesSubscribeEvent(JADA_Session sesSelf, int iEventType)
{
    int                     iRet;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_EventSubscriber    subSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    memset(&subSelf, 0, sizeof(subSelf));
    subSelf.iType = iEventType;

    iRet = JADA_LstAdd(sesPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");

    return(0);
}

int JADA_SesUnsubscribeEvent(JADA_Session sesSelf, int iEventType)
{
    int                     iRet;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_EventSubscriber    subSelf;
    CORE_Currency           curS;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    iRet = JADA_LstGetFirst(sesPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    while (iRet == 0) {
        if (subSelf.iType == iEventType) {
            iRet = JADA_LstDelete(sesPtr->lstEventSubscribers, &curS);
            break;
        }

        iRet = JADA_LstGetNext(sesPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    }

    return(0);
}

int JADA_SesWaitSubscribedEvent(JADA_Session sesSelf, int iEventType, int iSecs, JADA_Event *evtSelf)
{
    int             iRet, myEventType;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    iRet = JADA_LstWait(sesPtr->lstSubscribedEvents, evtSelf, sizeof(*evtSelf), iSecs * 1000);
    if (iRet == 0) {
        iRet = JADA_EvtGetType(*evtSelf, &myEventType);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");
        if (iEventType == myEventType) {
            return(0);
        }
    }

    return(-1);
}

JADA_Session JADA_SesNew(char *szFile, int iLine, JADA_Connection connSelf)
{
    int             iRet, i;
    JADA_SesSelf    *sesPtr;
    JADA_Parameter  parSelf;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);

    sesPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_SesSelf));
    CORE_ReturnValIfFail(sesPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    JADA_ClearArcaId(sesPtr->iId);
    sesPtr->iStatus = JADA_SESSTATUS_AVAILABLE;
    time(&sesPtr->timLastActivity);
    sesPtr->iMagic = JADA_MAGIC_SESSION;
    sesPtr->connSelf = connSelf;
    connPtr->sesSelf = (JADA_Session) sesPtr;

    sesPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Parameter), NULL);
    CORE_ReturnValIfFail(sesPtr->lstParameters != NULL, NULL, ;, "Errore nella creazione della lista di eventi");
    sesPtr->lstFtp = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_FtpSelf), NULL);
    CORE_ReturnValIfFail(sesPtr->lstFtp != NULL, NULL, ;, "Errore nella creazione della lista di descrittori FTP");
    sesPtr->lstEvents = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Event), NULL);
    CORE_ReturnValIfFail(sesPtr->lstEvents != NULL, NULL, ;, "Errore nella creazione della lista di eventi");
    sesPtr->lstEventSubscribers = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_EventSubscriber), NULL);
    CORE_ReturnValIfFail(sesPtr->lstEventSubscribers != NULL, NULL, ;, "Errore nella creazione della lista dei sottoscrittori di eventi");
    sesPtr->lstSubscribedEvents = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Event), NULL);
    CORE_ReturnValIfFail(sesPtr->lstSubscribedEvents != NULL, NULL, ;, "Errore nella creazione della lista di eventi sottoscritti");

    for (i = 0; SessionParams[i].szSection != NULL; i++) {
        memset(&parSelf, 0, sizeof(parSelf));
        strcpy(parSelf.szSection, SessionParams[i].szSection);
        strcpy(parSelf.szName, SessionParams[i].szName);
        if (SessionParams[i].iIniFile > 0) {
            iRet = JADA_IniGetString(parSelf.szSection, parSelf.szName, parSelf.szValue, sizeof(parSelf.szValue));
            CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del parametro dal file .ini");
        }
        parSelf.iIniFile = SessionParams[i].iIniFile;
        parSelf.iIniWhoAmI = SessionParams[i].iIniWhoAmI;

        iRet = JADA_LstAdd(sesPtr->lstParameters, &parSelf, sizeof(parSelf), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nell'aggiunta in lista del parametro");
    }

    //iRet = JADA_SesSetParameter((JADA_Session) sesPtr, "postazione", "tipo OS", "Windows");
    //CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nel settaggio del parametro 'tipo OS'");

    return((JADA_Session) sesPtr);
}

DllExport int __stdcall JADA_SesDestroy(char *szFile, int iLine, JADA_Session sesSelf)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_FtpSelf    ftpSelf;
    CORE_Currency   curC;

    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    iRet = JADA_SesClose(sesSelf);

    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstParameters);

    iRet = JADA_LstGetFirst(sesPtr->lstFtp, &ftpSelf, sizeof(ftpSelf), &curC);
    while (iRet == 0) {
        if (ftpSelf.fp != NULL) {
            fclose(ftpSelf.fp);
        }
        iRet = JADA_LstGetNext(sesPtr->lstFtp, &ftpSelf, sizeof(ftpSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstFtp);

    iRet = JADA_LstGetFirst(sesPtr->lstEvents, &evtSelf, sizeof(evtSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_EvtDestroy(__FILE__, __LINE__, evtSelf);
        iRet = JADA_LstGetNext(sesPtr->lstEvents, &evtSelf, sizeof(evtSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstEvents);

    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstEventSubscribers);

    iRet = JADA_LstGetFirst(sesPtr->lstSubscribedEvents, &evtSelf, sizeof(evtSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_EvtDestroy(__FILE__, __LINE__, evtSelf);
        iRet = JADA_LstGetNext(sesPtr->lstSubscribedEvents, &evtSelf, sizeof(evtSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstSubscribedEvents);

    JADA_Free(__FILE__, __LINE__, sesPtr);

    return(0);
}

static int JADA_SesReset(JADA_SesSelf *sesPtr)
{
    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    JADA_ClearArcaId(sesPtr->iId);
    sesPtr->iStatus = JADA_SESSTATUS_AVAILABLE;
    time(&sesPtr->timLastActivity);
    sesPtr->iMagic = JADA_MAGIC_SESSION;

    return(0);
}

DllExport int __stdcall JADA_SesGetRequest(JADA_Session sesSelf, JADA_Request *reqPtr)
{
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    *reqPtr = sesPtr->reqSelf;

    return(0);
}

DllExport int __stdcall JADA_SesOpen(JADA_Session sesSelf, int iWaitSeconds)
{
    int                 iRet, iResult = -1;
    JADA_SesSelf        *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_MsgOpenSession outMsg;
    JADA_Event          evtSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnSubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_OPENED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    memset(&outMsg, 0, sizeof(outMsg));
    outMsg.iWhoAmi = JADA_GetWhoAmI();
    outMsg.iSessionType = JADA_SESSTYPE_SUPPORT;
    iRet = JADA_SendConnectionMessage(sesPtr->connSelf, JADA_MSG_OPEN_SESSION, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_OPENED);, "Errore generico");
    sesPtr->iStatus = JADA_SESSTATUS_REQUESTED;

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(sesPtr->connSelf, JADA_EVT_SESSION_OPENED, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetResult(evtSelf, &iResult);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_OPENED);, "Errore generico");
            iRet = JADA_EvtGetSessionId(evtSelf, &sesPtr->iId);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_OPENED);, "Errore generico");
        }

        iRet = JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_OPENED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    return(iResult);
}

DllExport int __stdcall JADA_SesConnect(JADA_Session sesSelf, int iSessionId, int iWaitSeconds)
{
    int                     iRet, iResult = -1;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_MsgConnectSession  outMsg;
    JADA_Event              evtSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnSubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_CONNECTED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    memset(&outMsg, 0, sizeof(outMsg));
    outMsg.iWhoAmi = JADA_GetWhoAmI();
    outMsg.iSessionId = iSessionId;
    iRet = JADA_SendConnectionMessage(sesPtr->connSelf, JADA_MSG_CONNECT_SESSION, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_CONNECTED);, "Errore generico");
    sesPtr->iStatus = JADA_SESSTATUS_REQUESTED;

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(sesPtr->connSelf, JADA_EVT_SESSION_CONNECTED, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetResult(evtSelf, &iResult);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_CONNECTED);, "Errore generico");
            iRet = JADA_EvtGetSessionId(evtSelf, &sesPtr->iId);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_CONNECTED);, "Errore generico");
        }

        iRet = JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_CONNECTED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    return(iResult);
}

DllExport int __stdcall JADA_SesLoad(JADA_Session sesSelf, int iSessionId, int iWaitSeconds)
{
    int                 iRet, iResult = -1;
    JADA_SesSelf        *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_MsgLoadSession outMsg;
    pthread_t           pThreadId;
    pthread_attr_t      attr;
    JADA_Event          evtSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnSubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    memset(&outMsg, 0, sizeof(outMsg));
    outMsg.iWhoAmi = JADA_GetWhoAmI();
    outMsg.iSessionId = iSessionId;
    iRet = JADA_SendConnectionMessage(sesPtr->connSelf, JADA_MSG_LOAD_SESSION, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED);, "Errore generico");
    sesPtr->iStatus = JADA_SESSTATUS_REQUESTED;

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetResult(evtSelf, &iResult);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED);, "Errore generico");
            iRet = JADA_EvtGetSessionId(evtSelf, &sesPtr->iId);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED);, "Errore generico");
            iRet = JADA_EvtGetItemType(evtSelf, &sesPtr->iType);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED);, "Errore generico");
        }

        iRet = JADA_ConnUnsubscribeEvent(sesPtr->connSelf, JADA_EVT_SESSION_LOADED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    if (sesPtr->iType == JADA_SESSTYPE_FTP) { 
        sesPtr->iStatus = JADA_SESSTATUS_CONNECTED;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        iRet = pthread_create(&pThreadId, &attr, JADA_FtpSenderThread, sesPtr);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    return(iResult);
}

DllExport int __stdcall JADA_SesClose(JADA_Session sesSelf)
{
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    sesPtr->iStatus = JADA_SESSTATUS_AVAILABLE;

    return(0);
}

int JADA_SendSessionMessage(JADA_Session sesSelf, int iOpcode, void *pPayload, int iSize)
{
    int                 iRet;
    JADA_SesSelf        *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_ConnSelf       *connPtr;
    JADA_Message        msgOut;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  
    connPtr = (JADA_ConnSelf *) sesPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    /* Crea il messaggio in output */
    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), sesPtr->iId, iOpcode, iSize);
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

DllExport int __stdcall JADA_SesListRequests(JADA_Session sesSelf, char *szFilterCoda, char *szFileName, int iWaitSeconds)
{
    int                     iRet;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_MsgListRequests    outMsg;
    JADA_Event              evtSelf;
    JADA_EventData          evtParams;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    memset(&outMsg, 0, sizeof(outMsg));
    strcpy(outMsg.szCoda, szFilterCoda);

    iRet = JADA_SesSubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_SendSessionMessage(sesSelf, JADA_MSG_LIST_REQUESTS, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_SesUnsubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
 
    if (iWaitSeconds > 0) {
        iRet = JADA_SesWaitSubscribedEvent(sesSelf, JADA_EVT_REQUESTS_LIST, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetData(evtSelf, &evtParams);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_SesUnsubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
            _unlink(szFileName);
            rename(evtParams.szFileName, szFileName);
        }
    }

    iRet = JADA_SesUnsubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iRet);
}

DllExport int __stdcall JADA_SesListPendingRequests(JADA_Session sesSelf, char *szFilterCoda, char *szFileName, int iWaitSeconds)
{
    int                     iRet;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_MsgListRequests    outMsg;
    JADA_Event              evtSelf;
    JADA_EventData          evtParams;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    memset(&outMsg, 0, sizeof(outMsg));
    strcpy(outMsg.szCoda, szFilterCoda);

    iRet = JADA_SesSubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_SendSessionMessage(sesSelf, JADA_MSG_LIST_PENDING_REQUESTS, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_SesUnsubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
 
    if (iWaitSeconds > 0) {
        iRet = JADA_SesWaitSubscribedEvent(sesSelf, JADA_EVT_REQUESTS_LIST, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetData(evtSelf, &evtParams);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_SesUnsubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
            _unlink(szFileName);
            rename(evtParams.szFileName, szFileName);
        }
    }

    iRet = JADA_SesUnsubscribeEvent(sesSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iRet);
}

DllExport int __stdcall JADA_SesGetListCounters(JADA_Session sesSelf)
{
    int             iRet;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    iRet = JADA_SendSessionMessage(sesSelf, JADA_MSG_GET_LIST_COUNTERS, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_SesRaiseEvent(JADA_Session sesSelf, JADA_Event evtSelf)
{
    int                     iRet, iType;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_Event              evtNewSelf;
    JADA_EventSubscriber    subSelf;
    CORE_Currency           curS;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    iRet = JADA_EvtGetType(evtSelf, &iType);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");

    iRet = JADA_LstGetFirst(sesPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    while (iRet == 0) {
        if (subSelf.iType == iType) {
            evtNewSelf = JADA_EvtDuplicate(__FILE__, __LINE__, evtSelf);
            CORE_ReturnValIfFail(evtNewSelf != NULL, -1, ;, "errore nella duplicazione dell'evento");
            iRet = JADA_LstAdd(sesPtr->lstSubscribedEvents, &evtNewSelf, sizeof(evtNewSelf), NULL);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");
            break;
        }

        iRet = JADA_LstGetNext(sesPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    }

    iRet = JADA_LstAdd(sesPtr->lstEvents, &evtSelf, sizeof(evtSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");

    return(0);
}

DllExport int __stdcall JADA_SesGetNextEvent(JADA_Session sesSelf, int iMilliSecs, JADA_Event *evtSelf)
{
    int             iRet;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    iRet = JADA_LstWait(sesPtr->lstEvents, evtSelf, sizeof(*evtSelf), iMilliSecs);
    if (iRet == 0) {
        iRet = 0;
    }

    return(iRet);
}

DllExport int __stdcall JADA_SesGetParameter(JADA_Session sesSelf, char *szSection, char *szName, char *szValue, int iLength)
{
    int                     iRet;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_Parameter          parSelf;
    CORE_Currency           curP;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);
    memset(szValue, 0, iLength);

    iRet = JADA_LstGetFirst(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    while (iRet == 0) {
        if (strcmp(parSelf.szName, szName) == 0) {
            memset(szValue, 0, iLength);
            strncpy(szValue, parSelf.szValue, strlen(parSelf.szValue));
            return(0);
        }

        iRet = JADA_LstGetNext(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    }

    return(-1);
}

DllExport int __stdcall JADA_SesGetParameterInt(JADA_Session sesSelf, char *szSection, char *szName, int *piValue)
{
    int             iRet;
    ShortString     szTmp;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    *piValue = 0;
    iRet = JADA_SesGetParameter(sesSelf, szSection, szName, szTmp, sizeof(szTmp));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del parametro di sessione (int)");
    *piValue = atoi(szTmp);

    return(0);
}

DllExport int __stdcall JADA_SesSetParameter(JADA_Session sesSelf, char *szSection, char *szName, char *szValue)
{
    int                     iRet;
    JADA_SesSelf            *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_Parameter          parSelf;
    CORE_Currency           curP;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    iRet = JADA_LstGetFirst(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    while (iRet == 0) {
        if (strcmp(parSelf.szName, szName) == 0) {
            strcpy(parSelf.szValue, szValue);
            iRet = JADA_LstUpdate(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");
            if (parSelf.iIniFile > 0) {
                if ((parSelf.iIniWhoAmI == 0) || (parSelf.iIniWhoAmI == JADA_GetWhoAmI())) {
                    iRet = JADA_IniSetString(parSelf.szSection, parSelf.szName, parSelf.szValue);
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del parametro nel file .ini");
                }
            }
            return(0);
        }

        iRet = JADA_LstGetNext(sesPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    }

    return(-1);
}

DllExport int __stdcall JADA_SesSetParameterInt(JADA_Session sesSelf, char *szSection, char *szName, int iValue)
{
    int             iRet;
    ShortString     szTmp;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);  

    sprintf(szTmp, "%d", iValue);
    iRet = JADA_SesSetParameter(sesSelf, szSection, szName, szTmp);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del parametro di sessione (int)");

    return(0);
}

int JADA_SesEnqueueFtp(JADA_Session sesSelf, JADA_Id iRequestId, char *szLocalFile, char *szRemoteFile)
{
    int             iRet;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;
    JADA_FtpSelf    ftpSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    memset(&ftpSelf, 0, sizeof(ftpSelf));
    ftpSelf.iRequestId = iRequestId;
    strcpy(ftpSelf.szFileFrom, szLocalFile);
    strcpy(ftpSelf.szFileTo, szRemoteFile);
    JADA_ChangeStringChar(ftpSelf.szFileFrom, '\\', '/');
    JADA_ChangeStringChar(ftpSelf.szFileTo, '\\', '/');
    ftpSelf.iMagic = JADA_MAGIC_FTP;

    iRet = JADA_LstAdd(sesPtr->lstFtp, &ftpSelf, sizeof(ftpSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiunta in lista del descrittore FTP");

    return(0);
}

int JADA_SesCancelFtp(JADA_Session sesSelf)
{
    int             iRet;
    JADA_SesSelf    *sesPtr = (JADA_SesSelf *) sesSelf;

    JADA_ReturnValIfInvalidSession(sesPtr, -1);

    iRet = JADA_LstClear(sesPtr->lstFtp);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella cancellazione della lista dei descrittori FTP");

    return(0);
}
