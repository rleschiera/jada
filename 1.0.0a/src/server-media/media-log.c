#include "jada.h"
#include "server.h"

void ASRV_LogOpenCall(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
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

void ASRV_LogOpenCallError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogCallOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
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

void ASRV_LogCallOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di apertura chiamata della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di apertura chiamata della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogCloseCall(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
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

void ASRV_LogCloseCallError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di chiusura chiamata della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di chiusura chiamata della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogAbortCall(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Terminazione forzata chiamata sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Terminazione forzata chiamata sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogAbortCallError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di terminazione forzata chiamata della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di terminazione forzata chiamata della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

