#ifndef _JADA_INTERNAL_H_
#define _JADA_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*                                                                            */
/*   G e n e r a l     I n c l u d e     F i l e s                            */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <sys/timeb.h>
#include <time.h>
#ifndef UNIX
#include <windows.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#endif

/******************************************************************************/
/*                                                                            */
/*   M a c r o s     D e f i n i t i o n                                      */
/*                                                                            */
/******************************************************************************/
#define JADA_ReturnValIfInvalidConnection(self, val)                                \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: connection self e' NULL\n", __FILE__, __LINE__);\
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_CONNECTION) {                              \
            JADA_LogMessage("%s, %d: il self non e' CONNECTION\n", __FILE__, __LINE__);\
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidSupport(self, val)                                   \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: support self e' NULL\n", __FILE__, __LINE__);  \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_SUPPORT) {                                 \
            JADA_LogMessage("%s, %d: il self non e' SUPPORT\n", __FILE__, __LINE__);\
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidCall(self, val)                                       \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: Call self e' NULL\n", __FILE__, __LINE__);      \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_CALL) {                                     \
            JADA_LogMessage("%s, %d: il self non e' CALL\n", __FILE__, __LINE__);    \
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidVNC(self, val)                                   \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: VNC self e' NULL\n", __FILE__, __LINE__);  \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_VNC) {                                 \
            JADA_LogMessage("%s, %d: il self non e' VNC\n", __FILE__, __LINE__);\
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidFTP(self, val)                                       \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: FTP self e' NULL\n", __FILE__, __LINE__);      \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_FTP) {                                     \
            JADA_LogMessage("%s, %d: il self non e' FTP\n", __FILE__, __LINE__);    \
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidEvent(self, val)                                     \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: event self e' NULL\n", __FILE__, __LINE__);    \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_EVENT) {                                   \
            JADA_LogMessage("%s, %d: il self non e' EVENT\n", __FILE__, __LINE__);  \
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidFrame(self, val)                                     \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: frame self e' NULL\n", __FILE__, __LINE__);    \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_FRAME) {                                   \
            JADA_LogMessage("%s, %d: il self non e' FRAME\n", __FILE__, __LINE__);  \
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidDraw(self, val)                                      \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: draw self e' NULL\n", __FILE__, __LINE__);     \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_DRAW) {                                    \
            JADA_LogMessage("%s, %d: il self non e' DRAW\n", __FILE__, __LINE__);   \
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidCurve(self, val)                                     \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: curve self e' NULL\n", __FILE__, __LINE__);    \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_CURVE) {                                   \
            JADA_LogMessage("%s, %d: il self non e' CURVE\n", __FILE__, __LINE__);  \
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidMessage(self, val)                                   \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: MSG self e' NULL\n", __FILE__, __LINE__);      \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_MESSAGE) {                                 \
            JADA_LogMessage("%s, %d: il self non e' MESSAGE\n", __FILE__, __LINE__);\
            return(val);                                                            \
        } }

#define JADA_ReturnValIfInvalidStream(self, val)                                    \
        { if (self == NULL) {                                                       \
            JADA_LogMessage("%s, %d: stream self e' NULL\n", __FILE__, __LINE__);   \
            return(val);                                                            \
          }                                                                         \
          if (self->iMagic != JADA_MAGIC_STREAM) {                                  \
            JADA_LogMessage("%s, %d: il self non e' STREAM\n", __FILE__, __LINE__); \
            return(val);                                                            \
        } }



/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
/* Magic numbers */ 
#define JADA_MAGIC_CONNECTION           100000
#define JADA_MAGIC_SUPPORT              100002
#define JADA_MAGIC_CALL                 100003
#define JADA_MAGIC_VNC                  100004
#define JADA_MAGIC_FTP                  100005
#define JADA_MAGIC_EVENT                100006
#define JADA_MAGIC_FRAME                100007
#define JADA_MAGIC_DRAW                 100008
#define JADA_MAGIC_CURVE                100009
#define JADA_MAGIC_STREAM               100010
#define JADA_MAGIC_MESSAGE              100011
#define JADA_MAGIC_LIST                 100012

/* Errori dalle primitive base */ 
#define JADA_ERR_TCP_SELECT             -1001
#define JADA_ERR_TCP_ACCEPT             -1002
#define JADA_ERR_TCP_RECV               -1003
#define JADA_ERR_TCP_SEND               -1004
#define JADA_ERR_WRONG_FRAMETYPE        -1005
#define JADA_ERR_WRONG_FRAMELENGTH      -1006

#define JADA_CONNSTATUS_AVAILABLE       1
#define JADA_CONNSTATUS_CONNECTING      2
#define JADA_CONNSTATUS_CONNECTED       3
#define JADA_CONNSTATUS_OPENING         4
#define JADA_CONNSTATUS_OPENED          5
//#define JADA_CONNSTATUS_SESS_REQUESTED  6
//#define JADA_CONNSTATUS_SESS_CONNECTED  7

#define JADA_SUPPSTATUS_AVAILABLE       1
#define JADA_SUPPSTATUS_OPENING         2
#define JADA_SUPPSTATUS_OPENED          3
#define JADA_SUPPSTATUS_REQUESTING      4
#define JADA_SUPPSTATUS_REQUESTED       5
#define JADA_SUPPSTATUS_CONNECTING      6
#define JADA_SUPPSTATUS_CONNECTED       7
#define JADA_SUPPSTATUS_LOADING         8
#define JADA_SUPPSTATUS_LOADED          9
#define JADA_SUPPSTATUS_RESUMING        10
#define JADA_SUPPSTATUS_RESUMED         11
#define JADA_SUPPSTATUS_CLOSING         12
#define JADA_SUPPSTATUS_CLOSED          13
#define JADA_SUPPSTATUS_SUSPENDED       14

#define JADA_CALLSTATUS_AVAILABLE       1
#define JADA_CALLSTATUS_REQUESTED       2
#define JADA_CALLSTATUS_CONNECTED       3
#define JADA_CALLSTATUS_CLOSED          4

#define JADA_VNCSTATUS_AVAILABLE        1
#define JADA_VNCSTATUS_REQUESTED        2
#define JADA_VNCSTATUS_CONNECTED        3
#define JADA_VNCSTATUS_CLOSED           4

#define JADA_FTPSTATUS_AVAILABLE        1
#define JADA_FTPSTATUS_REQUESTED        2
#define JADA_FTPSTATUS_CONNECTED        3
#define JADA_FTPSTATUS_CLOSED           4

#define JADA_PROC_VNCSERVER             "jadavncserver.exe"
#define JADA_PROC_VNCVIEWER             "jadavncviewer.exe"
#define JADA_PROC_CALLAGENT             "jadacall.exe"
#define JADA_PROC_CALLMANAGER           "jadacall.exe"

/******************************************************************************
 *
 *   C l a s s     D e f i n i t i o n
 *
 ******************************************************************************/
typedef struct {
    Identifier      szSection;
    Identifier      szName;
    FixString       szValue;
    int             iIniFile;
    int             iIniWhoAmI;
    int             iUpdated;
} JADA_Parameter;

typedef struct {
    char            *szSection;
    char            *szName;
    int             iIniFile;
    int             iIniWhoAmI;
} JADA_ParameterEntry;

typedef struct {
    int             iSocket;
    int             iCounter;
    unsigned char   *szInBuffer;
    int             iInLength;
    unsigned char   *szOutBuffer;
    int             iOutLength;
    pthread_mutex_t socketMutex;
    pthread_mutex_t queueMutex;
    pthread_cond_t  queueCondVar;
    CORE_List       lstOutCallMsgs;
    CORE_List       lstOutVncMsgs;
    CORE_List       lstOutFtpMsgs;
    CORE_List       lstOutSupportMsgs;
} JADA_Socket;

typedef struct {
    JADA_ConnId     iId;
    int             iType;
    int             iTestMode;
    //ShortString     szServerType;
    ShortString     szServerName;
    int             iStatus;
    int             iClosingCall;
    int             iClosingVnc;
    int             iClosingFtp;
    JADA_Timet      timLastActivity;
    JADA_Support    suppSelf;
    JADA_Call       callSelf;
    JADA_VNC        vncSelf;
    JADA_FTP        ftpSelf;
    JADA_Socket     sokConn;
//    JADA_Socket     sokVNC;
    CORE_List       lstParameters;
    int             iReceiverThreadStatus;
    int             iSenderThreadStatus;
    CORE_List       lstEvents;
    CORE_List       lstEventSubscribers;
    CORE_List       lstSubscribedEvents;
    CORE_List       lstStreams;
    int             iMagic;
} JADA_ConnSelf;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    int             iStatus;
    JADA_Connection connSelf;
    JADA_Timet      timLastActivity;
    int             iMagic;
} JADA_SuppSelf;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    int             iStatus;
    int             iReset;
    JADA_Connection connSelf;
    JADA_Socket     sokCall;
    int             iAudioMuteStatus;
    int             iVideoMuteStatus;
    int             iVideoFrameDelay;
    CORE_List       lstInAudioFrames;
    CORE_List       lstInVideoFrames;
    CORE_List       lstOutVideoFrames;
    JADA_Timet      timLastActivity;
    int             iMagic;
} JADA_CallSelf;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    int             iStatus;
    JADA_Connection connSelf;
    JADA_Socket     sokServer;
    JADA_Socket     sokVNC;
    CORE_List       lstInDrawFrames;
    pthread_t       pThreadId;
    JADA_Timet      timLastActivity;
    int             iMagic;
} JADA_VncSelf;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    int             iStatus;
    JADA_Connection connSelf;
    CORE_List       lstTransfer;
    pthread_t       pThreadId;
    JADA_Timet      timLastActivity;
    int             iMagic;
} JADA_FtpSelf;

