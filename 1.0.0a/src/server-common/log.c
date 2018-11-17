#include "jada.h"
#include "server.h"

/******************************************************************************
 *
 *      U T I L I T Y
 *
 *****************************************************************************/
void LogMessageToFile(char *szFileName, char *szMessage)
{
    char        *szCurrDate;
    FILE        *fp;

    szCurrDate = JADA_TimeToString(time(0), NULL, 0);

    fp = fopen(szFileName, "a+");
    CORE_ReturnIfFail(fp != NULL, ;, "errore nell'apertura del log file");
    fprintf(fp, "%s - %s", szCurrDate, szMessage);
    fflush(fp);
    fclose(fp);
}

void LogConnectionMessage(JADA_ConnId iId, char *szFormat, ...)
{
    va_list     argPtr;
    FixString   szFileName;
    FixString   szMessage;

    sprintf(szFileName, "%s/connections/%d.log", JADA_GetLogDir(), iId);
    va_start(argPtr, szFormat);
    vsprintf(szMessage, szFormat, argPtr);
    va_end(argPtr);

    LogMessageToFile(szFileName, szMessage);
}

void LogSessionMessage(JADA_SessId iId, char *szFormat, ...)
{
    va_list     argPtr;
    FixString   szFileName;
    FixString   szMessage;

    sprintf(szFileName, "%s/sessions/%d.log", JADA_GetLogDir(), iId);
    va_start(argPtr, szFormat);
    vsprintf(szMessage, szFormat, argPtr);
    va_end(argPtr);

    LogMessageToFile(szFileName, szMessage);
}

void LogRequestMessage(JADA_ReqId iId, char *szFormat, ...)
{
    va_list     argPtr;
    FixString   szFileName;
    FixString   szMessage;

    sprintf(szFileName, "%s/requests/%d.log", JADA_GetLogDir(), iId);
    va_start(argPtr, szFormat);
    vsprintf(szMessage, szFormat, argPtr);
    va_end(argPtr);

    LogMessageToFile(szFileName, szMessage);
}

int LogGetSessionPartners(ASRV_Session *sesPtr, ASRV_DbAgent *myAgent, ASRV_DbManager *myManager)
{
    int     iRet, iResult = 0;

    if (myAgent != NULL) {
        iRet = ASRV_DbGetAgentByConnection(myAgent, sesPtr->dbSess.iIdConnAgent);
        if (iRet != 0) {
            iResult = -1;
        }
    }

    if (myManager != NULL) {
        iRet = ASRV_DbGetManagerByConnection(myManager, sesPtr->dbSess.iIdConnManager);
        if (iRet != 0) {
            iResult = -1;
        }
    }

    return(iResult);
}

/******************************************************************************
 *
 *   C O N N E C T I O N
 *
 ******************************************************************************/
void ASRV_LogConnectConnection(ASRV_Connection *connPtr)
{
    LogConnectionMessage(connPtr->dbConn.iId, "Connessione %d connessa dal PC %s (%s)\n", 
                      connPtr->dbConn.iId, connPtr->dbConn.szHostIP, connPtr->dbConn.szRealIP);
}

void ASRV_LogOpenAgentConnection(ASRV_Connection *connPtr, ASRV_DbAgent *agtPtr)
{
    LogConnectionMessage(connPtr->dbConn.iId, "Connessione %d (AGENT) aperta da %s (%s) dal PC %s (%s)\n", 
                      connPtr->dbConn.iId, agtPtr->szNome, agtPtr->szCliente, connPtr->dbConn.szHostIP, connPtr->dbConn.szRealIP);
}

void ASRV_LogOpenManagerConnection(ASRV_Connection *connPtr, ASRV_DbManager *mgrPtr)
{
    LogConnectionMessage(connPtr->dbConn.iId, "Connessione %d (MANAGER) aperta da %s (%s) dal PC %s (%s)\n", 
                      connPtr->dbConn.iId, mgrPtr->szNome, mgrPtr->szUsername, connPtr->dbConn.szHostIP, connPtr->dbConn.szRealIP);
}

