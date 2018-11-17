#include "jada.h"
#include "server.h"

void ASRV_LogOpenWhiteboard(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Apertura lavagna sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Apertura lavagna sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogOpenWhiteboardError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura lavagna della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura lavagna della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogWhiteboardOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Lavagna aperta sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Lavagna aperta sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogWhiteboardOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di apertura lavagna della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di apertura lavagna della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogCleanWhiteboard(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Pulizia della lavagna sulla richiesta %d\n", 
    //                  iId);
    LogRequestMessage(iId, "Pulizia della lavagna sulla richiesta %d\n", 
                      iId);
}

void ASRV_LogCleanWhiteboardError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella pulizia della lavagna della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella pulizia della lavagna della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogCloseWhiteboard(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Chiusura lavagna sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Chiusura lavagna sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}


void ASRV_LogCloseWhiteboardError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella chiusura della lavagna della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella chiusura della lavagna della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogWhiteboardClosed(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Lavagna chiusa sulla richiesta %d da %s verso %s\n", 
        //                  iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Lavagna chiusa sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogWhiteboardClosedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella conferma di chiusura lavagna della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di chiusura lavagna della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogOpenDesktop(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Apertura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Apertura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogOpenDesktopError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nell'apertura desktop sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogDesktopOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Conferma di apertura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Conferma di apertura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogDesktopOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di apertura desktop sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogCloseDesktop(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Chiusura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Chiusura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogCloseDesktopError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella chiusura desktop sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogDesktopClosed(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int         iRet;
	ASRV_DbAgent    myAgent;
	ASRV_DbManager  myManager;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, &myManager);
    if (iRet == 0) {
        LogSessionMessage(sesPtr->dbSess.iId, "Conferma di chiusura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
        LogRequestMessage(iId, "Conferma di chiusura desktop sulla richiesta %d da %s verso %s\n", 
                            iId, myManager.szNome, myAgent.szNome);
    }
}

void ASRV_LogDesktopClosedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nell'apertura chiamata sulla richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella conferma di chiusura desktop sulla richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogSetDrawPointer(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
	ASRV_DbAgent    myAgent;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, NULL);
	if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Spostato il puntatore della richiesta %d su %s\n", 
        //                  iId, myAgent.szNome);
        LogRequestMessage(iId, "Spostato il puntatore della richiesta %d su %s\n", 
                          iId, myAgent.szNome);
    }
}

void ASRV_LogSetDrawPointerError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nello spostamento del puntatore della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nello spostamento del puntatore della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

void ASRV_LogResetDrawPointer(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr)
{
    int             iRet;
	ASRV_DbAgent    myAgent;

    iRet = LogGetSessionPartners(sesPtr, &myAgent, NULL);
	if (iRet == 0) {
        //LogSessionMessage(sesPtr->dbSess.iId, "Cancellato il puntatore della richiesta %d su %s\n", 
        //                  iId, myAgent.szNome);
        LogRequestMessage(iId, "Cancellato il puntatore della richiesta %d su %s\n", 
                          iId, myAgent.szNome);
    }
}

void ASRV_LogResetDrawPointerError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg)
{
    //LogSessionMessage(sesPtr->dbSess.iId, "Errore nella cancellazione del puntatore della richiesta %d: %s\n", 
    //                  iId, szErrorMsg);
    LogRequestMessage(iId, "Errore nella cancellazione del puntatore della richiesta %d: %s\n", 
                      iId, szErrorMsg);
}

