#ifndef _SERVER_H_
#define _SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*                                                                            */
/*   G e n e r a l     I n c l u d e     F i l e s                            */
/*                                                                            */
/******************************************************************************/
#include "jada_messages.h"

/******************************************************************************/
/*                                                                            */
/*   M a c r o s     D e f i n i t i o n                                      */
/*                                                                            */
/******************************************************************************/
#define ASRV_IsValidConnection(self)                        \
        (self != NULL) && (self->iMagic == ASRV_MAGIC_CONNECTION) 
#define ASRV_IsValidSession(self)                           \
        (self != NULL) && (self->iMagic == ASRV_MAGIC_SESSION) 
#define ASRV_IsValidRequest(self)                           \
        (self != NULL) && (self->iMagic == ASRV_MAGIC_REQUEST) 
#define ASRV_IsValidPendingRequest(self)                    \
        (self != NULL) && (self->iMagic == ASRV_MAGIC_PENDING_REQUEST) 

/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
/* Magic numbers */
#define ASRV_MAGIC_CONNECTION       12340
#define ASRV_MAGIC_SESSION          12341
#define ASRV_MAGIC_REQUEST          12342
#define ASRV_MAGIC_PENDING_REQUEST  12343

/* Errori dalle primitive base */ 
#define JADA_ERR_TCP_CREATE         -1000
#define JADA_ERR_TCP_SELECT         -1001
#define JADA_ERR_TCP_ACCEPT         -1002
#define JADA_ERR_TCP_RECV           -1003
#define JADA_ERR_TCP_SEND           -1004
#define JADA_ERR_WRONG_FRAMETYPE    -1005
#define JADA_ERR_WRONG_FRAMELENGTH  -1006

#define STATUS_UNKNOWN              0
#define STATUS_AVAILABLE            1
#define STATUS_REQUESTED            2
#define STATUS_CONNECTED            3
#define STATUS_LOADED               4

#define OPCODE_UNKNOWN              0
#define OPCODE_REQUEST_SESSION      1
#define OPCODE_ACCEPT_SESSION       2
#define OPCODE_SEND_CHAT            3

/******************************************************************************/
/*                                                                            */
/*   T y p e s     D e f i n i t i o n                                        */
/*                                                                            */
/******************************************************************************/
#ifdef UNIX
#include <dirent.h>
typedef struct {
    DIR             *dirPtr;
} CORE_Directory;
#else
typedef struct {
    void            *dirPtr;
} CORE_Directory;
#endif

typedef struct {
    Identifier      szSection;
    Identifier      szName;
    FixString       szValue;
} ASRV_Parameter;

typedef struct {
    int             iSocket;
    int             iCounter;
    unsigned char   *szInBuffer;
    int             iInLength;
    unsigned char   *szOutBuffer;
    int             iOutLength;
} ASRV_Socket;

typedef struct {
    JADA_Id         iId;
    Identifier      szIP;
    ASRV_Socket     sokConn;
    JADA_Timet      timLastActivity;
    int             iMagic;
} ASRV_Connection;

typedef struct {
    JADA_Id         iId;
    int             iStatus;
    JADA_Timet      timLastActivity;
    Identifier      szIP;
    ASRV_Socket     sokSess;
    pthread_t       pThreadId;
    int             iWhoAmI;
    CORE_List       lstParameters;
    int             iMagic;
} ASRV_Session;

typedef struct {
    JADA_Id         iId;
    int             iStatus;
    int             iPassword;
    JADA_Id         iAgentSession;
    JADA_Id         iManagerSession;
    CORE_List       lstParameters;
    CORE_List       lstChatMsgs;
    int             iMagic;
} ASRV_Request;

typedef struct {
    JADA_Id         iId;
    int             iStatus;
    CORE_List       lstParameters;
    int             iMagic;
} ASRV_PendingRequest;

