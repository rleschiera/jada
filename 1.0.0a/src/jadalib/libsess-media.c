#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"



DllExport JADA_Call __stdcall JADA_CallNew(char *szFile, int iLine, JADA_Connection connSelf)
{
    int             iRet;
    JADA_CallSelf   *callPtr;
    JADA_ConnSelf   *connPtr = (JADA_ConnSelf *) connSelf;

    JADA_ReturnValIfInvalidConnection(connPtr, NULL);

    callPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_CallSelf));
    CORE_ReturnValIfFail(callPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &callPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &callPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del support");
    callPtr->iStatus = JADA_CALLSTATUS_AVAILABLE;
    callPtr->iReset = FALSE;
    callPtr->connSelf = connSelf;
    time(&callPtr->timLastActivity);
    callPtr->iMagic = JADA_MAGIC_CALL;

    connPtr->callSelf = (JADA_Call) callPtr;

    callPtr->lstInAudioFrames = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Frame), NULL);
    CORE_ReturnValIfFail(callPtr->lstInAudioFrames != NULL, NULL, ;, "Errore nella creazione della lista di frames");
    callPtr->lstInVideoFrames = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Frame), NULL);
    CORE_ReturnValIfFail(callPtr->lstInVideoFrames != NULL, NULL, ;, "Errore nella creazione della lista di frames (IN)");
    callPtr->lstOutVideoFrames = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Message), NULL);
    CORE_ReturnValIfFail(callPtr->lstOutVideoFrames != NULL, NULL, ;, "Errore nella creazione della lista di frames (OUT)");

    callPtr->iVideoFrameDelay = 2;
    iRet = JADA_IniGetInt("call", "delay", &callPtr->iVideoFrameDelay);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del parametro call/delay dal file .ini");

    return((JADA_Call) callPtr);
}

