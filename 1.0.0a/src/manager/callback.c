#include "jada.h"
#include "jada_messages.h"
#include "manager.h"

static int          iRequestId = 0;
static JADA_ReqId   iReqId;


static int AMGR_Trim(char *szString)
{
    int     iLength;

    iLength = strlen(szString);
    szString[iLength] = '\0';

    while (--iLength >= 0) {
        if ((szString[iLength] != '\0') && (!isspace(szString[iLength]))) {
            break;
        }
    }
    szString[iLength + 1] = '\0';

    return(0);
}

static int AMGR_FileExists(char *szFile)
{
    int     iRet = FALSE;
    FILE    *fp = NULL;

    fp = fopen(szFile, "r");
    if (fp != NULL) {
        fclose(fp);
        iRet = TRUE;
    }

    return(iRet);
}

static int AMGR_PrintStream(JADA_Stream stmSelf)
{
    int         iRet, iCounter = 0;
    LongString  szBuffer;

    iRet = JADA_StmGetFirstLine(stmSelf, szBuffer, sizeof(szBuffer));
    while (iRet == 0) {
        fprintf(stderr, "Record %d:\n    %s\n", ++iCounter, szBuffer);
        iRet = JADA_StmGetNextLine(stmSelf, szBuffer, sizeof(szBuffer));
    }

    return(0);
}

CORE_Callback AMGR_MnuOpenConnection(int iAutoLogin)
{
    int     iRet;
    ShortString  szMyName, szMyPhone, szMyCustomer;

    AMGR_Connection = JADA_ConnNew(__FILE__, __LINE__, TRUE);
    CORE_ReturnValIfFail(AMGR_Connection != NULL, -1, ;, "errore generico");

    iRet = JADA_ManageEvents(AMGR_Connection);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella connessione al server");

    iRet = JADA_ConnConnect(AMGR_Connection, JADA_SESSTYPE_SUPPORT, 30);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    if (iAutoLogin != TRUE) {
        iRet = JADA_ConnGetParameter(AMGR_Connection, "sessione", "utente", szMyName, sizeof(szMyName));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnGetParameter(AMGR_Connection, "sessione", "telefono", szMyPhone, sizeof(szMyPhone));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnGetParameter(AMGR_Connection, "sessione", "cliente", szMyCustomer, sizeof(szMyCustomer));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

        iRet = CORE_GetString("Nome utente", szMyName, sizeof(szMyName));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = CORE_GetString("Recapito telefonico", szMyPhone, sizeof(szMyPhone));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = CORE_GetString("Cliente", szMyCustomer, sizeof(szMyCustomer));
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

        iRet = JADA_ConnSetParameter(AMGR_Connection, "sessione", "utente", szMyName);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnSetParameter(AMGR_Connection, "sessione", "telefono", szMyPhone);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        iRet = JADA_ConnSetParameter(AMGR_Connection, "sessione", "cliente", szMyCustomer);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    }

    iRet = JADA_ConnOpen(AMGR_Connection, 30);
    switch (iRet) {
    case JADA_CREDENTIAL_OK:
        JADA_LogMessage("Connessione terminata correttamente.\n");
        break;
    case JADA_CREDENTIAL_UNKNOWN_USER:
        JADA_LogMessage("Utente sconosciuto.\n");
        break;
    case JADA_CREDENTIAL_WRONG_PASSWORD:
        JADA_LogMessage("Password non corretta.\n");
        break;
    }

    return(0);
}

