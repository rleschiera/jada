#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"

static JADA_ParameterEntry ConnectionParams[] = {
    /* Parametri di connessione */
    "connessione",  "username",     1,  0,                   /* Username usato in fase di connessione */
    "connessione",  "password",     1,  0,                   /* Username usata in fase di connessione */
    "connessione",  "nome",         1,  0,                   /* Nome identificativo dell'utente */
    "connessione",  "telefono",     1,  0,                   /* Telefono fisso dell'utente */
    "connessione",  "cellulare",    1,  0,                   /* Telefono cellulare dell'utente */
    "connessione",  "mail",         1,  0,                   /* Indirizzo e-mail dell'utente */
    "connessione",  "regione",      1,  JADA_WHOAMI_AGENT,   /* Regione a cui appartiene il cliente */
    "connessione",  "cliente",      1,  JADA_WHOAMI_AGENT,   /* Nome del cliente a cui appartiene l'utente */
    "connessione",  "coda",         0,  JADA_WHOAMI_MANAGER, /* Coda su cui il manager accetta richieste */
    "connessione",  "audio",        1,  0,                   /* Indica se l'utente puo' usare l'audio */
    /* Parametri di richiesta */
    "richiesta",    "coda",         1,  JADA_WHOAMI_AGENT,   /* Coda su cui e' stata instradata la richiesta di supporto */
    "richiesta",    "descrizione",  1,  JADA_WHOAMI_AGENT,   /* Descrizione dettagliata della richiesta di supporto */
    "richiesta",    "partner",      0,  0,                   /* Nome identificativo del partner di sessione dell'utente  */
    "richiesta",    "cliente",      0,  0,                   /* Nome del cliente da cui proviene la richiesta */
    "richiesta",    "telefono",     0,  0,                   /* Telefono fisso dell'utente da cui proviene la richiesta */
    "richiesta",    "cellulare",    0,  0,                   /* Telefono cellulare dell'utente da cui proviene la richiesta */
    "richiesta",    "mail",         0,  0,                   /* Indirizzo e-mail dell'utente da cui proviene la richiesta */
    "richiesta",    "audio",        0,  0,                   /* Indica se l'utente da cui proviene la richiesta puo' usare l'audio */
    "richiesta",    "password",     0,  0,                   /* Password utilizzata per la connessione al servizio VNC */
    /* Parametri di postazione */
    "postazione",   "host",         0,  0,
    "postazione",   "IP",           0,  0,
    "postazione",   "tipo OS",      0,  0,
    "postazione",   "computer",     0,  0,
    "postazione",   "domain user",  0,  0,
    "postazione",   "BIOS",         0,  0,
    "postazione",   "CPU",          0,  0,
    "postazione",   "RAM",          0,  0,
    "postazione",   "versione OS",  0,  0,
    "postazione",   "schermo",      0,  0,
    NULL,           NULL,          -1, -1
};


int JADA_ConnSubscribeEvent(JADA_Connection connSelf, int iEventType)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_EventSubscriber    subSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    memset(&subSelf, 0, sizeof(subSelf));
    subSelf.iType = iEventType;

    iRet = JADA_LstAdd(connPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");

    return(0);
}

