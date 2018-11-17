#ifndef _JADA_MESSAGES_H_
#define _JADA_MESSAGES_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef SERVER
#include <sys/timeb.h>
#include <time.h>
#endif


/******************************************************************************/
/*                                                                            */
/*   M a c r o s     D e f i n i t i o n                                      */
/*                                                                            */
/******************************************************************************/
#define JADA_IsFrameMsg(msgHeader)                       \
        (msgHeader.iType == JADA_MSGTYPE_AUDIOFRAME) || (msgHeader.iType == JADA_MSGTYPE_VIDEOFRAME)

/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
/* Tipologie di messaggio (ordinati per priorita' di invio) */
#define JADA_MSGTYPE_CONNECTION         1001
//#define JADA_MSGTYPE_REQUEST            2002
#define JADA_MSGTYPE_SUPPORT            3003
#define JADA_MSGTYPE_AUDIOFRAME         4004
#define JADA_MSGTYPE_VIDEOFRAME         5005
#define JADA_MSGTYPE_DESKTOPFRAME       6006
#define JADA_MSGTYPE_DRAWFRAME          7007
#define JADA_MSGTYPE_FTPFRAME           8008

/* Alcune dimensioni per il payload di messaggio */
#define JADA_CHATTEXT_SIZE              1024
#define JADA_DESKTOP_FRAME_LENGTH       8192
#define JADA_DRAW_FRAME_LENGTH          16384
#define JADA_FTP_FRAME_LENGTH           16384

/* Opcode dei messaggi di protocollo: CONNESSIONE */
#define JADA_MSG_CONNECT_CONNECTION     11
#define JADA_MSG_CONNECTION_CONNECTED   12
#define JADA_MSG_OPEN_CONNECTION        13
#define JADA_MSG_CONNECTION_OPENED      14
#define JADA_MSG_LIST_REQUESTS          21
#define JADA_MSG_REQUESTS_LIST          22
#define JADA_MSG_LIST_PENDING_REQUESTS  23
#define JADA_MSG_PENDING_REQUESTS_LIST  24
#define JADA_MSG_GET_LIST_COUNTERS      25
#define JADA_MSG_LIST_COUNTERS          26
#define JADA_MSG_CONN_PARAMETERS_LIST   28
#define JADA_MSG_GET_COMBO              29
#define JADA_MSG_COMBO_VALUES           30

/* Opcode dei messaggi di protocollo: tutte le sessioni */
#define JADA_MSG_PING                   91
#define JADA_MSG_PING_REPLY             92
#define JADA_MSG_RESULT                 93

/* Opcode dei messaggi di protocollo: SUPPORT */
#define JADA_MSG_SEND_CHAT_TEXT         101
#define JADA_MSG_OPEN_SUPPORT           102
#define JADA_MSG_SUPPORT_OPENED         103
#define JADA_MSG_ACCEPT_SUPPORT         104
#define JADA_MSG_SUPPORT_ACCEPTED       105
#define JADA_MSG_LOAD_SUPPORT           106
#define JADA_MSG_SUPPORT_LOADED         107
#define JADA_MSG_CONNECT_SUPPORT        108
#define JADA_MSG_SUPPORT_CONNECTED      109
#define JADA_MSG_SOLICIT_SUPPORT        110
#define JADA_MSG_SUPPORT_SOLICITED      111
#define JADA_MSG_CLOSE_SUPPORT          112
#define JADA_MSG_SUPPORT_CLOSED         113
#define JADA_MSG_RESUME_SUPPORT         114
#define JADA_MSG_SUPPORT_RESUMED        115
#define JADA_MSG_SUPPORT_SUSPENDED      116
#define JADA_MSG_LIST_PARAMETERS        119
#define JADA_MSG_PARAMETERS_LIST        120

/* Opcode dei messaggi di protocollo: MEDIA */
#define JADA_MSG_OPEN_CALL              201
#define JADA_MSG_CALL_OPENED            202
#define JADA_MSG_CLOSE_CALL             203
#define JADA_MSG_ABORT_CALL             204

/* Opcode dei messaggi di protocollo: VNC */
#define JADA_MSG_OPEN_DESKTOP           301
#define JADA_MSG_DESKTOP_OPENED         302
#define JADA_MSG_OPEN_WHITEBOARD        303
#define JADA_MSG_WHITEBOARD_OPENED      304
#define JADA_MSG_CLEAN_WHITEBOARD       305
#define JADA_MSG_CLOSE_WHITEBOARD       306
#define JADA_MSG_WHITEBOARD_CLOSED      307
#define JADA_MSG_SET_DRAW_POINTER       308
#define JADA_MSG_RESET_DRAW_POINTER     309
#define JADA_MSG_CLOSE_DESKTOP          310
#define JADA_MSG_DESKTOP_CLOSED         311

