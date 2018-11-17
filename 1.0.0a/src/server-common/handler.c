#include "jada.h"
#include "jada_internal.h"
#include "server.h"

/******************************************************************************
 *
 *      C O M M O N     S E R V I C E S
 *
 *****************************************************************************/
int ASRV_ForwardMessage(JADA_Message msgIn, JADA_ConnId iFromConnectionId, JADA_SessId iSessionId) 
{
    int             iRet;
    ASRV_Session    *sesPtr;
    ASRV_Connection *connAgent;
    ASRV_Connection *connManager;

    sesPtr = ASRV_GetSessionById(iSessionId);
    if (sesPtr != NULL) {
        connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
        if (connAgent != NULL) {
            connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
            if (connManager != NULL) {
                // JADA_LogMessage("%d: frame length %d\n", msgHeader.iType, msgHeader.iLength);
                if (JADA_CompareJadaId(&connAgent->dbConn.iId, &iFromConnectionId)) {
                    iRet = ASRV_TcpSendMessage(&connManager->sokConn, msgIn);
                    if (iRet != 0) {
                        iRet = ASRV_SetSessionManager(sesPtr, NULL); 
                    }
                }
                else if (JADA_CompareJadaId(&connManager->dbConn.iId, &iFromConnectionId)) {
                    iRet = ASRV_TcpSendMessage(&connAgent->sokConn, msgIn);
                    if (iRet != 0) {
                        iRet = ASRV_SetSessionAgent(sesPtr, NULL);
                    }
                }
            }
        }
    }

    return(0);
}

/******************************************************************************
 *
 *      H I G H     L E V E L     H A N D L E R S
 *
 *****************************************************************************/
