#ifndef _JADA_EVENTS_H_
#define _JADA_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/*                                                                            */
/*   M a c r o s     D e f i n i t i o n                                      */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
/* Opcode degli eventi: connessione */
#define JADA_EVT_CONNECTION_CONNECTED   3000
#define JADA_EVT_CONNECTION_OPENED      3001
#define JADA_EVT_CONNECTION_CLOSED      3002
//#define JADA_EVT_SESSION_OPENED         3003
//#define JADA_EVT_SESSION_CONNECTED      3004
//#define JADA_EVT_SESSION_LOADED         3005
//#define JADA_EVT_SESSION_CLOSED         3006
#define JADA_EVT_SESSIONS_LIST          3007
#define JADA_EVT_REQUESTS_LIST          3008
#define JADA_EVT_LIST_COUNTERS          3009
#define JADA_EVT_PARAMETERS_LIST        3010
#define JADA_EVT_COMBO_VALUES           3011
#define JADA_EVT_MSG_RESULT             3101
#define JADA_EVT_PING_REPLY             3102
/* Opcode degli eventi: supporto */
#define JADA_EVT_SUPPORT_OPENED         3201
#define JADA_EVT_SUPPORT_ACCEPTED       3202
#define JADA_EVT_SUPPORT_LOADED         3203
#define JADA_EVT_SUPPORT_CONNECTED      3204
#define JADA_EVT_SUPPORT_SOLICITED      3205
#define JADA_EVT_SUPPORT_CLOSED         3206
#define JADA_EVT_SUPPORT_SUSPENDED      3207
#define JADA_EVT_SUPPORT_RESUMED        3208
#define JADA_EVT_CHATTEXT_RECEIVED      3209
/* Opcode degli eventi: media */
#define JADA_EVT_CALL_OPENED            3301
#define JADA_EVT_CALL_CLOSED            3302
#define JADA_EVT_CALL_ABORTED           3303
/* Opcode degli eventi: VNC */
#define JADA_EVT_DESKTOP_OPENED         3401
#define JADA_EVT_DESKTOP_REQUESTED      3402
#define JADA_EVT_DESKTOP_CLOSED         3403
#define JADA_EVT_WHITEBOARD_OPENED      3404
#define JADA_EVT_WHITEBOARD_CLEANED     3405
#define JADA_EVT_WHITEBOARD_CLOSED      3406
#define JADA_EVT_SET_DRAW_POINTER       3407
#define JADA_EVT_RESET_DRAW_POINTER     3408
/* Opcode degli eventi: FTP */
#define JADA_EVT_FTP_OPENED             3501
#define JADA_EVT_DIRECTORY_LIST         3502
#define JADA_EVT_FILE_SENT              3503
#define JADA_EVT_FILE_RECEIVED          3504
#define JADA_EVT_FTP_CLOSED             3505

#define JADA_LISTTYPE_REQUESTS          1001
#define JADA_LISTTYPE_PENDING_REQUESTS  1002


/******************************************************************************
 *
 *   E V E N T S     D E F I N I T I O N
 *
 ******************************************************************************/
typedef struct {
    int             iResult;
    JADA_ConnId     iConnectionId;
    JADA_SessId     iSessionId;
    JADA_ReqId      iRequestId;
    int             iStreamId;
    int             iWhoAmI;
    int             iItemType;
    int             iPointX;
    int             iPointY;
    Identifier      szUtente;
    Identifier      szPassword;
    int             iUsaAudio;
    Identifier      szArea;
    LongString      szDescription;
    FixString       szFileName;
    LongString      szChatText;
    int             iAreaRequests;
    int             iTotalRequests;
    int             iAreaPendingRequests;
    int             iTotalPendingRequests;
    int             iCurrFrame;
    int             iFrames;
    JADA_Timet      timTime;
} JADA_EventData;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/
#ifdef ANSI_C

#define in
#define out

