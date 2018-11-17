#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>



/******************************************************************************
 *
 *   C O M M U N I C A T I O N S     S E R V I C E S
 *
 ******************************************************************************/
void *JADA_DeflateBuffer(void *pvBuffer, int iLength, int *piDeflatedLength) 
{
    int         iRet;
    z_stream    c_stream;
    int         iOutLength;
    void        *pvNewBuffer;

    /*- Create an internal temporary buffer */
    iOutLength = 2 * iLength;
    pvNewBuffer = calloc(iOutLength, sizeof(char));
    CORE_ReturnValIfFail(pvNewBuffer != NULL, NULL, ;, "generic error");
	
    /*- Initialize the deflate activity */
    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    iRet = deflateInit(&c_stream, Z_BEST_SPEED);
    CORE_ReturnValIfFail(iRet == Z_OK, NULL, free(pvNewBuffer);, "generic error");

    /*- Deflate the input buffer */
    c_stream.next_out = (unsigned char *)pvNewBuffer;
    c_stream.avail_out = (uInt)iOutLength;
    c_stream.next_in = pvBuffer;
    c_stream.avail_in = (uInt)iLength;
    iRet = deflate(&c_stream, Z_FINISH);
    CORE_ReturnValIfFail(iRet >= 0, NULL, free(pvNewBuffer);, "generic error");

    /*- Terminate the deflate activity */
    iRet = deflateEnd(&c_stream);
    CORE_ReturnValIfFail(iRet == Z_OK, NULL, free(pvNewBuffer);, "generic error");

    /*- Return the result */
    *piDeflatedLength = c_stream.total_out;
    return(pvNewBuffer);
}

void *JADA_InflateBuffer(int iRealLength, void *pvBuffer, int iLength)
{
    int         iRet;
    void        *pvNewBuffer;
    z_stream    d_stream;
    static int  iCounter = 0;

    /*- Create the temporary buffer */
    pvNewBuffer = calloc(iRealLength, sizeof(char));
    CORE_ReturnValIfFail(pvNewBuffer != NULL, NULL, ;, "generic error");
    
    /*- Initialize the inflate activity */
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    iRet = inflateInit(&d_stream);
    CORE_ReturnValIfFail(iRet == Z_OK, NULL, free(pvNewBuffer);, "generic error");

    /*- Inflate the input buffer */
    d_stream.next_in  = (Bytef *)pvBuffer;
    d_stream.avail_in = (uInt)iLength;
    d_stream.next_out = (Bytef *)pvNewBuffer;
	d_stream.avail_out = (uInt)iRealLength;
    iRet = inflate(&d_stream, Z_NO_FLUSH);
    CORE_ReturnValIfFail(iRet >= 0, NULL, free(pvNewBuffer);, "generic error");

    /*- Terminate the inflate activity */
    iRet = inflateEnd(&d_stream);
    CORE_ReturnValIfFail(iRet == Z_OK, NULL, free(pvNewBuffer);, "generic error");

    /*- Return the buffer */
    return(pvNewBuffer);
}

static int JADA_TcpSendMessageBlock(int iSocket, void *szBlock, int iLength)
{
    char    *ptrOut = szBlock;
    int     iBytes, iRemaining = iLength, iSent = 0;

    do {
        iBytes = send(iSocket, &ptrOut[iSent], iRemaining, 0);
        // CORE_SilentReturnValIfFail(iBytes >= 0, -1, ;);
        CORE_ReturnValIfFail(iBytes >= 0, -1, ;, StringFormat("errore nella send sul socket %d\n", iSocket));
        iSent += iBytes;
        iRemaining -= iBytes;
        CORE_LogIfFail(iRemaining == 0, StringFormat("mancano ancora %d bytes nella send sul socket.", iRemaining));
    } while (iRemaining > 0);

    return(0);
}

static int JADA_TcpReceiveMessageBlock(int iSocket, void *szBlock, int iLength)
{
    int     iBytes;
    int     iReceived = 0, iRemaining = iLength;
    char    *bufPtr = szBlock;

    do {
        iBytes = recv(iSocket, &bufPtr[iReceived], iRemaining, 0);
        CORE_LogIfFail(iBytes != 0, StringFormat("Zero bytes disponibili nella recv dal socket %d.", iSocket));
        CORE_SilentReturnValIfFail(iBytes > 0, -1, ;);
        // CORE_ReturnValIfFail(iBytes >= 0, -1, ;, StringFormat("errore nella recv dal socket %d\n", iSocket));
        iReceived += iBytes;
        iRemaining -= iBytes;
        // CORE_LogIfFail(iRemaining == 0, StringFormat("mancano ancora %d bytes nella recv dal socket.", iRemaining));
    } while (iRemaining > 0);

    return(0);
}

