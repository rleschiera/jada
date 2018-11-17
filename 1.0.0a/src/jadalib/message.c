#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"

static JADA_CodeValue lstTypes[] = {
    JADA_MSGTYPE_CONNECTION,             "JADA_MSGTYPE_CONNECTION",         
    //JADA_MSGTYPE_REQUEST,                "JADA_MSGTYPE_REQUEST",            
    JADA_MSGTYPE_SUPPORT,                "JADA_MSGTYPE_SUPPORT",            
    JADA_MSGTYPE_AUDIOFRAME,             "JADA_MSGTYPE_AUDIOFRAME",         
    JADA_MSGTYPE_VIDEOFRAME,             "JADA_MSGTYPE_VIDEOFRAME",         
    JADA_MSGTYPE_DESKTOPFRAME,           "JADA_MSGTYPE_DESKTOPFRAME",       
    JADA_MSGTYPE_DRAWFRAME,              "JADA_MSGTYPE_DRAWFRAME",          
    JADA_MSGTYPE_FTPFRAME,               "JADA_MSGTYPE_FTPFRAME",           
    -1,                                  NULL
};

static JADA_CodeValue lstMessages[] = {
/* Opcode dei messaggi di protocollo: CONNESSIONE */
    JADA_MSG_CONNECT_CONNECTION,         "JADA_MSG_CONNECT_CONNECTION",        
    JADA_MSG_CONNECTION_CONNECTED,       "JADA_MSG_CONNECTION_CONNECTED",      
    JADA_MSG_OPEN_CONNECTION,            "JADA_MSG_OPEN_CONNECTION",        
    JADA_MSG_CONNECTION_OPENED,          "JADA_MSG_CONNECTION_OPENED",      
    //JADA_MSG_LOAD_SESSION,               "JADA_MSG_LOAD_SESSION",       
    //JADA_MSG_SESSION_LOADED,             "JADA_MSG_SESSION_LOADED",     
    JADA_MSG_LIST_REQUESTS,              "JADA_MSG_LIST_REQUESTS",          
    JADA_MSG_REQUESTS_LIST,              "JADA_MSG_REQUESTS_LIST",          
    JADA_MSG_LIST_PENDING_REQUESTS,      "JADA_MSG_LIST_PENDING_REQUESTS",  
    JADA_MSG_PENDING_REQUESTS_LIST,      "JADA_MSG_PENDING_REQUESTS_LIST",  
    JADA_MSG_GET_LIST_COUNTERS,          "JADA_MSG_GET_LIST_COUNTERS",      
    JADA_MSG_LIST_COUNTERS,              "JADA_MSG_LIST_COUNTERS",    
    JADA_MSG_GET_COMBO,                  "JADA_MSG_GET_COMBO", 
    JADA_MSG_CONN_PARAMETERS_LIST,       "JADA_MSG_CONN_PARAMETERS_LIST",        
/* Opcode dei messaggi di protocollo: tutte le sessioni */
    JADA_MSG_PING,                       "JADA_MSG_PING",                   
    JADA_MSG_PING_REPLY,                 "JADA_MSG_PING_REPLY",             
    JADA_MSG_RESULT,                     "JADA_MSG_RESULT",
/* Opcode dei messaggi di protocollo: SUPPORT */
    JADA_MSG_OPEN_SUPPORT,               "JADA_MSG_OPEN_SUPPORT",           
    JADA_MSG_SUPPORT_OPENED,             "JADA_MSG_SUPPORT_OPENED",         
    JADA_MSG_ACCEPT_SUPPORT,             "JADA_MSG_ACCEPT_SUPPORT",         
    JADA_MSG_SUPPORT_ACCEPTED,           "JADA_MSG_SUPPORT_ACCEPTED",       
    JADA_MSG_LOAD_SUPPORT,               "JADA_MSG_LOAD_SUPPORT",           
    JADA_MSG_SUPPORT_LOADED,             "JADA_MSG_SUPPORT_LOADED",         
    JADA_MSG_CONNECT_SUPPORT,            "JADA_MSG_CONNECT_SUPPORT",        
    JADA_MSG_SUPPORT_CONNECTED,          "JADA_MSG_SUPPORT_CONNECTED",      
    JADA_MSG_SOLICIT_SUPPORT,            "JADA_MSG_SOLICIT_SUPPORT",        
    JADA_MSG_SUPPORT_SOLICITED,          "JADA_MSG_SUPPORT_SOLICITED",      
    JADA_MSG_CLOSE_SUPPORT,              "JADA_MSG_CLOSE_SUPPORT",          
    JADA_MSG_SUPPORT_CLOSED,             "JADA_MSG_SUPPORT_CLOSED",         
    JADA_MSG_RESUME_SUPPORT,             "JADA_MSG_RESUME_SUPPORT",         
    JADA_MSG_SUPPORT_RESUMED,            "JADA_MSG_SUPPORT_RESUMED",        
    JADA_MSG_SUPPORT_SUSPENDED,          "JADA_MSG_SUPPORT_SUSPENDED",      
    JADA_MSG_LIST_PARAMETERS,            "JADA_MSG_LIST_PARAMETERS",
    JADA_MSG_PARAMETERS_LIST,            "JADA_MSG_PARAMETERS_LIST",
    //JADA_MSG_REQUEST_SERVICE,            "JADA_MSG_REQUEST_SERVICE",        
    //JADA_MSG_ACCEPT_SERVICE,             "JADA_MSG_ACCEPT_SERVICE",        
/* Opcode dei messaggi di protocollo: MEDIA */
    JADA_MSG_OPEN_CALL,                  "JADA_MSG_OPEN_CALL",              
    JADA_MSG_CALL_OPENED,                "JADA_MSG_CALL_OPENED",            
    JADA_MSG_CLOSE_CALL,                 "JADA_MSG_CLOSE_CALL",             
    JADA_MSG_ABORT_CALL,                 "JADA_MSG_ABORT_CALL",             
    /* Opcode dei messaggi di protocollo: VNC */
    JADA_MSG_OPEN_DESKTOP,               "JADA_MSG_OPEN_DESKTOP",           
    JADA_MSG_DESKTOP_OPENED,             "JADA_MSG_DESKTOP_OPENED",         
    JADA_MSG_OPEN_WHITEBOARD,            "JADA_MSG_OPEN_WHITEBOARD",        
    JADA_MSG_WHITEBOARD_OPENED,          "JADA_MSG_WHITEBOARD_OPENED",      
    JADA_MSG_CLEAN_WHITEBOARD,           "JADA_MSG_CLEAN_WHITEBOARD",       
    JADA_MSG_CLOSE_WHITEBOARD,           "JADA_MSG_CLOSE_WHITEBOARD",       
    JADA_MSG_WHITEBOARD_CLOSED,          "JADA_MSG_WHITEBOARD_CLOSED",      
    JADA_MSG_SET_DRAW_POINTER,           "JADA_MSG_SET_DRAW_POINTER",       
    JADA_MSG_RESET_DRAW_POINTER,         "JADA_MSG_RESET_DRAW_POINTER",     
    JADA_MSG_CLOSE_DESKTOP,              "JADA_MSG_CLOSE_DESKTOP",          
    JADA_MSG_DESKTOP_CLOSED,             "JADA_MSG_DESKTOP_CLOSED",         
    /* Opcode dei messaggi di protocollo: FTP */
    JADA_MSG_OPEN_FTP,                   "JADA_MSG_OPEN_FTP",           
    JADA_MSG_FTP_OPENED,                 "JADA_MSG_FTP_OPENED",         
    JADA_MSG_LIST_DIRECTORY,             "JADA_MSG_LIST_DIRECTORY",         
    JADA_MSG_DIRECTORY_LIST,             "JADA_MSG_DIRECTORY_LIST",         
    JADA_MSG_GET_FILE,                   "JADA_MSG_GET_FILE",               
    JADA_MSG_SEND_CHAT_TEXT,             "JADA_MSG_SEND_CHAT_TEXT",         
    /* End */
    -1,                                  NULL
};


