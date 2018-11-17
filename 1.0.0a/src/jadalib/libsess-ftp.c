#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"


DllExport JADA_FTP __stdcall JADA_FtpNew(char *szFile, int iLine, JADA_Connection connSelf)
{
    int             iRet;
    JADA_FtpSelf    *ftpPtr;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);

    ftpPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_FtpSelf));
    CORE_ReturnValIfFail(ftpPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &ftpPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &ftpPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del support");
    ftpPtr->iStatus = JADA_FTPSTATUS_AVAILABLE;
    ftpPtr->connSelf = connSelf;
    time(&ftpPtr->timLastActivity);
    ftpPtr->iMagic = JADA_MAGIC_FTP;

    connPtr->ftpSelf = (JADA_FTP) ftpPtr;

    ftpPtr->lstTransfer = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_FileTransfer), NULL);
    CORE_ReturnValIfFail(ftpPtr->lstTransfer != NULL, NULL, ;, "Errore nella creazione della lista di FTP");

    return((JADA_FTP) ftpPtr);
}

DllExport int __stdcall JADA_FtpDestroy(char *szFile, int iLine, JADA_FTP ftpSelf)
{
    int                     iRet, iRetries = 100;
    JADA_FtpSelf            *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    if (JADA_FtpIsConnected(ftpSelf) == TRUE) {
        iRet = JADA_FtpClose(ftpSelf);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella chiusura della richiesta");

        while (iRetries-- > 0) {
            if (ftpPtr->iStatus != JADA_FTPSTATUS_CLOSED) {
                JADA_Sleep(100);
            }
        }
        CORE_ReturnValIfFail(iRetries > 0, -1, ;, "Timeout nella risposta alla chiusura della richiesta");
    }

    iRet = JADA_LstDestroy(__FILE__, __LINE__, ftpPtr->lstTransfer);

    JADA_Free(__FILE__, __LINE__, ftpPtr, sizeof(*ftpPtr));

    return(0);
}

DllExport int __stdcall JADA_FtpGetRequestId(JADA_FTP ftpSelf, JADA_ReqId *piId)
{
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    *piId = ftpPtr->iRequestId;

    return(0);
}

DllExport int __stdcall JADA_FtpGetSessionId(JADA_FTP ftpSelf, JADA_SessId *piId)
{
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    *piId = ftpPtr->iSessionId;

    return(0);
}

DllExport int __stdcall JADA_FtpIsConnected(JADA_FTP ftpSelf)
{
    int             iIsConnected;
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    switch (ftpPtr->iStatus) {
    case JADA_FTPSTATUS_CONNECTED:
        iIsConnected = TRUE;
        break;
    default:
        iIsConnected = FALSE;
        break;
    }

    return(iIsConnected);
}

DllExport int __stdcall JADA_FtpGetConnection(JADA_FTP ftpSelf, JADA_Connection *connSelf)
{
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    *connSelf = (JADA_Connection) &connPtr;

    return(0);
}