typedef struct {
    int             iType;
    int             iLength;
    void            *szPayload;
    int             iMagic;
} JADA_EvtSelf;

typedef struct {
    int             iType;
    int             iLength;
    void            *szPayload;
    int             iMagic;
} JADA_FrmSelf;

typedef struct {
    int             iAlpha;
    int             iRed;
    int             iGreen;
    int             iBlue;
    int             iWidth;
    CORE_List       lstCoordinates;
    int             iMagic;
} JADA_CrvSelf;

typedef struct {
    CORE_List       lstCurves;
    int             iMagic;
} JADA_DrwSelf;

typedef struct {
    JADA_SessId     iSessionId;
    FixString       szFileFrom;
    FixString       szFileTo;
    FILE            *fp;
    short           iCurrFrame;
    short           iFrames;
    int             iAbort;
} JADA_FileTransfer;

typedef struct {
    int             iId;
    CORE_List       lstLines;
    CORE_Currency   curL;
    int             iMagic;
} JADA_StmSelf;

typedef struct {
    int             iType;
} JADA_EventSubscriber;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/
#ifdef ANSI_C

#define in
#define out

/* Generic utilities */
int JADA_SetTimeSpec(struct timespec *Timespec, int iMsecsToWait);
int JADA_ChangeStringChar(char *szString, char chFrom, char chTo);
int JADA_MkTemp(char *szFileName, int iLength);
int JADA_FileWrite(char *szFullPath, void *szData, int iLength);
char *JADA_Base64Encode(const unsigned char *data, int input_length, int *output_length);
int JADA_GetTimeDelay(int iFromSecs, int iFromMsecs, int iToSecs, int iToMsecs);
int JADA_SessIdToMsgId(JADA_SessId *piSessId, JADA_MsgId *piMsgId);
int JADA_ReqIdToMsgId(JADA_ReqId *piReqId, JADA_MsgId *piMsgId);
int JADA_ConnIdToMsgId(JADA_ConnId *piConnId, JADA_MsgId *piMsgId);
int JADA_ConnIdToSessId(JADA_ConnId *piConnId, JADA_SessId *piSessId);
int JADA_StringToAgtId(char *szString, JADA_AgtId *piAgtId);
int JADA_StringToMgrId(char *szString, JADA_MgrId *piMgrId);
int JADA_StringToConnId(char *szString, JADA_ConnId *piConnId);
int JADA_StringToSessId(char *szString, JADA_SessId *piSessId);
int JADA_StringToReqId(char *szString, JADA_ReqId *piReqId);
int JADA_StringToQueueId(char *szString, JADA_QueueId *piQueueId);
int JADA_StringToTimet(char *szString, time_t *ptimTime);
void JADA_ClearJadaId(void *piId);
int JADA_CompareJadaId(void *piId1, void *piId2);
int JADA_IsValidJadaId(void *piId);

