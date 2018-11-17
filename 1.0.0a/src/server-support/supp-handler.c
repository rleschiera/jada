#include "jada.h"
#include "jada_internal.h"
#include "server.h"

/******************************************************************************
 *
 *      H I G H     L E V E L     H A N D L E R S
 *
 *****************************************************************************/
/* Support Message Handler */
int ASRV_HandleSupportMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet, iFound = 0;
    JADA_SessionMsgHeader   msgHeader;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

	switch (msgHeader.iOpcode) {
    case JADA_MSG_SOLICIT_SUPPORT:
        iRet = ASRV_HandleSolicitSupport(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_CLOSE_SUPPORT:
        iRet = ASRV_HandleCloseSupport(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_LIST_PARAMETERS:
        iRet = ASRV_HandleListParameters(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_PARAMETERS_LIST:
        iRet = ASRV_HandleParametersList(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_SEND_CHAT_TEXT:
        iRet = ASRV_HandleSendChatText(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_PING:
        iRet = ASRV_HandlePing(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    case JADA_MSG_PING_REPLY:
        iRet = ASRV_HandlePingReply(sesPtr, iIdConnSource, msgRecv);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;

    default:
        iFound = -1;
        //JADA_LogMessage("Messaggio sulla sessione di supporto %d non valido (%s)\n",
        //        sesPtr->dbSess.iId, JADA_MsgOpcodeToString(msgHeader.iOpcode));
        break;
    }

    return(iFound);
}

/******************************************************************************
 *
 *      S U P P O R T     H A N D L E R S
 *
 *****************************************************************************/
int ASRV_HandleSolicitSupport(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                      iRet;
    JADA_ReqId               iRequestId;
    JADA_MsgSolicitRequest   msgIn;
    JADA_MsgRequestSolicited msgOut;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRequestId = msgIn.iRequestId;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iRequestId = iRequestId;
    msgOut.timSolicitTime = time(0);

    iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_SOLICITED, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    ASRV_LogSolicitSupport(sesPtr, iRequestId);

    return(0);
}

int ASRV_HandleCloseSupport(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    ASRV_DbRichiesta        myRichiesta;
    JADA_MsgCloseRequest    msgIn;
    JADA_MsgRequestClosed   msgOut;
    ASRV_Connection         *connAgent, *connManager;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    CORE_ReturnValIfFail(JADA_CompareJadaId(&sesPtr->dbReq.iId, &msgIn.iRequestId), -1, ;, "Id richiesta non corrispondente");

    memset(&myRichiesta, 0, sizeof(myRichiesta));
    myRichiesta.iId = msgIn.iRequestId;
    iRet = ASRV_DbDestroySessione(&sesPtr->dbSess);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "descrittore di sessione non chiuso");
    iRet = ASRV_DbDestroyRichiesta(&myRichiesta);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "descrittore di richiesta non chiuso");

    connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iRequestId = myRichiesta.iId;
    msgOut.timCloseTime = time(0);
    if (ASRV_IsValidConnection(connManager)) { 
        iRet = ASRV_TcpSendConnectionMessage(connManager, JADA_MSG_SUPPORT_CLOSED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }
    if (ASRV_IsValidConnection(connAgent)) { 
        iRet = ASRV_TcpSendConnectionMessage(connAgent, JADA_MSG_SUPPORT_CLOSED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    ASRV_LogCloseSupport(sesPtr, myRichiesta.iId);

    memset(&sesPtr->dbReq, 0, sizeof(sesPtr->dbReq)); 
    if (ASRV_IsValidConnection(connAgent)) {
        ASRV_SetSessionAgent(sesPtr, NULL);
    }
    if (ASRV_IsValidConnection(connManager)) {
        ASRV_SetSessionManager(sesPtr, NULL); 
    }
    iRet = ASRV_DbSetSessione(&sesPtr->dbSess);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = ASRV_HandleBroadcastListCounters();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_HandleResumeSupport(ASRV_Connection *connPtr, JADA_Message msgRecv)
{
    int                     iRet;
    ASRV_DbRichiesta        myRichiesta;
    ASRV_DbAgent            myAgent;
    ASRV_DbManager          myManager;
    JADA_MsgResumeRequest   msgIn;
    JADA_MsgResult          msgErr;
    ASRV_Session            *sesPtr; 
    JADA_MsgRequestResumed  msgResumedOut;
    JADA_MsgRequestAccepted msgAcceptedOut;
    JADA_MsgRequestConnected msgConnectedOut;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    memset(&myRichiesta, 0, sizeof(myRichiesta));
    myRichiesta.iId = msgIn.iRequestId;
    iRet = ASRV_DbGetRichiesta(&myRichiesta);
    if (iRet != 0) {
		JADA_LogMessage("La richiesta %d non esiste\n", myRichiesta.iId);
        memset(&msgErr, 0, sizeof(msgErr));
        msgErr.iResult = -1;
        sprintf(msgErr.szDescription, "La richiesta %d non esiste", myRichiesta.iId);

        iRet = ASRV_TcpSendConnectionMessage(connPtr, JADA_MSG_RESULT, &msgErr, sizeof(msgErr));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        return(0);
    }

    /* Se la richiesta non e' in memoria... */ 
    sesPtr = ASRV_GetSessionByRequestId(myRichiesta.iId);
    if (sesPtr == NULL) {

        /* Apri una nuova sessione */ 
        sesPtr = ASRV_CreateSession(__FILE__, __LINE__, connPtr, ASRV_SESSION_TYPE_SUPPORT, myRichiesta.iId);
        CORE_ReturnValIfFail(sesPtr != NULL, -1, ;, "Errore generico");
        connPtr->iIdSession = sesPtr->dbSess.iId;

        sesPtr->dbSess.iIdRichiesta = myRichiesta.iId;
        iRet = ASRV_DbSetRichiestaIdSessione(&sesPtr->dbSess);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        switch (connPtr->dbConn.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, JADA_SUPPSTATUS_REQUESTED);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "stato della richiesta non aggiornato");
            iRet = ASRV_SetSessionAgent(sesPtr, connPtr);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "stato della richiesta non aggiornato");
            break;
        case JADA_WHOAMI_MANAGER:
            iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, JADA_SUPPSTATUS_LOADED);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "stato della richiesta non aggiornato");
            iRet = ASRV_SetSessionManager(sesPtr, connPtr);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "stato della richiesta non aggiornato");
            break;
        }

        iRet = ASRV_StartSession(sesPtr);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        /* Invia il messaggio di risposta */ 
        memset(&msgResumedOut, 0, sizeof(msgResumedOut));
        msgResumedOut.iRequestId = sesPtr->dbReq.iId;
        msgResumedOut.iStatus = sesPtr->dbReq.iStato;
        iRet = ASRV_GetPassword_DaButtare(0, msgResumedOut.szPassword_DaButtare);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        msgResumedOut.timResumeTime = time(0);
        strcpy(msgResumedOut.szCoda, sesPtr->dbReq.szCoda); 
        strcpy(msgResumedOut.szDescription, sesPtr->dbReq.szDescrizione); 
        memset(&myAgent, 0, sizeof(myAgent));
        myAgent.iId = sesPtr->dbReq.iIdAgent; 
        iRet = ASRV_DbGetAgent(&myAgent);
        if (iRet == 0) {
            strcpy(msgResumedOut.szUtente, myAgent.szNome); 
            strcpy(msgResumedOut.szCliente, myAgent.szCliente); 
            strcpy(msgResumedOut.szTelefono, myAgent.szTelefono); 
            strcpy(msgResumedOut.szCellulare, myAgent.szCellulare); 
            strcpy(msgResumedOut.szMail, myAgent.szMail); 
        }

        switch (connPtr->dbConn.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_RESUMED, &msgResumedOut, sizeof(msgResumedOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;
        case JADA_WHOAMI_MANAGER:
            iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_RESUMED, &msgResumedOut, sizeof(msgResumedOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "stato della richiesta non aggiornato");
            ////iRet = ASRV_SetSessionManager(sesPtr, connPtr);
            ////CORE_ReturnValIfFail(iRet == 0, -1, ;, "stato della richiesta non aggiornato");
            break;
        }
        ASRV_LogResumeSupport(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }
    /* Altrimenti la richiesta e' gia' in memoria: agganciala */ 
    else {
        iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_REQSTATUS_RUNNING);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nel cambio di stato della richiesta");

        switch (connPtr->dbConn.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            iRet = ASRV_SetSessionAgent(sesPtr, connPtr);
            break;
        case JADA_WHOAMI_MANAGER:
            iRet = ASRV_SetSessionManager(sesPtr, connPtr);
            break;
        }

        iRet = ASRV_DbGetAgentByConnection(&myAgent, sesPtr->dbSess.iIdConnAgent);
        iRet = ASRV_DbGetManagerByConnection(&myManager, sesPtr->dbSess.iIdConnManager);

        switch (connPtr->dbConn.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            memset(&msgConnectedOut, 0, sizeof(msgConnectedOut));
            msgConnectedOut.iResult = 0;
            msgConnectedOut.iRequestId = sesPtr->dbReq.iId;
            msgConnectedOut.iSessionId = sesPtr->dbSess.iId;
            iRet = ASRV_GetVncPassword_DaButtare(0, msgConnectedOut.szPassword_DaButtare);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            strcpy(msgConnectedOut.szCliente, myAgent.szCliente);
            strcpy(msgConnectedOut.szCoda, myRichiesta.szCoda);
            strcpy(msgConnectedOut.szDescription, myRichiesta.szDescrizione); 
            msgConnectedOut.timConnectTime = time(0);

            strcpy(msgConnectedOut.szUtente, myManager.szNome);
            strcpy(msgConnectedOut.szTelefono, myManager.szTelefono);
            strcpy(msgConnectedOut.szCellulare, myManager.szCellulare);
            strcpy(msgConnectedOut.szMail, myManager.szMail);
            msgConnectedOut.iUsaAudio = myManager.iUsaAudio;
            iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_CONNECTED, &msgConnectedOut, sizeof(msgConnectedOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            strcpy(msgConnectedOut.szUtente, myAgent.szNome);
            strcpy(msgConnectedOut.szTelefono, myAgent.szTelefono);
            strcpy(msgConnectedOut.szCellulare, myAgent.szCellulare);
            strcpy(msgConnectedOut.szMail, myAgent.szMail);
            msgConnectedOut.iUsaAudio = myAgent.iUsaAudio;
            iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_CONNECTED, &msgConnectedOut, sizeof(msgConnectedOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;

        case JADA_WHOAMI_MANAGER:
            memset(&msgAcceptedOut, 0, sizeof(msgAcceptedOut));
            msgAcceptedOut.iResult = 0;
            msgAcceptedOut.iRequestId = sesPtr->dbReq.iId;
            msgAcceptedOut.iSessionId = sesPtr->dbSess.iId;
            iRet = ASRV_GetVncPassword_DaButtare(0, msgAcceptedOut.szPassword_DaButtare);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            strcpy(msgAcceptedOut.szCliente, myAgent.szCliente);
            strcpy(msgAcceptedOut.szCoda, myRichiesta.szCoda);
            strcpy(msgAcceptedOut.szDescription, myRichiesta.szDescrizione); 
            msgAcceptedOut.timAcceptTime = time(0);

            strcpy(msgAcceptedOut.szUtente, myManager.szNome);
            strcpy(msgAcceptedOut.szTelefono, myManager.szTelefono);
            strcpy(msgAcceptedOut.szCellulare, myManager.szCellulare);
            strcpy(msgAcceptedOut.szMail, myManager.szMail);
            msgAcceptedOut.iUsaAudio = myManager.iUsaAudio;
            iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_ACCEPTED, &msgAcceptedOut, sizeof(msgAcceptedOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            strcpy(msgAcceptedOut.szUtente, myAgent.szNome);
            strcpy(msgAcceptedOut.szTelefono, myAgent.szTelefono);
            strcpy(msgAcceptedOut.szCellulare, myAgent.szCellulare);
            strcpy(msgAcceptedOut.szMail, myAgent.szMail);
            msgAcceptedOut.iUsaAudio = myAgent.iUsaAudio;
            iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_ACCEPTED, &msgAcceptedOut, sizeof(msgAcceptedOut));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;
        }

        iRet = ASRV_HandleBroadcastListCounters();
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        ASRV_LogResumeSupport(sesPtr, sesPtr->dbReq.iId, &sesPtr->dbReq);
    }

    return(0);
}

int ASRV_HandleParametersList(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet, i;
    JADA_MsgParametersList  msgIn;
    ASRV_Connection         *connAgent;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    CORE_ReturnValIfFail(connAgent != NULL, -1, ;, "errore nella lettura della connessione sorgente");

    for (i = 0; i < msgIn.iParameters; i++) {
        iRet = ASRV_DbSetParametro(&connAgent->dbConn, msgIn.lstParameters[i].szSection,  
                        msgIn.lstParameters[i].szName, msgIn.lstParameters[i].szValue); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    }

    return(0);
}

int ASRV_HandleListParameters(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet, i;
    JADA_MsgListParameters  msgIn;
    JADA_MsgParametersList  msgOut;
    CORE_List               lstParameters;
    ASRV_Parameter          parSelf;
    CORE_Currency           curP;
    ASRV_Connection         *connAgent, *connManager;

    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    CORE_ReturnValIfFail(connAgent != NULL, -1, ;, "errore nella lettura della connessione sorgente");
    CORE_ReturnValIfFail(ASRV_IsValidConnection(connAgent), -1, ;, "errore nella lettura della connessione sorgente");
    connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
    CORE_ReturnValIfFail(connManager != NULL, -1, ;, "errore nella lettura della connessione sorgente");
    CORE_ReturnValIfFail(ASRV_IsValidConnection(connManager), -1, ;, "errore nella lettura della connessione sorgente");

    lstParameters = ASRV_DbListParametri(&connAgent->dbConn, NULL, "connessione");
    CORE_ReturnValIfFail(lstParameters != NULL, -1, ;, "errore nella lettura della connessione sorgente");
    lstParameters = ASRV_DbListParametri(&connAgent->dbConn, lstParameters, "postazione");
    CORE_ReturnValIfFail(lstParameters != NULL, -1, ;, "errore nella lettura della connessione sorgente");
    lstParameters = ASRV_DbListParametri(&connManager->dbConn, lstParameters, "richiesta");
    CORE_ReturnValIfFail(lstParameters != NULL, -1, ;, "errore nella lettura della connessione sorgente");

    memset(&msgOut, 0, sizeof(msgOut));
    iRet = JADA_LstGetFirst(lstParameters, &parSelf, sizeof(parSelf), &curP);
    while (iRet == 0) {
        if (strlen(parSelf.szValue) > 0) { 
            i = msgOut.iParameters;
            strcpy(msgOut.lstParameters[i].szSection, parSelf.szSection); 
            strcpy(msgOut.lstParameters[i].szName, parSelf.szName); 
            strcpy(msgOut.lstParameters[i].szValue, parSelf.szValue); 
            msgOut.iParameters++;
            if (msgOut.iParameters >= JADA_MAX_PARAMETERS_IN_LIST) {
                break;
            }
        }

        iRet = JADA_LstGetNext(lstParameters, &parSelf, sizeof(parSelf), &curP);
    }

    iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_PARAMETERS_LIST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstParameters);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_HandlePing(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_SessionMsgHeader   msgHeader;
    ASRV_Connection         *connAgent;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    CORE_ReturnValIfFail(connAgent != NULL, -1, ;, "errore nella lettura della connessione sorgente");

    /* Fai il forward del messaggio all'agent */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        if (ASRV_IsValidConnection(connAgent)) {
            iRet = ASRV_TcpForwardMessage(connAgent, msgRecv);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        }
    }

    return(0);
}

int ASRV_HandlePingReply(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_SessionMsgHeader   msgHeader;
    ASRV_Connection         *connManager;

    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
    CORE_ReturnValIfFail(connManager != NULL, -1, ;, "errore nella lettura della connessione sorgente");

    /* Fai il forward del messaggio al manager */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        if (ASRV_IsValidConnection(connManager)) {
            iRet = ASRV_TcpForwardMessage(connManager, msgRecv);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        }
    }

    return(0);
}

int ASRV_HandleSendChatText(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_MsgChatText        msgIn;
    JADA_SessionMsgHeader   msgHeader;
    ASRV_Connection         *connAgent, *connManager;


    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
    iRet = JADA_MsgSesGetPayload(msgRecv, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);

    /* Se la sessione e' aperta ed operativa... */
    if (sesPtr->dbReq.iStato == ASRV_REQSTATUS_RUNNING) {
        /* Fai il forward del messaggio al partner di sessione */
        switch (msgHeader.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            if (ASRV_IsValidConnection(connManager)) {
                iRet = ASRV_TcpForwardMessage(connManager, msgRecv);
                CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            }
            break;

        case JADA_WHOAMI_MANAGER:
            if (ASRV_IsValidConnection(connAgent)) {
                iRet = ASRV_TcpForwardMessage(connAgent, msgRecv);
                CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            }
            break;
        }
        iRet = ASRV_DbSendChat(sesPtr->dbReq.iId, msgHeader.iWhoAmI, msgIn.szText);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }
    else {
        /* Metti il messaggio nella cache */
        iRet = ASRV_DbCacheChat(sesPtr->dbReq.iId, msgHeader.iWhoAmI, msgIn.szText);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        //iRet = JADA_LstAdd(reqPtr->lstChatMsgs, &msgIn, sizeof(msgIn), NULL);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");
        //iCount = JADA_LstGetCount(reqPtr->lstChatMsgs);
        //JADA_LogMessage("%d messaggi chat salvati in cache\n", iCount);
    }

    return(0);
}

