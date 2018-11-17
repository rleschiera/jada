#include "jada.h"
#include "jada_internal.h"
#include "server.h"

static FixString    szServerName;


/******************************************************************************/
/*                                                                            */
/*   G E N E R I C     S E R V I C E S                                        */
/*                                                                            */
/******************************************************************************/
int ASRV_TcpCreateListenPort(char *szHostName, int iPort)
{
    int                 iRet;
    struct hostent     *n;
    struct sockaddr_in  myAddress;
    int                 iSocket;

    memset(&myAddress, 0, sizeof(struct sockaddr_in));
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(iPort);
    myAddress.sin_addr.s_addr = inet_addr(szHostName);
    if (myAddress.sin_addr.s_addr == INADDR_NONE) {
        n = (struct hostent *) gethostbyname(szHostName);
        CORE_ReturnValIfFail(n != NULL, -1, ;, "Errore generico");
        memcpy((char *)&myAddress.sin_addr, n->h_addr, n->h_length);
    }

    /* Crea il listener socket */
    iSocket = socket(AF_INET, SOCK_STREAM, 0);
    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "Errore generico");
    iRet = bind(iSocket, (struct sockaddr *)&myAddress, sizeof(myAddress));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = listen(iSocket, 5);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iSocket);
}

int ASRV_WaitConnections(void)
{
    int                 iRet;
    pthread_t           pThreadId;
    pthread_attr_t      attr;


    /* Attiva il thread di gestione del listener socket */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&pThreadId, &attr, ASRV_ConnectionThread, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_TcpCreateSocket(ASRV_Socket *sokSelf, int iSocket)
{
    memset(sokSelf, 0, sizeof(*sokSelf));

    sokSelf->iInLength = 32768;
    sokSelf->szInBuffer = JADA_Calloc(__FILE__, __LINE__, 1, sokSelf->iInLength);
    CORE_ReturnValIfFail(sokSelf->szInBuffer != NULL, -1, ;, "generic error");
    sokSelf->iOutLength = 32768;
    sokSelf->szOutBuffer = JADA_Calloc(__FILE__, __LINE__, 1, sokSelf->iOutLength);
    CORE_ReturnValIfFail(sokSelf->szOutBuffer != NULL, -1, ;, "generic error");

    sokSelf->iSocket = iSocket;

    return(0);
}

int ASRV_TcpDestroySocket(ASRV_Socket *sokSelf)
{
    if (sokSelf->iSocket > 0) {
        closesocket(sokSelf->iSocket);
    }

    JADA_Free(__FILE__, __LINE__, sokSelf->szInBuffer, sokSelf->iInLength);
    JADA_Free(__FILE__, __LINE__, sokSelf->szOutBuffer, sokSelf->iOutLength);

    memset(sokSelf, 0, sizeof(*sokSelf));

    return(0);
}

//int ASRV_TcpSendMessageZipped(int iSocket, JADA_Message msgOut)
//{
//    int             iBytes, iLength;
//    int             iSent = 0, iRemaining;
//    char            *szZippedPayload, *ptrOut;
//    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgOut;
//    JADA_MsgFooter  *ftrPtr;
//
//    /* Controlla se il socket e' valido */
//    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "Errore generico");
//
//    /* Controlla il footer del messaggio */
//    ftrPtr = (JADA_MsgFooter *) &msgPtr->szPayload[msgPtr->szHeader.iLength];
//    CORE_ReturnValIfFail(msgPtr->szHeader.iType == ftrPtr->iType, -1, ;, "Errore generico");
//
//    /* Comprimi il payload del messaggio in uscita */
//    iLength = msgPtr->szHeader.iLength + sizeof(JADA_MsgFooter);
//    szZippedPayload = JADA_DeflateBuffer(msgPtr->szPayload, iLength, &msgPtr->szHeader.iZippedLength); 
//    CORE_ReturnValIfFail(szZippedPayload != NULL, -1, ;, "Errore generico");
//
//    /* Scrivi l'header del messaggio */
//    ptrOut = (char *)&msgPtr->szHeader;
//    iRemaining = sizeof(JADA_MsgHeader); iSent = 0;
//    do {
//        iBytes = send(iSocket, &ptrOut[iSent], iRemaining, 0);
//        CORE_SilentReturnValIfFail(iBytes > 0, -1, free(szZippedPayload););
//        iSent += iBytes;
//        iRemaining -= iBytes;
//    } while (iRemaining > 0);
//
//    /* Scrivi il payload del messaggio */
//    ptrOut = (char *)szZippedPayload;
//    iRemaining = msgPtr->szHeader.iZippedLength;  iSent = 0;
//    do {
//        iBytes = send(iSocket, &ptrOut[iSent], iRemaining, 0);
//        CORE_SilentReturnValIfFail(iBytes > 0, -1, free(szZippedPayload););
//        iSent += iBytes;
//        iRemaining -= iBytes;
//    } while (iRemaining > 0);
//
//    free(szZippedPayload);
//
//    return(0);
//}
//
//JADA_Message ASRV_TcpReceiveMessageZipped(int iSocket)
//{
//    int             iBytes, iLength;
//    int             iReceived, iRemaining;
//    JADA_MsgHeader  myHeader;
//    JADA_Message    myMessage;
//    JADA_MsgSelf    *msgPtr;
//    JADA_MsgFooter  *ftrPtr;
//    char            *bufPtr, *szUnzippedPayload;
//
//    /* Controlla se il socket e' valido */
//    CORE_ReturnValIfFail(iSocket > 0, NULL, ;, "Errore generico");
//
//    /* Leggi inizialmente l'header del messaggio */
//    iReceived = 0; iRemaining = sizeof(myHeader); 
//    bufPtr = (char *)&myHeader;
//    do {
//        iBytes = recv(iSocket, &bufPtr[iReceived], iRemaining, 0);
//        CORE_SilentReturnValIfFail(iBytes > 0, NULL, ;);
//        iReceived += iBytes;
//        iRemaining -= iBytes;
//    } while (iRemaining > 0);
//    CORE_ReturnValIfFail(iReceived == sizeof(myHeader), NULL, ;, "iReceived == sizeof(myHeader)");
//    CORE_ReturnValIfFail(myHeader.iLength > 0, NULL, ;, "myHeader.iLength > 0");
//
//    /* Crea il messaggio di dimensione adeguata */
//    iLength = CORE_Max(myHeader.iLength, myHeader.iZippedLength);
//    myMessage = JADA_MsgNew(__FILE__, __LINE__, myHeader.iType, myHeader.iId, iLength);
//    CORE_ReturnValIfFail(myMessage != NULL, NULL, ;, "errore sulla JADA_MsgNew");
//    msgPtr = (JADA_MsgSelf *) myMessage;
//    msgPtr->szHeader.iLength = myHeader.iLength;
//
//    /* A seguire leggi il body del messaggio */
//    iReceived = 0; iRemaining = myHeader.iZippedLength; 
//    bufPtr = (char *)&msgPtr->szPayload[0];
//    do {
//        iBytes = recv(iSocket, &bufPtr[iReceived], iRemaining, 0);
//        CORE_ReturnValIfFail(iBytes > 0, NULL, ;, "errore sulla recv");
//        iReceived += iBytes;
//        iRemaining -= iBytes;
//    } while (iRemaining > 0);
//
//    /* Decomprimi il payload del messaggio in ingresso */
//    iLength = msgPtr->szHeader.iLength +  sizeof(JADA_MsgFooter);
//    szUnzippedPayload = JADA_InflateBuffer(iLength, msgPtr->szPayload, myHeader.iZippedLength);
//    CORE_ReturnValIfFail(szUnzippedPayload != NULL, NULL, ;, "Errore generico");
//    memcpy(msgPtr->szPayload, szUnzippedPayload, iLength);
//    free(szUnzippedPayload);
//
//    /* Controlla il footer del messaggio */
//    ftrPtr = (JADA_MsgFooter *) &msgPtr->szPayload[myHeader.iLength];
//    CORE_ReturnValIfFail(msgPtr->szHeader.iType == ftrPtr->iType, NULL, ;, "Errore generico");
//
//    return(myMessage);
//}

//int ASRV_TcpSendMessageVersione2(int iSocket, JADA_Message msgOut)
//{
//    int             iRet;
//
//    /* Controlla se il socket e' valido */
//    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "Errore generico");
//
//    /* Scrivi il messaggio */
//    iRet = JADA_TcpSendMessageToSocket(iSocket, msgOut);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    return(0);
//}
//
//JADA_Message ASRV_TcpReceiveMessageVersione2(int iSocket)
//{
//    int             iRet, iTimeDelay;
//    JADA_Message    myMessage;
//    JADA_MsgHeader  msgHeader;
//
//    /* Controlla se il socket e' valido */
//    CORE_ReturnValIfFail(iSocket > 0, NULL, ;, "Errore generico");
//
//    /* Leggi il messaggio dal socket */
//    myMessage = JADA_TcpReceiveMessageFromSocket(__FILE__, __LINE__, iSocket);
//    CORE_SilentReturnValIfFail(myMessage != NULL, NULL, ;);
//
//    iRet = JADA_MsgGetHeader(myMessage, &msgHeader);
//	CORE_LogIfFail(iRet == 0, "Errore generico");
//    iRet = JADA_MsgGetTimeDelay(myMessage, &iTimeDelay);
//	//JADA_LogMessage("Time delay sul messaggio %s (IN): %d\n", 
//    //                JADA_MsgTypeToString(msgHeader.iType), iTimeDelay);
//
//    return(myMessage);
//}

int ASRV_TcpSendMessage(ASRV_Socket *sokSelf, JADA_Message msgOut)
{
    int             iRet;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(sokSelf->iSocket > 0, -1, ;, "Errore generico");

    /* Aggiorna il contatore di messaggi sul canale */
    iRet = JADA_MsgSetCounter(msgOut, sokSelf->iCounter++);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Scrivi il messaggio */
    iRet = JADA_TcpSendMessageToSocket(sokSelf->iSocket, sokSelf->szOutBuffer, sokSelf->iOutLength, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

JADA_Message ASRV_TcpReceiveMessage(ASRV_Socket *sokSelf)
{
    int             iRet, iTimeDelay;
    JADA_Message    myMessage;
    JADA_MsgHeader  msgHeader;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(sokSelf->iSocket > 0, NULL, ;, "Errore generico");

    /* Leggi il messaggio dal socket */
    myMessage = JADA_TcpReceiveMessageFromSocket(__FILE__, __LINE__, sokSelf->iSocket, sokSelf->szInBuffer, sokSelf->iInLength);
    CORE_SilentReturnValIfFail(myMessage != NULL, NULL, ;);

    iRet = JADA_MsgGetHeader(myMessage, &msgHeader);
	CORE_LogIfFail(iRet == 0, "Errore generico");
    iRet = JADA_MsgGetTimeDelay(myMessage, &iTimeDelay);
	//JADA_LogMessage("Time delay sul messaggio %s (IN): %d\n", 
    //                JADA_MsgTypeToString(msgHeader.iType), iTimeDelay);

    return(myMessage);
}

/******************************************************************************/
/*                                                                            */
/*   R E Q U E S T                                                            */
/*                                                                            */
/******************************************************************************/
//int __stdcall ASRV_TcpSendRequestMessageToAgent(ASRV_Request *reqPtr, int iOpcode, void *pPayload, int iLength)
//{
//    int             iRet;
//    JADA_Message    msgOut;
//    ASRV_Session    *sesAgent;
//
//    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, myWhoAmI, reqPtr->iId, iOpcode, iLength);
//    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
//    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iLength);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    sesAgent = ASRV_GetSessionById(__FILE__, __LINE__, reqPtr->iAgentSession);
//    CORE_ReturnValIfFail(sesAgent != NULL, -1, ;, "Errore generico");
//    iRet = ASRV_TcpSendMessage(&sesAgent->sokSess, msgOut);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    return(0);
//}

//int __stdcall ASRV_TcpSendRequestMessageToManager(ASRV_Request *reqPtr, int iOpcode, void *pPayload, int iLength)
//{
//    int             iRet;
//    JADA_Message    msgOut;
//    ASRV_Session    *sesManager;
//
//    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, myWhoAmI, reqPtr->iId, iOpcode, iLength);
//    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
//    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iLength);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    sesManager = ASRV_GetSessionById(__FILE__, __LINE__, reqPtr->iManagerSession);
//    CORE_ReturnValIfFail(sesManager != NULL, -1, ;, "Errore generico");
//    iRet = ASRV_TcpSendMessage(&sesManager->sokSess, msgOut);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    return(0);
//}

/******************************************************************************/
/*                                                                            */
/*   S E S S I O N                                                            */
/*                                                                            */
/******************************************************************************/
int ASRV_TcpSendSessionMessageToAgent(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iLength)
{
    int                 iRet;
    JADA_Message        msgOut;
    ASRV_Connection     *connPtr;

    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), sesPtr->dbSess.iId, iOpcode, iLength);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    connPtr = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    CORE_ReturnValIfFail(ASRV_IsValidConnection(connPtr), -1, ;, "Errore generico");
    iRet = ASRV_TcpSendMessage(&connPtr->sokConn, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_TcpSendSessionMessageToManager(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iLength)
{
    int                 iRet;
    JADA_Message        msgOut;
    ASRV_Connection     *connPtr;

    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), sesPtr->dbSess.iId, iOpcode, iLength);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    connPtr = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
    CORE_ReturnValIfFail(ASRV_IsValidConnection(connPtr), -1, ;, "Errore generico");
    iRet = ASRV_TcpSendMessage(&connPtr->sokConn, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_TcpSendSessionMessageToPartner(ASRV_Session *sesPtr, JADA_ConnId iSourceId, int iOpcode, void *pPayload, int iLength)
{
    int                 iRet;
    JADA_Message        msgOut;
    ASRV_Connection     *connPtr;

    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), sesPtr->dbSess.iId, iOpcode, iLength);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    if (JADA_CompareJadaId(&iSourceId, &sesPtr->dbSess.iIdConnAgent)) {
        connPtr = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
    }
    else {
        connPtr = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    }
    CORE_ReturnValIfFail(ASRV_IsValidConnection(connPtr), -1, ;, "Errore generico");
    iRet = ASRV_TcpSendMessage(&connPtr->sokConn, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_TcpForwardSessionMessageToPartner(ASRV_Session *sesPtr, JADA_ConnId iSourceId, JADA_Message msgOut)
{
    int                 iRet;
    ASRV_Connection     *connPtr;

    if (JADA_CompareJadaId(&iSourceId, &sesPtr->dbSess.iIdConnAgent)) {
        connPtr = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnManager);
    }
    else {
        connPtr = ASRV_GetConnectionById(sesPtr->dbSess.iIdConnAgent);
    }
    CORE_ReturnValIfFail(ASRV_IsValidConnection(connPtr), -1, ;, "Errore generico");
    iRet = ASRV_TcpSendMessage(&connPtr->sokConn, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_TcpForwardMessage(ASRV_Connection *connPtr, JADA_Message msgOut)
{
    int     iRet;

    iRet = ASRV_TcpSendMessage(&connPtr->sokConn, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

/******************************************************************************/
/*                                                                            */
/*   C O N N E C T I O N                                                      */
/*                                                                            */
/******************************************************************************/
int ASRV_TcpSendConnectionMessage(ASRV_Connection *connPtr, int iOpcode, void *pPayload, int iLength)
{
    int                 iRet;
    JADA_Message        msgOut;

    msgOut = JADA_MsgConnNew(__FILE__, __LINE__, JADA_GetWhoAmI(), connPtr->dbConn.iId, iOpcode, iLength);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = ASRV_TcpSendMessage(&connPtr->sokConn, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

