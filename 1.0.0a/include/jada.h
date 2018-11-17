#ifndef _JADA_H_
#define _JADA_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ANSI_C                      1

/******************************************************************************/
/*                                                                            */
/*   G e n e r a l     I n c l u d JADA_StmGetLength     F i l e s                            */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#ifndef UNIX
#include "oss/pthread.h"
#ifndef NO_WINDOWS_H
#include <windows.h>
#endif
#else
#include <pthread.h>
#include <stdarg.h>
#endif

/******************************************************************************/
/*                                                                            */
/*   M a c r o s     D e f i n i t i o n                                      */
/*                                                                            */
/******************************************************************************/
#ifndef UNIX
#define DllExport       __declspec(dllexport)
#else
#define __stdcall
#define DllExport
#define SOCKET          int
#define TRUE            1
#define FALSE           0
#define closesocket     close
#define _unlink         unlink
#endif

/* Assert if the condition fails */
#define CORE_AssertIfFail(expr, cmds, msg)                              \
        { if (!(expr)) {                                                \
            JADA_LogError("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            assert(0);                                                  \
            cmds;                                                       \
            return;                                                     \
        } }
#define CORE_AssertValIfFail(expr, val, cmds, msg)                      \
        { if (!(expr)) {                                                \
            JADA_LogError("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            assert(0);                                                  \
            cmds;                                                       \
            return(val);                                                \
        } }

/* Return if the condition fails */
#define CORE_ReturnIfFail(expr, cmds, msg)                              \
        { if (!(expr)) {                                                \
            JADA_LogError("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                       \
            return;                                                     \
        } }
#define CORE_SilentReturnIfFail(expr, cmds)                             \
        { if (!(expr)) {                                                \
            cmds;                                                       \
            return;                                                     \
        } }
#define CORE_ReturnValIfFail(expr, val, cmds, msg)                      \
        { if (!(expr)) {                                                \
            JADA_LogError("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                       \
            return(val);                                                \
        } }
#define CORE_SilentReturnValIfFail(expr, val, cmds)                     \
        { if (!(expr)) {                                                \
            cmds;                                                       \
            return(val);                                                \
        } }
#define CORE_LogIfFail(expr, msg)                                       \
        { if (!(expr)) {                                                \
            JADA_LogMessage("%s, %d: %s\n", __FILE__, __LINE__, msg);   \
        } }

/* Execute statements if the condition fails */
#define CORE_ExecuteIfFail(expr, cmds, msg)                             \
        { if (!(expr)) {                                                \
            JADA_LogMessage("%s, %d: %s\n", __FILE__, __LINE__, msg);   \
            cmds;                                                       \
        } }
#define CORE_SilentExecuteIfFail(expr, cmds)                            \
        { if (!(expr)) {                                                \
            cmds;                                                       \
        } }

/* Goto label if the condition fails */
#define CORE_GotoIfFail(expr, label, cmds, msg)                         \
        { if (!(expr)) {                                                \
            JADA_LogError("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                       \
            goto label;                                                 \
        } }
#define CORE_SilentGotoIfFail(expr, label, cmds)                        \
        { if (!(expr)) {                                                \
            cmds;                                                       \
            goto label;                                                 \
        } }

#define CORE_Max(a,b)               (((a) > (b)) ? (a) : (b))
#define CORE_Min(a,b)               (((a) < (b)) ? (a) : (b))

/* Swap memory buffers */
#define CORE_Swap(a, b, size)                                           \
    do {                                                                \
        register size_t __size = (size);                                \
        register char *__a = (a), *__b = (b);                           \
        do {                                                            \
            char __tmp = *__a;                                          \
            *__a++ = *__b;                                              \
            *__b++ = __tmp;                                             \
        } while (--__size > 0);                                         \
    } while (0)

/* Evaluate the condition and set the value accordlingly */
#define CORE_Evaluate(cond,yes,no)  ((cond) ? (yes) : (no))

#define JADA_IsValidConnection(self)                                    \
        (self != NULL) 
#define JADA_IsValidSupport(self)                                       \
        (self != NULL) 
#define JADA_IsValidCall(self)                                         \
        (self != NULL) 
#define JADA_IsValidVNC(self)                                           \
        (self != NULL) 
#define JADA_IsValidFTP(self)                                           \
        (self != NULL) 


/******************************************************************************
 *
 *   C o n s t a n t s     D e f i n i t i o n
 *
 ******************************************************************************/
#ifndef TRUE
#define TRUE                   1
#endif
#ifndef FALSE
#define FALSE                  0
#endif

/* Moduli dell'architettura */
#define JADA_WHOAMI_UNKNOWN         1111
#define JADA_WHOAMI_AGENT           2222
#define JADA_WHOAMI_MANAGER         3333
#define JADA_WHOAMI_SERVER          4444

/* Tipi di sessione */
#define JADA_SESSTYPE_FTP           3131
#define JADA_SESSTYPE_CALL          4242
#define JADA_SESSTYPE_SUPPORT       5353
#define JADA_SESSTYPE_VNC           6464

/* Porte TCP di servizio */
#define JADA_REMOTE_DESKTOP_PORT    40000
// #define JADA_AUDIOVIDEO_PORT        41000
#define JADA_VNCSERVER_PORT         45900

/* Destinatari dei messaggi */
#define JADA_SENDTO_AGENT           4321
#define JADA_SENDTO_MANAGER         1234

/* Tipi di frame */
#define JADA_FRAMETYPE_AUDIO        12341
#define JADA_FRAMETYPE_VIDEO        12342
#define JADA_FRAMETYPE_DESKTOP      12343
#define JADA_FRAMETYPE_DRAW         12344
#define JADA_FRAMETYPE_FTP          12345

/* Tipi di servizio */
#define JADA_SVCTYPE_CALL           43100
#define JADA_SVCTYPE_VNC            43200
#define JADA_SVCTYPE_FTP            43300

/* Tipi di combo */
#define JADA_COMBOTYPE_REGIONE      4101
#define JADA_COMBOTYPE_CLIENTE      4202
#define JADA_COMBOTYPE_CODA         4303
#define JADA_COMBOTYPE_MANAGER_CODA 4404

/* Codice di ritorno per la apertura di sessione */
#define JADA_PROXY_AUTHENTICATE     -9876

/* Timeout sulla select nei thread di elaborazione */
#define JADA_SELECT_TIMEOUT         60

/******************************************************************************
 *
 *   B a s i c     T y p e s     D e f i n i t i o n
 *
 ******************************************************************************/
typedef char            LongString[1024];
typedef char            FixString[256];
typedef char            ShortString[64];
typedef char            Identifier[32];
typedef int             CORE_Currency;
typedef unsigned int    JADA_Timet; 

#ifndef UNIX
#define CORE_Callback       int __stdcall
typedef int (__stdcall *CORE_CallbackPtr)();
#define CORE_Function       int __stdcall
typedef int (__stdcall *CORE_FunctionPtr)();
#else
#define CORE_Callback       int
typedef int (*CORE_CallbackPtr)();
#define CORE_Function       int
typedef int (*CORE_FunctionPtr)();
#endif

typedef struct {
    char                *szMenuLevel;
    int                 iOption;
    char                *szDescription;
    CORE_CallbackPtr    cbkFunction;
    int                 iPressKeyWhenFinished;
} JADA_Command;

typedef struct {
    int                 iCode;
    char                *szValue;
} JADA_CodeValue;

//typedef struct {
//    unsigned int    Server  : 8;
//    unsigned int    Id      : 24;
//} JADA_Id;
/* Generic identifier */
/* Connection identifier */
typedef struct {
    int         iConnId;
} JADA_ConnId;    
/* Session identifier */
typedef struct {
    int         iSessId;
} JADA_SessId;    
/* Support identifier */
typedef struct {
    int         iSuppId;
} JADA_SuppId;    
/* Agent identifier */
typedef struct {
    int         iAgtId;
} JADA_AgtId;     
/* Manager identifier */
typedef struct {
    int         iMgrId;
} JADA_MgrId;     
/* Message identifier */
//typedef struct {
//    int         iMsgId;
//} JADA_MsgId;     
typedef int JADA_MsgId;     
/* Request identifier */
typedef struct {
    int         iReqId;
} JADA_ReqId;     
/* Queue identifier */
typedef struct {
    int         iQueueId;
} JADA_QueueId;   

/******************************************************************************
 *
 *   C l a s s     D e f i n i t i o n
 *
 ******************************************************************************/
typedef struct {
    void    *ptr;
} JADA_ConnectionStruct;
typedef JADA_ConnectionStruct *JADA_Connection;

typedef struct {
    void    *ptr;
} JADA_SupportStruct;
typedef JADA_SupportStruct *JADA_Support;

typedef struct {
    void    *ptr;
} JADA_CallStruct;
typedef JADA_CallStruct *JADA_Call;

typedef struct {
    void    *ptr;
} JADA_VNCStruct;
typedef JADA_VNCStruct *JADA_VNC;

typedef struct {
    void    *ptr;
} JADA_FTPStruct;
typedef JADA_FTPStruct *JADA_FTP;

typedef struct {
    void    *ptr;
} CORE_ListStruct;
typedef CORE_ListStruct *CORE_List;

typedef struct {
    void    *ptr;
} JADA_MessageStruct;
typedef JADA_MessageStruct *JADA_Message;

typedef struct {
    void    *ptr;
} JADA_EventStruct;
typedef JADA_EventStruct *JADA_Event;

typedef struct {
    void    *ptr;
} JADA_FrameStruct;
typedef JADA_FrameStruct *JADA_Frame;

typedef struct {
    void    *ptr;
} JADA_DrawStruct;
typedef JADA_DrawStruct *JADA_Draw;

typedef struct {
    void    *ptr;
} JADA_CurveStruct;
typedef JADA_CurveStruct *JADA_Curve;

typedef struct {
    void    *ptr;
} JADA_StreamStruct;
typedef JADA_StreamStruct *JADA_Stream;

/******************************************************************************
 *
 *   M e t h o d s     D e f i n i t i o n
 *
 ******************************************************************************/
#ifdef ANSI_C

#define in
#define out

/* Common */
char *JADA_GetLogDir(void);
//int JADA_GetSocketVNC(void);
DllExport char * __stdcall JADA_TimeToString(JADA_Timet timSelf, char *szTimeString, int iLength);
DllExport char * __stdcall JADA_WhoAmIToString(int iWhoAmI, char *szString, int iLength);
DllExport char * __stdcall JADA_SessionIdToToken(JADA_SessId iSessId, char *szSessionIdString, int iLength);
DllExport int __stdcall JADA_TokenToSessionId(char *szSessionIdString, JADA_SessId *piSessId);
DllExport int __stdcall JADA_IntToRequestId(int irequestId, JADA_ReqId *piReqId);
DllExport int __stdcall JADA_Sleep(int iMilliSecs);
DllExport int __stdcall JADA_LogStart(void);
DllExport void __stdcall JADA_LogMessage(char *szFormat, ...);
DllExport void __stdcall JADA_LogError(char *szFormat, ...);
DllExport void __stdcall JADA_LogToFile(int iWhoAmI, char *szFile, unsigned char *szBuffer, int iLength);
int JADA_GetWhoAmI(void);
void * JADA_Calloc(char *szFile, int iLine, int iEntries, int iSize);
void *JADA_Realloc(void *szOldPtr, int iSize);
void JADA_Free(char *szFile, int iLine, void *szPtr, int iLength);
int JADA_FileExists(char *szFile);
int JADA_FileRemove(char *szFile);
void *JADA_DeflateBuffer(void *pvBuffer, int iLength, int *piDeflatedLength);
void *JADA_InflateBuffer(int iRealLength, void *pvBuffer, int iLength);
char *StringFormat(char *szFormat, ...);
int JADA_BufSetInt32(char *szBuffer, int iLength, int iValue, int *piOffset);
int JADA_BufGetInt32(char *szBuffer, int iLength, int *piValue, int *piOffset);
int JADA_BufSetString(char *szBuffer, int iLength, char *szValue, int iValueLength, int *piOffset);
int JADA_BufGetString(char *szBuffer, int iLength, char *szValue, int iValueLength, int *piOffset);
int JADA_BufSetJadaMsgId(char *szBuffer, int iLength, JADA_MsgId iValue, int *piOffset);
int JADA_BufGetJadaMsgId(char *szBuffer, int iLength, JADA_MsgId *piValue, int *piOffset);
int JADA_BufSetJadaSessId(char *szBuffer, int iLength, JADA_SessId iValue, int *piOffset);
int JADA_BufGetJadaSessId(char *szBuffer, int iLength, JADA_SessId *piValue, int *piOffset);
int JADA_StringToJadaId(char *szString, void *piId);
char *JADA_JadaIdToString(void *piId, char *szString);

/* TCP communications */
DllExport int __stdcall JADA_TcpCreateListenPort(char *szHostName, int iPort);
DllExport int __stdcall JADA_TcpSetUseProxy(int iUseProxy);
DllExport int __stdcall JADA_TcpGetUseProxy(int *piUseProxy);
DllExport int __stdcall JADA_TcpSetProxyAddress(char *szProxyName, int iPort);
DllExport int __stdcall JADA_TcpSetProxyCredentials(char *szUser, char *szPassword, char *szOpenPassword);
DllExport int __stdcall JADA_TcpGetProxyCredentials(char *szUser, int iUserLength, char *szPassword, int iPasswordLength);
int JADA_TcpSendMessageToSocket(int iSocket, unsigned char *szBuffer, int iLength, JADA_Message msgOut);
JADA_Message JADA_TcpReceiveMessageFromSocket(char *szFile, int iLine, int iSocket, unsigned char *szBuffer, int iLength);

/* Connection */
DllExport JADA_Connection __stdcall JADA_ConnNew(char *szFile, int iLine, int iTestMode);
DllExport int __stdcall JADA_ConnDestroy(char *szFile, int iLine, JADA_Connection connSelf);
DllExport int __stdcall JADA_ConnConnect(JADA_Connection connSelf, int iSessionType, int iWaitSeconds);
DllExport int __stdcall JADA_ConnOpen(JADA_Connection connSelf, int iWaitSeconds);
DllExport int __stdcall JADA_ConnClose(JADA_Connection connSelf);
DllExport int __stdcall JADA_ConnListRequests(JADA_Connection connSelf, char *szFilterCoda, int *piStreamId, int iWaitSeconds);
DllExport int __stdcall JADA_ConnListPendingRequests(JADA_Connection connSelf, char *szFilterCoda, int *piStreamId, int iWaitSeconds);
DllExport int __stdcall JADA_ConnGetListCounters(JADA_Connection connSelf);
DllExport int __stdcall JADA_ConnRaiseEvent(JADA_Connection connSelf, JADA_Event evtSelf);
DllExport int __stdcall JADA_ConnGetNextEvent(JADA_Connection connSelf, int iMilliSecs, JADA_Event *evtSelf);
DllExport int __stdcall JADA_ConnGetParameter(JADA_Connection connSelf, char *szSection, char *szName, char *szValue, int iLength);
DllExport int __stdcall JADA_ConnGetParameterInt(JADA_Connection connSelf, char *szSection, char *szName, int *piValue);
DllExport int __stdcall JADA_ConnSetParameter(JADA_Connection connSelf, char *szSection, char *szName, char *szValue);
DllExport int __stdcall JADA_ConnSetParameterInt(JADA_Connection connSelf, char *szSection, char *szName, int iValue);
DllExport int __stdcall JADA_ConnCopyParameterValue(JADA_Connection connSelf, char *szFromSection, char *szFromName, char *szToSection, char *szToName);
DllExport int __stdcall JADA_ConnGetCombo(JADA_Connection connSelf, int iComboType, char *szFilter, int *piStreamId, int iWaitSeconds);
DllExport int __stdcall JADA_ConnGetSetting(JADA_Connection connSelf, char *szKey, char *szValue, int iLength);
DllExport JADA_Stream __stdcall JADA_ConnGetStream(JADA_Connection connSelf, int iStreamId);
DllExport int __stdcall JADA_ConnAddStream(JADA_Connection connSelf, JADA_Stream stmSelf);

/* Ini file management */
int JADA_MyIniGetString(char *szSection, char *szKey, char *szOut, int iOutLength, char *szFileName);
int JADA_MyIniSetString(char *szSection, char *szKey, char *szIn, char *szFileName);
int JADA_MyIniGetInt(char *szSection, char *szKey, int *piOut, char *szFileName);
int JADA_MyIniSetInt(char *szSection, char *szKey, int iIn, char *szFileName);
int JADA_MyIniGetJadaId(char *szSection, char *szKey, void *piOut, char *szFileName);
int JADA_MyIniSetJadaId(char *szSection, char *szKey, void *piId, char *szFileName);
DllExport int __stdcall JADA_IniGetString(char *szSection, char *szKey, char *szOut, int iOutLength);
DllExport int __stdcall JADA_IniSetString(char *szSection, char *szKey, char *szIn);
DllExport int __stdcall JADA_IniGetInt(char *szSection, char *szKey, int *piOut);
DllExport int __stdcall JADA_IniSetInt(char *szSection, char *szKey, int iIn);
DllExport int __stdcall JADA_IniGetJadaId(char *szSection, char *szKey, void *piId);
DllExport int __stdcall JADA_IniSetJadaId(char *szSection, char *szKey, void *piId);
DllExport int __stdcall JADA_ServerGetString(char *szSection, char *szKey, char *szOut, int iOutLength);
DllExport int __stdcall JADA_ServerGetInt(char *szSection, char *szKey, int *piOut);
DllExport int __stdcall CORE_GetString(char *szPrompt, char *szString, int iLength);
DllExport int __stdcall CORE_GetInteger(char *szPrompt, int *piInteger);
DllExport int __stdcall CORE_GetTrueFalse(char *szPrompt);

/* Support */
DllExport JADA_Support __stdcall JADA_SuppNew(char *szFile, int iLine, JADA_Connection connSelf);
DllExport int __stdcall JADA_SuppDestroy(char *szFile, int iLine, JADA_Support suppSelf);
// DllExport int __stdcall JADA_SuppSetSessionId(JADA_Support suppSelf, JADA_Id iId);
DllExport int __stdcall JADA_SuppGetRequestId(JADA_Support suppSelf, JADA_ReqId *piId);
DllExport int __stdcall JADA_SuppGetSessionId(JADA_Support suppSelf, JADA_SessId *piId);
DllExport int __stdcall JADA_SuppOpen(JADA_Support suppSelf, int iWaitSeconds);
DllExport int __stdcall JADA_SuppConnect(JADA_Support suppSelf, JADA_ReqId iRequestId);
DllExport int __stdcall JADA_SuppAccept(JADA_Support suppSelf, JADA_ReqId iRequestId);
DllExport int __stdcall JADA_SuppLoad(JADA_Support suppSelf, JADA_ReqId iRequestId);
DllExport int __stdcall JADA_SuppResume(JADA_Support suppSelf, JADA_ReqId iRequestId);
DllExport int __stdcall JADA_SuppSolicit(JADA_Support suppSelf);
DllExport int __stdcall JADA_SuppClose(JADA_Support suppSelf);
DllExport int __stdcall JADA_SuppIsConnected(JADA_Support suppSelf);
DllExport int __stdcall JADA_SuppSendChat(JADA_Support suppSelf, char *szText);
DllExport int __stdcall JADA_SuppSetStatus(JADA_Support suppSelf, int iStatus);
//DllExport int __stdcall JADA_SuppRequestService(JADA_Support suppSelf, int iType);
//DllExport int __stdcall JADA_SuppAcceptService(JADA_Support suppSelf, int iType, int iAccepted);
DllExport int __stdcall JADA_SuppPing(JADA_Support suppSelf);
DllExport int __stdcall JADA_SuppGetConnection(JADA_Support suppSelf, JADA_Connection *connSelf);
DllExport int __stdcall JADA_SuppListParameters(JADA_Support suppSelf, int *piStreamId, int iWaitSeconds);
DllExport int __stdcall JADA_SuppGetSuspendedRequestId(JADA_Support suppSelf, int *piId);
DllExport int __stdcall JADA_SuppSetSuspendedRequestId(JADA_Support suppSelf, int iSuspendedId);

/* Call */
DllExport JADA_Call __stdcall JADA_CallNew(char *szFile, int iLine, JADA_Connection connSelf);
DllExport int __stdcall JADA_CallDestroy(char *szFile, int iLine, JADA_Call callSelf);
DllExport int __stdcall JADA_CallGetRequestId(JADA_Call callSelf, JADA_ReqId *piId);
DllExport int __stdcall JADA_CallGetSessionId(JADA_Call callSelf, JADA_SessId *piId);
DllExport int __stdcall JADA_CallGetConnection(JADA_Call callSelf, JADA_Connection *connSelf);
DllExport int __stdcall JADA_CallIsConnected(JADA_Call callSelf);
DllExport int __stdcall JADA_CallOpen(JADA_Call callSelf, JADA_ReqId iRequestId);
DllExport int __stdcall JADA_CallReset(JADA_Call callSelf, int iReset);
DllExport int __stdcall JADA_CallIsReset(JADA_Call callSelf);
DllExport int __stdcall JADA_CallMuteAudio(JADA_Call callSelf, int iMuteStatus);
DllExport int __stdcall JADA_CallMuteVideo(JADA_Call callSelf, int iMuteStatus);
DllExport int __stdcall JADA_CallIsAudioMuted(JADA_Call callSelf);
DllExport int __stdcall JADA_CallIsVideoMuted(JADA_Call callSelf);
DllExport int __stdcall JADA_CallClose(JADA_Call callSelf);
DllExport int __stdcall JADA_CallAbort(JADA_Call callSelf, int iRetries);
DllExport int __stdcall JADA_CallGetNextAudioFrame(JADA_Call callSelf, int iMilliSecs, JADA_Frame *frmSelf);
DllExport int __stdcall JADA_CallGetNextVideoFrame(JADA_Call callSelf, int iMilliSecs, JADA_Frame *frmSelf);
DllExport int __stdcall JADA_CallSendAudioFrame(JADA_Call callSelf, void *szFrameData, int iLength);
DllExport int __stdcall JADA_CallSendVideoFrame(JADA_Call callSelf, void *szFrameData, int iLength);
DllExport int __stdcall JADA_CallEnqueueAudioFrame(JADA_Call callSelf, JADA_Frame *frmSelf);
DllExport int __stdcall JADA_CallEnqueueVideoFrame(JADA_Call callSelf, JADA_Frame *frmSelf);
DllExport int __stdcall JADA_CallSetVideoFrameDelay(JADA_Call callSelf, int iDelay);
DllExport int __stdcall JADA_CallGetVideoFrameDelay(JADA_Call callSelf, int *piDelay);
int JADA_SendCallMessage(JADA_Call callSelf, int iOpcode, void *pPayload, int iSize);

/* VNC */
DllExport JADA_VNC __stdcall JADA_VncNew(char *szFile, int iLine, JADA_Connection connSelf);
DllExport int __stdcall JADA_VncDestroy(char *szFile, int iLine, JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncGetRequestId(JADA_VNC vncSelf, JADA_ReqId *piId);
DllExport int __stdcall JADA_VncGetSessionId(JADA_VNC vncSelf, JADA_SessId *piId);
DllExport int __stdcall JADA_VncIsConnected(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncGetConnection(JADA_VNC vncSelf, JADA_Connection *connSelf);
DllExport int __stdcall JADA_VncOpenDesktop(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncAcceptDesktop(JADA_VNC vncSelf, int iAccepted);
DllExport int __stdcall JADA_VncCloseDesktop(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncOpenWhiteboard(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncCleanWhiteboard(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncCloseWhiteboard(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncGetNextDrawFrame(JADA_VNC vncSelf, int iMilliSecs, JADA_Frame *frmSelf);
DllExport int __stdcall JADA_VncSendDrawFrame(JADA_VNC vncSelf, JADA_Draw drwSelf);
DllExport int __stdcall JADA_VncEnqueueDrawFrame(JADA_VNC vncSelf, JADA_Draw *drwSelf);
DllExport int __stdcall JADA_VncSetDrawPointer(JADA_VNC vncSelf, int iPointerType, int iPointX, int iPointY);
DllExport int __stdcall JADA_VncResetDrawPointer(JADA_VNC vncSelf);
DllExport int __stdcall JADA_VncSendDesktopFrame(JADA_VNC vncSelf, void *szFrameData, int iLength);
int JADA_SendVncMessage(JADA_VNC vncSelf, int iOpcode, void *pPayload, int iSize);

/* FTP */
DllExport JADA_FTP __stdcall JADA_FtpNew(char *szFile, int iLine, JADA_Connection connSelf);
DllExport int __stdcall JADA_FtpDestroy(char *szFile, int iLine, JADA_FTP ftpSelf);
DllExport int __stdcall JADA_FtpGetRequestId(JADA_FTP ftpSelf, JADA_ReqId *piId);
DllExport int __stdcall JADA_FtpGetSessionId(JADA_FTP ftpSelf, JADA_SessId *piId);
DllExport int __stdcall JADA_FtpIsConnected(JADA_FTP ftpSelf);
DllExport int __stdcall JADA_FtpGetConnection(JADA_FTP ftpSelf, JADA_Connection *connSelf);
DllExport int __stdcall JADA_FtpOpen(JADA_FTP ftpSelf);
DllExport int __stdcall JADA_FtpClose(JADA_FTP ftpSelf);
DllExport int __stdcall JADA_FtpSendFtpFrame(JADA_FTP ftpSelf, void *szFrameData, int iLength);
DllExport int __stdcall JADA_FtpListDirectory(JADA_FTP ftpSelf, char *szDirName);
DllExport int __stdcall JADA_FtpGetFile(JADA_FTP ftpSelf, char *szRemoteFile, char *szLocalFile);
DllExport int __stdcall JADA_FtpCancelGetFile(JADA_FTP ftpSelf);
DllExport int __stdcall JADA_FtpPutFile(JADA_FTP ftpSelf, char *szLocalFile, char *szRemoteFile);
DllExport int __stdcall JADA_FtpCancelPutFile(JADA_FTP ftpSelf);
int JADA_SendFtpMessage(JADA_FTP ftpSelf, int iOpcode, void *pPayload, int iSize);
int JADA_FtpEnqueueFtp(JADA_FTP ftpSelf, char *szLocalFile, char *szRemoteFile);
int JADA_FtpCancelFtp(JADA_FTP ftpSelf);

/* Frame */
DllExport JADA_Frame __stdcall JADA_FrmNew(char *szFile, int iLine, int iType);
DllExport JADA_Frame __stdcall JADA_FrmNewFromMessage(char *szFile, int iLine, JADA_Message msgSelf);
DllExport int __stdcall JADA_FrmDestroy(char *szFile, int iLine, JADA_Frame evtSelf);
DllExport int __stdcall JADA_FrmGetType(JADA_Frame evtSelf, int *piType);
DllExport int __stdcall JADA_FrmGetLength(JADA_Frame evtSelf, int *piLength);
DllExport int __stdcall JADA_FrmSetData(JADA_Frame evtSelf, void *szPayload, int iLength);
DllExport int __stdcall JADA_FrmGetData(JADA_Frame evtSelf, void *szPayload, int iLength);

/* Stream */
DllExport JADA_Stream __stdcall JADA_StmNew(char *szLine, int iLine, JADA_Connection connSelf);
DllExport int __stdcall JADA_StmDestroy(char *szLine, int iLine, JADA_Stream stmSelf);
DllExport int __stdcall JADA_StmGetId(JADA_Stream stmSelf, int *piId);
//DllExport int __stdcall JADA_StmGetLength(JADA_Stream stmSelf, int *piLength);
//DllExport int __stdcall JADA_StmSetData(JADA_Stream stmSelf, char *szPayload, int iLength);
//DllExport int __stdcall JADA_StmGetData(JADA_Stream stmSelf, char *szPayload, int iLength);
DllExport int __stdcall JADA_StmAddLine(JADA_Stream stmSelf, char *szLine);
DllExport int __stdcall JADA_StmGetFirstLine(JADA_Stream stmSelf, char *szLine, int iLength);
DllExport int __stdcall JADA_StmGetNextLine(JADA_Stream stmSelf, char *szLine, int iLength);

/* Ini file management */
DllExport int __stdcall JADA_Start(int iWhoAmI, char *szHomeDir);
DllExport int __stdcall JADA_Stop(void);

/* Menu management */
DllExport CORE_Callback __stdcall JADA_MnuExit(void);
DllExport CORE_Callback __stdcall JADA_MnuNext(void);
DllExport CORE_Callback __stdcall JADA_MnuBack(void);
DllExport int __stdcall JADA_WaitKey(char *szPrompt);
DllExport int __stdcall JADA_StartMenu(char *szCmdName, char *szProcessName, JADA_Command *Command);

/* List */
DllExport CORE_List __stdcall JADA_LstNew(char *szFile, int iLine, int iEntrySize, CORE_FunctionPtr pfnCompare);
DllExport int __stdcall JADA_LstIsValid(CORE_List lstSelf);
DllExport int __stdcall JADA_LstDestroy(char *szFile, int iLine, CORE_List lstSelf);
DllExport int __stdcall JADA_LstClear(CORE_List lstSelf);
DllExport int __stdcall JADA_LstAdd(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstDelete(CORE_List lstSelf, CORE_Currency *curC);
DllExport int __stdcall JADA_LstUpdate(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstFind(CORE_List lstSelf, CORE_FunctionPtr pfnCompare, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGet(CORE_List lstSelf, int iEntryNumber, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetCurrent(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetIndex(CORE_List lstSelf, CORE_Currency *curC, int *piIndex);
DllExport int __stdcall JADA_LstSetIndex(CORE_List lstSelf, int iIndex, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetFirst(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetNext(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetPrev(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetLast(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC);
DllExport int __stdcall JADA_LstGetRandom(CORE_List lstSelf, void *pvEntry, int iLength,  CORE_Currency *curC);
DllExport int __stdcall JADA_LstWait(CORE_List lstSelf, void *pvEntry, int iLength, int iMsecsToWait);
DllExport int __stdcall JADA_LstSort(CORE_List lstSelf, CORE_FunctionPtr pfnSort);
DllExport int __stdcall JADA_LstGetCount(CORE_List lstSelf);
DllExport int __stdcall JADA_LstDebugAppend(char *szFile, int iLine, CORE_List lstDest, CORE_List lstSrc);
DllExport int __stdcall JADA_LstPrintDescr(CORE_List lstSelf);

/* draw */
DllExport JADA_Draw __stdcall JADA_DrwNew(char *szLine, int iLine);
DllExport int __stdcall JADA_DrwDestroy(char *szLine, int iLine, JADA_Draw drwSelf);
DllExport int __stdcall JADA_DrwGetCurveCount(JADA_Draw drwSelf, int *piCount);
DllExport int __stdcall JADA_DrwGetCurve(JADA_Draw drwSelf, int iIndex, JADA_Curve *crvSelf);
DllExport int __stdcall JADA_DrwAddCurve(JADA_Draw drwSelf, JADA_Curve crvSelf);
DllExport JADA_Curve __stdcall JADA_CrvNew(char *szLine, int iLine);
DllExport int __stdcall JADA_CrvDestroy(char *szLine, int iLine, JADA_Curve crvSelf);
DllExport int __stdcall JADA_CrvSetColor(JADA_Curve crvSelf, int iAlpha, int iRed, int iGreen, int iBlue);
DllExport int __stdcall JADA_CrvGetColor(JADA_Curve crvSelf, int *piAlpha, int *piRed, int *piGreen, int *piBlue);
DllExport int __stdcall JADA_CrvSetWidth(JADA_Curve crvSelf, int iWidth);
DllExport int __stdcall JADA_CrvGetWidth(JADA_Curve crvSelf, int *piWidth);
DllExport int __stdcall JADA_CrvGetCoordinatesCount(JADA_Curve crvSelf, int *piCount);
DllExport int __stdcall JADA_CrvGetCoordinate(JADA_Curve crvSelf, int iIndex, int *pX, int *pY);
DllExport int __stdcall JADA_CrvAddCoordinate(JADA_Curve crvSelf, int X, int Y);
DllExport int __stdcall JADA_DrwSerialize(JADA_Draw drwSelf, void *szBuffer, int iLength, int *piOutLength);
DllExport JADA_Draw __stdcall JADA_DrwDeserialize(void *szBuffer, int iLength);
DllExport int __stdcall JADA_CrvSerialize(JADA_Curve crvSelf, void *szBuffer, int iLength, int *piOutLength);
DllExport JADA_Curve __stdcall JADA_CrvDeserialize(void *szBuffer, int iLength);
DllExport int __stdcall JADA_DrwTest(void);
DllExport int __stdcall JADA_CrvTest(void);

#undef in
#undef out

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_JADA_H_ */


 