/* Events */
DllExport JADA_Event __stdcall JADA_EvtNew(char *szFile, int iLine, int iType);
DllExport JADA_Event __stdcall JADA_EvtDuplicate(char *szFile, int iLine, JADA_Event evtSelf);
DllExport int __stdcall JADA_EvtDestroy(char *szFile, int iLine, JADA_Event reqSelf);
DllExport int __stdcall JADA_EvtGetType(JADA_Event reqSelf, int *piType);
DllExport int __stdcall JADA_EvtSetData(JADA_Event reqSelf, JADA_EventData *szPayload);
DllExport int __stdcall JADA_EvtGetData(JADA_Event reqSelf, JADA_EventData *szPayload);
DllExport int __stdcall JADA_EvtGetResult(JADA_Event evtSelf, int *piResult);
DllExport int __stdcall JADA_EvtGetConnectionId(JADA_Event evtSelf, JADA_ConnId *piConnectionId);
DllExport int __stdcall JADA_EvtGetSessionId(JADA_Event evtSelf, JADA_SessId *piSessionId);
DllExport int __stdcall JADA_EvtGetRequestId(JADA_Event evtSelf, JADA_ReqId *piRequestId);
DllExport int __stdcall JADA_EvtGetStreamId(JADA_Event evtSelf, int *piStreamId);
DllExport int __stdcall JADA_EvtGetAreaRequests(JADA_Event evtSelf, int *piRequests);
DllExport int __stdcall JADA_EvtGetTotalRequests(JADA_Event evtSelf, int *piRequests);
DllExport int __stdcall JADA_EvtGetAreaPendingRequests(JADA_Event evtSelf, int *piPendingRequests);
DllExport int __stdcall JADA_EvtGetTotalPendingRequests(JADA_Event evtSelf, int *piPendingRequests);
DllExport int __stdcall JADA_EvtGetWhoAmI(JADA_Event evtSelf, int *piWhoAmI);
DllExport int __stdcall JADA_EvtGetItemType(JADA_Event evtSelf, int *piItemType);
DllExport int __stdcall JADA_EvtGetPointX(JADA_Event evtSelf, int *piPointX);
DllExport int __stdcall JADA_EvtGetPointY(JADA_Event evtSelf, int *piPointY);
DllExport int __stdcall JADA_EvtGetCurrFrame(JADA_Event evtSelf, int *piCurrFrame);
DllExport int __stdcall JADA_EvtGetFrames(JADA_Event evtSelf, int *piFrames);
DllExport int __stdcall JADA_EvtGetUtente(JADA_Event evtSelf, char *szUtente, int iLength);
DllExport int __stdcall JADA_EvtGetPassword(JADA_Event evtSelf, char *szPassword, int iLength);
DllExport int __stdcall JADA_EvtGetArea(JADA_Event evtSelf, char *szArea, int iLength);
DllExport int __stdcall JADA_EvtGetDescription(JADA_Event evtSelf, char *szDescription, int iLength);
DllExport int __stdcall JADA_EvtGetUsaAudio(JADA_Event evtSelf, int *piUsaAudio);
DllExport int __stdcall JADA_EvtGetFileName(JADA_Event evtSelf, char *szFileName, int iLength);
DllExport int __stdcall JADA_EvtGetChatText(JADA_Event evtSelf, char *szChatText, int iLength);
DllExport int __stdcall JADA_EvtGetTime(JADA_Event evtSelf, char *szTime, int iLength);
DllExport char * __stdcall JADA_EvtTypeToString(int iOpcode);

int JADA_RaiseEventConnectionClosed(JADA_ConnSelf *connPtr, JADA_ConnId iId, JADA_Timet timTime);
// int JADA_RaiseEventSupportClosed(JADA_ConnSelf *connPtr, JADA_SessId iId, JADA_Timet timTime);

#undef in
#undef out

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_JADA_EVENTS_H_ */
