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

/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
/* Who Am I? */
#ifdef SERVER
#define ASRV_WHOAMI         JADA_WHOAMI_SERVER
#endif
#ifndef ASRV_WHOAMI
"Errore: tipo di processo server non definito."
#endif

/* Magic numbers */
#define ASRV_MAGIC_CONNECTION       12340
#define ASRV_MAGIC_SESSION          12341

/* Session types */
#define ASRV_SESSION_TYPE_FTP       51001
#define ASRV_SESSION_TYPE_MEDIA     52002
#define ASRV_SESSION_TYPE_SUPPORT   53003
#define ASRV_SESSION_TYPE_VNC       54004

/* Errori dalle primitive base */ 
#define JADA_ERR_TCP_CREATE         -1000
#define JADA_ERR_TCP_SELECT         -1001
#define JADA_ERR_TCP_ACCEPT         -1002
#define JADA_ERR_TCP_RECV           -1003
#define JADA_ERR_TCP_SEND           -1004
#define JADA_ERR_WRONG_FRAMETYPE    -1005
#define JADA_ERR_WRONG_FRAMELENGTH  -1006

#define ASRV_CONNSTATUS_UNKNOWN        '?'
#define ASRV_CONNSTATUS_OPENED         'O'
#define ASRV_CONNSTATUS_RUNNING        'R'
#define ASRV_CONNSTATUS_CLOSED         'C'

#define ASRV_SESSSTATUS_UNKNOWN        '?'
#define ASRV_SESSSTATUS_OPENED         'O'
#define ASRV_SESSSTATUS_RUNNING        'R'
#define ASRV_SESSSTATUS_CLOSED         'C'

#define ASRV_REQSTATUS_UNKNOWN         '?'
#define ASRV_REQSTATUS_OPENED          'O'
#define ASRV_REQSTATUS_LOADED          'L'
#define ASRV_REQSTATUS_RUNNING         'R'
//#define ASRV_REQSTATUS_PENDING         'P'
#define ASRV_REQSTATUS_CLOSED          'C'

//#define OPCODE_UNKNOWN                  0
//#define OPCODE_REQUEST_SESSION          1
//#define OPCODE_ACCEPT_SESSION           2
//#define OPCODE_SEND_CHAT                3

/******************************************************************************
 *
 *   T y p e s     D e f i n i t i o n
 *
 ******************************************************************************/
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

/******************************************************************************
 *
 *   D a t a     T y p e s     D e f i n i t i o n
 *
 ******************************************************************************/
typedef struct {
    JADA_ConnId     iIdSource;
    int             iWhoAmI;
    JADA_Message    msgIn;
} ASRV_IncomingMsg;

/* AGENT */ 
typedef struct {
    JADA_AgtId          iId;
    Identifier          szNome;
    Identifier          szMail;
    Identifier          szTelefono;
    Identifier          szCellulare;
    int                 iClienteId;
    Identifier          szCliente;
    int                 iRegioneId;
    Identifier          szRegione;
    int                 iUsaAudio;
} ASRV_DbAgent;

/* MANAGER */ 
typedef struct {
    JADA_MgrId          iId;
    Identifier          szNome;
    Identifier          szUsername;
    Identifier          szPassword;
    Identifier          szMail;
    Identifier          szTelefono;
    Identifier          szCellulare;
    int                 iUsaAudio;
    Identifier          szArea;
    CORE_List           lstCoda;
} ASRV_DbManager;

/* RICHIESTA */ 
typedef struct {
    JADA_ReqId          iId;
    int                 iStato;
    JADA_QueueId        iIdCoda;
    Identifier          szCoda;
    JADA_AgtId          iIdAgent;
    LongString          szDescrizione;
    JADA_Timet          timDataInizio;
} ASRV_DbRichiesta;

/* CONNESSIONE */ 
typedef struct {
    JADA_ConnId         iId;
    Identifier          szHostIP;
    Identifier          szRealIP;
    int                 iStato;
    int                 iUsaAudio;
    int                 iWhoAmI;
    JADA_AgtId          iIdAgent;
    JADA_MgrId          iIdManager;
} ASRV_DbConnessione;