void ASRV_LogOpenConnectionError(JADA_ConnId iId, char *szUtente, char *szErrorMsg)
{
    LogConnectionMessage(iId, "Errore nella connessione %d (da %s): %s\n", 
                      iId, szUtente, szErrorMsg);
}

//void ASRV_LogSetSystemInfo(ASRV_Session *sesPtr)
//{
//    int         iRet;
//    ShortString szComputerName, szUserName, szOS, szCPU, szRAM, szBIOS, szScreenSize;
//
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "computer", szComputerName, sizeof(szComputerName));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "utente", szUserName, sizeof(szUserName));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "OS", szOS, sizeof(szOS));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "CPU", szCPU, sizeof(szCPU));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "RAM", szRAM, sizeof(szRAM));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "BIOS", szBIOS, sizeof(szBIOS));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "schermo", szScreenSize, sizeof(szScreenSize));
//
//    LogSessionMessage(sesPtr->dbSess.iId, "System info del PC %s (utente %s), OS %s\n", szComputerName, szUserName, szOS);
//    LogSessionMessage(sesPtr->dbSess.iId, "CPU %s, RAM %s, BIOS %s, Screen %s\n", szCPU, szRAM, szBIOS, szScreenSize);
//}
                      
//void ASRV_LogGetSystemInfo(ASRV_Session *sesPtr)
//{
//    int         iRet;
//    ShortString szUtente;
//
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "sessione", "utente", szUtente, sizeof(szUtente));
//
//    LogSessionMessage(sesPtr->dbSess.iId, "Lettura System info del PC di %s\n", szUtente);
//}
                      
/******************************************************************************
 *
 *   S E S S I O N
 *
 ******************************************************************************/
void ASRV_LogOpenSession(ASRV_Session *sesPtr)
{
    LogSessionMessage(sesPtr->dbSess.iId, "Sessione %d aperta\n", 
                      sesPtr->dbSess.iId);
}


//void ASRV_LogSetSystemInfo(ASRV_Session *sesPtr)
//{
//    int         iRet;
//    ShortString szComputerName, szUserName, szOS, szCPU, szRAM, szBIOS, szScreenSize;
//
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "computer", szComputerName, sizeof(szComputerName));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "utente", szUserName, sizeof(szUserName));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "OS", szOS, sizeof(szOS));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "CPU", szCPU, sizeof(szCPU));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "RAM", szRAM, sizeof(szRAM));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "BIOS", szBIOS, sizeof(szBIOS));
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "postazione", "schermo", szScreenSize, sizeof(szScreenSize));
//
//    LogSessionMessage(sesPtr->dbSess.iId, "System info del PC %s (utente %s), OS %s\n", szComputerName, szUserName, szOS);
//    LogSessionMessage(sesPtr->dbSess.iId, "CPU %s, RAM %s, BIOS %s, Screen %s\n", szCPU, szRAM, szBIOS, szScreenSize);
//}
//                      
//void ASRV_LogGetSystemInfo(ASRV_Session *sesPtr)
//{
//    int         iRet;
//    ShortString szUtente;
//
//    iRet = ASRV_GetParameter(sesPtr->lstParameters, "sessione", "utente", szUtente, sizeof(szUtente));
//
//    LogSessionMessage(sesPtr->dbSess.iId, "Lettura System info del PC di %s\n", szUtente);
//}
                      
/******************************************************************************
 *
 *   R E Q U E S T
 *
 ******************************************************************************/
void ASRV_LogOpenSupport(ASRV_Session *sesPtr, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, NULL);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d aperta da %s (%s) sull'area %s: %s\n", 
                          reqPtr->iId, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
        LogRequestMessage(reqPtr->iId, "Richiesta %d aperta da %s (%s) sull'area %s: %s\n", 
                          reqPtr->iId, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
    }
}