int JADA_ConnUnsubscribeEvent(JADA_Connection connSelf, int iEventType)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_EventSubscriber    subSelf;
    CORE_Currency           curS;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstGetFirst(connPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    while (iRet == 0) {
        if (subSelf.iType == iEventType) {
            iRet = JADA_LstDelete(connPtr->lstEventSubscribers, &curS);
            break;
        }

        iRet = JADA_LstGetNext(connPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    }

    return(0);
}

int JADA_ConnWaitSubscribedEvent(JADA_Connection connSelf, int iEventType, int iSecs, JADA_Event *evtSelf)
{
    int             iRet, myEventType;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstWait(connPtr->lstSubscribedEvents, evtSelf, sizeof(*evtSelf), iSecs * 1000);
    if (iRet == 0) {
        iRet = JADA_EvtGetType(*evtSelf, &myEventType);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");
        if (iEventType == myEventType) {
            return(0);
        }
    }

    return(-1);
}

DllExport JADA_Connection __stdcall JADA_ConnNew(char *szFile, int iLine, int iTestMode)
{
    int             iRet, i;
    JADA_ConnSelf   *connPtr;
    JADA_Parameter  parSelf;

    connPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_ConnSelf));
    CORE_ReturnValIfFail(connPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    JADA_ClearJadaId(&connPtr->iId);
    connPtr->iStatus = JADA_CONNSTATUS_AVAILABLE;
    connPtr->iTestMode = iTestMode;
    time(&connPtr->timLastActivity);
    connPtr->iMagic = JADA_MAGIC_CONNECTION;

    connPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Parameter), NULL);
    CORE_ReturnValIfFail(connPtr->lstParameters != NULL, NULL, ;, "Errore nella creazione della lista di eventi");
    connPtr->lstEvents = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Event), NULL);
    CORE_ReturnValIfFail(connPtr->lstEvents != NULL, NULL, ;, "Errore nella creazione della lista di eventi");
    connPtr->lstEventSubscribers = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_EventSubscriber), NULL);
    CORE_ReturnValIfFail(connPtr->lstEventSubscribers != NULL, NULL, ;, "Errore nella creazione della lista dei sottoscrittori di eventi");
    connPtr->lstSubscribedEvents = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Event), NULL);
    CORE_ReturnValIfFail(connPtr->lstSubscribedEvents != NULL, NULL, ;, "Errore nella creazione della lista di eventi sottoscritti");
    connPtr->lstStreams = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Stream), NULL);
    CORE_ReturnValIfFail(connPtr->lstStreams != NULL, NULL, ;, "Errore nella creazione della lista di stream");

    for (i = 0; ConnectionParams[i].szSection != NULL; i++) {
        memset(&parSelf, 0, sizeof(parSelf));
        strcpy(parSelf.szSection, ConnectionParams[i].szSection);
        strcpy(parSelf.szName, ConnectionParams[i].szName);
        if (ConnectionParams[i].iIniFile > 0) {
            iRet = JADA_IniGetString(parSelf.szSection, parSelf.szName, parSelf.szValue, sizeof(parSelf.szValue));
            CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del parametro dal file .ini");
        }
        parSelf.iIniFile = ConnectionParams[i].iIniFile;
        parSelf.iIniWhoAmI = ConnectionParams[i].iIniWhoAmI;
        parSelf.iUpdated = TRUE;

        iRet = JADA_LstAdd(connPtr->lstParameters, &parSelf, sizeof(parSelf), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nell'aggiunta in lista del parametro");
    }

    iRet = JADA_ConnSetParameter((JADA_Connection) connPtr, "postazione", "tipo OS", "Windows");
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nel settaggio del parametro 'tipo OS'");

    return((JADA_Connection) connPtr);
}