typedef struct {
    ASRV_DbConnessione  dbConn;
    ASRV_Socket         sokConn;
    JADA_SessId         iIdSession;
    JADA_Timet          timLastActivity;
//    CORE_List           lstParameters;
    int                 iMagic;
} ASRV_Connection;

/* SESSIONE */ 
typedef struct {
    JADA_SessId         iId;
    JADA_ConnId         iIdConnAgent;
    JADA_ConnId         iIdConnManager;
    JADA_ReqId          iIdRichiesta;
    int                 iStato;
} ASRV_DbSessione;

typedef struct {
    ASRV_DbSessione     dbSess;
    ASRV_DbRichiesta    dbReq;
    JADA_AgtId          iIdAgent;
    JADA_MgrId          iIdManager;
    CORE_List           lstInMessages;
    CORE_List           lstParameters;
    JADA_Timet          timLastActivity;
    pthread_t           pThreadId;
    int                 iExitThread;
    int                 iType;
    int                 iMagic;
} ASRV_Session;

/* CHAT */ 
typedef struct {
    JADA_ReqId          iIdRichiesta;
    int                 iMittente;
    LongString          szTesto;
    JADA_Timet          timDataInvio;
    JADA_Timet          timDataRicezione;
} ASRV_DbChat;

/* FTP */ 
//typedef struct {
//    JADA_SessId         iId;
//    int                 iDirezione;
//    FixString           szDaFile;
//    FixString           szAFile;
//    JADA_Timet          timDataInizio;
//    JADA_Timet          timDataFine;
//} ASRV_DbFtp;

/* CALL */ 
//typedef struct {
//    JADA_SessId         iId;
//    int                 iUsaAudio;
//    int                 iUsaVideo;
//    JADA_Timet          timDataInizio;
//    JADA_Timet          timDataFine;
//} ASRV_DbCall;

/* VNC */ 
//typedef struct {
//    JADA_SessId         iId;
//    FixString           szCartellaRegistrazione;
//    int                 iStatoRegistrazione;
//    JADA_Timet          timDataInizio;
//    JADA_Timet          timDataFine;
//} ASRV_DbVnc;

/* CODA */ 
//typedef struct {
//    JADA_QueueId        iId;
//    Identifier          szNome;
//    JADA_QueueId        iIdPadre;
//} ASRV_DbCoda;