DllExport int __stdcall JADA_FtpOpen(JADA_FTP ftpSelf)
{
    int                 iRet;
    JADA_FtpSelf        *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf       *connPtr;
    JADA_MsgOpenCall    msgPayload;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &ftpPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &ftpPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = ftpPtr->iRequestId;

    iRet = JADA_SendFtpMessage(ftpSelf, JADA_MSG_OPEN_FTP, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio OPEN_FTP");

    return(0);
}

DllExport int __stdcall JADA_FtpClose(JADA_FTP ftpSelf)
{
    int                 iRet;
    JADA_FtpSelf        *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf       *connPtr;
    JADA_MsgCloseCall   msgPayload;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = ftpPtr->iRequestId;
    msgPayload.timCloseTime = time(0);

    iRet = JADA_SendFtpMessage(ftpSelf, JADA_MSG_CLOSE_FTP, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio CLOSE_FTP");

    iRet = JADA_HandleFtpClosed(connPtr, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

DllExport int __stdcall JADA_FtpSendFtpFrame(JADA_FTP ftpSelf, void *szFrameData, int iLength)
{
    int             iRet;
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut = NULL;
    JADA_MsgId      iMsgId;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (connPtr->sokConn.iSocket > 0) {
        JADA_ReqIdToMsgId(&ftpPtr->iRequestId, &iMsgId);
        msgOut = JADA_MsgNew(__FILE__, __LINE__, JADA_MSGTYPE_FTPFRAME, iMsgId, iLength);
        CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "errore nella creazione del FTP frame message");
        iRet = JADA_MsgSetPayload(msgOut, szFrameData, iLength);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione del FTP frame message");
        iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_FTP);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "Errore nell'invio del FTP frame message");
    }

    return(0);
}

DllExport int __stdcall JADA_FtpListDirectory(JADA_FTP ftpSelf, char *szDirName)
{
    int                     iRet;
    JADA_FtpSelf            *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_MsgListDirectory   msgIn;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    memset(&msgIn, 0, sizeof(msgIn));
    msgIn.iSessionId = ftpPtr->iSessionId;
    strcpy(msgIn.szDirName, szDirName);
    iRet = JADA_SendFtpMessage(ftpSelf, JADA_MSG_LIST_DIRECTORY, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
 
    return(0);
}

DllExport int __stdcall JADA_FtpGetFile(JADA_FTP ftpSelf, char *szRemoteFile, char *szLocalFile)
{
    int                     iRet;
    JADA_FtpSelf            *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_MsgFileTransfer    msgIn;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    memset(&msgIn, 0, sizeof(msgIn));
    msgIn.iSessionId = ftpPtr->iSessionId;
    strcpy(msgIn.szFileFrom, szRemoteFile);
    strcpy(msgIn.szFileTo, szLocalFile);
    iRet = JADA_SendFtpMessage(ftpSelf, JADA_MSG_GET_FILE, &msgIn, sizeof(msgIn));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
 
    return(0);
}

DllExport int __stdcall JADA_FtpCancelGetFile(JADA_FTP ftpSelf)
{
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
 
    return(0);
}

DllExport int __stdcall JADA_FtpPutFile(JADA_FTP ftpSelf, char *szLocalFile, char *szRemoteFile)
{
    int             iRet;
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    iRet = JADA_FtpEnqueueFtp(ftpSelf, szLocalFile, szRemoteFile);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'accodamento dell'attività FTP");

    return(0);
}

DllExport int __stdcall JADA_FtpCancelPutFile(JADA_FTP ftpSelf)
{
    int             iRet;
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  

    iRet = JADA_FtpCancelFtp(ftpSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'annullamento dell'attività FTP");

    return(0);
}

int JADA_SendFtpMessage(JADA_FTP ftpSelf, int iOpcode, void *pPayload, int iSize)
{
    int             iRet;
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    /* Crea il messaggio in output */
    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), ftpPtr->iSessionId, iOpcode, iSize);
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSesSetPayload(msgOut, pPayload, iSize);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Invia il messagio in output */
    CORE_ReturnValIfFail(connPtr->sokConn.iSocket > 0, -1, ;, "Errore generico");
    iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_SESSION);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Cancella il messaggio creato */
    //iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int JADA_FtpEnqueueFtp(JADA_FTP ftpSelf, char *szLocalFile, char *szRemoteFile)
{
    int                 iRet;
    JADA_FtpSelf        *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf       *connPtr;
    JADA_FileTransfer   xmitSelf;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&xmitSelf, 0, sizeof(xmitSelf));
    xmitSelf.iSessionId = ftpPtr->iSessionId;
    strcpy(xmitSelf.szFileFrom, szLocalFile);
    strcpy(xmitSelf.szFileTo, szRemoteFile);
    JADA_ChangeStringChar(xmitSelf.szFileFrom, '\\', '/');
    JADA_ChangeStringChar(xmitSelf.szFileTo, '\\', '/');

    iRet = JADA_LstAdd(ftpPtr->lstTransfer, &xmitSelf, sizeof(xmitSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiunta in lista del descrittore FTP");

    return(0);
}

int JADA_FtpCancelFtp(JADA_FTP ftpSelf)
{
    int             iRet;
    JADA_FtpSelf    *ftpPtr = (JADA_FtpSelf *) ftpSelf;
    JADA_ConnSelf   *connPtr;

    JADA_ReturnValIfInvalidFTP(ftpPtr, -1);  
    connPtr = (JADA_ConnSelf *) ftpPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_LstClear(ftpPtr->lstTransfer);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella cancellazione della lista dei descrittori FTP");

    return(0);
}