DllExport int __stdcall JADA_ConnDestroy(char *szFile, int iLine, JADA_Connection connSelf)
{
    int             iRet;
    JADA_Event      evtSelf;
    JADA_Stream     stmSelf;
    CORE_Currency   curC;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_ConnClose(connSelf);

    iRet = JADA_LstDestroy(__FILE__, __LINE__, connPtr->lstParameters);

    iRet = JADA_LstGetFirst(connPtr->lstEvents, &evtSelf, sizeof(evtSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_EvtDestroy(__FILE__, __LINE__, evtSelf);
        iRet = JADA_LstGetNext(connPtr->lstEvents, &evtSelf, sizeof(evtSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, connPtr->lstEvents);

    iRet = JADA_LstDestroy(__FILE__, __LINE__, connPtr->lstEventSubscribers);

    iRet = JADA_LstGetFirst(connPtr->lstSubscribedEvents, &evtSelf, sizeof(evtSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_EvtDestroy(__FILE__, __LINE__, evtSelf);
        iRet = JADA_LstGetNext(connPtr->lstSubscribedEvents, &evtSelf, sizeof(evtSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, connPtr->lstSubscribedEvents);

    iRet = JADA_LstGetFirst(connPtr->lstStreams, &stmSelf, sizeof(stmSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
        iRet = JADA_LstGetNext(connPtr->lstStreams, &stmSelf, sizeof(stmSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, connPtr->lstStreams);

    JADA_Free(__FILE__, __LINE__, connPtr, sizeof(*connPtr));

    return(0);
}

static int JADA_ConnReset(JADA_ConnSelf *connPtr)
{
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (connPtr->sokConn.iSocket > 0) {
        closesocket(connPtr->sokConn.iSocket);
    }
    JADA_ClearJadaId(&connPtr->iId);
    connPtr->iStatus = JADA_CONNSTATUS_AVAILABLE;
    connPtr->sokConn.iSocket = -1;
    time(&connPtr->timLastActivity);
    connPtr->iMagic = JADA_MAGIC_CONNECTION;

    return(0);
}

DllExport int __stdcall JADA_ConnConnect(JADA_Connection connSelf, int iSessionType, int iWaitSeconds)
{
    int             iRet, iResult = -1;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;
    int             iPort;
    pthread_t       pThreadId;
    pthread_attr_t  attr;
    JADA_Event      evtSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;

    connPtr->iType = iSessionType;
    //switch (connPtr->iType) {
    //case JADA_SESSTYPE_FTP:
    //    strcpy(connPtr->szServerType, "ftp");
    //    break;
    //case JADA_SESSTYPE_CALL:
    //    strcpy(connPtr->szServerType, "call");
    //    break;
    //case JADA_SESSTYPE_SUPPORT:
    //    strcpy(connPtr->szServerType, "support");
    //    break;
    //case JADA_SESSTYPE_VNC:
    //    strcpy(connPtr->szServerType, "vnc");
    //    break;
    //default:
    //    CORE_ReturnValIfFail(0, -1, ;, "Errore generico");
    //}

    iRet = JADA_ServerGetString("server", "ip", connPtr->szServerName, sizeof(connPtr->szServerName));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del nome del server dal file .ini");
    CORE_ReturnValIfFail(strlen(connPtr->szServerName) > 0, -1, ;, "Errore nella lettura del nome del server dal file .ini");
    iRet = JADA_ServerGetInt("server", "port", &iPort);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura della porta IP del server dal file .ini");

    iRet = JADA_TcpConnectToPort(connPtr->szServerName, iPort, &connPtr->sokConn);
    if (iRet == JADA_PROXY_AUTHENTICATE) {
        return(iRet);
    }
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnSubscribeEvent(connSelf, JADA_EVT_CONNECTION_CONNECTED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    connPtr->iStatus = JADA_CONNSTATUS_CONNECTING; 
    connPtr->iReceiverThreadStatus = 1;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&pThreadId, &attr, JADA_ConnectionReceiverThread, connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, closesocket(connPtr->sokConn.iSocket);, "Errore generico");

    connPtr->iSenderThreadStatus = 1;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&pThreadId, &attr, JADA_ConnectionSenderThread, connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, closesocket(connPtr->sokConn.iSocket);, "Errore generico");

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(connSelf, JADA_EVT_CONNECTION_CONNECTED, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetResult(evtSelf, &iResult);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_CONNECTION_CONNECTED);, "Errore generico");
        }

        iRet = JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_CONNECTION_CONNECTED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    return(iResult);
}

DllExport int __stdcall JADA_ConnOpen(JADA_Connection connSelf, int iWaitSeconds)
{
    int                     iRet, iResult = -1;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_MsgOpenConnection  msgOpenConnection;
    JADA_Event              evtSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;
    CORE_ReturnValIfFail(connPtr->iStatus = JADA_CONNSTATUS_CONNECTED, -1, ;, "Apertura connessione senza connect");
    ;

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnSubscribeEvent(connSelf, JADA_EVT_CONNECTION_OPENED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    memset(&msgOpenConnection, 0, sizeof(msgOpenConnection));
    msgOpenConnection.iConnectionType = connPtr->iType;
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "nome", msgOpenConnection.szNome, sizeof(msgOpenConnection.szNome));
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "cliente", msgOpenConnection.szCliente, sizeof(msgOpenConnection.szCliente));
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "regione", msgOpenConnection.szRegione, sizeof(msgOpenConnection.szRegione));
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "mail", msgOpenConnection.szMail, sizeof(msgOpenConnection.szMail));
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "telefono", msgOpenConnection.szTelefono, sizeof(msgOpenConnection.szTelefono));
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "cellulare", msgOpenConnection.szCellulare, sizeof(msgOpenConnection.szCellulare));
    iRet = JADA_ConnGetParameterInt(connSelf, "connessione", "audio", &msgOpenConnection.iUsaAudio);
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "username", msgOpenConnection.szUsername, sizeof(msgOpenConnection.szUsername));
    iRet = JADA_ConnGetParameter(connSelf, "connessione", "password", msgOpenConnection.szPassword, sizeof(msgOpenConnection.szPassword));
    iRet = JADA_ConnGetParameter(connSelf, "postazione", "IP", msgOpenConnection.szHostIP, sizeof(msgOpenConnection.szHostIP));
    iRet = JADA_SendConnectionMessage((JADA_Connection) connPtr, JADA_MSG_OPEN_CONNECTION, &msgOpenConnection, sizeof(msgOpenConnection));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella definizione del messaggio OPEN_CONNECTION");
    connPtr->iStatus = JADA_CONNSTATUS_OPENING;

    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(connSelf, JADA_EVT_CONNECTION_OPENED, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetResult(evtSelf, &iResult);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_CONNECTION_OPENED);, "Errore generico");
        }

        iRet = JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_CONNECTION_OPENED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    if (iResult == JADA_CREDENTIAL_OK) {
        connPtr->iStatus = JADA_CONNSTATUS_OPENED; 
    }
    else {
        connPtr->iStatus = JADA_CONNSTATUS_CONNECTED;
    }

    return(iResult);
}

