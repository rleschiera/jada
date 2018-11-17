#include "jada.h"
#include "jada_internal.h"
#include "jada_events.h"


static JADA_CodeValue lstTypes[] = {
    JADA_EVT_CONNECTION_CONNECTED,       "JADA_EVT_CONNECTION_CONNECTED",      
    JADA_EVT_CONNECTION_OPENED,          "JADA_EVT_CONNECTION_OPENED",      
    JADA_EVT_CONNECTION_CLOSED,          "JADA_EVT_CONNECTION_CLOSED",      
//    JADA_EVT_SESSION_LOADED,             "JADA_EVT_SESSION_LOADED",         
//    JADA_EVT_SESSION_CLOSED,             "JADA_EVT_SESSION_CLOSED",         
    JADA_EVT_MSG_RESULT,                 "JADA_EVT_MSG_RESULT",             
    JADA_EVT_SESSIONS_LIST,              "JADA_EVT_SESSIONS_LIST",          
    JADA_EVT_REQUESTS_LIST,              "JADA_EVT_REQUESTS_LIST",          
    JADA_EVT_LIST_COUNTERS,              "JADA_EVT_LIST_COUNTERS",          
    JADA_EVT_PARAMETERS_LIST,            "JADA_EVT_PARAMETERS_LIST",   
    JADA_EVT_COMBO_VALUES,               "JADA_EVT_COMBO_VALUES",
    JADA_EVT_DIRECTORY_LIST,             "JADA_EVT_DIRECTORY_LIST",         
    JADA_EVT_SUPPORT_OPENED,             "JADA_EVT_SUPPORT_OPENED",         
    JADA_EVT_SUPPORT_ACCEPTED,           "JADA_EVT_SUPPORT_ACCEPTED",       
    JADA_EVT_SUPPORT_LOADED,             "JADA_EVT_SUPPORT_LOADED",         
    JADA_EVT_SUPPORT_CONNECTED,          "JADA_EVT_SUPPORT_CONNECTED",      
    JADA_EVT_SUPPORT_SOLICITED,          "JADA_EVT_SUPPORT_SOLICITED",      
    JADA_EVT_SUPPORT_CLOSED,             "JADA_EVT_SUPPORT_CLOSED",         
    JADA_EVT_SUPPORT_SUSPENDED,          "JADA_EVT_SUPPORT_SUSPENDED",      
    JADA_EVT_SUPPORT_RESUMED,            "JADA_EVT_SUPPORT_RESUMED",        
    JADA_EVT_CHATTEXT_RECEIVED,          "JADA_EVT_CHATTEXT_RECEIVED",      
    JADA_EVT_CALL_OPENED,                "JADA_EVT_CALL_OPENED",            
    JADA_EVT_CALL_CLOSED,                "JADA_EVT_CALL_CLOSED",            
    JADA_EVT_CALL_ABORTED,               "JADA_EVT_CALL_ABORTED",           
    JADA_EVT_DESKTOP_OPENED,             "JADA_EVT_DESKTOP_OPENED",         
    JADA_EVT_DESKTOP_REQUESTED,          "JADA_EVT_DESKTOP_REQUESTED",      
    JADA_EVT_DESKTOP_CLOSED,             "JADA_EVT_DESKTOP_CLOSED",         
    JADA_EVT_WHITEBOARD_OPENED,          "JADA_EVT_WHITEBOARD_OPENED",      
    JADA_EVT_WHITEBOARD_CLEANED,         "JADA_EVT_WHITEBOARD_CLEANED",     
    JADA_EVT_WHITEBOARD_CLOSED,          "JADA_EVT_WHITEBOARD_CLOSED",      
    JADA_EVT_SET_DRAW_POINTER,           "JADA_EVT_SET_DRAW_POINTER",       
    JADA_EVT_RESET_DRAW_POINTER,         "JADA_EVT_RESET_DRAW_POINTER",     
    JADA_EVT_FILE_SENT,                  "JADA_EVT_FILE_SENT",              
    JADA_EVT_FILE_RECEIVED,              "JADA_EVT_FILE_RECEIVED",          
    JADA_EVT_PING_REPLY,                 "JADA_EVT_PING_REPLY",             
    -1,                                  NULL
};

DllExport JADA_Event __stdcall JADA_EvtNew(char *szFile, int iLine, int iType)
{
    JADA_EvtSelf    *evtPtr;

    evtPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_EvtSelf));
    CORE_ReturnValIfFail(evtPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    evtPtr->iType = iType;
    evtPtr->iMagic = JADA_MAGIC_EVENT;

    return((JADA_Event) evtPtr);
}

DllExport JADA_Event __stdcall JADA_EvtDuplicate(char *szFile, int iLine, JADA_Event evtSelf)
{
    JADA_EvtSelf    *evtNewPtr;
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;

    evtNewPtr = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_EvtSelf));
    CORE_ReturnValIfFail(evtNewPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di sessione");

    memcpy(evtNewPtr, evtPtr, sizeof(*evtPtr));

    return((JADA_Event) evtNewPtr);
}

DllExport int __stdcall JADA_EvtDestroy(char *szFile, int iLine, JADA_Event evtSelf)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  

    if (evtPtr->szPayload != NULL) {
        JADA_Free(szFile, iLine, evtPtr->szPayload, sizeof(evtPtr->iLength));
    }
    JADA_Free(szFile, iLine, evtPtr, sizeof(*evtPtr));

    return(0);
}

DllExport int __stdcall JADA_EvtGetType(JADA_Event evtSelf, int *piType)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  

    *piType = evtPtr->iType;

    return(0);
}

