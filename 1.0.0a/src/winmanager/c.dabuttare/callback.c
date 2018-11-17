/******************************************************************************/
/*                                                                            */
/*   File    : main.c                                                         */
/*   Scope   : FLOW Framework System Manager main program                      */
/*   Module  :                                                                */
/*   Author  :                                                                */
/*   Release : 5                                                              */
/*   Version : 0                                                              */
/*   Data    : 11/10/2001                                                     */
/*   Note    :                                                                */
/*                                                                            */
/******************************************************************************/
#include "eass.h"
#include "agent.h"



static int EASS_ShowFile(char *szFileName) 
{
    FixString   szCmd;

#ifdef ON_WINDOWS
    sprintf(szCmd, "write.exe %s", szFileName);
    system(szCmd);
#else
#endif

    return(0);
}

CORE_Callback EASS_MnuOpenSession(void)
{
    int     iRet;
    CoreId  szUtente, szTelefono, szCliente;

    iRet = ECOM_IniGetString("sessione", "utente", szUtente, sizeof(szUtente));
    iRet = ECOM_IniGetString("sessione", "telefono", szTelefono, sizeof(szTelefono));
    iRet = ECOM_IniGetString("sessione", "cliente", szCliente, sizeof(szCliente));

    iRet = CORE_GetString("Nome utente", szUtente, sizeof(szUtente));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    iRet = CORE_GetString("Recapito telefonico", szTelefono, sizeof(szTelefono));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    iRet = CORE_GetString("Cliente", szCliente, sizeof(szCliente));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    iRet = ECOM_IniSetString("sessione", "utente", szUtente);
    iRet = ECOM_IniSetString("sessione", "telefono", szTelefono);
    iRet = ECOM_IniSetString("sessione", "cliente", szCliente);

    iRet = EASS_OpenSession(szUtente, szTelefono, szCliente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    strcpy(EASS_UserName, szUtente);

    return(0);
}

CORE_Callback EASS_MnuCloseSession(void)
{
    int     iRet;

    iRet = EASS_CloseSession();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(iRet);
}

CORE_Callback EASS_MnuCreateRequest(void)      
{
    int     iRet;
    CoreId  szArea, szDescription;

    iRet = ECOM_IniGetString("richiesta", "area", szArea, sizeof(szArea));
    iRet = ECOM_IniGetString("richiesta", "descrizione", szDescription, sizeof(szDescription));

    iRet = CORE_GetString("Area", szArea, sizeof(szArea));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    iRet = CORE_GetString("Descrizione del problema", szDescription, sizeof(szDescription));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    iRet = ECOM_IniSetString("richiesta", "area", szArea);
    iRet = ECOM_IniSetString("richiesta", "descrizione", szDescription);

    iRet = EASS_CreateRequest(szArea, szDescription);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(iRet);
}

CORE_Callback EASS_MnuConnectRequest(void)      
{
    int     iRet;
    int     iRequestId = 567;

    iRet = CORE_GetInteger("ID Richiesta", &iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    iRet = EASS_ConnectRequest(iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

CORE_Callback EASS_MnuCloseRequest(void)
{
    int     iRet;

    iRet = EASS_CloseRequest();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(iRet);
}

CORE_Callback EASS_MnuSendMessages(void)
{
    int     iRet;
    char    szText[EASS_MSG_PAYLOAD_SIZE];

    if (EASS_RequestId < 0) {
        printf("Nessuna richiesta aperta.\n");
        return(0);
    }

    iRet = ECOM_IniGetString("chat", "messaggio", szText, sizeof(szText));

    iRet = CORE_GetString("Messaggio da inviare", szText, sizeof(szText) - 1);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");

    while (strcmp(szText, "esci") != 0) {
        iRet = ECOM_IniSetString("chat", "messaggio", szText);

        iRet = ECOM_SendRequestMessage(EASS_MSG_SEND_CHAT_TEXT, szText, sizeof(szText));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

        iRet = CORE_GetString("Messaggio da inviare", szText, sizeof(szText) - 1);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "errore generico");
    }


    return(0);
}
