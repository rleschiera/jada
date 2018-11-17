#include "jada.h"
#include "jada_internal.h"
#include "server.h"

/******************************************************************************
 *
 *      V O C I     D I     M E N U
 *
 *****************************************************************************/
JADA_Command Command[] = {
    "1",  1,  "Lista connessioni",                ASRV_MnuListConnections,              TRUE,
    "1",  2,  "Lista sessioni",                   ASRV_MnuListSessions,                 TRUE,
    "1",  3,  "Lista richieste attive",           ASRV_MnuListRequests,                 TRUE,
    "1",  4,  "Lista richieste pending",          ASRV_MnuListPendingRequests,          TRUE,
    "1",  5,  "Lista agent",                      ASRV_MnuListAgents,                   TRUE,
    "1",  6,  "Lista manager",                    ASRV_MnuListManagers,                 TRUE,
    "1",  99, "Esci",                             JADA_MnuExit,                         FALSE, 
    NULL,   0,  NULL,                             NULL,						            -1
};


/******************************************************************************
 *
 *      C O M M O N     S E R V IC E S
 *
 *****************************************************************************/
char *ASRV_ConnStatusToString(int iStatus, char *szString, int iLength)
{
    static FixString    myString;

    switch (iStatus) {
    case ASRV_CONNSTATUS_OPENED:
        strcpy(myString, "Aperta");
        break;
    case ASRV_CONNSTATUS_CLOSED:
        strcpy(myString, "Chiusa");
        break;
    case ASRV_CONNSTATUS_UNKNOWN:
        strcpy(myString, "Sconosciuto");
        break;
    default:
        strcpy(myString, "???");
        break;
    }
  
    if (szString != NULL) {
        strcpy(szString, myString);
    }
    return (myString);
}

char *ASRV_SessStatusToString(int iStatus, char *szString, int iLength)
{
    static FixString    myString;

    switch (iStatus) {
    case ASRV_SESSSTATUS_OPENED:
        strcpy(myString, "Aperta");
        break;
    case ASRV_SESSSTATUS_RUNNING:
        strcpy(myString, "Connessa");
        break;
    case ASRV_SESSSTATUS_CLOSED:
        strcpy(myString, "Chiusa");
        break;
    case ASRV_SESSSTATUS_UNKNOWN:
        strcpy(myString, "Sconosciuto");
        break;
    default:
        strcpy(myString, "???");
        break;
    }
  
    if (szString != NULL) {
        strcpy(szString, myString);
    }
    return (myString);
}

char *ASRV_ReqStatusToString(int iStatus, char *szString, int iLength)
{
    static FixString    myString;

    switch (iStatus) {
    case ASRV_REQSTATUS_OPENED:
        strcpy(myString, "Aperta");
        break;
    case ASRV_REQSTATUS_LOADED:
        strcpy(myString, "Caricata");
        break;
    case ASRV_REQSTATUS_RUNNING:
        strcpy(myString, "Connessa");
        break;
    //case ASRV_REQSTATUS_PENDING:
    //    strcpy(myString, "Pendente");
    //    break;
    case ASRV_REQSTATUS_CLOSED:
        strcpy(myString, "Chiusa");
        break;
    case ASRV_REQSTATUS_UNKNOWN:
        strcpy(myString, "Sconosciuto");
        break;
    default:
        strcpy(myString, "???");
        break;
    }
  
    if (szString != NULL) {
        strcpy(szString, myString);
    }
    return (myString);
}

/******************************************************************************
 *
 *      M E N U     C A L L B A C K S
 *
 *****************************************************************************/
CORE_Callback ASRV_MnuListConnections(void)
{
    int                 iRet;
    ASRV_Connection     *connPtr;
    ASRV_DbAgent        myAgent;
    ASRV_DbManager      myManager;
    CORE_Currency       curC;

    iRet = JADA_LstGetFirst(lstConnections, &connPtr, sizeof(connPtr), &curC);
    while (iRet == 0) {
		printf("Connessione %d: IP %s, Real IP %s, stato %s, whoami %s, lastTime %s\n", 
                connPtr->dbConn.iId,
                connPtr->dbConn.szHostIP,
                connPtr->dbConn.szRealIP,
                ASRV_ConnStatusToString(connPtr->dbConn.iStato, NULL, 0),
                JADA_WhoAmIToString(connPtr->dbConn.iWhoAmI, NULL, 0),
                JADA_TimeToString(connPtr->timLastActivity, NULL, 0));

        if (JADA_IsValidJadaId(&connPtr->dbConn.iIdAgent)) {
            myAgent.iId = connPtr->dbConn.iIdAgent;
            iRet = ASRV_DbGetAgent(&myAgent);
            if (iRet == 0) {
                printf("         Agent %s (%d), Mail %s, Telefono %s, Cellulare %s, Cliente %s (%s)\n", 
                        myAgent.szNome, myAgent.iId, myAgent.szMail, myAgent.szTelefono, 
                        myAgent.szCellulare, myAgent.szCliente, myAgent.szRegione);
            }
        }

        if (JADA_IsValidJadaId(&connPtr->dbConn.iIdManager)) {
            myManager.iId = connPtr->dbConn.iIdManager;
            iRet = ASRV_DbGetManager(&myManager);
            if (iRet == 0) {
                printf("         Manager %s (%d), Username %s, Mail %s, Telefono %s, Cellulare %s, Area %s\n", 
                        myManager.szNome, myManager.iId, myManager.szUsername, myManager.szMail, myManager.szTelefono, 
                        myManager.szCellulare, myManager.szArea);
            }
        }

        iRet = JADA_LstGetNext(lstConnections, &connPtr, sizeof(connPtr), &curC);
    }

    return(0);
}