/******************************************************************************
 *
 *      G E N E R I C     M E S S A G E
 *
 *****************************************************************************/
DllExport JADA_Message __stdcall JADA_MsgNew(char *szFile, int iLine, int iType, JADA_MsgId iId, int iLength)
{
    JADA_MsgSelf    *msgPtr;
#ifdef UNIX
    struct timeb    timCreation;
#else
    struct _timeb   timCreation;
#endif

    msgPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_MsgSelf));
    CORE_ReturnValIfFail(msgPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di messaggio");

#ifdef UNIX
    ftime(&timCreation);
#else
    _ftime(&timCreation);
#endif
    msgPtr->szHeader.iCreationSecs = timCreation.time;
    msgPtr->szHeader.iCreationMsecs = timCreation.millitm;
    msgPtr->szHeader.iType = iType;
    msgPtr->szHeader.iMsgId = iId;
    msgPtr->szHeader.iLength = iLength;
    msgPtr->szHeader.iZippedLength = 0;
    msgPtr->szPayload = JADA_Calloc(szFile, iLine, 1, iLength);
    CORE_ReturnValIfFail(msgPtr->szPayload != NULL, NULL, ;, "Errore nell'allocazione del payload di messaggio");
    msgPtr->iMagic = JADA_MAGIC_MESSAGE;

    switch (iType) {
    case JADA_MSGTYPE_AUDIOFRAME:
    case JADA_MSGTYPE_DRAWFRAME:
    case JADA_MSGTYPE_FTPFRAME:
    case JADA_MSGTYPE_VIDEOFRAME:
        msgPtr->iZipped = TRUE;
        break;
    case JADA_MSGTYPE_DESKTOPFRAME:
        // Il protocollo VNC e' gia' compresso!
        msgPtr->iZipped = FALSE;
        if (iLength > 1024) {
            msgPtr->iZipped = FALSE;
        }
        break;
    case JADA_MSGTYPE_CONNECTION:
    //case JADA_MSGTYPE_REQUEST:
    case JADA_MSGTYPE_SUPPORT:
        msgPtr->iZipped = TRUE;
        if (iLength > 1024) {
            msgPtr->iZipped = TRUE;
        }
        break;
    default:
        CORE_ReturnValIfFail(0, NULL, ;, "Tipo di messaggio non valido");
    }

    return((JADA_Message) msgPtr);
}

