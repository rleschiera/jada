#include "eass.h"
#include "manager.h"

int __stdcall EASS_OpenSession(char *szUtente, char *szTelefono, char *szCliente)
{
    int                     iRet;
    EASS_MsgOpenSession     msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    strcpy(msgOut.szHostName, ECOM_GetMyName());
    strcpy(msgOut.szHostIP, ECOM_GetMyIP());
    strcpy(msgOut.szUtente, szUtente);
    strcpy(msgOut.szTelefono, szTelefono);
    strcpy(msgOut.szCliente, szCliente);

    iRet = ECOM_ConnectToServer();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = ECOM_SendSessionMessage(EASS_MSG_OPEN_SESSION, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_CloseSession(void)
{
    return(0);
}

int __stdcall EASS_ListRequests(void)
{
    int                     iRet;

    iRet = ECOM_SendSessionMessage(EASS_MSG_LIST_REQUESTS, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_ListPendingRequests(void)
{
    int                     iRet;

    iRet = ECOM_SendSessionMessage(EASS_MSG_LIST_PENDING_REQUESTS, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_AcceptRequest(int iRequestId)
{
    int                     iRet;
    EASS_MsgAcceptRequest   msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iRequestId = iRequestId;

    iRet = ECOM_SendSessionMessage(EASS_MSG_ACCEPT_REQUEST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_LoadRequest(int iRequestId)
{
    int                 iRet;
    EASS_MsgLoadRequest msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iPendingRequestId = iRequestId;

    iRet = ECOM_SendSessionMessage(EASS_MSG_LOAD_REQUEST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_CloseRequest(void)
{
    int                  iRet;
    EASS_MsgCloseRequest msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iRequestId = EASS_RequestId;

    iRet = ECOM_SendSessionMessage(EASS_MSG_CLOSE_REQUEST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_OpenRemoteDesktop_UUU(void)
{
    int         iRet, iPort = EASS_REMOTE_DESKTOP_PORT + EASS_RequestId;
    FixString   szCmdParams;

    if (EASS_RequestId >= 0) {
        sprintf(szCmdParams, "%s:%d", ECOM_GetServerName(), iPort);
        iRet = ECOM_ProcExecute(EASS_VncViewer, szCmdParams);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

        printf("Ho lanciato il comando %s %s\n", EASS_VncViewer, szCmdParams);
    }
    else {
        printf("Nessuna richiesta attualmente aperta\n");
    }

    return(0);
}

int __stdcall EASS_OpenRemoteDesktop(void)
{
    int     iRet;

    iRet = ECOM_SendRequestMessage(EASS_MSG_OPEN_REMOTEDESKTOP, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_OpenAudioCall(void)
{
    int     iRet;

    iRet = ECOM_SendRequestMessage(EASS_MSG_OPEN_AUDIOCALL, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_OpenAudioVideoCall(void)
{
    int     iRet;

    iRet = ECOM_SendRequestMessage(EASS_MSG_OPEN_AUDIOVIDEOCALL, NULL, 0);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall EASS_HandleSessionMessage(EASS_Message *msgIn)
{
	switch (msgIn->iOpcode) {
    case EASS_MSG_SESSION_OPENED:
        {
        EASS_MsgSessionOpened   *ptrPayload = (EASS_MsgSessionOpened *) msgIn->szPayload;

        EASS_SessionId = ptrPayload->iSessionId;
        printf ("\nSessione aperta il %s", ctime(&ptrPayload->timOpenTime));
        printf ("L'ID di sessione e' %d\n", EASS_SessionId);
        }
        break;

    case EASS_MSG_REQUESTS_LIST:
        {
        int                 i;
        EASS_MsgRequestList *ptrPayload = (EASS_MsgRequestList *) msgIn->szPayload;

        printf("\nRichieste attualmente in coda:\n");

        for (i = 0; i < EASS_MAX_REQUESTS; i++) {
            if (ptrPayload->lstRequests[i].iStatus != 0) {
                printf("    Id %d, da %s (%s), telefono %s\n",
                        ptrPayload->lstRequests[i].iId,
                        ptrPayload->lstRequests[i].szUtente,
                        ptrPayload->lstRequests[i].szCliente,
                        ptrPayload->lstRequests[i].szTelefono);
            }
        }
        }
        break;

    case EASS_MSG_PENDING_REQUESTS_LIST:
        {
        int                  i;
        EASS_MsgRequestList *ptrPayload = (EASS_MsgRequestList *) msgIn->szPayload;

        printf("\nRichieste attualmente pendenti:\n");

        for (i = 0; i < EASS_MAX_REQUESTS; i++) {
            if (ptrPayload->lstRequests[i].iStatus != 0) {
                printf("    Id %d, da %s (%s), telefono %s\n",
                        ptrPayload->lstRequests[i].iId,
                        ptrPayload->lstRequests[i].szUtente,
                        ptrPayload->lstRequests[i].szCliente,
                        ptrPayload->lstRequests[i].szTelefono);
            }
        }
        }
        break;

    case EASS_MSG_REQUEST_ACCEPTED:
        {
        EASS_MsgRequestAccepted   *ptrPayload = (EASS_MsgRequestAccepted *) msgIn->szPayload;

        EASS_RequestId = ptrPayload->iRequestId;
        printf ("\nRichiesta %d proveniente da %s (tel. %s) di %s presa in carico il %s", 
                ptrPayload->iRequestId,
                ptrPayload->szUtente,
                ptrPayload->szTelefono,
                ptrPayload->szCliente,
                ctime(&ptrPayload->timAcceptTime));
        }
        break;

    case EASS_MSG_REQUEST_LOADED:
        {
        EASS_MsgRequestLoaded   *ptrPayload = (EASS_MsgRequestLoaded *) msgIn->szPayload;

        EASS_RequestId = ptrPayload->iRequestId;
        printf ("\nRichiesta %d proveniente da %s (tel. %s) di %s caricata il %s", 
                ptrPayload->iRequestId,
                ptrPayload->szUtente,
                ptrPayload->szTelefono,
                ptrPayload->szCliente,
                ctime(&ptrPayload->timOpenTime));
        }
        break;

    case EASS_MSG_REQUEST_CONNECTED:
        {
        EASS_MsgRequestConnected   *ptrPayload = (EASS_MsgRequestConnected *) msgIn->szPayload;

        EASS_RequestId = ptrPayload->iRequestId;
        printf ("\nRichiesta %d connessa da %s (tel. %s) di %s il %s", 
                ptrPayload->iRequestId,
                ptrPayload->szUtente,
                ptrPayload->szTelefono,
                ptrPayload->szCliente,
                ctime(&ptrPayload->timConnectTime));
        }
        break;

    case EASS_MSG_REQUEST_CLOSED:
        {
        EASS_MsgRequestClosed   *ptrPayload = (EASS_MsgRequestClosed *) msgIn->szPayload;

        if (EASS_RequestId == ptrPayload->iRequestId) {
            printf ("\nRichiesta %d chiusa il %s", ptrPayload->iRequestId, ctime(&ptrPayload->timCloseTime));
            EASS_RequestId = -1;
        }
        }
        break;

    case EASS_MSG_RESULT:
        {
        EASS_MsgResult  *ptrPayload = (EASS_MsgResult *) msgIn->szPayload;

        printf ("\nEsito ricevuto: %d (%s)\n", ptrPayload->iResult, ptrPayload->szDescription);
        }
        break;

    case EASS_MSG_SEND_CHAT_TEXT:
        {
        EASS_MsgChatText    *ptrPayload = (EASS_MsgChatText *) msgIn->szPayload;

        printf ("\nAgent-> %s\n", ptrPayload->szChatText);
        }
        break;

    case EASS_MSG_AUDIOCALL_OPENED:
        {
        int                 iRet;
        EASS_MsgCallOpened *ptrPayload = (EASS_MsgCallOpened *) msgIn->szPayload;
        FixString           szCmdParams;

        if (ptrPayload->iResult == 0) {

            sprintf(szCmdParams, "-m \"%s\" -n %d -p %s -s %s -y %d", 
                    EASS_UserName, 1010 + EASS_SessionId, "1234", "172.30.51.30", ptrPayload->iNumberToCall);
            iRet = ECOM_ProcExecute(EASS_SoftPhone, szCmdParams);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            printf ("\nChiamata audio aperta verso %d.\n", ptrPayload->iNumberToCall);
            printf("Ho lanciato il comando %s %s...\n", EASS_SoftPhone, szCmdParams);
        }
        else {
            printf ("\nChiamata audio non accettata.\n");
        }
        }
        break;

    case EASS_MSG_AUDIOVIDEOCALL_OPENED:
        {
        int                 iRet, iHasVideo = 0, iPort = EASS_AUDIOVIDEO_PORT + EASS_RequestId;
        EASS_MsgCallOpened *ptrPayload = (EASS_MsgCallOpened *) msgIn->szPayload;
        FixString           szCmdParams;

        if (ptrPayload->iResult == 0) {

            sprintf(szCmdParams, "-i %s -p %d -c -m", ECOM_GetServerName(), iPort);
            iRet = ECOM_IniGetInt("config", "video", &iHasVideo);
            CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
            if (iHasVideo != 0) {
                strcat(szCmdParams, " -v");
            }
            iRet = ECOM_ProcExecute(EASS_AudioVideo, szCmdParams);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            printf ("\nChiamata audiovideo aperta.\n");
            printf("Ho lanciato il comando %s %s...\n", EASS_AudioVideo, szCmdParams);
        }
        else {
            printf ("\nChiamata audiovideo non accettata.\n");
        }
        }
        break;

    case EASS_MSG_REMOTEDESKTOP_OPENED:
        {
        int                         iRet, iPort = EASS_REMOTE_DESKTOP_PORT + EASS_RequestId;
        EASS_MsgRemoteDesktopOpened *ptrPayload = (EASS_MsgRemoteDesktopOpened *) msgIn->szPayload;
        FixString                   szCmdParams;

        if (ptrPayload->iResult == 0) {
            ECOM_Sleep(1000);
            sprintf(szCmdParams, "%s:%d", ECOM_GetServerName(), iPort);
            iRet = ECOM_ProcExecute(EASS_VncViewer, szCmdParams);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

            printf ("\nSessione desktop remoto aperta: la password temporanea e': %s.\n", ptrPayload->szPassword);
            printf("Ho lanciato il comando %s %s\n", EASS_VncViewer, szCmdParams);
        }
        else {
            printf ("\nSessione desktop remoto non accettata.\n");
        }
        }
        break;

    default:
        printf ("\nComando ricevuto non valido (%d)\n", msgIn->iOpcode);
    }

    return(0);
}