CORE_Callback ASRV_MnuListSessions(void)
{
    int                 iRet;
    ASRV_Session        *sesPtr;
    ASRV_DbRichiesta    myRichiesta;
    ASRV_DbAgent        myAgent;
    ASRV_DbManager      myManager;
    CORE_Currency       curC;

    iRet = JADA_LstGetFirst(lstSessions, &sesPtr, sizeof(sesPtr), &curC);
    while (iRet == 0) {
		printf("Sessione %d: tipo %d, stato %s, stringa %s, lastTime %s\n", 
                sesPtr->dbSess.iId,
                sesPtr->iType,
                ASRV_SessStatusToString(sesPtr->dbSess.iStato, NULL, 0),
                JADA_SessionIdToToken(sesPtr->dbSess.iId, NULL, 0),
                JADA_TimeToString(sesPtr->timLastActivity, NULL, 0));

        memset(&myRichiesta, 0, sizeof(myRichiesta));
        myRichiesta.iId = sesPtr->dbReq.iId;
        iRet = ASRV_DbGetRichiesta(&myRichiesta);
        if (iRet == 0) {
		    printf("         Richiesta %d: Stato %s, Id Coda %s (%d), Descrizione %s\n", 
                    myRichiesta.iId,
                    ASRV_ReqStatusToString(myRichiesta.iStato, NULL, 0),
                    myRichiesta.szCoda,
                    myRichiesta.iIdCoda,
                    myRichiesta.szDescrizione);
        }

        if (JADA_IsValidJadaId(&sesPtr->iIdAgent)) {
            myAgent.iId = sesPtr->iIdAgent;
            iRet = ASRV_DbGetAgent(&myAgent);
            if (iRet == 0) {
                printf("         Agent %s (%d), Mail %s, Telefono %s, Cellulare %s, Cliente %s (%s)\n", 
                        myAgent.szNome, myAgent.iId, myAgent.szMail, myAgent.szTelefono, 
                        myAgent.szCellulare, myAgent.szCliente, myAgent.szRegione);
            }
            else {
                    printf("         Agent (%d) non trovato in connessione.\n", sesPtr->iIdAgent);
            }
        }
        else {
                printf("         Nessun Agent collegato.\n");
        }

        if (JADA_IsValidJadaId(&sesPtr->iIdManager)) {
            myManager.iId = sesPtr->iIdManager;
            iRet = ASRV_DbGetManager(&myManager);
            if (iRet == 0) {
                printf("         Manager %s (%d), Username %s, Mail %s, Telefono %s, Cellulare %s, Area %s\n", 
                        myManager.szNome, myManager.iId, myManager.szUsername, myManager.szMail, myManager.szTelefono, 
                        myManager.szCellulare, myManager.szArea);
            }
            else {
                    printf("         Manager (%d) non trovato in connessione.\n", sesPtr->iIdManager);
            }
        }
        else {
                printf("         Nessun Manager collegato.\n");
        }

        iRet = JADA_LstGetNext(lstSessions, &sesPtr, sizeof(sesPtr), &curC);
    }

    return(0);
}

CORE_Callback ASRV_MnuListRequests(void)
{
    int                 iRet, iCount = 0;
    ASRV_DbRichiesta    myRichiesta;
    ASRV_DbAgent        myAgent;
    CORE_Currency       curC;
    CORE_List           lstRichieste;
    ASRV_Connection     *connTmp;


    lstRichieste = ASRV_DbListRichiesteAperte("");
    CORE_ReturnValIfFail(lstRichieste != NULL, -1, ;, "Errore generico");

    iRet = JADA_LstGetFirst(lstRichieste, &myRichiesta, sizeof(myRichiesta), &curC);
    while (iRet == 0) {
        connTmp = ASRV_GetConnectionByAgentId(myRichiesta.iIdAgent);
        if (connTmp != NULL) { 
            iCount++;
		    printf("Richiesta %d: Stato %s, Id Coda %d, Descrizione %s\n",
                    myRichiesta.iId, 
                    ASRV_ReqStatusToString(myRichiesta.iStato, NULL, 0),
                    myRichiesta.iIdCoda, myRichiesta.szDescrizione);

            myAgent.iId = myRichiesta.iIdAgent;
            iRet = ASRV_DbGetAgent(&myAgent);
            if (iRet == 0) {
                printf("         Agent %s (%d), Mail %s, Telefono %s, Cellulare %s, Cliente %s (%s)\n", 
                        myAgent.szNome, myAgent.iId, myAgent.szMail, myAgent.szTelefono, 
                        myAgent.szCellulare, myAgent.szCliente, myAgent.szRegione);
            }
        }

        iRet = JADA_LstGetNext(lstRichieste, &myRichiesta, sizeof(myRichiesta), &curC);
    }
	printf("%d richieste aperte.\n", iCount);

    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);

    return(0);
}