DllExport int __stdcall JADA_MsgDestroy(char *szFile, int iLine, JADA_Message msgSelf)
{
    JADA_MsgSelf *msgPtr = (JADA_MsgSelf *) msgSelf;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

    if (msgPtr->szPayload != NULL) {
        JADA_Free(szFile, iLine, msgPtr->szPayload, 0);
    }
    JADA_Free(szFile, iLine, msgSelf, sizeof(*msgSelf));

    return(0);
}

DllExport int __stdcall JADA_MsgGetHeader(JADA_Message msgSelf, JADA_MsgHeader *szHeader)
{
    JADA_MsgSelf *msgPtr = (JADA_MsgSelf *) msgSelf;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

    memcpy(szHeader, &msgPtr->szHeader, sizeof(msgPtr->szHeader));

    return(0);
}

DllExport int __stdcall JADA_MsgGetCounter(JADA_Message msgSelf, int *piCounter)
{
    JADA_MsgSelf *msgPtr = (JADA_MsgSelf *) msgSelf;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

    *piCounter = msgPtr->szHeader.iCounter;

    return(0);
}

DllExport int __stdcall JADA_MsgSetCounter(JADA_Message msgSelf, int iCounter)
{
    JADA_MsgSelf *msgPtr = (JADA_MsgSelf *) msgSelf;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

    msgPtr->szHeader.iCounter = iCounter;

    return(0);
}

DllExport int __stdcall JADA_MsgGetTimeDelay(JADA_Message msgSelf, int *piTimeDelay)
{
    int             iTimeDelay;
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgSelf;
#ifdef UNIX
    struct timeb    timNow;
#else
    struct _timeb   timNow;
#endif

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

#ifdef UNIX
    ftime(&timNow);
#else
    _ftime(&timNow);
#endif
    iTimeDelay = JADA_GetTimeDelay(msgPtr->szHeader.iCreationSecs, msgPtr->szHeader.iCreationMsecs, timNow.time, timNow.millitm);
    *piTimeDelay = iTimeDelay;

    return(0);
}