/******************************************************************************/
/*                                                                            */
/*   V a r i a b l e s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
extern CORE_List    lstConnections;
extern CORE_List    lstSessions;
extern FixString    JADA_TmpFile;
extern ShortString  JADA_WhoAmI;

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
int ASRV_DbIsEnabledCliente(char *szCliente, char *szRegione);
int ASRV_DbCreateAgent(ASRV_DbAgent *mySelf);
int ASRV_DbGetAgent(ASRV_DbAgent *mySelf);
int ASRV_DbGetAgentByConnection(ASRV_DbAgent *mySelf, JADA_ConnId iConnId);
int ASRV_DbCheckManager(ASRV_DbManager *mySelf, char *szUsername, char *szPassword, int *piResult);
int ASRV_DbSetManager(ASRV_DbManager *mySelf);
int ASRV_DbGetManager(ASRV_DbManager *mySelf);
int ASRV_DbGetManagerByConnection(ASRV_DbManager *mySelf, JADA_ConnId iConnId);
int ASRV_DbCreateConnessione(ASRV_DbConnessione *mySelf);
int ASRV_DbSetConnessione(ASRV_DbConnessione *mySelf);
int ASRV_DbDestroyConnessione(ASRV_DbConnessione *mySelf);
int ASRV_DbSetParametro(ASRV_DbConnessione *mySelf, char *szSection, char *szName, char *szValue);
CORE_List ASRV_DbListParametri(ASRV_DbConnessione *mySelf, CORE_List lstSelf, char *szFilter);
int ASRV_DbCreateRichiesta(ASRV_DbRichiesta *mySelf);
int ASRV_DbSetRichiesta(ASRV_DbRichiesta *mySelf);
int ASRV_DbGetRichiesta(ASRV_DbRichiesta *mySelf);
int ASRV_DbSetStatusRichiesta(ASRV_DbRichiesta *mySelf, int iStato);
int ASRV_DbDestroyRichiesta(ASRV_DbRichiesta *mySelf);
CORE_List ASRV_DbListRichiesteAperte(char *szCoda);
//CORE_List ASRV_DbListRichiestePending(char *szCoda);
CORE_List ASRV_DbCountRichiesteAperte(JADA_MgrId *piIdManager);
//int ASRV_DbGetCountRichieste(JADA_MgrId *piIdManager, int iStato, int *piCount);
int ASRV_DbCreateSessione(ASRV_DbSessione *mySelf);
int ASRV_DbSetSessione(ASRV_DbSessione *mySelf);
int ASRV_DbSetStatusSessione(ASRV_DbSessione *mySelf, int iStato);
int ASRV_DbSetAgentSessione(ASRV_DbSessione *mySelf, JADA_ConnId *piConnId);
int ASRV_DbSetManagerSessione(ASRV_DbSessione *mySelf, JADA_ConnId *piConnId);
int ASRV_DbSetRichiestaIdSessione(ASRV_DbSessione *mySelf);
int ASRV_DbDestroySessione(ASRV_DbSessione *mySelf);
int ASRV_DbGetSessione(ASRV_DbSessione *mySelf);
int ASRV_DbSendChat(JADA_ReqId iIdRichiesta, int iMittente, char *szText);
int ASRV_DbCacheChat(JADA_ReqId iIdRichiesta, int iMittente, char *szText);
CORE_List ASRV_DbListCachedChat(JADA_ReqId iIdRichiesta);
int ASRV_DbCachedChatSent(JADA_ReqId iIdRichiesta);
int ASRV_DbStartVnc(JADA_SessId iIdSessione);
int ASRV_DbStopVnc(JADA_SessId iIdSessione);
CORE_List ASRV_DbGetComboRegione(char *szFilter);
CORE_List ASRV_DbGetComboCliente(char *szFilter);
CORE_List ASRV_DbGetComboCoda(char *szFilter);
CORE_List ASRV_DbGetComboManagerCoda(char *szFilter);
int ASRV_DbGetManagerDefaultCoda(JADA_MgrId iIdManager, char *szCoda, int iLength); 

/* callback.c */
CORE_Callback ASRV_MnuListConnections(void);
CORE_Callback ASRV_MnuListSessions(void);
CORE_Callback ASRV_MnuListRequests(void);
CORE_Callback ASRV_MnuListPendingRequests(void);
CORE_Callback ASRV_MnuListAgents(void);
CORE_Callback ASRV_MnuListManagers(void);

/*- session.c */
int ASRV_HandleConnectionMessage(ASRV_Connection *connPtr, JADA_Message msgIn, int *piCloseMe);
int ASRV_HandleSupportMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgIn);
int ASRV_HandleFtpMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgIn);
int ASRV_HandleMediaMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgIn);
int ASRV_HandleVncMessage(ASRV_Session *sesPtr, JADA_ConnId iIdConnSource, JADA_Message msgIn);

/* objects.c */
//int ASRV_InitIds(void);
//int ASRV_InitProcessIds(void);
int ASRV_GetPassword_DaButtare(int iIndex, char *szPassword);
int ASRV_GetVncPassword_DaButtare(int iIndex, char *szPassword);
//int ASRV_GetParameter(CORE_List lstParameters, char *szSection, char *szName, char *szValue, int iLength);
//int ASRV_GetParameterInt(CORE_List lstParameters, char *szSection, char *szName, int *piValue);
//int ASRV_SetParameter(CORE_List lstParameters, char *szSection, char *szName, char *szValue);
//int ASRV_SetParameterInt(CORE_List lstParameters, char *szSection, char *szName, int iValue);
//int ASRV_CopyParameter(CORE_List lstFrom, CORE_List lstTo, char *szSection, char *szName);
ASRV_Connection *ASRV_CreateConnection(char *szFile, int iLine, char *szIP, int iSocket);
int ASRV_DestroyConnection(ASRV_Connection *connPtr);
int ASRV_RealDestroyConnection(char *szFile, int iLine, ASRV_Connection *connPtr);
ASRV_Connection *ASRV_GetConnectionById(JADA_ConnId iConnectionId);
ASRV_Connection *ASRV_GetConnectionByAgentId(JADA_AgtId iAgentId);
ASRV_Session *ASRV_CreateSession(char *szFile, int iLine, ASRV_Connection *connPtr, int iSessionType, JADA_ReqId iRequestId);
int ASRV_DestroySession(ASRV_Session *sesPtr);
int ASRV_RealDestroySession(char *szFile, int iLine, ASRV_Session *sesPtr);
int ASRV_StartSession(ASRV_Session *sesPtr);
ASRV_Session *ASRV_GetSessionById(JADA_SessId iSessionId);
ASRV_Session *ASRV_GetSessionByRequestId(JADA_ReqId iRequestId);
int ASRV_SetSessionAgent(ASRV_Session *ptrSess, ASRV_Connection *ptrConn);
int ASRV_SetSessionManager(ASRV_Session *ptrSess, ASRV_Connection *ptrConn);
int ASRV_SendCachedChat(ASRV_Session *ptrSess);