/* Opcode dei messaggi di protocollo: FTP */
#define JADA_MSG_OPEN_FTP               401
#define JADA_MSG_FTP_OPENED             402
#define JADA_MSG_LIST_DIRECTORY         403
#define JADA_MSG_DIRECTORY_LIST         404
#define JADA_MSG_GET_FILE               405
#define JADA_MSG_CLOSE_FTP              406

/* Dimensioni di alcuni messaggi */
#define JADA_MAX_REQUESTS_IN_LIST       48
#define JADA_MAX_PARAMETERS_IN_LIST     64
#define JADA_MAX_FILES_IN_LIST          64
#define JADA_MAX_COMBO_VALUES_IN_LIST   64

/* Risposte sulla apertura di sessione */
#define JADA_CREDENTIAL_OK              121
#define JADA_CREDENTIAL_UNKNOWN_USER    232
#define JADA_CREDENTIAL_WRONG_PASSWORD  343

/* Tipologie di accodamento dei messaggi */
#define JADA_MSG_QUEUE_CALL             10
#define JADA_MSG_QUEUE_DESKTOP          11
#define JADA_MSG_QUEUE_FTP              12
#define JADA_MSG_QUEUE_SESSION          13

/******************************************************************************
 *
 *   M e s s a g e s     D e f i n i t i o n
 *
 ******************************************************************************/
typedef struct {
    int             iCounter;
    int             iCreationSecs;
    int             iCreationMsecs;
    int             iType;
    JADA_MsgId      iMsgId;
    int             iLength;
    int             iZippedLength;
} JADA_MsgHeader;

typedef struct {
    JADA_SessId     iId;
    int             iWhoAmI;
    int             iOpcode;
} JADA_SessionMsgHeader;

typedef struct {
    int             iType;
    JADA_MsgId      iMsgId;
} JADA_MsgFooter;

typedef struct {
    JADA_MsgHeader          szHeader;
    JADA_SessionMsgHeader   szSesHeader;
    int                     iZipped;
    unsigned char           *szPayload;
    int                     iMagic;
} JADA_MsgSelf;

/******************************************************************************
 *
 *   P r o t o c o l     M e s s a g e s
 *
 ******************************************************************************/
/******************************************************************************
 *
 *      C O N N E C T I O N
 *
 ******************************************************************************/
typedef struct {
    int             iConnectionType;
    Identifier      szHostIP;
} JADA_MsgConnectConnection;

typedef struct {
    int             iResult;
    JADA_ConnId     iConnectionId;
    int             iConnectionType;
    JADA_Timet      timOpenTime;
} JADA_MsgConnectionConnected;

typedef struct {
    int             iConnectionType;
    Identifier      szNome;
    Identifier      szCliente;
    Identifier      szRegione;
    Identifier      szMail;
    Identifier      szTelefono;
    Identifier      szCellulare;
    Identifier      szHostIP;
    int             iUsaAudio;
    /* Specifici del manager */
    Identifier      szUsername;
    Identifier      szPassword;
} JADA_MsgOpenConnection;

typedef struct {
    int             iResult;
    JADA_ConnId     iConnectionId;
    int             iConnectionType;
    Identifier      szCoda;
    JADA_Timet      timOpenTime;
} JADA_MsgConnectionOpened;

typedef struct {
    int             iWhoAmi;
    int             iSessionType;
} JADA_MsgOpenSession;

typedef struct {
    int             iResult;
    JADA_SessId     iSessionId;
    int             iSessionType;
    JADA_Timet      timOpenTime;
} JADA_MsgSessionOpened;

typedef struct {
    int             iWhoAmi;
    int             iSessionId;
    int             iSessionType;
} JADA_MsgLoadSession;

typedef struct {
    int             iResult;
    JADA_SessId     iSessionId;
    int             iSessionType;
    JADA_Timet      timLoadTime;
} JADA_MsgSessionLoaded;

typedef struct {
    int             iWhoAmi;
    int             iSessionId;
    int             iSessionType;
} JADA_MsgConnectSession;

typedef struct {
    int             iResult;
    JADA_SessId     iSessionId;
    int             iSessionType;
    JADA_Timet      timConnectTime;
} JADA_MsgSessionConnected;

/******************************************************************************
 *
 *      S U P P O R T
 *
 ******************************************************************************/