int JADA_TcpSendMessageToSocketVersione2(int iSocket, JADA_Message msgOut)
{
    int             iRet;
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgOut;
    JADA_MsgFooter  myFooter;
    static char     *myOutBuffer;
    static int      myOutBufferLen;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "Errore generico");

    /* Scrivi l'header del messaggio */
    iRet = JADA_TcpSendMessageBlock(iSocket, &msgPtr->szHeader, sizeof(msgPtr->szHeader));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Scrivi il session header del messaggio */
    if (msgPtr->szHeader.iType == JADA_MSGTYPE_SUPPORT) {
        iRet = JADA_TcpSendMessageBlock(iSocket, &msgPtr->szSesHeader, sizeof(msgPtr->szSesHeader));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    /* Scrivi il payload del messaggio */
    iRet = JADA_TcpSendMessageBlock(iSocket, msgPtr->szPayload, msgPtr->szHeader.iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Scrivi il footer del messaggio */
    myFooter.iType = msgPtr->szHeader.iType;
    myFooter.iMsgId = msgPtr->szHeader.iMsgId;
    iRet = JADA_TcpSendMessageBlock(iSocket, &myFooter, sizeof(myFooter));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    //JADA_LogMessage("Messaggio %s inviato: %d caratteri (compressi %d)\n",
    //                JADA_MsgTypeToString(msgPtr->szHeader.iType), 
    //                msgPtr->szHeader.iLength, msgPtr->szHeader.iZippedLength);
    return(0);
}

JADA_Message JADA_TcpReceiveMessageFromSocketVersione2(char *szFile, int iLine, int iSocket)
{
    int                     iRet;
    JADA_MsgHeader          myHeader;
    JADA_SessionMsgHeader   mySesHeader;
    JADA_Message            myMessage;
    JADA_MsgSelf            *msgPtr;
    JADA_MsgFooter          myFooter;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(iSocket > 0, NULL, ;, "Errore generico");

    /* Leggi inizialmente l'header del messaggio */
    iRet = JADA_TcpReceiveMessageBlock(iSocket, &myHeader, sizeof(myHeader));
    CORE_SilentReturnValIfFail(iRet == 0, NULL, ;);

    /* Se presente, leggi il session header del messaggio */
    if (myHeader.iType == JADA_MSGTYPE_SUPPORT) {
        iRet = JADA_TcpReceiveMessageBlock(iSocket, &mySesHeader, sizeof(mySesHeader));
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
    }

    /* Crea il messaggio di dimensione adeguata */
    if (myHeader.iType == JADA_MSGTYPE_SUPPORT) {
        myMessage = JADA_MsgSesNew(szFile, iLine, mySesHeader.iWhoAmI, 
                                   mySesHeader.iId, mySesHeader.iOpcode, myHeader.iLength);
        CORE_ReturnValIfFail(myMessage != NULL, NULL, ;, "errore sulla JADA_MsgSesNew");
    }
    else {
        myMessage = JADA_MsgNew(szFile, iLine, myHeader.iType, myHeader.iMsgId, myHeader.iLength);
        CORE_ReturnValIfFail(myMessage != NULL, NULL, ;, "errore sulla JADA_MsgNew");
    }

    msgPtr = (JADA_MsgSelf *) myMessage;
    msgPtr->szHeader.iCreationSecs = myHeader.iCreationSecs;
    msgPtr->szHeader.iCreationMsecs = myHeader.iCreationMsecs;
    msgPtr->szHeader.iZippedLength = myHeader.iZippedLength;

    /* A seguire leggi il payload del messaggio */
    iRet = JADA_TcpReceiveMessageBlock(iSocket, msgPtr->szPayload, myHeader.iLength);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");

    /* Leggi infine il footer del messaggio */
    iRet = JADA_TcpReceiveMessageBlock(iSocket, &myFooter, sizeof(myFooter));
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");

    /* Controlla il footer del messaggio */
    if (msgPtr->szHeader.iType != myFooter.iType) {
        CORE_ReturnValIfFail(msgPtr->szHeader.iType == myFooter.iType, NULL, ;, "Errore generico");
    }
    CORE_ReturnValIfFail(JADA_CompareJadaId(&msgPtr->szHeader.iMsgId, &myFooter.iMsgId), NULL, ;, "Errore generico");

    //JADA_LogMessage("Messaggio %s ricevuto: %d caratteri (compressi %d)\n",
    //                JADA_MsgTypeToString(msgPtr->szHeader.iType), 
    //                msgPtr->szHeader.iLength, msgPtr->szHeader.iZippedLength);
    return(myMessage);
}

int JADA_TcpSendMessageToSocket(int iSocket, unsigned char *szBuffer, int iLength, JADA_Message msgOut)
{
    int             iRet, iCounter, iOutLength;
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgOut;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "il socket non e' aperto");

    /* Inizializza il buffer di lavoro */
    memset(szBuffer, 0, iLength);

    /* Serializza il messaggio nel buffer */
    iRet = JADA_MsgGetCounter(msgOut, &iCounter);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella JADA_MsgGetCounter");
    iRet = JADA_MsgSerialize(msgOut, szBuffer, iLength, &iOutLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, StringFormat("errore nella JADA_MsgSerialize(%d)", iOutLength));

    /* Invia il messaggio serializzato */
    iRet = JADA_TcpSendMessageBlock(iSocket, szBuffer, iOutLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, StringFormat("Errore nella JADA_TcpSendMessageBlock(%d, %d)", iSocket, iOutLength));
    // JADA_LogMessage("Inviato messaggio %d (%d chars) sul socket %d\n", iCounter, iOutLength, iSocket);

    return(0);
}