/* tcp.c */
int ASRV_TcpCreateListenPort(char *szHostName, int iPort);
int ASRV_TcpCreateSocket(ASRV_Socket *sokSelf, int iSocket);
int ASRV_TcpDestroySocket(ASRV_Socket *sokSelf);
int ASRV_WaitConnections(void);
int ASRV_TcpSendMessage(ASRV_Socket *sokSelf, JADA_Message msgOut);
JADA_Message ASRV_TcpReceiveMessage(ASRV_Socket *sokSelf);
int ASRV_TcpSendRequestMessage(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iSize);
int ASRV_TcpSendSessionMessageToAgent(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iSize);
int ASRV_TcpSendSessionMessageToManager(ASRV_Session *sesPtr, int iOpcode, void *pPayload, int iSize);
int ASRV_TcpSendSessionMessageToPartner(ASRV_Session *sesPtr, JADA_ConnId iSourceId, int iOpcode, void *pPayload, int iLength);
int ASRV_TcpSendConnectionMessage(ASRV_Connection *connPtr, int iOpcode, void *pPayload, int iLength);
//int __stdcall ASRV_TcpSendRequestMessageToAgent(ASRV_Request *reqPtr, int iOpcode, void *pPayload, int iLength);
//int __stdcall ASRV_TcpSendRequestMessageToManager(ASRV_Request *reqPtr, int iOpcode, void *pPayload, int iLength);
int ASRV_TcpForwardSessionMessageToPartner(ASRV_Session *sesPtr, JADA_ConnId iSourceId, JADA_Message msgOut);
int ASRV_TcpForwardMessage(ASRV_Connection *connPtr, JADA_Message msgOut);

/* threads.c */
void *ASRV_ConnectionThread(void *FuncArg);
void *ASRV_SessionThread(void *FuncArg);