/******************************************************************************/
/*                                                                            */
/*   V a r i a b l e s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
extern CORE_List    lstConnections;
extern CORE_List    lstSessions;
extern CORE_List    lstRequests;
extern FixString    JADA_TmpFile;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/
#define in
#define out

/* database */
int ASRV_DbStart(void);
int ASRV_DbStop(void);
ASRV_PendingRequest *ASRV_DbLoadPendingRequest(char *szFile, int iLine, JADA_Id iRequestId);
int ASRV_DbSavePendingRequest(ASRV_PendingRequest *reqPtr);
int ASRV_DbDeletePendingRequest(JADA_Id iRequestId);
int ASRV_DbExistsPendingRequest(JADA_Id iRequestId);
ASRV_PendingRequest *ASRV_DbGetFirstPendingRequest(char *szFile, int iLine, CORE_Directory *dirSelf);
ASRV_PendingRequest *ASRV_DbGetNextPendingRequest(char *szFile, int iLine, CORE_Directory *dirSelf);
int ASRV_DbGetPendingRequestCount(char *szArea, int *piAreaCount, int *piTotalCount);
int ASRV_DbGetTotalPendingRequestCount(int *piTotalCount);

/* callback.c */
CORE_Callback ASRV_MnuListSessions(void);
CORE_Callback ASRV_MnuListRequests(void);
CORE_Callback ASRV_MnuListPendingRequests(void);
CORE_Callback ASRV_MnuListAgents(void);
CORE_Callback ASRV_MnuListManagers(void);
CORE_Callback ASRV_MnuShowRequest(void);

/*- session.c */
int ASRV_HandleConnectionMessage(ASRV_Connection *conPtr, JADA_Message msgIn, int *piCloseMe);
int ASRV_HandleSessionMessage(ASRV_Session *sesPtr, JADA_Message msgIn);

/* objects.c */
int ASRV_InitIds(void);
int ASRV_GetPassword(int iIndex, char *szPassword);
int ASRV_GetVncPassword(int iIndex, char *szPassword);
int ASRV_GetParameter(CORE_List lstParameters, char *szSection, char *szName, char *szValue, int iLength);
int ASRV_GetParameterInt(CORE_List lstParameters, char *szSection, char *szName, int *piValue);
int ASRV_SetParameter(CORE_List lstParameters, char *szSection, char *szName, char *szValue);
int ASRV_SetParameterInt(CORE_List lstParameters, char *szSection, char *szName, int iValue);
ASRV_Connection *ASRV_CreateConnection(char *szFile, int iLine, char *szIP, int iSocket);
int ASRV_DestroyConnection(char *szFile, int iLine, ASRV_Connection *conPtr, CORE_Currency *curS);
ASRV_Session *ASRV_CreateSession(char *szFile, int iLine, ASRV_Connection *conPtr);
int ASRV_DestroySession(char *szFile, int iLine, ASRV_Session *sesPtr, CORE_Currency *curS);
ASRV_Session *ASRV_GetSessionById(char *szFile, int iLine, JADA_Id iSessionId);
ASRV_Request *ASRV_CreateRequest(char *szFile, int iLine, ASRV_Session *sesPtr);
ASRV_Request *ASRV_CreateRequestFromPending(char *szFile, int iLine, ASRV_Session *sesPtr, ASRV_PendingRequest *penPtr);
int ASRV_DestroyRequest(char *szFile, int iLine, ASRV_Request *reqPtr, CORE_Currency *curR);
int ASRV_MoveRequestToPending(char *szFile, int iLine, ASRV_Request *reqPtr);
ASRV_Request *ASRV_GetRequestById(char *szFile, int iLine, JADA_Id iRequestId);
ASRV_Request *ASRV_GetRequestByPendingId(char *szFile, int iLine, JADA_Id iRequestId);
ASRV_PendingRequest *ASRV_CreatePendingRequest(char *szFile, int iLine);
int ASRV_DestroyPendingRequest(char *szFile, int iLine, ASRV_PendingRequest *reqPtr, int iFromDb);
ASRV_PendingRequest *ASRV_GetPendingRequestById(char *szFile, int iLine, JADA_Id iRequestId);