typedef struct {
    Identifier      szCoda;
    LongString      szDescription;
} JADA_MsgOpenRequest;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    Identifier      szPassword_DaButtare;
    JADA_Timet      timOpenTime;
} JADA_MsgRequestOpened;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgAcceptRequest;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    Identifier      szPassword_DaButtare;
    Identifier      szCliente;
    Identifier      szUtente;
    Identifier      szTelefono;
    Identifier      szCellulare;
    Identifier      szMail;
    int             iUsaAudio;
    Identifier      szCoda;
    LongString      szDescription;
    JADA_Timet      timAcceptTime;
} JADA_MsgRequestAccepted;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgLoadRequest;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    FixString       szPassword_DaButtare;
    Identifier      szCliente;
    Identifier      szUtente;
    Identifier      szTelefono;
    Identifier      szCellulare;
    Identifier      szMail;
    int             iUsaAudio;
    Identifier      szCoda;
    LongString      szDescription;
    JADA_Timet      timLoadTime;
} JADA_MsgRequestLoaded;

typedef struct {
    JADA_ReqId      iRequestId;
    Identifier      szUtente;
    JADA_Timet      timSuspendTime;
} JADA_MsgRequestSuspended;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgResumeRequest;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    int             iStatus;
    Identifier      szPassword_DaButtare;
    Identifier      szCliente;
    Identifier      szUtente;
    Identifier      szTelefono;
    Identifier      szCellulare;
    Identifier      szMail;
    int             iUsaAudio;
    Identifier      szCoda;
    LongString      szDescription;
    JADA_Timet      timResumeTime;
} JADA_MsgRequestResumed;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgConnectRequest;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    Identifier      szPassword_DaButtare;
    Identifier      szCliente;
    Identifier      szUtente;
    Identifier      szTelefono;
    Identifier      szCellulare;
    Identifier      szMail;
    int             iUsaAudio;
    Identifier      szCoda;
    LongString      szDescription;
    JADA_Timet      timConnectTime;
} JADA_MsgRequestConnected;

typedef struct {
    Identifier      szCoda;
} JADA_MsgListRequests;

typedef struct {
    int         iRequests;
    struct {
        JADA_ReqId      iRequestId;
        int             iStatus;
        Identifier      szCliente;
        Identifier      szUtente;
        Identifier      szTelefono;
        Identifier      szCellulare;
        Identifier      szMail;
        FixString      szDescrizione;
    } lstRequests[JADA_MAX_REQUESTS_IN_LIST];
} JADA_MsgRequestList;

typedef struct {
    int             iAreaRequests;
    int             iTotalRequests;
    int             iAreaPendingRequests;
    int             iTotalPendingRequests;
} JADA_MsgListCounters;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgListParameters;

typedef struct {
    int         iParameters;
    struct {
        Identifier      szSection;
        Identifier      szName;
        ShortString     szValue;
    } lstParameters[JADA_MAX_PARAMETERS_IN_LIST];
} JADA_MsgParametersList;

typedef struct {
    int             iComboType;
    ShortString     szFilter;
} JADA_MsgGetCombo;

typedef struct {
    int             iValues;
    ShortString     lstValues[JADA_MAX_COMBO_VALUES_IN_LIST];
} JADA_MsgComboValues;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgSolicitRequest;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timSolicitTime;
} JADA_MsgRequestSolicited;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgCloseRequest;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timCloseTime;
} JADA_MsgRequestClosed;

typedef struct {
    JADA_ReqId      iRequestId;
    int             iWhoAmI;
    char            szText[JADA_CHATTEXT_SIZE];
} JADA_MsgChatText;

//typedef struct {
//    int             iType;
//    JADA_ReqId      iRequestId;
//    JADA_Timet      timRequestTime;
//} JADA_MsgRequestService;
//
//typedef struct {
//    int             iType;
//    JADA_ReqId      iRequestId;
//    int             iAccepted;
//    JADA_Timet      timAcceptTime;
//} JADA_MsgAcceptService;

typedef struct {
    int             iResult;
    LongString      szDescription;
} JADA_MsgResult;

#ifndef SERVER
typedef struct {
    JADA_ReqId      iRequestId;
    struct _timeb   timPingTime;
    struct _timeb   timReplyTime;
} JADA_MsgPing;
#endif

/******************************************************************************
 *
 *      M E D I A
 *
 ******************************************************************************/
typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timOpenTime;
} JADA_MsgOpenCall;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_Timet      timOpenTime;
} JADA_MsgCallOpened;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timCloseTime;
} JADA_MsgCloseCall;

typedef struct {
    JADA_ReqId      iRequestId;
    int             iRetries;
    JADA_Timet      timAbortTime;
} JADA_MsgAbortCall;