DllExport int __stdcall JADA_ConnClose(JADA_Connection connSelf)
{
    JADA_ConnSelf    *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    connPtr->iReceiverThreadStatus++;
    //if (connPtr->sokConn.iSocket > 0) {
    //    closesocket(connPtr->sokConn.iSocket);
    //}

    return(0);
}

int JADA_SendConnectionMessage(JADA_Connection connSelf, int iOpcode, void *pPayload, int iSize)
{
    int                 iRet;
    JADA_ConnSelf       *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_Message        msgOut;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    /* Crea il messaggio in output */
    msgOut = JADA_MsgConnNew(__FILE__, __LINE__, JADA_GetWhoAmI(), connPtr->iId, iOpcode, iSize);
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

DllExport int __stdcall JADA_ConnListRequests(JADA_Connection connSelf, char *szFilterCoda, int *piStreamId, int iWaitSeconds)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_MsgListRequests    outMsg;
    JADA_Event              evtSelf;
    JADA_EventData          evtParams;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;

    memset(&outMsg, 0, sizeof(outMsg));
    strcpy(outMsg.szCoda, szFilterCoda);

    iRet = JADA_ConnSubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_SendConnectionMessage(connSelf, JADA_MSG_LIST_REQUESTS, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
 
    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(connSelf, JADA_EVT_REQUESTS_LIST, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetData(evtSelf, &evtParams);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
            *piStreamId = evtParams.iStreamId; 
        }
    }

    iRet = JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iRet);
}

DllExport int __stdcall JADA_ConnListPendingRequests(JADA_Connection connSelf, char *szFilterCoda, int *piStreamId, int iWaitSeconds)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_MsgListRequests    outMsg;
    JADA_Event              evtSelf;
    JADA_EventData          evtParams;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;

    memset(&outMsg, 0, sizeof(outMsg));
    strcpy(outMsg.szCoda, szFilterCoda);

    iRet = JADA_ConnSubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_SendConnectionMessage(connSelf, JADA_MSG_LIST_PENDING_REQUESTS, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
 
    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(connSelf, JADA_EVT_REQUESTS_LIST, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetData(evtSelf, &evtParams);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);, "Errore generico");
            *piStreamId = evtParams.iStreamId; 
        }
    }

    iRet = JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_REQUESTS_LIST);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iRet);
}