/* handlers.c */
int ASRV_ForwardMessage(JADA_Message msgIn, JADA_ConnId iFromConnectionId, JADA_SessId iSessionId); 
int ASRV_HandleConnectConnection(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe);
int ASRV_HandleOpenAgentConnection(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe);
int ASRV_HandleOpenManagerConnection(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe);
int ASRV_HandleOpenSession(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe);
int ASRV_HandleLoadSession(ASRV_Connection *connPtr, JADA_Message msgRecv, int *piCloseMe);
int ASRV_HandleGetListCounters(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleListConnectionParameters(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleConnectionParametersList(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleConnectionGetCombo(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleListRequests(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleListPendingRequests(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleGetCombo(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleOpenSupport(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleAcceptSupport(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleLoadSupport(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleConnectSupport(ASRV_Connection *connPtr, JADA_Message msgRecv);
int ASRV_HandleResumeSupport(ASRV_Connection *connPtr, JADA_Message msgRecv);

//int ASRV_HandleSetSystemInfo(ASRV_Session *sesPtr, JADA_Message msgRecv);
//int ASRV_HandleGetSystemInfo(ASRV_Session *sesPtr, JADA_Message msgRecv);
int ASRV_HandleSolicitSupport(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCloseSupport(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleListParameters(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleParametersList(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleListDirectory(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleDirectoryList(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleBroadcastListCounters(void);
int ASRV_HandleSendChatText(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandlePing(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandlePingReply(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleOpenCall(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCallOpened(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCloseCall(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleAbortCall(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleOpenDesktop(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleDesktopOpened(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCloseDesktop(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleDesktopClosed(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleOpenWhiteboard(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleWhiteboardOpened(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCleanWhiteboard(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCloseWhiteboard(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleWhiteboardClosed(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleSetDrawPointer(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleResetDrawPointer(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
/* FTP */
int ASRV_HandleOpenFtp(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleFtpOpened(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleCloseFtp(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleListDirectory(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleDirectoryList(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);
int ASRV_HandleGetFile(ASRV_Session *sesPtr, JADA_ConnId iIdSource, JADA_Message msgRecv);

/* log */
void LogMessageToFile(char *szFileName, char *szMessage);
void LogConnectionMessage(JADA_ConnId iId, char *szFormat, ...);
void LogSessionMessage(JADA_SessId iId, char *szFormat, ...);
void LogRequestMessage(JADA_ReqId iId, char *szFormat, ...);
int LogGetSessionPartners(ASRV_Session *sesPtr, ASRV_DbAgent *myAgent, ASRV_DbManager *myManager);
void ASRV_LogConnectConnection(ASRV_Connection *connPtr);
void ASRV_LogOpenAgentConnection(ASRV_Connection *connPtr, ASRV_DbAgent *agtPtr);
void ASRV_LogOpenManagerConnection(ASRV_Connection *connPtr, ASRV_DbManager *mgrPtr);
void ASRV_LogOpenConnectionError(JADA_ConnId iId, char *szUtente, char *szErrorMsg);
void ASRV_LogOpenSession(ASRV_Session *sesPtr);
void ASRV_LogOpenSessionError(JADA_SessId iId, char *szUtente, char *szErrorMsg);
void ASRV_LogSetSystemInfo(ASRV_Session *sesPtr);
void ASRV_LogGetSystemInfo(ASRV_Session *sesPtr);
void ASRV_LogOpenSupport(ASRV_Session *sesPtr, ASRV_DbRichiesta *reqPtr);
void ASRV_LogSolicitSupport(ASRV_Session *sesPtr, JADA_ReqId iId);
void ASRV_LogCloseSupport(ASRV_Session *sesPtr, JADA_ReqId iId);
void ASRV_LogAcceptSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogAcceptSupportError(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr, char *szErrorMsg);
void ASRV_LogLoadSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogLoadSupportError(ASRV_Connection *connPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr, char *szErrorMsg);
void ASRV_LogResumeSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogResumeSupportError(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr, char *szErrorMsg);
void ASRV_LogConnectSupport(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogConnectSupportError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogOpenCall(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogOpenCallError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogCallOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogCallOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogCloseCall(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogCloseCallError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogAbortCall(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogAbortCallError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogOpenWhiteboard(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogOpenWhiteboardError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogWhiteboardOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogWhiteboardOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogCleanWhiteboard(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogCleanWhiteboardError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogCloseWhiteboard(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogCloseWhiteboardError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogWhiteboardClosed(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogWhiteboardClosedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogOpenDesktop(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogOpenDesktopError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogDesktopOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogDesktopOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogCloseDesktop(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogCloseDesktopError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogDesktopClosed(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogDesktopClosedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogSetDrawPointer(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogSetDrawPointerError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogResetDrawPointer(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogResetDrawPointerError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogOpenFtp(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogOpenFtpError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogFtpOpened(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogFtpOpenedError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogCloseFtp(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogCloseFtpError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogListDirectory(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogListDirectoryError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogDirectoryList(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogDirectoryListError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);
void ASRV_LogGetFile(ASRV_Session *sesPtr, JADA_ReqId iId, ASRV_DbRichiesta *reqPtr);
void ASRV_LogGetFileError(ASRV_Session *sesPtr, JADA_ReqId iId, char *szErrorMsg);

#undef in
#undef out

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_SERVER_H_ */