DllExport int __stdcall JADA_CallDestroy(char *szFile, int iLine, JADA_Call callSelf)
{
    int             iRet, iRetries = 100;
    JADA_CallSelf   *callPtr = (JADA_CallSelf *) callSelf;
    JADA_Frame      frmSelf;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidCall(callPtr, -1);  

    if (JADA_CallIsConnected(callSelf) == TRUE) {
        iRet = JADA_CallClose(callSelf);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella chiusura della richiesta");

        while (iRetries-- > 0) {
            if (callPtr->iStatus != JADA_CALLSTATUS_CLOSED) {
                JADA_Sleep(100);
            }
        }
        CORE_ReturnValIfFail(iRetries > 0, -1, ;, "Timeout nella risposta alla chiusura della richiesta");
    }

    iRet = JADA_LstGetFirst(callPtr->lstInAudioFrames, &frmSelf, sizeof(frmSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_FrmDestroy(__FILE__, __LINE__, frmSelf);
        iRet = JADA_LstGetNext(callPtr->lstInAudioFrames, &frmSelf, sizeof(frmSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, callPtr->lstInAudioFrames);

    iRet = JADA_LstGetFirst(callPtr->lstInVideoFrames, &frmSelf, sizeof(frmSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_FrmDestroy(__FILE__, __LINE__, frmSelf);
        iRet = JADA_LstGetNext(callPtr->lstInVideoFrames, &frmSelf, sizeof(frmSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, callPtr->lstInVideoFrames);

    iRet = JADA_LstGetFirst(callPtr->lstOutVideoFrames, &frmSelf, sizeof(frmSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_FrmDestroy(__FILE__, __LINE__, frmSelf);
        iRet = JADA_LstGetNext(callPtr->lstOutVideoFrames, &frmSelf, sizeof(frmSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, callPtr->lstOutVideoFrames);

    JADA_Free(__FILE__, __LINE__, callPtr, sizeof(*callPtr));

    return(0);
}

DllExport int __stdcall JADA_CallGetRequestId(JADA_Call callSelf, JADA_ReqId *piId)
{
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidCall(callPtr, -1);  

    *piId = callPtr->iRequestId;

    return(0);
}

DllExport int __stdcall JADA_CallGetSessionId(JADA_Call callSelf, JADA_SessId *piId)
{
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidCall(callPtr, -1);  

    *piId = callPtr->iSessionId;

    return(0);
}

DllExport int __stdcall JADA_CallGetConnection(JADA_Call callSelf, JADA_Connection *connSelf)
{
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidCall(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    *connSelf = (JADA_Connection) &connPtr;

    return(0);
}

DllExport int __stdcall JADA_CallIsConnected(JADA_Call callSelf)
{
    int             iIsConnected;
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidCall(callPtr, -1);  

    switch (callPtr->iStatus) {
    case JADA_CALLSTATUS_CONNECTED:
        iIsConnected = TRUE;
        break;
    default:
        iIsConnected = FALSE;
        break;
    }

    return(iIsConnected);
}

DllExport int __stdcall JADA_CallOpen(JADA_Call callSelf, JADA_ReqId iRequestId)
{
    int                 iRet;
    JADA_CallSelf       *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf       *connPtr;
    JADA_MsgOpenCall    msgPayload;
    FixString           szParams;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  
    callPtr->iRequestId = iRequestId;

    iRet = JADA_SuppGetRequestId(connPtr->suppSelf, &callPtr->iRequestId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");
    iRet = JADA_SuppGetSessionId(connPtr->suppSelf, &callPtr->iSessionId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del support");

    iRet = JADA_ProcExists(JADA_PROC_CALLMANAGER);
    if (iRet == 0) {
        iRet = JADA_ProcKill(JADA_PROC_CALLMANAGER);
    }

    sprintf(szParams, "-audio 1 -video 1 -id %d", connPtr->szServerName, connPtr->iId);
    iRet = JADA_ProcExecute(JADA_PROC_CALLMANAGER, szParams);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    JADA_LogMessage("Ho lanciato il comando %s %s\n", JADA_PROC_CALLMANAGER, szParams);

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = callPtr->iRequestId;

    callPtr->iAudioMuteStatus = FALSE;
    callPtr->iVideoMuteStatus = FALSE;
    iRet = JADA_SendCallMessage(callSelf, JADA_MSG_OPEN_CALL, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio OPEN_CALL");

    return(0);
}

DllExport int __stdcall JADA_CallReset(JADA_Call callSelf, int iReset)
{
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    callPtr->iReset = iReset;

    return(0);
}

DllExport int __stdcall JADA_CallIsReset(JADA_Call callSelf)
{
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    return(callPtr->iReset);
}

DllExport int __stdcall JADA_CallMuteAudio(JADA_Call callSelf, int iMuteStatus)
{
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    if (iMuteStatus != 0) {
        callPtr->iAudioMuteStatus = TRUE;
    }
    else {
        callPtr->iAudioMuteStatus = FALSE;
    }
    return(0);
}

DllExport int __stdcall JADA_CallMuteVideo(JADA_Call callSelf, int iMuteStatus)
{
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf        *connPtr;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (iMuteStatus != 0) {
        callPtr->iVideoMuteStatus = TRUE;
    }
    else {
        callPtr->iVideoMuteStatus = FALSE;
    }
    return(0);
}

DllExport int __stdcall JADA_CallIsAudioMuted(JADA_Call callSelf)
{
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    return(callPtr->iAudioMuteStatus);
}

DllExport int __stdcall JADA_CallIsVideoMuted(JADA_Call callSelf)
{
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    return(callPtr->iVideoMuteStatus);
}

DllExport int __stdcall JADA_CallClose(JADA_Call callSelf)
{
    int                 iRet;
    JADA_CallSelf       *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf       *connPtr;
    JADA_MsgCloseCall   msgPayload;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = callPtr->iRequestId;
    msgPayload.timCloseTime = time(0);

    iRet = JADA_SendCallMessage(callSelf, JADA_MSG_CLOSE_CALL, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio CLOSE_CALL");

    iRet = JADA_HandleCallClosed(connPtr, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura del payload");

    return(0);
}

DllExport int __stdcall JADA_CallAbort(JADA_Call callSelf, int iRetries)
{
    int                 iRet;
    JADA_CallSelf        *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf        *connPtr;
    JADA_Message        msgOut;
    JADA_MsgAbortCall   msgPayload;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    memset(&msgPayload, 0, sizeof(msgPayload));
    msgPayload.iRequestId = callPtr->iRequestId;
    msgPayload.iRetries = iRetries;
    msgPayload.timAbortTime = time(0);

    if (iRetries >= 3) {
        callPtr->iAudioMuteStatus = TRUE;
        callPtr->iVideoMuteStatus = TRUE;

        iRet = JADA_SendCallMessage(callSelf, JADA_MSG_ABORT_CALL, &msgPayload, sizeof(msgPayload));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'invio del messaggio ABORT_CALL");
    }

    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), callPtr->iSessionId, JADA_MSG_ABORT_CALL, sizeof(msgPayload));
    CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "Errore generico");
    iRet = JADA_MsgSetPayload(msgOut, &msgPayload, sizeof(msgPayload));
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella lettura del payload");
    iRet = JADA_HandleCallAborted(connPtr, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella lettura del payload");
    iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgOut);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella lettura del payload");

    return(0);
}

DllExport int __stdcall JADA_CallGetNextAudioFrame(JADA_Call callSelf, int iMilliSecs, JADA_Frame *frmSelf)
{
    int             iRet;
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_LstWait(callPtr->lstInAudioFrames, frmSelf, sizeof(*frmSelf), iMilliSecs);
    if (iRet == 0) {
        iRet = 0;
    }

    return(iRet);
}

DllExport int __stdcall JADA_CallGetNextVideoFrame(JADA_Call callSelf, int iMilliSecs, JADA_Frame *frmSelf)
{
    int             iRet;
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf    *connPtr;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    iRet = JADA_LstWait(callPtr->lstInVideoFrames, frmSelf, sizeof(*frmSelf), iMilliSecs);
    if (iRet == 0) {
        iRet = 0;
    }

    return(iRet);
}

DllExport int __stdcall JADA_CallSendAudioFrame(JADA_Call callSelf, void *szFrameData, int iLength)
{
    int             iRet;
    JADA_CallSelf   *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut = NULL;
    JADA_MsgId      iMsgId;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (callPtr->iAudioMuteStatus == FALSE) {
        if (connPtr->sokConn.iSocket > 0) {
            JADA_SessIdToMsgId(&callPtr->iSessionId, &iMsgId);
            msgOut = JADA_MsgNew(__FILE__, __LINE__, JADA_MSGTYPE_AUDIOFRAME, iMsgId, iLength);
            CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "errore nella creazione dell'audio frame message");
            iRet = JADA_MsgSetPayload(msgOut, szFrameData, iLength);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione dell'audio frame message");
#ifdef TEST_AUDIO
            iRet = JADA_HandleAudioFrame(callPtr, msgOut);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione dell'audio frame message");
            return(0);
#endif
            iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_CALL);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "Errore nell'invio dell'audio frame message");
        }
    }

    return(0);
}

DllExport int __stdcall JADA_CallSendVideoFrame(JADA_Call callSelf, void *szFrameData, int iLength)
{
    int             iRet, iCount;
    JADA_CallSelf   *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut;
    JADA_MsgId      iMsgId;
    // JADA_Frame      frmSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    if (callPtr->iVideoMuteStatus == FALSE) {
        if (connPtr->sokConn.iSocket > 0) {
            JADA_SessIdToMsgId(&callPtr->iSessionId, &iMsgId);
            msgOut = JADA_MsgNew(__FILE__, __LINE__, JADA_MSGTYPE_VIDEOFRAME, iMsgId, iLength);
            CORE_ReturnValIfFail(msgOut != NULL, -1, ;, "errore nella creazione del video frame message");
            iRet = JADA_MsgSetPayload(msgOut, szFrameData, iLength);
            CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "errore nella valorizzazione del video frame message");
            iRet = JADA_LstAdd(callPtr->lstOutVideoFrames, &msgOut, sizeof(msgOut), NULL);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in coda del frame video (OUT)");

            msgOut = NULL;
            iCount = JADA_LstGetCount(callPtr->lstOutVideoFrames) - callPtr->iVideoFrameDelay;
            while (iCount-- > 0) {
                iRet = JADA_LstWait(callPtr->lstOutVideoFrames, &msgOut, sizeof(msgOut), 0);
                CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella lettura dalla coda del frame video (OUT)");
            }
            if (msgOut != NULL) {
                // Se ho troppi messaggi call, salto il video per privilegiare l'audio
                iCount = JADA_LstGetCount(connPtr->sokConn.lstOutCallMsgs);
                if (iCount < 10) {
                    iRet = JADA_TcpEnqueueMessage(&connPtr->sokConn, msgOut, JADA_MSG_QUEUE_CALL);
                    CORE_ReturnValIfFail(iRet == 0, -1, JADA_MsgDestroy(__FILE__, __LINE__, msgOut);, "Errore nell'invio del video frame message");
            
                    /* Genera l'evento locale, per dare localmente il corretto feedback sul ritardo
                    frmSelf = JADA_FrmNew(__FILE__, __LINE__, JADA_FRAMETYPE_VIDEO);
                    CORE_ReturnValIfFail(frmSelf != NULL, -1, ;, "errore nella creazione del frame video");
                    iRet = JADA_FrmSetData(frmSelf, szFrameData, iLength);
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella scrittura dei dati di frame");
                    iRet = JADA_LstAdd(callPtr->lstInVideoFrames, &frmSelf, sizeof(frmSelf), NULL);
                    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del frame video"); */
                }
            }
        }
    }

    return(0);
}

DllExport int __stdcall JADA_CallEnqueueAudioFrame(JADA_Call callSelf, JADA_Frame *frmSelf)
{
    int             iRet;
    JADA_CallSelf   *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    iRet = JADA_LstAdd(callPtr->lstInAudioFrames, frmSelf, sizeof(*frmSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del frame audio");

    return(0);
}

DllExport int __stdcall JADA_CallEnqueueVideoFrame(JADA_Call callSelf, JADA_Frame *frmSelf)
{
    int             iRet;
    JADA_CallSelf   *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    iRet = JADA_LstAdd(callPtr->lstInVideoFrames, frmSelf, sizeof(*frmSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'inserimento in lista del frame audio");

    return(0);
}

DllExport int __stdcall JADA_CallSetVideoFrameDelay(JADA_Call callSelf, int iDelay)
{
    int             iRet;
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    callPtr->iVideoFrameDelay = iDelay;
    iRet = JADA_IniSetInt("call", "delay", callPtr->iVideoFrameDelay);
    CORE_LogIfFail(iRet == 0, "Errore nella scrittura del parametro call/delay nel file .ini");

    return(0);
}

DllExport int __stdcall JADA_CallGetVideoFrameDelay(JADA_Call callSelf, int *piDelay)
{
    JADA_CallSelf    *callPtr = (JADA_CallSelf *) callSelf;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  

    *piDelay = callPtr->iVideoFrameDelay;

    return(0);
}

int JADA_SendCallMessage(JADA_Call callSelf, int iOpcode, void *pPayload, int iSize)
{
    int             iRet;
    JADA_CallSelf   *callPtr = (JADA_CallSelf *) callSelf;
    JADA_ConnSelf   *connPtr;
    JADA_Message    msgOut;

    JADA_ReturnValIfInvalidSupport(callPtr, -1);  
    connPtr = (JADA_ConnSelf *) callPtr->connSelf;
    JADA_ReturnValIfInvalidConnection(connPtr, -1);  

    /* Crea il messaggio in output */
    msgOut = JADA_MsgSesNew(__FILE__, __LINE__, JADA_GetWhoAmI(), callPtr->iSessionId, iOpcode, iSize);
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