/* tcp.c */
int ASRV_TcpCreateListenPort(char *szHostName, int iPort);
int ASRV_TcpCreateSocket(ASRV_Socket *sokSelf, int iSocket);
int ASRV_TcpDestroySocket(ASRV_Socket *sokSelf);
int ASRV_WaitConnections(void);
int ASRV_TcpSendMessage(ASRV_Socket *sokSelf, JADA_Message msgOut);
JADA_Message ASRV_TcpReceiveMessage(ASRV_Socket *sokSelf);
int ASRV_TcpSendRequestMessage(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iSize);
int ASRV_TcpSendSessionMessage(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iSize);
int ASRV_TcpSendConnectionMessage(ASRV_Connection *conPtr, int iOpcode, void *pPayload, int iLength);
int __stdcall ASRV_TcpSendRequestMessageToAgent(ASRV_Request *reqPtr, int iOpcode, void *pPayload, int iLength);
int __stdcall ASRV_TcpSendRequestMessageToManager(ASRV_Request *reqPtr, int iOpcode, void *pPayload, int iLength);
int ASRV_TcpForwardMessage(ASRV_Session *sesPtr, JADA_Message msgOut);

/* threads.c */
void *ASRV_ConnectionThread(void *FuncArg);
void *ASRV_SessionThread(void *FuncArg);

/* handlers.c */
int ASRV_HandleOpenSession(ASRV_Connection *conPtr, JADA_Message msgRecv, int *piCloseMe);
int ASRV_HandleSetSystemInfo(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleGetSystemInfo(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleOpenRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleSolicitRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleCloseRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleAcceptRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleLoadRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleResumeRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleConnectRequest(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleListRequests(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleListPendingRequests(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleListParameters(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleParametersList(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleListDirectory(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleDirectoryList(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleGetListCounters(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleBroadcastListCounters(void);
int ASRV_HandleSendChatText(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleOpenCall(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleCallOpened(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleCloseCall(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleAbortCall(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleOpenDesktop(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleDesktopOpened(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleCloseDesktop(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleOpenWhiteboard(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleWhiteboardOpened(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleCleanWhiteboard(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleCloseWhiteboard(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleWhiteboardClosed(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleSetDrawPointer(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleResetDrawPointer(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleGetFile(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandlePing(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandlePingReply(ASRV_Session *sesPtr, JADA_Message msgRecv);

/* log */
void ASRV_LogOpenSession(ASRV_Session *sesPtr);
void ASRV_LogOpenSessionError(JADA_Id iId, char *szUtente, char *szErrorMsg);
void ASRV_LogSetSystemInfo(ASRV_Session *sesPtr);
void ASRV_LogGetSystemInfo(ASRV_Session *sesPtr);
void ASRV_LogOpenRequest(ASRV_Session *sesPtr, ASRV_Request *reqPtr);
void ASRV_LogSolicitRequest(ASRV_Session *sesPtr, JADA_Id iId);
void ASRV_LogCloseRequest(ASRV_Session *sesPtr, JADA_Id iId);
void ASRV_LogAcceptRequest(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogAcceptRequestError(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr, char *szErrorMsg);
void ASRV_LogLoadRequest(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogLoadRequestError(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr, char *szErrorMsg);
void ASRV_LogResumeRequest(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogResumeRequestError(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr, char *szErrorMsg);
void ASRV_LogConnectRequest(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogConnectRequestError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogOpenCall(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogOpenCallError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogCallOpened(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogCallOpenedError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogCloseCall(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogCloseCallError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogAbortCall(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogAbortCallError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogOpenWhiteboard(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogOpenWhiteboardError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogWhiteboardOpened(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogWhiteboardOpenedError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogCleanWhiteboard(ASRV_Session *sesPtr, JADA_Id iId);
void ASRV_LogCleanWhiteboardError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogCloseWhiteboard(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogCloseWhiteboardError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogWhiteboardClosed(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogWhiteboardClosedError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogOpenDesktop(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogDesktopOpened(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogCloseDesktop(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogSetDrawPointer(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogSetDrawPointerError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);
void ASRV_LogResetDrawPointer(ASRV_Session *sesPtr, JADA_Id iId, ASRV_Request *reqPtr);
void ASRV_LogResetDrawPointerError(ASRV_Session *sesPtr, JADA_Id iId, char *szErrorMsg);

#undef in
#undef out

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_SERVER_H_ */
