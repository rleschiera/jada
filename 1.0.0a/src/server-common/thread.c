#include "jada.h"
#include "jada_internal.h"
#include "server.h"


static FixString    szServerName;
 
/******************************************************************************
 *
 *      C O N N E C T I O N     T H R E A D
 *
 *****************************************************************************/
static int CloseConnection(ASRV_Connection *connPtr, CORE_Currency *curC)
{
    int                         iRet;
    ASRV_Session                *sesPtr;
    ASRV_Connection             *connAgent, *connManager;
    JADA_MsgRequestSuspended    msgOut;
    ASRV_DbAgent                dbAgent;
    ASRV_DbManager              dbManager;

    /* Aggiorna lo stato della richiesta associata e ripulisci i puntatori di sessione della connessione caduta */
    sesPtr = ASRV_GetSessionById(connPtr->iIdSession);
    if (ASRV_IsValidSession(sesPtr)) {

        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iRequestId = sesPtr->dbReq.iId;
        msgOut.timSuspendTime = time(0);

        switch (connPtr->dbConn.iWhoAmI) {
        case JADA_WHOAMI_AGENT:
            dbAgent.iId = sesPtr->iIdAgent;
            iRet = ASRV_DbGetAgent(&dbAgent);
            if (iRet == 0) {
                strcpy(msgOut.szUtente, dbAgent.szNome);
            }

            ASRV_SetSessionAgent(sesPtr, NULL);
            connManager = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
            if (ASRV_IsValidConnection(connManager)) { 
                iRet = ASRV_DbSetStatusSessione(&sesPtr->dbSess, ASRV_SESSSTATUS_OPENED);
                iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_REQSTATUS_LOADED);
                JADA_ClearJadaId(&connPtr->iIdSession);
                JADA_LogMessage("Richiesta %d messa in loaded per caduta dell'agent.\n", sesPtr->dbReq.iId);

                iRet = ASRV_TcpSendSessionMessageToManager(sesPtr, JADA_MSG_SUPPORT_SUSPENDED, &msgOut, sizeof(msgOut));
                CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            }
            else {
                iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_SESSSTATUS_OPENED);
                JADA_LogMessage("Sessione %d distrutta: entrambi i partner sono caduti.\n", sesPtr->dbSess.iId);
                iRet = ASRV_DbDestroySessione(&sesPtr->dbSess);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                iRet = ASRV_DestroySession(sesPtr);
            }
            break;
        case JADA_WHOAMI_MANAGER:
            dbManager.iId = sesPtr->iIdManager;
            iRet = ASRV_DbGetManager(&dbManager);
            if (iRet == 0) {
                strcpy(msgOut.szUtente, dbManager.szNome);
            }

            ASRV_SetSessionManager(sesPtr, NULL); 
            connAgent = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
            if (ASRV_IsValidConnection(connAgent)) { 
                iRet = ASRV_DbSetStatusSessione(&sesPtr->dbSess, ASRV_SESSSTATUS_OPENED);
                iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_REQSTATUS_OPENED);
                JADA_ClearJadaId(&connPtr->iIdSession);
                JADA_LogMessage("Richiesta %d messa in pending per caduta del manager.\n", sesPtr->dbReq.iId);

                iRet = ASRV_TcpSendSessionMessageToAgent(sesPtr, JADA_MSG_SUPPORT_SUSPENDED, &msgOut, sizeof(msgOut));
                CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            }
            else {
                iRet = ASRV_DbSetStatusRichiesta(&sesPtr->dbReq, ASRV_SESSSTATUS_OPENED);
                JADA_LogMessage("Sessione %d distrutta: entrambi i partner sono caduti.\n", sesPtr->dbSess.iId);
                iRet = ASRV_DbDestroySessione(&sesPtr->dbSess);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                iRet = ASRV_DestroySession(sesPtr);
            }
            break;
        default:
            JADA_LogMessage("Richiesta %d messa in pending, ma non so chi e' caduto.\n", sesPtr->dbReq.iId);
            break;
        }
    }

    /* Pulisci la connessione, non piu' aperta */