DllExport int __stdcall JADA_EvtSetData(JADA_Event evtSelf, JADA_EventData *szPayload)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    int             iLength = sizeof(JADA_EventData);

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  

    if (evtPtr->szPayload != NULL) {
        JADA_Free(__FILE__, __LINE__, evtPtr->szPayload, sizeof(evtPtr->iLength));
    }
    evtPtr->iLength = iLength;
    evtPtr->szPayload = JADA_Calloc(__FILE__, __LINE__, 1, evtPtr->iLength);
    CORE_ReturnValIfFail(evtPtr->szPayload != NULL, -1, ;, "Errore nella creazione del payload dell'evento");

    memcpy(evtPtr->szPayload, szPayload, evtPtr->iLength);

    return(0);
}

DllExport int __stdcall JADA_EvtGetData(JADA_Event evtSelf, JADA_EventData *szPayload)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    int             iLength = sizeof(JADA_EventData);

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    CORE_ReturnValIfFail(evtPtr->iLength <= iLength, -1, ;, "Buffer troppo piccolo per estrarre il payload dell'evento");

    memcpy(szPayload, evtPtr->szPayload, iLength);

    return(0);
}

DllExport int __stdcall JADA_EvtGetResult(JADA_Event evtSelf, int *piResult)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piResult = ptrData->iResult;

    return(0);
}

DllExport int __stdcall JADA_EvtGetSessionId(JADA_Event evtSelf, JADA_SessId *piSessionId)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piSessionId = ptrData->iSessionId;

    return(0);
}

DllExport int __stdcall JADA_EvtGetRequestId(JADA_Event evtSelf, JADA_ReqId *piRequestId)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piRequestId = ptrData->iRequestId;

    return(0);
}

DllExport int __stdcall JADA_EvtGetStreamId(JADA_Event evtSelf, int *piStreamId)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piStreamId = ptrData->iStreamId;

    return(0);
}

DllExport int __stdcall JADA_EvtGetAreaRequests(JADA_Event evtSelf, int *piRequests)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piRequests = ptrData->iAreaRequests;

    return(0);
}

DllExport int __stdcall JADA_EvtGetAreaPendingRequests(JADA_Event evtSelf, int *piPendingRequests)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piPendingRequests = ptrData->iAreaPendingRequests;

    return(0);
}

DllExport int __stdcall JADA_EvtGetTotalRequests(JADA_Event evtSelf, int *piRequests)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piRequests = ptrData->iTotalRequests;

    return(0);
}

DllExport int __stdcall JADA_EvtGetTotalPendingRequests(JADA_Event evtSelf, int *piPendingRequests)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piPendingRequests = ptrData->iTotalPendingRequests;

    return(0);
}

DllExport int __stdcall JADA_EvtGetWhoAmI(JADA_Event evtSelf, int *piWhoAmI)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piWhoAmI = ptrData->iWhoAmI;

    return(0);
}

DllExport int __stdcall JADA_EvtGetUsaAudio(JADA_Event evtSelf, int *piUsaAudio)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piUsaAudio = ptrData->iUsaAudio;

    return(0);
}

DllExport int __stdcall JADA_EvtGetItemType(JADA_Event evtSelf, int *piItemType)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piItemType = ptrData->iItemType;

    return(0);
}

DllExport int __stdcall JADA_EvtGetPointX(JADA_Event evtSelf, int *piPointX)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piPointX = ptrData->iPointX;

    return(0);
}

DllExport int __stdcall JADA_EvtGetPointY(JADA_Event evtSelf, int *piPointY)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piPointY = ptrData->iPointY;

    return(0);
}

DllExport int __stdcall JADA_EvtGetCurrFrame(JADA_Event evtSelf, int *piCurrFrame)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piCurrFrame = ptrData->iCurrFrame;

    return(0);
}

DllExport int __stdcall JADA_EvtGetFrames(JADA_Event evtSelf, int *piFrames)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    *piFrames = ptrData->iFrames;

    return(0);
}

DllExport int __stdcall JADA_EvtGetUtente(JADA_Event evtSelf, char *szUtente, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    strcpy(szUtente, ptrData->szUtente);

    return(0);
}

DllExport int __stdcall JADA_EvtGetPassword(JADA_Event evtSelf, char *szPassword, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    strcpy(szPassword, ptrData->szPassword);

    return(0);
}

DllExport int __stdcall JADA_EvtGetArea(JADA_Event evtSelf, char *szArea, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    strcpy(szArea, ptrData->szArea);

    return(0);
}

DllExport int __stdcall JADA_EvtGetDescription(JADA_Event evtSelf, char *szDescription, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    strcpy(szDescription, ptrData->szDescription);

    return(0);
}

DllExport int __stdcall JADA_EvtGetFileName(JADA_Event evtSelf, char *szFileName, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    strcpy(szFileName, ptrData->szFileName);

    return(0);
}

DllExport int __stdcall JADA_EvtGetChatText(JADA_Event evtSelf, char *szChatText, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    strcpy(szChatText, ptrData->szChatText);

    return(0);
}

DllExport int __stdcall JADA_EvtGetTime(JADA_Event evtSelf, char *szTime, int iLength)
{
    JADA_EvtSelf    *evtPtr = (JADA_EvtSelf *) evtSelf;
    JADA_EventData *ptrData = (JADA_EventData *) evtPtr->szPayload;

    JADA_ReturnValIfInvalidEvent(evtPtr, -1);  
    JADA_TimeToString(ptrData->timTime, szTime, iLength);

    return(0);
}

DllExport char * __stdcall JADA_EvtTypeToString(int iType)
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