/***********************************************/
void ASRV_LogSolicitSupport(ASRV_Session *sesPtr, JADA_ReqId iId)
{
    LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d sollecitata\n", 
                      iId);
    LogRequestMessage(iId, "Richiesta %d sollecitata\n", 
                      iId);
}

/***********************************************/
void ASRV_LogCloseSupport(ASRV_Session *sesPtr, JADA_ReqId iId)
{
    LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d chiusa\n", 
                      iId);
    LogRequestMessage(iId, "Richiesta %d chiusa\n", 
                      iId);
}

/***********************************************/
void ASRV_LogAcceptSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d accettata da %s per %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
        LogRequestMessage(iId, "Richiesta %d accettata da %s per %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
    }
}

void ASRV_LogAcceptSupportError(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr, char *szErrorMsg)
{
    int             iRet;
	ASRV_DbManager  myManager;

    if (sesPtr != NULL) {
        iRet = LogGetSessionPartners(sesPtr, NULL, &myManager);
        if (iRet == 0) {
            LogSessionMessage(sesPtr->dbSess.iId, "Errore nella accettazione della richiesta %d da %s: %s\n", 
                              iId, myManager.szNome, szErrorMsg);
            LogRequestMessage(iId, "Errore nella accettazione della richiesta %d da %s: %s\n", 
                              iId, myManager.szNome, szErrorMsg);
        }
    }
    else {
        LogRequestMessage(iId, "Sessione non esistente per l'accettazione della richiesta %d: agent non connesso (%s)\n", 
                            iId, myManager.szNome, szErrorMsg);
    }
}

/***********************************************/
void ASRV_LogLoadSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d ripresa in carico da %s per %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
        LogRequestMessage(iId, "Richiesta %d ripresa in carico da %s per %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
    }
}

void ASRV_LogLoadSupportError(ASRV_Connection *connPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr, char *szErrorMsg)
{
    int             iRet;
	ASRV_DbManager  myManager;

    iRet = ASRV_DbGetManagerByConnection(&myManager, connPtr->dbConn.iId);
    if (iRet == 0) {
        LogConnectionMessage(connPtr->dbConn.iId, "Errore nel caricamento della richiesta %d da %s: %s\n", 
                          iId, myManager.szNome, szErrorMsg);
        LogRequestMessage(iId, "Errore nel caricamento della richiesta %d da %s: %s\n", 
                          iId, myManager.szNome, szErrorMsg);
    }
}

/***********************************************/
void ASRV_LogResumeSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d riesumata da %s per %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
        LogRequestMessage(iId, "Richiesta %d riesumata da %s per %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
    }
}

void ASRV_LogResumeSupportError(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr, char *szErrorMsg)
{
    LogSessionMessage(sesPtr->dbSess.iId, "Errore nella resume della richiesta %d: %s\n", 
                      iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella resume della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

/***********************************************/
void ASRV_LogConnectSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Richiesta %d di %s connessa da %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
        LogRequestMessage(iId, "Richiesta %d di %s connessa da %s (%s) sull'area %s: %s\n", 
                            iId, myManager.szNome, myAgent.szNome, myAgent.szCliente, reqPtr->szCoda, reqPtr->szDescrizione);
    }
}

void ASRV_LogConnectSupportError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    if (sesPtr != NULL) {
        LogSessionMessage(sesPtr->dbSess.iId, "Errore nella connessione della richiesta %d: %s\n", 
                          iId, szErrorMsg);
        LogRequestMessage(iId, "Errore nella connessione della richiesta %d: %s\n", 
                          iId, szErrorMsg);
    }
    else {
        LogRequestMessage(iId, "La sessione per la connessione della richiesta %d non e' stata trovata (%s)\n", 
                          iId, szErrorMsg);
    }
}