/* Message Handlers */
int JADA_SendParametersToServer(JADA_ConnSelf *connPtr);
int JADA_HandleConnectionConnected(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleConnectionOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
//int JADA_HandleSessionLoaded(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleConnectionParametersList(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleRequestsList(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleComboValues(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandlePendingRequestsList(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleParametersList(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleListCounters(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandlePing(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandlePingReply(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportAccepted(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportConnected(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportLoaded(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportSolicited(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportSuspended(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportResumed(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSupportClosed(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleResult(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleTextChatReceived(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleOpenCall(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCallOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCloseCall(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCallClosed(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleAbortCall(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCallAborted(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleOpenDesktop(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleDesktopOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCloseDesktop(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleOpenWhiteboard(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleWhiteboardOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCleanWhiteboard(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCloseWhiteboard(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleSetDrawPointer(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleResetDrawPointer(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleOpenFtp(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleFtpOpened(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleCloseFtp(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleFtpClosed(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleListDirectory(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleDirectoryList(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleGetFile(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleAudioFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleVideoFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleDrawFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleDesktopFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv);
int JADA_HandleFtpFrame(JADA_ConnSelf *connPtr, JADA_Message msgRecv);

/* VNC services */
//int SetVNCPassword(char *szNewPassword);
//int SetVNCServer(void);

/* Message communications */
int  JADA_ConnectToServer(void);
int  JADA_SendConnectionMessage(JADA_Connection connSelf, int iOpcode, void *pPayload, int iSize);
int  JADA_SendSupportMessage(JADA_Support suppSelf, int iOpcode, void *pPayload, int iSize);

/* Processes management */
int  JADA_ProcExists(char *szProcName);
int  JADA_ProcKill(char *szProcName);
int  JADA_ProcExecute(char *szProcName, char *szProcParams);

/* Connections */
int JADA_ConnSubscribeEvent(JADA_Connection connSelf, int iEventType);
int JADA_ConnUnsubscribeEvent(JADA_Connection connSelf, int iEventType);
int JADA_ConnWaitSubscribedEvent(JADA_Connection connSelf, int iEventType, int iSecs, JADA_Event *evtSelf);

/* threads */
void *JADA_ConnectionReceiverThread(void *FuncArg);
void *JADA_ConnectionSenderThread(void *FuncArg);
void *JADA_FtpSenderThread(void *FuncArg);

/* desktop
JADA_Desktop JADA_DskNew(char *szFile, int iLine, JADA_Request dskSelf);
int JADA_DskDestroy(char *szFile, int iLine, JADA_Desktop dskSelf);
//int JADA_DskSetId(JADA_Desktop dskSelf, JADA_Id iId);
//int JADA_DskGetId(JADA_Desktop dskSelf, JADA_Id *piId);
int JADA_DskOpen(JADA_Desktop dskSelf);
int JADA_DskAccept(JADA_Desktop dskSelf);
int JADA_DskSetStatus(JADA_Desktop dskSelf, int iStatus);
int JADA_DskGetRequest(JADA_Desktop dskSelf, JADA_Request *reqSelf);
int JADA_SendDesktopMessage(JADA_Desktop dskSelf, int iOpcode, void *pPayload, int iSize);
 */

/* tcp */
int JADA_TcpConnectToPort(char *szHostName, int iPort, JADA_Socket *sokSelf);
int JADA_TcpEnqueueMessage(JADA_Socket *sokSelf, JADA_Message msgOut, int iPriority);
int JADA_TcpWaitEnqueuedMessage(JADA_Socket *sokSelf, int iMsecsToWait);
int JADA_TcpCreateSocket(JADA_Socket *sokSelf, int iSocket);
int JADA_TcpDestroySocket(JADA_Socket *sokSelf);
int JADA_TcpIsSocketOpened(JADA_Socket *sokSelf);
int JADA_TcpSendMessage(JADA_Socket *sokSelf, JADA_Message msgOut);
JADA_Message JADA_TcpReceiveMessage(char *szFile, int iLine, JADA_Socket *sokSelf);
int JADA_VncSendDataToSocket_DaButtare(int iSocket, void *szBlock, int iLength);
int JADA_VncReceiveDataFromSocket_DaButtare(int iSocket, void *szBlock, int iLength);

#undef in
#undef out

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_JADA_INTERNAL_H_ */