DllExport int __stdcall JADA_ConnGetListCounters(JADA_Connection connSelf)
{
    int             iRet;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_SendConnectionMessage(connSelf, JADA_MSG_GET_LIST_COUNTERS, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_ConnRaiseEvent(JADA_Connection connSelf, JADA_Event evtSelf)
{
    int                     iRet, iType;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_Event              evtNewSelf;
    JADA_EventSubscriber    subSelf;
    CORE_Currency           curS;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_EvtGetType(evtSelf, &iType);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");

    iRet = JADA_LstGetFirst(connPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    while (iRet == 0) {
        if (subSelf.iType == iType) {
            evtNewSelf = JADA_EvtDuplicate(__FILE__, __LINE__, evtSelf);
            CORE_ReturnValIfFail(evtNewSelf != NULL, -1, ;, "errore nella duplicazione dell'evento");
            iRet = JADA_LstAdd(connPtr->lstSubscribedEvents, &evtNewSelf, sizeof(evtNewSelf), NULL);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");
            break;
        }

        iRet = JADA_LstGetNext(connPtr->lstEventSubscribers, &subSelf, sizeof(subSelf), &curS);
    }

    iRet = JADA_LstAdd(connPtr->lstEvents, &evtSelf, sizeof(evtSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista dell'evento");

    return(0);
}

DllExport int __stdcall JADA_ConnGetNextEvent(JADA_Connection connSelf, int iMilliSecs, JADA_Event *evtSelf)
{
    int             iRet;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_LstWait(connPtr->lstEvents, evtSelf, sizeof(*evtSelf), iMilliSecs);
    if (iRet == 0) {
        iRet = 0;
    }

    return(iRet);
}

DllExport int __stdcall JADA_ConnGetParameter(JADA_Connection connSelf, char *szSection, char *szName, char *szValue, int iLength)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_Parameter          parSelf;
    CORE_Currency           curP;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);
    memset(szValue, 0, iLength);

    iRet = JADA_LstGetFirst(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    while (iRet == 0) {
        if ((strcmp(parSelf.szSection, szSection) == 0) && (strcmp(parSelf.szName, szName) == 0)) {
            memset(szValue, 0, iLength);
            strncpy(szValue, parSelf.szValue, strlen(parSelf.szValue));
            return(0);
        }

        iRet = JADA_LstGetNext(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    }

    return(-1);
}

DllExport int __stdcall JADA_ConnGetParameterInt(JADA_Connection connSelf, char *szSection, char *szName, int *piValue)
{
    int             iRet;
    ShortString     szTmp;

    *piValue = 0;
    iRet = JADA_ConnGetParameter(connSelf, szSection, szName, szTmp, sizeof(szTmp));
    if (iRet == 0) {
        *piValue = atoi(szTmp);
    }

    return(0);
}

DllExport int __stdcall JADA_ConnSetParameter(JADA_Connection connSelf, char *szSection, char *szName, char *szValue)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_Parameter          parSelf;
    CORE_Currency           curP;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_LstGetFirst(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    while (iRet == 0) {
        if ((strcmp(parSelf.szSection, szSection) == 0) && (strcmp(parSelf.szName, szName) == 0)) {
            memset(parSelf.szValue, 0, sizeof(parSelf.szValue));
            strncpy(parSelf.szValue, szValue, sizeof(parSelf.szValue) - 1);
            parSelf.iUpdated = TRUE;
            iRet = JADA_LstUpdate(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");
            if (parSelf.iIniFile > 0) {
                if ((parSelf.iIniWhoAmI == 0) || (parSelf.iIniWhoAmI == JADA_GetWhoAmI())) {
                    iRet = JADA_IniSetString(parSelf.szSection, parSelf.szName, parSelf.szValue);
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del parametro nel file .ini");
                }
            }
            // JADA_LogMessage("Aggiornato parametro: %s:%s * %s\n", szSection, szName, szValue);

            return(0);
        }

        iRet = JADA_LstGetNext(connPtr->lstParameters, &parSelf, sizeof(parSelf), &curP);
    }

    return(-1);
}

DllExport int __stdcall JADA_ConnSetParameterInt(JADA_Connection connSelf, char *szSection, char *szName, int iValue)
{
    int             iRet;
    ShortString     szTmp;

    sprintf(szTmp, "%d", iValue);
    iRet = JADA_ConnSetParameter(connSelf, szSection, szName, szTmp);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura del parametro di sessione (int)");

    return(0);
}

DllExport int __stdcall JADA_ConnCopyParameterValue(JADA_Connection connSelf, char *szFromSection, char *szFromName, char *szToSection, char *szToName)
{
    int                     iRet;
    FixString               szValue;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);

    iRet = JADA_ConnGetParameter(connSelf, szFromSection, szFromName, szValue, sizeof(szValue));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");
    iRet = JADA_ConnSetParameter(connSelf, szToSection, szToName, szValue);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");

    return(0);
}

DllExport int __stdcall JADA_ConnGetCombo(JADA_Connection connSelf, int iComboType, char *szFilter, int *piStreamId, int iWaitSeconds)
{
    int                     iRet, iResult = -1;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;
    JADA_MsgGetCombo        outMsg;
    JADA_Event              evtSelf;
    JADA_EventData          evtParams;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    if (connPtr->iTestMode) iWaitSeconds = 86400;

    memset(&outMsg, 0, sizeof(outMsg));
    outMsg.iComboType = iComboType;
    strcpy(outMsg.szFilter, szFilter);

    iRet = JADA_ConnSubscribeEvent(connSelf, JADA_EVT_COMBO_VALUES);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_SendConnectionMessage(connSelf, JADA_MSG_GET_COMBO, &outMsg, sizeof(outMsg));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_COMBO_VALUES);, "Errore generico");
 
    if (iWaitSeconds > 0) {
        iRet = JADA_ConnWaitSubscribedEvent(connSelf, JADA_EVT_COMBO_VALUES, iWaitSeconds, &evtSelf);
        if (iRet == 0) {
            iRet = JADA_EvtGetData(evtSelf, &evtParams);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_COMBO_VALUES);, "Errore generico");
            *piStreamId = evtParams.iStreamId;
            iResult = 0;
        }
    }

    iRet = JADA_ConnUnsubscribeEvent(connSelf, JADA_EVT_COMBO_VALUES);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iResult);
}

DllExport int __stdcall JADA_ConnGetSetting(JADA_Connection connSelf, char *szKey, char *szValue, int iLength)
{
    int                     iRet;
    JADA_ConnSelf           *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_ServerGetString("settings", szKey, szValue, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iRet);
}

DllExport int __stdcall JADA_ConnAddStream(JADA_Connection connSelf, JADA_Stream stmSelf)
{
    int             iRet;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_LstAdd(connPtr->lstStreams, &stmSelf, sizeof(stmSelf), &curC);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport JADA_Stream __stdcall JADA_ConnGetStream(JADA_Connection connSelf, int iStreamId)
{
    int             iRet, iCurrId;
    JADA_Stream     stmSelf;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);  

    iRet = JADA_LstGetFirst(connPtr->lstStreams, &stmSelf, sizeof(stmSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_StmGetId(stmSelf, &iCurrId);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
        if (iStreamId == iCurrId) {
            iRet = JADA_LstDelete(connPtr->lstStreams, &curC);
            CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
            return(stmSelf);
        }

        iRet = JADA_LstGetNext(connPtr->lstStreams, &stmSelf, sizeof(stmSelf), &curC);
    }

    return(NULL);
}