lblNext:
    JADA_LogMessage("Connessione %d chiusa.\n", connPtr->dbConn.iId);
    iRet = ASRV_DbDestroyConnessione(&connPtr->dbConn);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = ASRV_DestroyConnection(connPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

void *ASRV_ConnectionThread(void *FuncArg)
{
    int                    iRet, iPort, iSize, iCloseMe;
    int                    iSockets;
    //ShortString            szServerType;
    struct sockaddr_in     tmpAddress;
    char                  *szIP;
    SOCKET                 iListenSocket = -1, iNewSocket = -1;
    fd_set                 fdSet;
    struct timeval         tv = {JADA_SELECT_TIMEOUT, 0};
    JADA_Message           msgIn;
    JADA_MsgHeader         msgHeader;
    ASRV_IncomingMsg       messageIn; 
    int                    iError;
    ASRV_Connection       *connPtr;
    ASRV_Session          *sesPtr;
    CORE_Currency          curC;

    //switch (JADA_GetWhoAmI()) {
    //case JADA_WHOAMI_SERVER_ALL:
    //    strcpy(szServerType, "all");
    //    break;
    //case JADA_WHOAMI_SERVER_FTP:
    //    strcpy(szServerType, "ftp");
    //    break;
    //case JADA_WHOAMI_SERVER_MEDIA:
    //    strcpy(szServerType, "media");
    //    break;
    //case JADA_WHOAMI_SERVER_SUPPORT:
    //    strcpy(szServerType, "support");
    //    break;
    //case JADA_WHOAMI_SERVER_VNC:
    //    strcpy(szServerType, "vnc");
    //    break;
    //default:
    //    CORE_ReturnValIfFail(0, NULL, ;, "Errore generico");
    //}

    iRet = JADA_ServerGetString("server", "ip", szServerName, sizeof(szServerName));
    CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore generico");
    CORE_GotoIfFail(strlen(szServerName) > 0, lblError, ;, "Errore generico");
    iRet = JADA_ServerGetInt("server", "port", &iPort);
    CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore generico");

    /* Crea il listener socket */
    iListenSocket = ASRV_TcpCreateListenPort(szServerName, iPort);
    CORE_GotoIfFail(iListenSocket > 0, lblError, iError = JADA_ERR_TCP_CREATE;, "Errore generico");

    while (TRUE) {
        iSockets = -1; tv.tv_sec = JADA_SELECT_TIMEOUT;
        FD_ZERO(&fdSet);
        FD_SET(iListenSocket, &fdSet);
#ifdef UNIX
            iSockets = iListenSocket;
#else
            iSockets = 1;
#endif

        iRet = JADA_LstGetFirst(lstConnections, &connPtr, sizeof(connPtr), &curC);
        while (iRet == 0) {
            FD_SET(connPtr->sokConn.iSocket, &fdSet);
#ifdef UNIX
            iSockets = CORE_Max(iSockets, connPtr->sokConn.iSocket);
#else
            iSockets++;
#endif
            iRet = JADA_LstGetNext(lstConnections, &connPtr, sizeof(connPtr), &curC);
        }

#ifdef UNIX
        iSockets++;
#endif
        iRet = select(iSockets, &fdSet, NULL, NULL, &tv);
        // JADA_LogMessage("ConnectionThread: select = %d\n", iRet);
        CORE_GotoIfFail(iRet >= 0, lblError, iError = JADA_ERR_TCP_SELECT;, "Errore generico");

        /* Accetta la nuova richiesta di connessione */
        if (FD_ISSET(iListenSocket, &fdSet)) {
            JADA_LogMessage("Nuova connessione entrante ricevuta.\n");
            iNewSocket = accept(iListenSocket, NULL, 0);
            CORE_GotoIfFail(iNewSocket > 0, lblNext, iError = JADA_ERR_TCP_ACCEPT;, "Errore generico");
     
            /* Leggi l'indirizzo IP del partner di comunicazione */
            iSize = sizeof(tmpAddress);
            iRet = getpeername(iNewSocket, (struct sockaddr *)&tmpAddress, &iSize);
            CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
            szIP = inet_ntoa(tmpAddress.sin_addr);
            CORE_GotoIfFail(szIP != NULL, lblNext, ;, "Errore generico");

            /* Inserisci la richiesta di connessione nella lista */
            connPtr = ASRV_CreateConnection(__FILE__, __LINE__, szIP, iNewSocket);
            CORE_GotoIfFail(connPtr != NULL, lblNext, iError = JADA_ERR_TCP_ACCEPT;, "Errore generico");
            JADA_LogMessage("Nuova connessione %d aperta da %s\n", connPtr->dbConn.iId, connPtr->dbConn.szHostIP);
        }

        /* Gestisci la richiesta proveniente dalla connessione */
        iRet = JADA_LstGetFirst(lstConnections, &connPtr, sizeof(connPtr), &curC);
        while (iRet == 0) {
            if (FD_ISSET(connPtr->sokConn.iSocket, &fdSet)) {
                // JADA_LogMessage("Nuovo messaggio disponibile dalla connessione %d.\n", connPtr->iId);
                msgIn = ASRV_TcpReceiveMessage(&connPtr->sokConn);
                // JADA_LogMessage("Il messaggio dalla connessione %d e' ora disponibile .\n", connPtr->iId);
                if (msgIn != NULL) {
                    connPtr->timLastActivity = time(NULL);

                    iRet = JADA_MsgGetHeader(msgIn, &msgHeader);
                    CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                    // JADA_LogMessage("Il messaggio letto sulla connessione %d e' di tipo %d.\n", sesPtr->dbSess.iId, msgHeader.iType);

                    switch (msgHeader.iType) {
                    case JADA_MSGTYPE_CONNECTION:
                        iRet = ASRV_HandleConnectionMessage(connPtr, msgIn, &iCloseMe);
                        CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                        if (iCloseMe != 0) {
                            //JADA_LogMessage("Connessione %d chiusa dopo la ricezione del messaggio.\n", connPtr->iId);
                            //iRet = ASRV_DestroyConnection(__FILE__, __LINE__, connPtr, &curC);
                            //CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                        }
                        break;

                    case JADA_MSGTYPE_SUPPORT:
                    case JADA_MSGTYPE_AUDIOFRAME:
                    case JADA_MSGTYPE_VIDEOFRAME:
                    case JADA_MSGTYPE_DESKTOPFRAME:
                    case JADA_MSGTYPE_DRAWFRAME:
                    case JADA_MSGTYPE_FTPFRAME:
                        sesPtr = ASRV_GetSessionById(connPtr->iIdSession);
                        if (ASRV_IsValidSession(sesPtr)) {
                            memset(&messageIn, 0, sizeof(messageIn));
                            messageIn.iIdSource = connPtr->dbConn.iId;
                            messageIn.iWhoAmI = connPtr->dbConn.iWhoAmI;
                            memcpy(&messageIn.msgIn, &msgIn, sizeof(msgIn));
                            iRet = JADA_LstAdd(sesPtr->lstInMessages, &messageIn, sizeof(messageIn), NULL);
                            if (iRet != 0) {
                                JADA_LogMessage ("Errore nell'accodamento del messaggio sulla sessione (%d)\n", msgHeader.iType);
                            }
                        }
                        else {
                            JADA_SessionMsgHeader   msgSesHeader;

                            iRet = JADA_MsgSesGetHeader(msgIn, &msgSesHeader);

                            JADA_LogMessage ("Messaggio ricevuto ma nessuna sessione aperta (%s, %s, %s)\n", 
                                             JADA_MsgTypeToString(msgHeader.iType), JADA_MsgOpcodeToString(msgSesHeader.iOpcode),
                                             JADA_WhoAmIToString(msgSesHeader.iWhoAmI, NULL, 0));
                        }
                    }
                }
                else {
                    if (JADA_IsValidJadaId(&connPtr->dbConn.iId)) {
                        JADA_LogMessage("Connessione %d chiusa.\n", connPtr->dbConn.iId);
                    }
                    else {
                        JADA_LogMessage("Connessione da %s chiusa.\n", connPtr->dbConn.szHostIP);
                    }
                    iRet = CloseConnection(connPtr, &curC);
                    CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                }
            } 

            iRet = JADA_LstGetNext(lstConnections, &connPtr, sizeof(connPtr), &curC);
        }

lblNext:
        ;
    }

lblError:
    if (iListenSocket > 0) {
        closesocket(iListenSocket);
    }
    JADA_LogMessage("ASRV_ConnectionThread e' stato chiuso a causa di un errore.\n");

    return(NULL);
}

/******************************************************************************
 *
 *      S E S S I O N     T H R E A D
 *
 *****************************************************************************/
int ASRV_HandleSessionMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgRecv)
{
    int                     iRet;
    JADA_SessionMsgHeader   msgHeader;


    iRet = ASRV_HandleSupportMessage(sesPtr, iIdConnSource, msgRecv);
    if (iRet != 0) {
        iRet = ASRV_HandleVncMessage(sesPtr, iIdConnSource, msgRecv);
        if (iRet != 0) {
            iRet = ASRV_HandleMediaMessage(sesPtr, iIdConnSource, msgRecv);
            if (iRet != 0) {
                iRet = ASRV_HandleFtpMessage(sesPtr, iIdConnSource, msgRecv);
                if (iRet != 0) {
                    iRet = JADA_MsgSesGetHeader(msgRecv, &msgHeader);
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
                    JADA_LogMessage("Messaggio sulla sessione %d non valido (%s)\n",
                            sesPtr->dbSess.iId, JADA_MsgOpcodeToString(msgHeader.iOpcode));
                }
            }
        }
    }

    return(iRet);

}

void *ASRV_SessionThread(void *FuncArg)
{
    int                     iRet;
    JADA_SessId             iSessionId;
    JADA_MsgHeader          msgHeader;
    ASRV_IncomingMsg        messageIn; 
    ASRV_Session            *sesPtr = FuncArg;

    iSessionId = sesPtr->dbSess.iId;

    while (sesPtr->iExitThread == FALSE) {

        memset(&messageIn, 0, sizeof(messageIn));
        iRet = JADA_LstWait(sesPtr->lstInMessages, &messageIn, sizeof(messageIn), 5000);
        if (iRet == 0) {
            sesPtr->timLastActivity = time(NULL);

            iRet = JADA_MsgGetHeader(messageIn.msgIn, &msgHeader);
            CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
            // JADA_LogMessage("Il messaggio ricevuto sulla sessione %d e' di tipo %d.\n", sesPtr->dbSess.iId, msgHeader.iType);

            switch (msgHeader.iType) {
            case JADA_MSGTYPE_FTPFRAME:
                iRet = ASRV_ForwardMessage(messageIn.msgIn, messageIn.iIdSource, sesPtr->dbSess.iId);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                break;
            case JADA_MSGTYPE_AUDIOFRAME:
                iRet = ASRV_ForwardMessage(messageIn.msgIn, messageIn.iIdSource, sesPtr->dbSess.iId);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                break;

            case JADA_MSGTYPE_VIDEOFRAME:
                iRet = ASRV_ForwardMessage(messageIn.msgIn, messageIn.iIdSource, sesPtr->dbSess.iId);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                break;
            case JADA_MSGTYPE_SUPPORT:
                iRet = ASRV_HandleSessionMessage(sesPtr, messageIn.iIdSource, messageIn.msgIn);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                break;
            case JADA_MSGTYPE_DRAWFRAME:
                iRet = ASRV_ForwardMessage(messageIn.msgIn, messageIn.iIdSource, sesPtr->dbSess.iId);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                break;
            case JADA_MSGTYPE_DESKTOPFRAME:
                iRet = ASRV_ForwardMessage(messageIn.msgIn, messageIn.iIdSource, sesPtr->dbSess.iId);
                CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
                break;
            default:
                JADA_LogMessage ("Tipo messaggio ricevuto non valido (%d)\n", msgHeader.iType);
            }
        }

lblNext:
        if (messageIn.msgIn != NULL) {
            iRet = JADA_MsgDestroy(__FILE__, __LINE__, messageIn.msgIn);
        }
    }

    // AMMAZZARE IL DESCRITTOREE RIPULIRE I PUNTATOSI IN CONNECTION!!!
    iRet = ASRV_RealDestroySession(__FILE__, __LINE__, sesPtr);
    JADA_LogMessage("ASRV_SessionThread per la sessione %d e' stato chiuso.\n", iSessionId);

    return(NULL);
}