DllExport int __stdcall JADA_MsgSetPayload(JADA_Message msgSelf, void *szPayload, int iLength)
{
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgSelf;
    int             iZippedLength;
    unsigned char   *szRealPayload;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1); 
    CORE_SilentReturnValIfFail(szPayload != NULL, 0, ;); 
    CORE_SilentReturnValIfFail(iLength > 0, 0, ;); 

    if (msgPtr->iZipped == TRUE) {
        szRealPayload = JADA_DeflateBuffer(szPayload, iLength, &iZippedLength); 
        CORE_ReturnValIfFail(szRealPayload != NULL, -1, ;, "Errore nella compressione del payload");
        //if (iZippedLength < iLength) {
            msgPtr->szHeader.iZippedLength = iZippedLength;
        //}
        //else {
        //    msgPtr->iZipped = FALSE;
        //    memcpy(szRealPayload, szPayload, iLength);
        //    JADA_LogMessage("Payload non comprimibile (%d vs. %d)\n", iLength, iZippedLength);
        //}
    }
    else {
        szRealPayload = JADA_Calloc(__FILE__, __LINE__, 1, iLength); 
        CORE_ReturnValIfFail(szRealPayload != NULL, -1, ;, "Errore nella compressione del payload");
        memcpy(szRealPayload, szPayload, iLength);
    }

    msgPtr->szHeader.iLength = iLength;
    if (msgPtr->szPayload != NULL) {
        JADA_Free(__FILE__, __LINE__, msgPtr->szPayload, 0);
    }
    msgPtr->szPayload = szRealPayload;

    return(0);
}

DllExport int __stdcall JADA_MsgGetPayload(JADA_Message msgSelf, void *szPayload, int iLength)
{
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgSelf;
    unsigned char   *szRealPayload;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  
    if (iLength < msgPtr->szHeader.iLength)
        CORE_ReturnValIfFail(iLength >= msgPtr->szHeader.iLength, -1, ;, "Payload troppo lungo per il buffer");

    if (msgPtr->iZipped == TRUE) {
        szRealPayload = JADA_InflateBuffer(msgPtr->szHeader.iLength, msgPtr->szPayload, msgPtr->szHeader.iZippedLength);
        CORE_ReturnValIfFail(szRealPayload != NULL, -1, ;, "Errore nella decompressione del payload");
        memcpy(szPayload, szRealPayload, msgPtr->szHeader.iLength);
        JADA_Free(__FILE__, __LINE__, szRealPayload, 0);
    }
    else {
        memcpy(szPayload, msgPtr->szPayload, msgPtr->szHeader.iLength);
    }

    return(0);
}