JADA_Message JADA_TcpReceiveMessageFromSocket(char *szFile, int iLine, int iSocket, unsigned char *szBuffer, int iLength)
{
    int                     iRet, iOffset = 0, iCounter;
    int                     iMsgLength;
    JADA_Message            myMessage;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(iSocket > 0, NULL, ;, "Errore generico");

    /* Inizializza il buffer di lavoro */
    memset(szBuffer, 0, iLength);

    /* Leggi la lunghezza del messaggio */
    iRet = JADA_TcpReceiveMessageBlock(iSocket, &szBuffer[0], sizeof(iMsgLength));
    CORE_SilentReturnValIfFail(iRet == 0, NULL, ;);
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iMsgLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nella JADA_BufGetInt32");
    CORE_ReturnValIfFail(iMsgLength <= iLength, NULL, ;, "lunghezza del buffer insufficiente");

    /* Leggi il buffer contenente del messaggio */
    iRet = JADA_TcpReceiveMessageBlock(iSocket, &szBuffer[iOffset], iMsgLength);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, StringFormat("errore nella JADA_TcpReceiveMessageBlock(%d)", sizeof(iMsgLength)));

    /* Deserializza il messaggio dal buffer */
    myMessage = JADA_MsgDeserialize(szBuffer, iOffset + iMsgLength);
    CORE_ReturnValIfFail(myMessage != NULL, NULL, ;, StringFormat("errore nella JADA_MsgDeserialize(%d)", iOffset + iMsgLength));
    iRet = JADA_MsgGetCounter(myMessage, &iCounter);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nella lettura del counter dal messaggio");
    // JADA_LogMessage("Messaggio %d ricevuto (%d chars) dal socket %d\n", iCounter, iMsgLength, iSocket);

    return(myMessage);
}

int JADA_VncSendDataToSocket_DaButtare(int iSocket, void *szBlock, int iLength)
{
    char    *ptrOut = szBlock;
    int     iBytes, iRemaining = iLength, iSent = 0;

    do {
        iBytes = send(iSocket, &ptrOut[iSent], iRemaining, 0);
        // CORE_SilentReturnValIfFail(iBytes >= 0, -1, ;);
        CORE_ReturnValIfFail(iBytes >= 0, -1, ;, StringFormat("errore nella send sul socket %d\n", iSocket));
        if (iBytes == 0)
            break;
        iSent += iBytes;
        iRemaining -= iBytes;
        // CORE_LogIfFail(iRemaining == 0, StringFormat("mancano ancora %d bytes nella send sul socket VNC.", iRemaining));
    } while (iRemaining > 0);

    return(iSent);
}

int JADA_VncReceiveDataFromSocket_DaButtare(int iSocket, void *szBlock, int iLength)
{
    int             iRet, iBytes, iLoops = 0;
    int             iReceived = 0, iRemaining = iLength;
    char            *bufPtr = szBlock;
    fd_set          fdSet;
    struct timeval  tv = {0, 10};

    do {
        tv.tv_sec = 0; tv.tv_usec = 1000; // E' 1 ms espresso in microsecondi!!!
        FD_ZERO(&fdSet);
        FD_SET(iSocket, &fdSet);

        iRet = select(2, &fdSet, NULL, NULL, &tv);
        if (iRet > 0) {
	        if (FD_ISSET(iSocket, &fdSet)) {
                iLoops++;
                iBytes = recv(iSocket, &bufPtr[iReceived], iRemaining, 0);
                // CORE_SilentReturnValIfFail(iBytes >= 0, -1, ;);
                CORE_ReturnValIfFail(iBytes >= 0, -1, ;, StringFormat("errore nella recv dal socket %d\n", iSocket));
                iReceived += iBytes;
                iRemaining -= iBytes;
                // CORE_LogIfFail(iRemaining == 0, StringFormat("mancano ancora %d bytes nella recv dal socket VNC.", iRemaining));
            }
        }
        else {
            break;
        }
    } while (iRemaining > 0);

    //CORE_LogIfFail(iLoops <= 1, StringFormat("***** %d cicli nella JADA_VncReceiveDataFromSocket\n", iLoops));
    return(iReceived);
}