/* UUU Connection Message Handler */
int ASRV_HandleConnectionMessage(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe)
{
    int                     iRet;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    *piCloseMe = 0;
	switch (msgHeader.iOpcode) {
    case JADA_MSG_CONNECT_CONNECTION:
        iRet = ASRV_HandleConnectConnection(connPtr, msgRecv, piCloseMe);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_OPEN_CONNECTION:
        switch (msgHeader.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            iRet = ASRV_HandleOpenAgentConnection(connPtr, msgRecv, piCloseMe);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;
        case JADA_WHOAMI_MANAGER:
            iRet = ASRV_HandleOpenManagerConnection(connPtr, msgRecv, piCloseMe);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;
        }
        break;

    //case JADA_MSG_LOAD_SESSION:
    //    iRet = ASRV_HandleLoadSession(connPtr, msgRecv, piCloseMe);
    //    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    //    break;

    case JADA_MSG_LIST_REQUESTS:
        iRet = ASRV_HandleListRequests(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_LIST_PENDING_REQUESTS:
        iRet = ASRV_HandleListPendingRequests(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_GET_LIST_COUNTERS:
        iRet = ASRV_HandleGetListCounters(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CONN_PARAMETERS_LIST:
        iRet = ASRV_HandleConnectionParametersList(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_GET_COMBO:
        iRet = ASRV_HandleGetCombo(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_OPEN_SUPPORT:
        iRet = ASRV_HandleOpenSupport(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_ACCEPT_SUPPORT:
        iRet = ASRV_HandleAcceptSupport(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_LOAD_SUPPORT:
        iRet = ASRV_HandleLoadSupport(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CONNECT_SUPPORT:
        iRet = ASRV_HandleConnectSupport(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_RESUME_SUPPORT:
        iRet = ASRV_HandleResumeSupport(connPtr, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    default:
        JADA_LogMessage("Messaggio sulla connessione %d non valido (%s)\n",
                connPtr->dbConn.iId, JADA_MsgOpcodeToString(msgHeader.iOpcode));
        break;
    }

    return(0);
}

/******************************************************************************
 *
 *      C O N N E C T I O N     H A N D L E R S
 *
 *****************************************************************************/
int ASRV_HandleConnectConnection(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe)
{
    int                         iRet;
    JADA_MsgConnectConnection   msgIn;
    JADA_MsgConnectionConnected msgOut;
    JADA_SessionMsgHeader       msgHeader;
    struct sockaddr_in          myAddress;
    int                         iLength;

    *piCloseMe = 0;
    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Crea la connessione nel DB */
    strcpy(connPtr->dbConn.szHostIP, msgIn.szHostIP);   
    iLength = sizeof(myAddress);  
    iRet = getsockname(connPtr->sokConn.iSocket, (struct sockaddr *)&myAddress, &iLength);  
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
    strcpy(connPtr->dbConn.szRealIP, inet_ntoa(myAddress.sin_addr));
    iRet = ASRV_DbCreateConnessione(&connPtr->dbConn);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "salvataggio della connessione nel DB");
	JADA_LogMessage("Nuova connessione aperta da %s (host %s)\n", connPtr->dbConn.szRealIP, connPtr->dbConn.szHostIP);

    /* Invia il messaggio di risposta */
    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iResult = 0;
    msgOut.iConnectionId = connPtr->dbConn.iId;
    msgOut.timOpenTime = time(0);

    iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_CONNECTION_CONNECTED, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    //iRet = ASRV_HandleGetListCounters(sesPtr, msgRecv);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    ASRV_LogConnectConnection(connPtr);

    return(0);
}

static int ASRV_CheckCredentials(int iWhoAmI, char *szUtente, char *szPassword)
{
    int         iRet, iResult;
    FixString   szIniPassword;

    if (iWhoAmI == JADA_WHOAMI_AGENT)
        return(JADA_CREDENTIAL_OK);

    iRet = JADA_IniGetString("utenti", szUtente, szIniPassword, sizeof(szIniPassword));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    if (strlen(szIniPassword) == 0) {
        iResult = JADA_CREDENTIAL_UNKNOWN_USER;
    }
    else {
        if (strcmp(szIniPassword, "*") == 0) {
            strcpy(szIniPassword, szPassword);
            iRet = JADA_IniSetString("utenti", szUtente, szIniPassword);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        }
        if (strcmp(szIniPassword, szPassword) == 0) {
            iResult = JADA_CREDENTIAL_OK;
        }
        else {
            iResult = JADA_CREDENTIAL_WRONG_PASSWORD;
        }
    }

    return(iResult);
}

int ASRV_HandleOpenAgentConnection(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe)
{
    int                         iRet;
    JADA_MsgOpenConnection      msgIn;
    JADA_MsgConnectionOpened    msgOut;
    JADA_SessionMsgHeader       msgHeader;
    ASRV_DbAgent                myAgent;
    struct sockaddr_in          myAddress;
    int                         iLength;

    *piCloseMe = 0;
    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Se l'utente e' abilitato alla connessione... */
    iRet = ASRV_DbIsEnabledCliente(msgIn.szCliente, msgIn.szRegione);  
    if (iRet == 0) {

        /* Salva l'agent nel DB */
        memset(&myAgent, 0, sizeof(myAgent));
        strcpy(myAgent.szNome, msgIn.szNome);
        strcpy(myAgent.szMail, msgIn.szMail); 
        strcpy(myAgent.szTelefono, msgIn.szTelefono); 
        strcpy(myAgent.szCellulare, msgIn.szCellulare);
        myAgent.iUsaAudio = msgIn.iUsaAudio;
        strcpy(myAgent.szCliente, msgIn.szCliente);
        strcpy(myAgent.szRegione, msgIn.szRegione);
        iRet = ASRV_DbCreateAgent(&myAgent);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "salvataggio dell'agent nel DB");

        /* Aggiorna la connessione nel DB */
        connPtr->dbConn.iWhoAmI = JADA_WHOAMI_AGENT;
        connPtr->dbConn.iIdAgent = myAgent.iId;
        connPtr->dbConn.iUsaAudio = msgIn.iUsaAudio;
        strcpy(connPtr->dbConn.szHostIP, msgIn.szHostIP);   
        iLength = sizeof(myAddress);  
        iRet = getsockname(connPtr->sokConn.iSocket, (struct sockaddr *)&myAddress, &iLength);  
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
        strcpy(connPtr->dbConn.szRealIP, inet_ntoa(myAddress.sin_addr));
        iRet = ASRV_DbSetConnessione(&connPtr->dbConn);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "salvataggio della connessione nel DB");
	    JADA_LogMessage("Nuova connessione aperta da %s (host %s)\n", connPtr->dbConn.szRealIP, connPtr->dbConn.szHostIP);

        /* Invia il messaggio di risposta */
        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = JADA_CREDENTIAL_OK;
        msgOut.iConnectionId = connPtr->dbConn.iId;
        msgOut.timOpenTime = time(0);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_CONNECTION_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        //iRet = ASRV_HandleGetListCounters(sesPtr, msgRecv);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenAgentConnection(connPtr, &myAgent);
    }
    else {
        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = JADA_CREDENTIAL_UNKNOWN_USER;
        msgOut.iConnectionId = connPtr->dbConn.iId;
        msgOut.timOpenTime = time(0);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_CONNECTION_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenConnectionError(connPtr->dbConn.iId, myAgent.szCliente, "cliente non abilitato");
    }
    return(0);
}

int ASRV_HandleOpenManagerConnection(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe)
{
    int                             iRet, iResult;
    JADA_MsgOpenConnection          msgIn;
    JADA_MsgConnectionOpened        msgOut;
    JADA_SessionMsgHeader           msgHeader;
    ASRV_DbManager                  myManager;
    struct sockaddr_in              myAddress;
    int                             iLength;

    *piCloseMe = 0;
    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    /* Controlla la password */
    iRet = ASRV_DbCheckManager(&myManager, msgIn.szUsername, msgIn.szPassword, &iResult);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nel controllo delle credenziali del manager");
    switch (iResult) {
    case JADA_CREDENTIAL_OK:
        /* Aggiorna l'agent nel DB */
        strcpy(myManager.szNome, msgIn.szNome);
        strcpy(myManager.szMail, msgIn.szMail); 
        strcpy(myManager.szTelefono, msgIn.szTelefono); 
        strcpy(myManager.szCellulare, msgIn.szCellulare);
        myManager.iUsaAudio = msgIn.iUsaAudio;
        iRet = ASRV_DbSetManager(&myManager);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "salvataggio dell'agent nel DB");

        /* Aggiorna la connessione nel DB */
        connPtr->dbConn.iWhoAmI = JADA_WHOAMI_MANAGER;
        connPtr->dbConn.iIdManager = myManager.iId;
        connPtr->dbConn.iUsaAudio = msgIn.iUsaAudio;
        strcpy(connPtr->dbConn.szHostIP, msgIn.szHostIP);   
        iLength = sizeof(myAddress);  
        iRet = getsockname(connPtr->sokConn.iSocket, (struct sockaddr *)&myAddress, &iLength);  
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
        strcpy(connPtr->dbConn.szRealIP, inet_ntoa(myAddress.sin_addr));
        iRet = ASRV_DbSetConnessione(&connPtr->dbConn);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "salvataggio della connessione nel DB");

        /* Aggiorna la connessione nel descrittore */
	    JADA_LogMessage("Nuova connessione aperta da %s (host %s)\n", connPtr->dbConn.szRealIP, connPtr->dbConn.szHostIP);

        /* Invia il messaggio di risposta */
        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = JADA_CREDENTIAL_OK;
        msgOut.iConnectionId = connPtr->dbConn.iId;
        iRet = ASRV_DbGetManagerDefaultCoda(connPtr->dbConn.iIdManager, msgOut.szCoda, sizeof(msgOut.szCoda)); 
        CORE_LogIfFail(iRet == 0, "non riesco a leggere la coda di default");
        msgOut.timOpenTime = time(0);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_CONNECTION_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        iRet = ASRV_HandleGetListCounters(connPtr, NULL);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenManagerConnection(connPtr, &myManager);
        break;

    case JADA_CREDENTIAL_UNKNOWN_USER:
		JADA_LogMessage("L'utente %s non e' abilitato all'accesso\n", msgIn.szUsername);
        *piCloseMe = 1;

        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = JADA_CREDENTIAL_UNKNOWN_USER;
        msgOut.iConnectionId = connPtr->dbConn.iId;
        msgOut.timOpenTime = time(0);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_CONNECTION_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenConnectionError(connPtr->dbConn.iId, msgIn.szUsername, "utente sconosciuto");
        break;

    case JADA_CREDENTIAL_WRONG_PASSWORD:
		JADA_LogMessage("Password '%s' errata per l'utente %s\n", msgIn.szPassword, msgIn.szUsername);
        *piCloseMe = 1;

        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = JADA_CREDENTIAL_WRONG_PASSWORD;
        msgOut.iConnectionId = connPtr->dbConn.iId;
        msgOut.timOpenTime = time(0);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_CONNECTION_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogOpenConnectionError(connPtr->dbConn.iId, msgIn.szUsername, "password errata");
        break;
    }

    return(0);
}

int ASRV_HandleOpenSupport(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgOpenRequest     msgIn;
    JADA_MsgRequestOpened   msgOut;
    ASRV_Session            *sesPtr;
    ASRV_DbRichiesta        myRichiesta;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&myRichiesta, 0, sizeof(myRichiesta));
    strcpy(myRichiesta.szCoda, msgIn.szCoda); 
    strcpy(myRichiesta.szDescrizione, msgIn.szDescription);
    myRichiesta.iIdAgent = connPtr->dbConn.iIdAgent;
    iRet = ASRV_DbCreateRichiesta(&myRichiesta);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    sesPtr = ASRV_CreateSession(__FILE__, __LINE__, connPtr, JADA_SESSTYPE_SUPPORT, myRichiesta.iId);
    CORE_ReturnValIfFail(sesPtr != NULL, -1, ;, "Errore nella creazione della sessione");

    iRet = ASRV_DbSetRichiestaIdSessione(&sesPtr->dbSess);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iRequestId = sesPtr->dbReq.iId;
    msgOut.iSessionId = sesPtr->dbSess.iId;
    iRet = ASRV_GetVncPassword_DaButtare(0, msgOut.szPassword_DaButtare);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    msgOut.timOpenTime = time(0);

    iRet = ASRV_StartSession(sesPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_OPENED, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = ASRV_HandleBroadcastListCounters();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    ASRV_LogOpenSupport(sesPtr, &sesPtr->dbReq);

    return(0);
}

int ASRV_HandleAcceptSupport(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    ASRV_Session            *sesPtr;
    ASRV_DbRichiesta        myRichiesta;
    JADA_MsgAcceptRequest   msgIn;
    JADA_MsgRequestAccepted msgOut;
    JADA_MsgResult          msgErr;
    ASRV_DbAgent            myAgent;
	ASRV_DbManager          myManager;
    ASRV_Connection         *connAgent;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&myRichiesta, 0, sizeof(myRichiesta));
    myRichiesta.iId = msgIn.iRequestId;
    iRet = ASRV_DbGetRichiesta(&myRichiesta);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "descrittore di richiesta non trovato");

    sesPtr = ASRV_GetSessionByRequestId(myRichiesta.iId);
    if (sesPtr != NULL) {
        switch (myRichiesta.iStato) {
        case ASRV_REQSTATUS_OPENED:
            iRet = ASRV_SetSessionManager(sesPtr, connPtr);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento della sessione nel DB");
            iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_REQSTATUS_RUNNING);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nel cambio di stato della richiesta");
            iRet = ASRV_DbSetStatusSessione(&sesPtr->dbSess, ASRV_SESSSTATUS_RUNNING);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nel cambio di stato della richiesta");
            iRet = ASRV_DbSetManagerSessione(&sesPtr->dbSess, &sesPtr->dbSess.iIdConnManager);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento dello stato nel DB");

            connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
            CORE_ReturnValIfFail(ASRV_IsValidConnection(connAgent), -1, ;, "connessione all'agent non trovata");

	        iRet = ASRV_DbGetAgentByConnection(&myAgent, connAgent->dbConn.iId);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
		    myManager.iId = sesPtr->iIdManager; 
		    iRet = ASRV_DbGetManager(&myManager);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            memset(&msgOut, 0, sizeof(msgOut));
            msgOut.iRequestId = sesPtr->dbReq.iId;
            msgOut.iSessionId = sesPtr->dbSess.iId;
            msgOut.timAcceptTime = time(0);
            strcpy(msgOut.szCliente, myAgent.szCliente);
            strcpy(msgOut.szCoda, myRichiesta.szCoda);
            strcpy(msgOut.szDescription, myRichiesta.szDescrizione);

            strcpy(msgOut.szUtente, myManager.szNome);
            strcpy(msgOut.szTelefono, myManager.szTelefono);
            strcpy(msgOut.szCellulare, myManager.szCellulare);
            strcpy(msgOut.szMail, myManager.szMail);
            msgOut.iUsaAudio = connPtr->dbConn.iUsaAudio;
            iRet = ASRV_GetVncPassword_DaButtare(0, msgOut.szPassword_DaButtare);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_ACCEPTED, &msgOut, sizeof(msgOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            strcpy(msgOut.szUtente, myAgent.szNome);
            strcpy(msgOut.szTelefono, myAgent.szTelefono);
            strcpy(msgOut.szMail, myAgent.szMail);
            msgOut.iUsaAudio = connAgent->dbConn.iUsaAudio;
            iRet = ASRV_GetPassword_DaButtare(0, msgOut.szPassword_DaButtare);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_ACCEPTED, &msgOut, sizeof(msgOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            iRet = ASRV_SendCachedChat(sesPtr); 
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            iRet = ASRV_HandleBroadcastListCounters();
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            ASRV_LogAcceptSupport(sesPtr, myRichiesta.iId, &myRichiesta);
            break;

        default:
		    JADA_LogMessage("Ricevuta accettazione richiesta %d da connessione %d ma lo stato non e' OPENED (%d)\n",
                    myRichiesta.iId, connPtr->dbConn.iId, myRichiesta.iStato);
            memset(&msgErr, 0, sizeof(msgErr));
            msgOut.iResult = -1;
            sprintf(msgErr.szDescription, "Richiesta non accettata: la richiesta non e' in stato OPENED (%d)", 
                    myRichiesta.iStato);

            iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            ASRV_LogAcceptSupportError(sesPtr, myRichiesta.iId, &myRichiesta, "stato non valido");
            break;
        }
    }
    else {
		JADA_LogMessage("Ricevuta accettazione richiesta %d da connessione %d ma la sessione non esiste\n",
                myRichiesta.iId, connPtr->dbConn.iId);
        memset(&msgErr, 0, sizeof(msgErr));
        msgOut.iResult = -1;
        sprintf(msgErr.szDescription, "Richiesta non accettata: la richiesta non esiste");

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogAcceptSupportError(sesPtr, myRichiesta.iId, &myRichiesta, "stato non valido");
    }

    iRet = ASRV_HandleBroadcastListCounters();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_HandleLoadSupport(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    ASRV_Session            *sesPtr;
    ASRV_DbRichiesta        myRichiesta;
    ASRV_DbAgent            myAgent;
    JADA_MsgLoadRequest     msgIn;
    JADA_MsgRequestLoaded   msgOut;
    JADA_MsgResult          msgErr;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&myRichiesta, 0, sizeof(myRichiesta));
    myRichiesta.iId = msgIn.iRequestId; 
    iRet = ASRV_DbGetRichiesta(&myRichiesta);
    if (iRet == 0) {

        sesPtr = ASRV_CreateSession(__FILE__, __LINE__, connPtr, ASRV_SESSION_TYPE_SUPPORT, myRichiesta.iId);
        CORE_ReturnValIfFail(sesPtr != NULL, -1, ;, "Errore generico");
        iRet = ASRV_SetSessionManager(sesPtr, connPtr);

        sesPtr->dbSess.iIdRichiesta = myRichiesta.iId;
        iRet = ASRV_DbSetRichiestaIdSessione(&sesPtr->dbSess);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_REQSTATUS_LOADED);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iRequestId = myRichiesta.iId;
        iRet = ASRV_GetPassword_DaButtare(0, msgOut.szPassword_DaButtare);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        msgOut.timLoadTime = time(0);
        strcpy(msgOut.szCoda, myRichiesta.szCoda); 
        strcpy(msgOut.szDescription, myRichiesta.szDescrizione); 

        memset(&myAgent, 0, sizeof(myAgent));
        myAgent.iId = myRichiesta.iIdAgent; 
        iRet = ASRV_DbGetAgent(&myAgent);
        if (iRet == 0) {
            strcpy(msgOut.szUtente, myAgent.szNome); 
            strcpy(msgOut.szCliente, myAgent.szCliente); 
            strcpy(msgOut.szTelefono, myAgent.szTelefono); 
            strcpy(msgOut.szCellulare, myAgent.szCellulare); 
            strcpy(msgOut.szMail, myAgent.szMail); 
        }

        iRet = ASRV_StartSession(sesPtr);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_LOADED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogLoadSupport(sesPtr, myRichiesta.iId, &myRichiesta);
    }
    else {
		JADA_LogMessage("Ricevuto caricamento richiesta %d da connessione %d ma la richiesta non esiste\n",
                myRichiesta.iId, connPtr->dbConn.iId);
        memset(&msgErr, 0, sizeof(msgErr));
        msgOut.iResult = -1;
        sprintf(msgErr.szDescription, "Caricamento richiesta non accettato: la richiesta non esiste");

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogLoadSupportError(connPtr, myRichiesta.iId, &myRichiesta, "non esiste");
    }

    iRet = ASRV_HandleBroadcastListCounters();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_HandleConnectSupport(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                         iRet;
    ASRV_Session                *sesPtr;
    JADA_MsgConnectRequest      msgIn;
    JADA_MsgRequestConnected    msgOut;
    JADA_MsgResult              msgErr;
    ASRV_DbAgent                myAgent;
    ASRV_DbManager              myManager;
    ASRV_Connection             *connAgent, *connManager;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    sesPtr = ASRV_GetSessionByRequestId(msgIn.iRequestId); 
    if (sesPtr != NULL) {
        if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_LOADED) {
            iRet = ASRV_SetSessionAgent(sesPtr, connPtr);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento della sessione nel DB");
            iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_REQSTATUS_RUNNING);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento dello stato nel DB");
            iRet = ASRV_DbSetStatusSessione(&sesPtr->dbSess, ASRV_SESSSTATUS_RUNNING);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento dello stato nel DB");
            iRet = ASRV_DbSetAgentSessione(&sesPtr->dbSess, &sesPtr->dbSess.iIdConnAgent);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento dello stato nel DB");

            memset(&msgOut, 0, sizeof(msgOut));
            msgOut.iRequestId = sesPtr->dbReq.iId;
            iRet = ASRV_GetVncPassword_DaButtare(0, msgOut.szPassword_DaButtare);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            msgOut.timConnectTime = time(0);
            strcpy(msgOut.szCoda, sesPtr->dbReq.szCoda);
            strcpy(msgOut.szDescription, sesPtr->dbReq.szDescrizione);

            connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent); 
            connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager); 

            if (ASRV_IsValidConnection(connManager)) {
                iRet = ASRV_DbGetManagerByConnection(&myManager, connManager->dbConn.iId);
                if (iRet == 0) {
                    strcpy(msgOut.szUtente, myManager.szNome);
                    strcpy(msgOut.szTelefono, myManager.szTelefono);
                    strcpy(msgOut.szCellulare, myManager.szCellulare);
                    strcpy(msgOut.szMail, myManager.szMail);
                    msgOut.iUsaAudio = connManager->dbConn.iUsaAudio;
                }
            }
            iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_CONNECTED, &msgOut, sizeof(msgOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            iRet = ASRV_GetPassword_DaButtare(0, msgOut.szPassword_DaButtare);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            if (ASRV_IsValidConnection(connAgent)) {
                iRet = ASRV_DbGetAgentByConnection(&myAgent, connAgent->dbConn.iId);
                if (iRet == 0) {
                    strcpy(msgOut.szUtente, myAgent.szNome);
                    strcpy(msgOut.szCliente, myAgent.szCliente);
                    strcpy(msgOut.szTelefono, myAgent.szTelefono);
                    strcpy(msgOut.szMail, myAgent.szMail);
                    msgOut.iUsaAudio = connAgent->dbConn.iUsaAudio;
                }
            }
            iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_CONNECTED, &msgOut, sizeof(msgOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            iRet = ASRV_SendCachedChat(sesPtr); 
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            iRet = ASRV_HandleBroadcastListCounters();
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            ASRV_LogConnectSupport(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
        }
        else {
		    JADA_LogMessage("Ricevuta connessione richiesta %d da sessione %d ma la richiesta non e' in stato LOADED (%d)\n", 
                    sesPtr->dbReq.iId, sesPtr->dbSess.iId, sesPtr->dbReq.iStato);
            memset(&msgErr, 0, sizeof(msgErr));
            msgOut.iResult = -1;
            sprintf(msgErr.szDescription, "Richiesta non connessa: la richiesta non e' in stato LOADED (%d)", 
                    sesPtr->dbReq.iStato);

            iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            ASRV_LogConnectSupportError(sesPtr, sesPtr->dbReq.iId, "stato non e' LOADED");
        }
    }
    else {
		JADA_LogMessage("La sessione associata alla richiesta %d non e' stata trovata\n", msgIn.iRequestId);
        memset(&msgErr, 0, sizeof(msgErr));
        msgOut.iResult = -1;
        sprintf(msgErr.szDescription, "La sessione associata alla richiesta %d non e' stata trovata\n", msgIn.iRequestId);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        ASRV_LogConnectSupportError(sesPtr, msgIn.iRequestId, "stato non e' LOADED");
    }

    return(0);
}

int ASRV_HandleConnectionParametersList(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                     iRet, i;
    JADA_MsgParametersList  msgIn;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    for (i = 0; i < msgIn.iParameters; i++) {
        iRet = ASRV_DbSetParametro(&connPtr->dbConn, msgIn.lstParameters[i].szSection,  
                        msgIn.lstParameters[i].szName, msgIn.lstParameters[i].szValue); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }

    return(0);
}

int ASRV_HandleGetCombo(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                  iRet, i;
    JADA_MsgGetCombo     msgIn;
    JADA_MsgComboValues  msgOut;
    CORE_List            lstValues = NULL;
    CORE_Currency        curC;
    ShortString          currValue;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&msgOut, 0, sizeof(msgOut));

    switch(msgIn.iComboType) {
    case JADA_COMBOTYPE_REGIONE:
        lstValues = ASRV_DbGetComboRegione(msgIn.szFilter);
        break;
    case JADA_COMBOTYPE_CLIENTE:
        lstValues = ASRV_DbGetComboCliente(msgIn.szFilter);
        break;
    case JADA_COMBOTYPE_CODA:
        lstValues = ASRV_DbGetComboCoda(msgIn.szFilter);
        break;
    case JADA_COMBOTYPE_MANAGER_CODA:
        lstValues = ASRV_DbGetComboManagerCoda(msgIn.szFilter);
        break;
   }

    if (lstValues != NULL) {
        iRet = JADA_LstGetFirst(lstValues, currValue, sizeof(currValue), &curC);
        while (iRet == 0) {
            i = msgOut.iValues;
            strcpy(msgOut.lstValues[i], currValue); 
            msgOut.iValues++;
            if (msgOut.iValues >= JADA_MAX_COMBO_VALUES_IN_LIST) {
                break;
            }

            iRet = JADA_LstGetNext(lstValues, currValue, sizeof(currValue), &curC);
        }

        iRet = JADA_LstDestroy(__FILE__, __LINE__, lstValues);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_COMBO_VALUES, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    
    return(0);
}

int ASRV_HandleListRequests(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                  iRet, i;
    ASRV_DbRichiesta     tmpRichiesta;
    JADA_MsgListRequests msgIn;
    JADA_MsgRequestList  msgOut;
    CORE_List            lstOpenedRequests;
    CORE_Currency        curC;
    ASRV_DbAgent         myAgent;
    ASRV_Session         *sessTmp;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&msgOut, 0, sizeof(msgOut));

    lstOpenedRequests = ASRV_DbListRichiesteAperte(msgIn.szCoda); 
    if (lstOpenedRequests != NULL) {
        iRet = JADA_LstGetFirst(lstOpenedRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        while (iRet == 0) {
            sessTmp = ASRV_GetSessionByRequestId(tmpRichiesta.iId);
            if (sessTmp != NULL) { 
                if ((strlen(msgIn.szCoda) == 0) || (strcmp(msgIn.szCoda, tmpRichiesta.szCoda) == 0)) {
                    i = msgOut.iRequests;
                    msgOut.lstRequests[i].iRequestId = tmpRichiesta.iId; 
                    msgOut.lstRequests[i].iStatus = tmpRichiesta.iStato; 
                    memset(&myAgent, 0, sizeof(myAgent));
                    myAgent.iId = tmpRichiesta.iIdAgent; 
                    iRet = ASRV_DbGetAgent(&myAgent); 
                    if (iRet == 0) {
                        strcpy(msgOut.lstRequests[i].szUtente, myAgent.szNome);
                        strcpy(msgOut.lstRequests[i].szTelefono, myAgent.szTelefono);
                        strcpy(msgOut.lstRequests[i].szCellulare, myAgent.szCellulare);
                        strcpy(msgOut.lstRequests[i].szMail, myAgent.szMail);
                        strcpy(msgOut.lstRequests[i].szCliente, myAgent.szCliente);
                    }
                    strncpy(msgOut.lstRequests[i].szDescrizione, tmpRichiesta.szDescrizione, sizeof(msgOut.lstRequests[i].szDescrizione) - 1);
                    CORE_LogIfFail(iRet == 0, "Errore generico");
                    msgOut.iRequests++;
                    if (msgOut.iRequests >= JADA_MAX_REQUESTS_IN_LIST) {
                        break;
                    }
                }
            }

            iRet = JADA_LstGetNext(lstOpenedRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        }
    }

    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstOpenedRequests);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_REQUESTS_LIST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    
    return(0);
}

int ASRV_HandleListPendingRequests(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                  iRet, i;
    ASRV_DbRichiesta     tmpRichiesta;
    JADA_MsgListRequests msgIn;
    JADA_MsgRequestList  msgOut;
    CORE_List            lstPendingRequests;
    CORE_Currency        curC;
    ASRV_DbAgent         myAgent;
    ASRV_Session         *sessTmp;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&msgOut, 0, sizeof(msgOut));

    lstPendingRequests = ASRV_DbListRichiesteAperte(msgIn.szCoda); 
    if (lstPendingRequests != NULL) {
        iRet = JADA_LstGetFirst(lstPendingRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        while (iRet == 0) {
            sessTmp = ASRV_GetSessionByRequestId(tmpRichiesta.iId);
            if (sessTmp == NULL) { 
                if ((strlen(msgIn.szCoda) == 0) || (strcmp(msgIn.szCoda, tmpRichiesta.szCoda) == 0)) {
                    i = msgOut.iRequests;
                    msgOut.lstRequests[i].iRequestId = tmpRichiesta.iId; 
                    msgOut.lstRequests[i].iStatus = tmpRichiesta.iStato; 
                    memset(&myAgent, 0, sizeof(myAgent));
                    myAgent.iId = tmpRichiesta.iIdAgent; 
                    iRet = ASRV_DbGetAgent(&myAgent); 
                    if (iRet == 0) {
                        strcpy(msgOut.lstRequests[i].szUtente, myAgent.szNome);
                        strcpy(msgOut.lstRequests[i].szTelefono, myAgent.szTelefono);
                        strcpy(msgOut.lstRequests[i].szCellulare, myAgent.szCellulare);
                        strcpy(msgOut.lstRequests[i].szMail, myAgent.szMail);
                        strcpy(msgOut.lstRequests[i].szCliente, myAgent.szCliente);
                    }
                    strncpy(msgOut.lstRequests[i].szDescrizione, tmpRichiesta.szDescrizione, sizeof(msgOut.lstRequests[i].szDescrizione) - 1);
                    CORE_LogIfFail(iRet == 0, "Errore generico");
                    msgOut.iRequests++;
                    if (msgOut.iRequests >= JADA_MAX_REQUESTS_IN_LIST) {
                        break;
                    }
                }
            }

            iRet = JADA_LstGetNext(lstPendingRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        }
    }

    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstPendingRequests);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_PENDING_REQUESTS_LIST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    
    return(0);
}

int ASRV_HandleGetListCounters(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgListCounters    msgOut;
    ASRV_DbRichiesta        tmpRichiesta;
    CORE_List               lstOpenedRequests;
    CORE_Currency           curC;
    ASRV_Session            *sessTmp;


    memset(&msgOut, 0, sizeof(msgOut));

    lstOpenedRequests = ASRV_DbCountRichiesteAperte(&connPtr->dbConn.iIdManager); 
    if (lstOpenedRequests != NULL) {
        iRet = JADA_LstGetFirst(lstOpenedRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        while (iRet == 0) {
            sessTmp = ASRV_GetSessionByRequestId(tmpRichiesta.iId);
            if (sessTmp != NULL) { 
                msgOut.iAreaRequests++;
            }
            else {
                msgOut.iAreaPendingRequests++;
            }

            iRet = JADA_LstGetNext(lstOpenedRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        }
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstOpenedRequests);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    lstOpenedRequests = ASRV_DbCountRichiesteAperte(NULL); 
    if (lstOpenedRequests != NULL) {
        iRet = JADA_LstGetFirst(lstOpenedRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        while (iRet == 0) {
            sessTmp = ASRV_GetSessionByRequestId(tmpRichiesta.iId);
            if (sessTmp != NULL) { 
                msgOut.iTotalRequests++;
            }
            else {
                msgOut.iTotalPendingRequests++;
            }

            iRet = JADA_LstGetNext(lstOpenedRequests, &tmpRichiesta, sizeof(tmpRichiesta), &curC);
        }
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstOpenedRequests);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_LIST_COUNTERS, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    
    return(0);
}

int ASRV_HandleBroadcastListCounters(void)
{
    int                     iRet;
    ASRV_Connection         *connPtr;
    CORE_Currency           curS;

    iRet = JADA_LstGetFirst(lstConnections, &connPtr, sizeof(connPtr), &curS);
    while (iRet == 0) {
        if (connPtr->dbConn.iWhoAmI == JADA_WHOAMI_MANAGER) {
            iRet = ASRV_HandleGetListCounters(connPtr, NULL);
        }
        iRet = JADA_LstGetNext(lstConnections, &connPtr, sizeof(connPtr), &curS);
    }
    
    return(0);
}