DllExport int __stdcall JADA_MsgSerialize(JADA_Message msgSelf, void *szBuffer, int iLength, int *piOutLength)
{
    int             iRet, iTotalLength;
    int             iOffset = 0, iOffset2 = 0;
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgSelf;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

    /*- Inizia a mettere in intero fittizio che alla fine conterrà la lunghezza totale del messaggio */
    iRet = JADA_BufSetInt32(szBuffer, iLength, 0, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Inserisci il magic di apertura */
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Inserisci il flag 'zipped' */
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->iZipped, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Inserisci l'header del messaggio */
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iCounter, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iType, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetJadaMsgId(szBuffer, iLength, msgPtr->szHeader.iMsgId, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iZippedLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iCreationSecs, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iCreationMsecs, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Inserisci la session header del messaggio */
    iRet = JADA_BufSetJadaSessId(szBuffer, iLength, msgPtr->szSesHeader.iId, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szSesHeader.iWhoAmI, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szSesHeader.iOpcode, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Inserisci il payload del messaggio */
    if (msgPtr->iZipped == TRUE) {
        iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iZippedLength, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetString(szBuffer, iLength, msgPtr->szPayload, msgPtr->szHeader.iZippedLength, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }
    else {
        iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->szHeader.iLength, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetString(szBuffer, iLength, msgPtr->szPayload, msgPtr->szHeader.iLength, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    /*- Inserisci il magic di chiusura */
    iRet = JADA_BufSetInt32(szBuffer, iLength, msgPtr->iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Inserisci la lunghezza totale del messaggio */
    iTotalLength = iOffset;
    iRet = JADA_BufSetInt32(szBuffer, iLength, iTotalLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    /*- Aggiorna la lunghezza totale del messaggio in testa al buffer */
    iRet = JADA_BufSetInt32(szBuffer, iLength, iTotalLength, &iOffset2);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    *piOutLength = iOffset;

    return(0);
}

DllExport JADA_Message __stdcall JADA_MsgDeserialize(void *szBuffer, int iLength)
{
    int             iRet, iOffset = 0;
    int             iZipped, iPayloadLength, iTotalLength, iTotalLength2;
    int             iMagic;
    JADA_Message    msgSelf;
    JADA_MsgHeader  szHeader;
    JADA_MsgSelf    *msgPtr;

    /*- Leggi la lunghezza totale del messaggio */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iTotalLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, StringFormat("errore nella JADA_BufGetInt32(%d)\n", iOffset));
    CORE_ReturnValIfFail(iTotalLength > 0, NULL, ;, "la lunghezza del messaggio e' negativa");

    /*- Leggi il magic di apertura */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    CORE_ReturnValIfFail(iMagic == JADA_MAGIC_MESSAGE, NULL, ;, "errore generico");

    /*- Leggi il flag 'zipped' */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iZipped, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");

    /*- Leggi l'header del messaggio */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &szHeader.iCounter, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &szHeader.iType, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetJadaMsgId(szBuffer, iLength, &szHeader.iMsgId, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &szHeader.iLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &szHeader.iZippedLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &szHeader.iCreationSecs, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &szHeader.iCreationMsecs, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");

    /* Crea il messaggio di uscita */
    if (iZipped == TRUE) {
        msgSelf = JADA_MsgNew(__FILE__, __LINE__, szHeader.iType, szHeader.iMsgId, szHeader.iZippedLength);
        CORE_ReturnValIfFail(msgSelf != NULL, NULL, ;, "errore generico");
    }
    else {
        msgSelf = JADA_MsgNew(__FILE__, __LINE__, szHeader.iType, szHeader.iMsgId, szHeader.iLength);
        CORE_ReturnValIfFail(msgSelf != NULL, NULL, ;, "errore generico");
    }

    /* Inizializza i valori del messaggio creato con quelli gia' letti */
    msgPtr = (JADA_MsgSelf *) msgSelf;
    msgPtr->iZipped = iZipped;
    msgPtr->szHeader.iCounter = szHeader.iCounter;
    msgPtr->szHeader.iLength = szHeader.iLength;
    msgPtr->szHeader.iZippedLength = szHeader.iZippedLength;
    msgPtr->szHeader.iCreationSecs = szHeader.iCreationSecs;
    msgPtr->szHeader.iCreationMsecs = szHeader.iCreationMsecs;

    /*- Leggi la session header del messaggio */
    iRet = JADA_BufGetJadaSessId(szBuffer, iLength, &msgPtr->szSesHeader.iId, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &msgPtr->szSesHeader.iWhoAmI, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &msgPtr->szSesHeader.iOpcode, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");

    /*- Leggi il payload del messaggio */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iPayloadLength, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    if (msgPtr->iZipped == TRUE) {
        CORE_ReturnValIfFail(iPayloadLength == msgPtr->szHeader.iZippedLength, NULL, ;, "errore generico");
        iRet = JADA_BufGetString(szBuffer, iLength, msgPtr->szPayload, msgPtr->szHeader.iZippedLength, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    }
    else {
        CORE_ReturnValIfFail(iPayloadLength == msgPtr->szHeader.iLength, NULL, ;, "errore generico");
        iRet = JADA_BufGetString(szBuffer, iLength, msgPtr->szPayload, msgPtr->szHeader.iLength, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    }

    /*- Leggi il magic di chiusura */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    CORE_ReturnValIfFail(iMagic == JADA_MAGIC_MESSAGE, NULL, ;, "errore generico");

    /*- Leggi la lunghezza totale del messaggio */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iTotalLength2, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    CORE_ReturnValIfFail(iTotalLength == iTotalLength2, NULL, ;, "errore generico");

    return(msgSelf);
}

/******************************************************************************
 *
 *      C O N N E C T I O N     M E S S A G E
 *
 *****************************************************************************/
DllExport JADA_Message __stdcall JADA_MsgConnNew(char *szFile, int iLine, int iWhoAmI, JADA_ConnId iConnId, int iOpcode, int iLength)
{
    JADA_Message    msgSelf;
    JADA_MsgSelf    *msgPtr;
    JADA_MsgId      iMsgId;


    JADA_ConnIdToMsgId(&iConnId, &iMsgId);
    msgSelf = JADA_MsgNew(szFile, iLine, JADA_MSGTYPE_CONNECTION, iMsgId, iLength);
    CORE_ReturnValIfFail(msgSelf != NULL, NULL, ;, "Errore nell'allocazione del descrittore di messaggio");
    msgPtr = (JADA_MsgSelf *) msgSelf;

    msgPtr->szSesHeader.iWhoAmI = iWhoAmI;
    JADA_ConnIdToSessId(&iConnId, &msgPtr->szSesHeader.iId);
    msgPtr->szSesHeader.iOpcode = iOpcode;

    return((JADA_Message) msgPtr);
}

/******************************************************************************
 *
 *      S E S S I O N     M E S S A G E
 *
 *****************************************************************************/
DllExport JADA_Message __stdcall JADA_MsgSesNew(char *szFile, int iLine, int iWhoAmI, JADA_SessId iSessionId, int iOpcode, int iLength)
{
    JADA_Message    msgSelf;
    JADA_MsgSelf    *msgPtr;
    JADA_MsgId      iMsgId;

    JADA_SessIdToMsgId(&iSessionId, &iMsgId);
    msgSelf = JADA_MsgNew(szFile, iLine, JADA_MSGTYPE_SUPPORT, iMsgId, iLength);
    CORE_ReturnValIfFail(msgSelf != NULL, NULL, ;, "Errore nell'allocazione del descrittore di messaggio");
    msgPtr = (JADA_MsgSelf *) msgSelf;

    msgPtr->szSesHeader.iWhoAmI = iWhoAmI;
    msgPtr->szSesHeader.iId = iSessionId;
    msgPtr->szSesHeader.iOpcode = iOpcode;

    return((JADA_Message) msgPtr);
}

DllExport int __stdcall JADA_MsgSesSetPayload(JADA_Message msgSelf, void *szPayload, int iLength)
{
    int     iRet;

    iRet = JADA_MsgSetPayload(msgSelf, szPayload, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del payload di sessione");

    return(0);
}

DllExport int __stdcall JADA_MsgSesGetPayload(JADA_Message msgSelf, void *szPayload, int iLength)
{
    int     iRet;

    iRet = JADA_MsgGetPayload(msgSelf, szPayload, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del payload di sessione");

    return(0);
}

DllExport int __stdcall JADA_MsgSesGetHeader(JADA_Message msgSelf, JADA_SessionMsgHeader *szHeader)
{
    JADA_MsgSelf *msgPtr = (JADA_MsgSelf *) msgSelf;

    JADA_ReturnValIfInvalidMessage(msgPtr, -1);  

    memcpy(szHeader, &msgPtr->szSesHeader, sizeof(msgPtr->szSesHeader));

    return(0);
}

DllExport char * __stdcall JADA_MsgTypeToString(int iType)
{
    static FixString    myTypeString;
    int                 i;

    sprintf(myTypeString, "Opcode sconosciuto (%d)", iType);

    for (i = 0; lstTypes[i].szValue != NULL; i++) {
        if (lstTypes[i].iCode == iType) {
            strcpy(myTypeString, lstTypes[i].szValue);
            break;
        }
    }

    return(myTypeString);
}


DllExport char * __stdcall JADA_MsgOpcodeToString(int iOpcode)
{
    static FixString    myOpcodeString;
    int                 i;

    sprintf(myOpcodeString, "Opcode sconosciuto (%d)", iOpcode);

    for (i = 0; lstMessages[i].szValue != NULL; i++) {
        if (lstMessages[i].iCode == iOpcode) {
            strcpy(myOpcodeString, lstMessages[i].szValue);
            break;
        }
    }

    return(myOpcodeString);
}
