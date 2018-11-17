#include "eass.h"
#include "agent.h"

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
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    iRet = ECOM_SendSessionMessage(EASS_MSG_OPEN_SESSION, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

int __stdcall EASS_CloseSession(void)
{
    return(0);
}

int __stdcall EASS_CreateRequest(char *szArea, char *szDescription)
{
    int                     iRet;
    EASS_MsgOpenRequest     msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    strcpy(msgOut.szArea, szArea);
    strcpy(msgOut.szDescription, szDescription);

    iRet = ECOM_SendSessionMessage(EASS_MSG_OPEN_REQUEST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    return(0);
}

int __stdcall EASS_ConnectRequest(int iRequestId)
{
    int                     iRet;
    EASS_MsgConnectRequest  msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iRequestId  = iRequestId;

    iRet = ECOM_SendSessionMessage(EASS_MSG_CONNECT_REQUEST, &msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

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

    case EASS_MSG_REQUEST_OPENED:
        {
        EASS_MsgRequestOpened   *ptrPayload = (EASS_MsgRequestOpened *) msgIn->szPayload;

        EASS_RequestId = ptrPayload->iRequestId;
        printf ("\nRichiesta aperta il %s", ctime(&ptrPayload->timOpenTime));
        printf ("L'ID di richiesta e' %d\n", EASS_RequestId);
        }
        break;

    case EASS_MSG_REQUEST_ACCEPTED:
        {
        int                     iRet;
        EASS_MsgRequestAccepted *ptrPayload = (EASS_MsgRequestAccepted *) msgIn->szPayload;

        EASS_RequestId = ptrPayload->iRequestId;
        printf ("\nRichiesta %d presa in carico da %s il %s", ptrPayload->iRequestId, ptrPayload->szUtente, ctime(&ptrPayload->timAcceptTime));

        iRet = ECOM_ConnectToRemoteDesktopServer();
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        }
        break;

    case EASS_MSG_REQUEST_CONNECTED:
        {
        EASS_MsgRequestConnected   *ptrPayload = (EASS_MsgRequestConnected *) msgIn->szPayload;

        EASS_RequestId = ptrPayload->iRequestId;
        printf ("\nRichiesta %d connessa, in carico a %s il %s", ptrPayload->iRequestId, ptrPayload->szUtente, ctime(&ptrPayload->timConnectTime));
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

        printf ("\nManager-> %s\n", ptrPayload->szChatText);
        }
        break;

    case EASS_MSG_OPEN_AUDIOCALL:
        {
        int                 iRet;
        FixString           szCmdParams;
        EASS_MsgCallOpened  msgOut;

        sprintf(szCmdParams, "-m \"%s\" -n %d -p %s -s %s", EASS_UserName, 1000 + EASS_SessionId, "1234", "172.30.51.30");
        iRet = ECOM_ProcExecute(EASS_SoftPhone, szCmdParams);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        printf ("\nChiamata audio richiesta.\n");
        printf("Ho lanciato il comando %s %s...\n", EASS_SoftPhone, szCmdParams);

        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = 0;
        msgOut.iNumberToCall = 1000 + EASS_SessionId;

        iRet = ECOM_SendRequestMessage(EASS_MSG_AUDIOCALL_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        }
        break;

    case EASS_MSG_OPEN_AUDIOVIDEOCALL:
        {
        int                 iRet, iHasVideo = 0, iPort = EASS_AUDIOVIDEO_PORT + EASS_RequestId;
        FixString           szCmdParams;
        EASS_MsgCallOpened  msgOut;

        sprintf(szCmdParams, "-i %s -p %d -c", ECOM_GetServerName(), iPort);
        iRet = ECOM_IniGetInt("config", "video", &iHasVideo);
        CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
        if (iHasVideo != 0) {
            strcat(szCmdParams, " -v");
        }
        iRet = ECOM_ProcExecute(EASS_AudioVideo, szCmdParams);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        printf ("\nChiamata audiovideo richiesta.\n");
        printf("Ho lanciato il comando %s %s...\n", EASS_AudioVideo, szCmdParams);

        memset(&msgOut, 0, sizeof(msgOut));
        msgOut.iResult = 0;

        iRet = ECOM_SendRequestMessage(EASS_MSG_AUDIOVIDEOCALL_OPENED, &msgOut, sizeof(msgOut));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        }
        break;

    case EASS_MSG_OPEN_REMOTEDESKTOP:
        {
        int                         iRet;
        EASS_MsgOpenRemoteDesktop   *ptrPayload = (EASS_MsgOpenRemoteDesktop *) msgIn->szPayload;

        /* Cambia la password di accesso a VNC */
        iRet = SetVNCPassword(ptrPayload->szPassword);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'aggiornamento della password temporanea");

        iRet = ECOM_ConnectToRemoteDesktopServer();
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione al VNC Server");
        }
        break;

    default:
        printf ("\nComando ricevuto non valido (%d)\n", msgIn->iOpcode);
    }

    return(0);
}