CORE_Callback AMGR_MnuCloseConnection(void)
{
    int     iRet;

    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    iRet = JADA_ConnClose(AMGR_Connection);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuListRequests(void)
{
    int             iRet, iStreamId;
    JADA_Stream     stmSelf;


    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    iRet = JADA_ConnListRequests(AMGR_Connection, "", &iStreamId, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    stmSelf = JADA_ConnGetStream(AMGR_Connection, iStreamId);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore generico");
    iRet = AMGR_PrintStream(stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuListPendingRequests(void)
{
    int             iRet, iStreamId;
    JADA_Stream     stmSelf;

    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    iRet = JADA_ConnListPendingRequests(AMGR_Connection, "", &iStreamId, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    stmSelf = JADA_ConnGetStream(AMGR_Connection, iStreamId);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore generico");
    iRet = AMGR_PrintStream(stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuListParameters(void)
{
    int             iRet, iStreamId;
    JADA_Stream     stmSelf;

    if (! JADA_IsValidSupport(AMGR_Support)) {
        JADA_LogMessage("La sessione non e' stata aperta.\n");
        return(0);
    }

    iRet = JADA_SuppListParameters(AMGR_Support, &iStreamId, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    stmSelf = JADA_ConnGetStream(AMGR_Connection, iStreamId);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore generico");
    iRet = AMGR_PrintStream(stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuGetCombo(void)
{
    int             iRet, iStreamId;
    JADA_Stream     stmSelf;

    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    iRet = JADA_ConnGetCombo(AMGR_Connection, JADA_COMBOTYPE_CODA, "", &iStreamId, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    stmSelf = JADA_ConnGetStream(AMGR_Connection, iStreamId);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore generico");
    iRet = AMGR_PrintStream(stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    JADA_WaitKey(""); 

    iRet = JADA_ConnGetCombo(AMGR_Connection, JADA_COMBOTYPE_REGIONE, "", &iStreamId, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    stmSelf = JADA_ConnGetStream(AMGR_Connection, iStreamId);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore generico");
    iRet = AMGR_PrintStream(stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    JADA_WaitKey(""); 

    iRet = JADA_ConnGetCombo(AMGR_Connection, JADA_COMBOTYPE_CLIENTE, "Campania", &iStreamId, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    stmSelf = JADA_ConnGetStream(AMGR_Connection, iStreamId);
    CORE_ReturnValIfFail(stmSelf != NULL, -1, ;, "errore generico");
    iRet = AMGR_PrintStream(stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_StmDestroy(__FILE__, __LINE__, stmSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    JADA_WaitKey(""); 

    return(0);
}

CORE_Callback AMGR_MnuGetRequestCount(void)
{
    int     iRet;

    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    iRet = JADA_ConnGetListCounters(AMGR_Connection);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuAcceptSupport(void)
{
    int         iRet;

    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    AMGR_Support = JADA_SuppNew(__FILE__, __LINE__, AMGR_Connection);
    CORE_ReturnValIfFail(AMGR_Support != NULL, -1, ;, "errore generico");

    iRet = CORE_GetInteger("ID Richiesta", &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_IntToRequestId(iRequestId, &iReqId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_SuppAccept(AMGR_Support, iReqId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuLoadSupport(void)
{
    int     iRet;

    if (! JADA_IsValidConnection(AMGR_Connection)) {
        JADA_LogMessage("La connessione non e' statat aperta.\n");
        return(0);
    }

    AMGR_Support = JADA_SuppNew(__FILE__, __LINE__, AMGR_Connection);
    CORE_ReturnValIfFail(AMGR_Support != NULL, -1, ;, "errore generico");

    iRet = CORE_GetInteger("ID Richiesta", &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    iRet = JADA_IntToRequestId(iRequestId, &iReqId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_SuppLoad(AMGR_Support, iReqId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback AMGR_MnuCloseSupport(void)
{
    int     iRet;

    iRet = JADA_SuppIsConnected(AMGR_Support);
    if (iRet == TRUE) {
        iRet = JADA_SuppClose(AMGR_Support);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_SuppDestroy(__FILE__, __LINE__, AMGR_Support);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    AMGR_Support = NULL;

    return(0);
}

CORE_Callback AMGR_MnuSendMessages(void)
{
    int     iRet;
    char    szText[JADA_CHATTEXT_SIZE] = "";

    iRet = JADA_SuppIsConnected(AMGR_Support);
    if (iRet == TRUE) {
        iRet = CORE_GetString("Messaggio da inviare (bye per terminare)", szText, sizeof(szText) - 1);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

        while (strcmp(szText, "bye") != 0) {
            iRet = JADA_SuppSendChat(AMGR_Support, szText);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

            iRet = CORE_GetString("Messaggio da inviare (bye per terminare)", szText, sizeof(szText) - 1);
            CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
        }
    }

    return(0);
}

CORE_Callback AMGR_MnuRequestCall(void)
{
    int     iRet;


    iRet = JADA_SuppIsConnected(AMGR_Support);
    if (iRet == TRUE) {
        AMGR_Call = JADA_CallNew(__FILE__, __LINE__, AMGR_Connection);
        CORE_ReturnValIfFail(AMGR_Call != NULL, -1, ;, "errore generico");

        iRet = JADA_CallOpen(AMGR_Call, iReqId);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    return(0);
}

CORE_Callback AMGR_MnuRequestVnc(void)

{
    int     iRet;


    iRet = JADA_SuppIsConnected(AMGR_Support);
    if (iRet == TRUE) {
        AMGR_VNC = JADA_VncNew(__FILE__, __LINE__, AMGR_Connection);
        CORE_ReturnValIfFail(AMGR_VNC != NULL, -1, ;, "errore generico");

        iRet = JADA_VncOpenDesktop(AMGR_VNC);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    return(0);
}

CORE_Callback AMGR_MnuRequestFtp(void)
{
    int     iRet;


    iRet = JADA_SuppIsConnected(AMGR_Support);
    if (iRet == TRUE) {
        AMGR_FTP = JADA_FtpNew(__FILE__, __LINE__, AMGR_Connection);
        CORE_ReturnValIfFail(AMGR_Call != NULL, -1, ;, "errore generico");

        iRet = JADA_FtpOpen(AMGR_FTP);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    return(0);
}
