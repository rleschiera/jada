#include "jada.h"
#include "jada_messages.h"
#include "agent.h"

static int          iRequestId = 0;
static JADA_ReqId   iReqId;

CORE_Callback AAGT_MnuOpenConnection(int iAutoLogin)
{
    int     iRet;
    ShortString  szMyName, szMyPhone, szMyCustomer;

    AAGT_Connection = JADA_ConnNew(__FILE__, __LINE__, TRUE);
    CORE_ReturnValIfFail(AAGT_Connection != NULL, -1, ;, "errore generico");

    iRet = JADA_ManageEvents(AAGT_Connection);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella connessione al server");

    if (iAutoLogin != TRUE) {
        iRet = JADA_ConnGetParameter(AAGT_Connection, "connessione", "utente", szMyName, sizeof(szMyName));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnGetParameter(AAGT_Connection, "connessione", "telefono", szMyPhone, sizeof(szMyPhone));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnGetParameter(AAGT_Connection, "connessione", "cliente", szMyCustomer, sizeof(szMyCustomer));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

        iRet = CORE_GetString("Nome utente", szMyName, sizeof(szMyName));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = CORE_GetString("Recapito telefonico", szMyPhone, sizeof(szMyPhone));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = CORE_GetString("Cliente", szMyCustomer, sizeof(szMyCustomer));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

        iRet = JADA_ConnSetParameter(AAGT_Connection, "connessione", "utente", szMyName);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnSetParameter(AAGT_Connection, "connessione", "telefono", szMyPhone);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnSetParameter(AAGT_Connection, "connessione", "cliente", szMyCustomer);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    }

    iRet = JADA_ConnConnect(AAGT_Connection, JADA_SESSTYPE_SUPPORT, 30);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_ConnOpen(AAGT_Connection, 30);
    CORE_ReturnValIfFail(iRet == JADA_CREDENTIAL_OK, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AAGT_MnuCloseConnection(void)
{
    int     iRet;

    iRet = JADA_ConnClose(AAGT_Connection);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AAGT_MnuCreateSupport(void)      
{
    int         iRet;
    ShortString szMyQueue, szMyDescription;

    AAGT_Support = JADA_SuppNew(__FILE__, __LINE__, AAGT_Connection);
    CORE_ReturnValIfFail(AAGT_Support != NULL, -1, ;, "errore generico");

    iRet = JADA_ConnGetParameter(AAGT_Connection, "richiesta", "coda", szMyQueue, sizeof(szMyQueue));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    iRet = JADA_ConnGetParameter(AAGT_Connection, "richiesta", "descrizione", szMyDescription, sizeof(szMyDescription));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    iRet = CORE_GetString("Area", szMyQueue, sizeof(szMyQueue));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    iRet = CORE_GetString("Descrizione del problema", szMyDescription, sizeof(szMyDescription));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    iRet = JADA_ConnSetParameter(AAGT_Connection, "richiesta", "coda", szMyQueue);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    iRet = JADA_ConnSetParameter(AAGT_Connection, "richiesta", "descrizione", szMyDescription);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    iRet = JADA_SuppOpen(AAGT_Support, 30);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AAGT_MnuConnectSupport(void)      
{
    int         iRet;

    AAGT_Support = JADA_SuppNew(__FILE__, __LINE__, AAGT_Connection);
    CORE_ReturnValIfFail(AAGT_Support != NULL, -1, ;, "errore generico");

    iRet = CORE_GetInteger("ID Richiesta", &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    iRet = JADA_IntToRequestId(iRequestId, &iReqId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_SuppConnect(AAGT_Support, iReqId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AAGT_MnuCloseSupport(void)
{
    int     iRet;

    iRet = JADA_SuppIsConnected(AAGT_Support);
    if (iRet != 1) {
        iRet = JADA_SuppClose(AAGT_Support);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_SuppDestroy(__FILE__, __LINE__, AAGT_Support);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    AAGT_Support = NULL;

    return(0);
}

CORE_Callback AAGT_MnuSendMessages(void)
{
    int     iRet;
    char    szText[JADA_CHATTEXT_SIZE] = "";

    iRet = JADA_SuppIsConnected(AAGT_Support);
    if (iRet != 1) {
        JADA_LogMessage("Nessuna richiesta aperta.\n");
        return(0);
    }

    iRet = CORE_GetString("Messaggio da inviare (bye per terminare)", szText, sizeof(szText) - 1);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    while (strcmp(szText, "bye") != 0) {
        iRet = JADA_SuppSendChat(AAGT_Support, szText);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

        iRet = CORE_GetString("Messaggio da inviare (bye per terminare)", szText, sizeof(szText) - 1);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    }

    return(0);
}

CORE_Callback AAGT_MnuAcceptCall(void)
{
    int     iRet, iAccepted;


    iRet = JADA_SuppIsConnected(AAGT_Support);
    if (iRet != 1) {
        iAccepted = CORE_GetTrueFalse("Accetti la richiesta (call)");

        AAGT_Call = JADA_CallNew(__FILE__, __LINE__, AAGT_Connection);
        CORE_ReturnValIfFail(AAGT_Call != NULL, -1, ;, "errore generico");

        //iRet = JADA_CallAccept(AAGT_Call, iReqId);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    return(0);
}

CORE_Callback AAGT_MnuAcceptVnc(void)
{
    int     iRet, iAccepted;


    iRet = JADA_SuppIsConnected(AAGT_Support);
    if (iRet != 1) {
        iAccepted = CORE_GetTrueFalse("Accetti la richiesta (VNC)");

        AAGT_VNC = JADA_VncNew(__FILE__, __LINE__, AAGT_Connection);
        CORE_ReturnValIfFail(AAGT_Call != NULL, -1, ;, "errore generico");

        iRet = JADA_VncAcceptDesktop(AAGT_VNC, iAccepted);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    return(0);
}

CORE_Callback AAGT_MnuAcceptFtp(void)
{
    int     iRet, iAccepted;


    iRet = JADA_SuppIsConnected(AAGT_Support);
    if (iRet != 1) {
        iAccepted = CORE_GetTrueFalse("Accetti la richiesta (FTP)");

        AAGT_FTP = JADA_FtpNew(__FILE__, __LINE__, AAGT_Connection);
        CORE_ReturnValIfFail(AAGT_FTP != NULL, -1, ;, "errore generico");

        //iRet = JADA_FtpAccept(AAGT_FTP);
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    return(0);
}