CORE_Callback ASRV_MnuListPendingRequests(void)
{
    int                 iRet, iCount = 0;
    ASRV_DbRichiesta    myRichiesta;
    ASRV_DbAgent        myAgent;
    CORE_Currency       curC;
    CORE_List           lstRichieste;
    ASRV_Connection     *connTmp;


    lstRichieste = ASRV_DbListRichiesteAperte("");
    CORE_ReturnValIfFail(lstRichieste != NULL, -1, ;, "Errore generico");

    iRet = JADA_LstGetFirst(lstRichieste, &myRichiesta, sizeof(myRichiesta), &curC);
    while (iRet == 0) {
        connTmp = ASRV_GetConnectionByAgentId(myRichiesta.iIdAgent);
        if (connTmp == NULL) { 
            iCount++;
            printf("Richiesta %d: Stato %s, Id Coda %d, Descrizione %s\n",
                    myRichiesta.iId, 
                    ASRV_ReqStatusToString(myRichiesta.iStato, NULL, 0),
                    myRichiesta.iIdCoda, myRichiesta.szDescrizione);

            myAgent.iId = myRichiesta.iIdAgent;
            iRet = ASRV_DbGetAgent(&myAgent);
            if (iRet == 0) {
                printf("         Agent %s (%d), Mail %s, Telefono %s, Cellulare %s, Cliente %s (%s)\n", 
                        myAgent.szNome, myAgent.iId, myAgent.szMail, myAgent.szTelefono, 
                        myAgent.szCellulare, myAgent.szCliente, myAgent.szRegione);
            }
        }

        iRet = JADA_LstGetNext(lstRichieste, &myRichiesta, sizeof(myRichiesta), &curC);
    }
	printf("%d richieste pending.\n", iCount);

    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);

    return(0);
}

CORE_Callback ASRV_MnuListAgents(void)
{
    int             iRet;
    ASRV_Connection *connPtr;
    ASRV_DbAgent    myAgent;
    CORE_Currency   curC;

    iRet = JADA_LstGetFirst(lstConnections, &connPtr, sizeof(connPtr), &curC);
    while (iRet == 0) {
        if (JADA_IsValidJadaId(&connPtr->dbConn.iIdAgent)) {
		    printf("Sessione %d: status %s, IP %s, lastTime %s\n", 
                    connPtr->dbConn.iId,
                    ASRV_ConnStatusToString(connPtr->dbConn.iStato, NULL, 0),
                    connPtr->dbConn.szHostIP,
                    JADA_TimeToString(connPtr->timLastActivity, NULL, 0));

            myAgent.iId = connPtr->dbConn.iIdAgent;
            iRet = ASRV_DbGetAgent(&myAgent);
            if (iRet == 0) {
                printf("         Agent %s (%d), Mail %s, Telefono %s, Cellulare %s, Cliente %s (%s)\n", 
                        myAgent.szNome, myAgent.iId, myAgent.szMail, myAgent.szTelefono, 
                        myAgent.szCellulare, myAgent.szCliente, myAgent.szRegione);
            }
        }

        iRet = JADA_LstGetNext(lstConnections, &connPtr, sizeof(connPtr), &curC);
    }

    return(0);
}

CORE_Callback ASRV_MnuListManagers(void)
{
    int             iRet;
    ASRV_Connection *connPtr;
    ASRV_DbManager  myManager;
    CORE_Currency   curC;

    iRet = JADA_LstGetFirst(lstConnections, &connPtr, sizeof(connPtr), &curC);
    while (iRet == 0) {
        if (JADA_IsValidJadaId(&connPtr->dbConn.iIdManager)) {
		    printf("Sessione %d: status %s, IP %s, lastTime %s\n", 
                    connPtr->dbConn.iId,
                    ASRV_ConnStatusToString(connPtr->dbConn.iStato, NULL, 0),
                    connPtr->dbConn.szHostIP,
                    JADA_TimeToString(connPtr->timLastActivity, NULL, 0));

            myManager.iId = connPtr->dbConn.iIdManager;
            iRet = ASRV_DbGetManager(&myManager);
            if (iRet == 0) {
                printf("         Manager %s (%d), Username %s, Mail %s, Telefono %s, Cellulare %s, Area %s\n", 
                        myManager.szNome, myManager.iId, myManager.szUsername, myManager.szMail, myManager.szTelefono, 
                        myManager.szCellulare, myManager.szArea);
            }
        }

        iRet = JADA_LstGetNext(lstConnections, &connPtr, sizeof(connPtr), &curC);
    }

    return(0);
}