/******************************************************************************
 *
 *      V N C
 *
 ******************************************************************************/
typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timOpenTime;
} JADA_MsgOpenDesktop;

typedef struct {
    JADA_ReqId      iRequestId;
    int             iAccepted;
    JADA_Timet      timOpenTime;
} JADA_MsgDesktopOpened;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timOpenTime;
} JADA_MsgOpenWhiteboard;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_Timet      timOpenTime;
} JADA_MsgWhiteboardOpened;

typedef struct {
    JADA_ReqId      iRequestId;
} JADA_MsgCleanWhiteboard;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timCloseTime;
} JADA_MsgCloseWhiteboard;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_Timet      timCloseTime;
} JADA_MsgWhiteboardClosed;

typedef struct {
    JADA_ReqId      iRequestId;
    int             iPointerType;
    int             iPointX;
    int             iPointY;
} JADA_MsgSetDrawPointer;

typedef struct {
    JADA_ReqId      iRequestId;
    JADA_Timet      timCloseTime;
} JADA_MsgCloseDesktop;

/******************************************************************************
 *
 *      F T P
 *
 ******************************************************************************/
typedef struct {
    int             iFTPId;
} JADA_MsgOpenFTP;

typedef struct {
    int             iResult;
    JADA_ReqId      iRequestId;
    JADA_SessId     iSessionId;
    JADA_Timet      timOpenTime;
} JADA_MsgFTPOpened;

typedef struct {
    JADA_SessId     iSessionId;
    LongString      szDirName;
} JADA_MsgListDirectory;

typedef struct {
    int         iFiles;
    struct {
        FixString       szFileName;
    } lstFiles[JADA_MAX_FILES_IN_LIST];
} JADA_MsgDirectoryList;

typedef struct {
    JADA_SessId     iSessionId;
    FixString       szFileFrom;
    FixString       szFileTo;
} JADA_MsgFileTransfer;

typedef struct {
    FixString       szFileName;
    short           iCurrFrame;
    short           iFrames;
    int             iLength;
} JADA_MsgFtpInfo;

typedef struct {
    JADA_MsgFtpInfo ftpInfo;
    unsigned char   szPayload[JADA_FTP_FRAME_LENGTH];
} JADA_MsgFtpFrame;


/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/
#ifdef ANSI_C

#define in
#define out

/* Generic Messages */
DllExport JADA_Message __stdcall JADA_MsgNew(char *szFile, int iLine, int iType, JADA_MsgId iId, int iLength);
DllExport int __stdcall JADA_MsgDestroy(char *szFile, int iLine, JADA_Message msgSelf);
DllExport int __stdcall JADA_MsgGetHeader(JADA_Message msgSelf, JADA_MsgHeader *szHeader);
DllExport int __stdcall JADA_MsgSetPayload(JADA_Message msgSelf, void *szPayload, int iLength);
DllExport int __stdcall JADA_MsgGetPayload(JADA_Message msgSelf, void *szPayload, int iLength);
DllExport int __stdcall JADA_MsgGetTimeDelay(JADA_Message msgSelf, int *piTimeDelay);
DllExport int __stdcall JADA_MsgSetCounter(JADA_Message msgSelf, int iCounter);
DllExport int __stdcall JADA_MsgGetCounter(JADA_Message msgSelf, int *piCounter);
DllExport int __stdcall JADA_MsgSerialize(JADA_Message msgSelf, void *szBuffer, int iLength, int *piOutLength);
DllExport JADA_Message __stdcall JADA_MsgDeserialize(void *szBuffer, int iLength);

/* Session Messages */
DllExport JADA_Message __stdcall JADA_MsgConnNew(char *szFile, int iLine, int iWhoAmI, JADA_ConnId iConnectionId, int iOpcode, int iLength);
DllExport JADA_Message __stdcall JADA_MsgSesNew(char *szFile, int iLine, int iWhoAmI, JADA_SessId iSessionId, int iOpcode, int iLength);
DllExport int __stdcall JADA_MsgSesSetPayload(JADA_Message msgSelf, void *szPayload, int iLength);
DllExport int __stdcall JADA_MsgSesGetPayload(JADA_Message msgSelf, void *szPayload, int iLength);
DllExport int __stdcall JADA_MsgSesGetHeader(JADA_Message msgSelf, JADA_SessionMsgHeader *szHeader);

/* Utilities */
DllExport char * __stdcall JADA_MsgTypeToString(int iType);
DllExport char * __stdcall JADA_MsgOpcodeToString(int iOpcode);

#undef in
#undef out

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_JADA_MESSAGES_H_ */
