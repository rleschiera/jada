#include "jada.h"
#include "server.h"

/******************************************************************************
 *
 *   F T P
 *
 ******************************************************************************/
void ASRV_LogOpenFtp(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogOpenFtpError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogFtpOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Chiamata aperta sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Chiamata aperta sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogFtpOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di apertura chiamata della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di apertura chiamata della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogCloseFtp(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Chiusura chiamata sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Chiusura chiamata sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogCloseFtpError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di chiusura chiamata della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di chiusura chiamata della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogListDirectory(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogListDirectoryError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogDirectoryList(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogDirectoryListError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogGetFile(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
    ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Apertura chiamata sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogGetFileError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

