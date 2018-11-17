#ifndef _EASS_H_
#define _EASS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ON_WIN32                    1
#define ON_WINDOWS                  1
#define ANSI_C                      1

/******************************************************************************/
/*                                                                            */
/*   G e n e r a l     I n c l u d e     F i l e s                            */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#ifdef ON_WINDOWS
#include "oss/pthread.h"
#ifndef NO_WINDOWS_H
#include <windows.h>
#endif
#else
#ifdef solaris
#include <sys/types.h>
#endif
#include <pthread.h>
#endif

#include "eass_common.h"


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
#define EASS_MAX_SESSIONS            8
#define EASS_MAX_REQUESTS            8

#define EASS_ERR_TCP_SELECT         -1001
#define EASS_ERR_TCP_ACCEPT         -1002
#define EASS_ERR_TCP_RECV           -1003
#define EASS_ERR_TCP_SEND           -1004
#define EASS_ERR_WRONG_FRAMETYPE    -1005
#define EASS_ERR_WRONG_FRAMELENGTH  -1006

#define EASS_WHOAMI_AGENT           11111
#define EASS_WHOAMI_MANAGER         33333
#define EASS_WHOAMI_SERVER          55555

#define EASS_AGENT_PORT             10080
#define EASS_MANAGER_PORT           10081
#define EASS_SERVER_PORT            45678
#define EASS_REMOTE_DESKTOP_PORT    40000
#define EASS_AUDIOVIDEO_PORT        41000
#define EASS_VNCSERVER_PORT         5900

/******************************************************************************/
/*                                                                            */
/*   M  e  s  s  a  g  e  s                                                   */
/*                                                                            */
/******************************************************************************/
#define EASS_MSG_PAYLOAD_SIZE           2040

#define EASS_MSG_OPEN_SESSION           1
#define EASS_MSG_SESSION_OPENED         2
#define EASS_MSG_RESULT                 99
#define EASS_MSG_OPEN_REQUEST           101
#define EASS_MSG_REQUEST_OPENED         102
#define EASS_MSG_ACCEPT_REQUEST         103
#define EASS_MSG_REQUEST_ACCEPTED       104
#define EASS_MSG_LOAD_REQUEST           105
#define EASS_MSG_REQUEST_LOADED         106
#define EASS_MSG_CONNECT_REQUEST        107
#define EASS_MSG_REQUEST_CONNECTED      108
#define EASS_MSG_LIST_REQUESTS          109
#define EASS_MSG_REQUESTS_LIST          110
#define EASS_MSG_LIST_PENDING_REQUESTS  111
#define EASS_MSG_PENDING_REQUESTS_LIST  112
#define EASS_MSG_CLOSE_REQUEST          113
#define EASS_MSG_REQUEST_CLOSED         114
#define EASS_MSG_SEND_CHAT_TEXT         142
#define EASS_MSG_OPEN_AUDIOCALL         201
#define EASS_MSG_AUDIOCALL_OPENED       202
#define EASS_MSG_OPEN_AUDIOVIDEOCALL    203
#define EASS_MSG_AUDIOVIDEOCALL_OPENED  204
#define EASS_MSG_OPEN_REMOTEDESKTOP     205
#define EASS_MSG_REMOTEDESKTOP_OPENED   206

#define EASS_FRAMETYPE_AUDIO            1001
#define EASS_FRAMETYPE_VIDEO            2002
#define EASS_FRAME_LENGTH               16384

/******************************************************************************/
/*                                                                            */
/*   T y p e s     D e f i n i t i o n                                        */
/*                                                                            */
/******************************************************************************/
typedef struct {
    int             iId;
    int             iWhoAmI;
    int             iOpcode;
    unsigned char   szPayload[EASS_MSG_PAYLOAD_SIZE];
} EASS_Message;

typedef struct {
    CoreId          szHostName;
    CoreId          szHostIP;
    CoreId          szCliente;
    CoreId          szUtente;
    CoreId          szTelefono;
} EASS_MsgOpenSession;

typedef struct {
    int             iResult;
    int             iSessionId;
    time_t          timOpenTime;
} EASS_MsgSessionOpened;

typedef struct {
    CoreId          szArea;
    CoreId          szDescription;
} EASS_MsgOpenRequest;

typedef struct {
    int             iResult;
    int             iRequestId;
    time_t          timOpenTime;
} EASS_MsgRequestOpened;

typedef struct {
    int             iRequestId;
} EASS_MsgAcceptRequest;

typedef struct {
    int             iResult;
    int             iRequestId;
    time_t          timAcceptTime;
    CoreId          szCliente;
    CoreId          szUtente;
    CoreId          szTelefono;
} EASS_MsgRequestAccepted;

typedef struct {
    int             iPendingRequestId;
} EASS_MsgLoadRequest;

typedef struct {
    int             iResult;
    int             iRequestId;
    time_t          timOpenTime;
    CoreId          szCliente;
    CoreId          szUtente;
    CoreId          szTelefono;
} EASS_MsgRequestLoaded;

typedef struct {
    int             iRequestId;
} EASS_MsgConnectRequest;

typedef struct {
    int             iResult;
    int             iRequestId;
    time_t          timConnectTime;
    CoreId          szCliente;
    CoreId          szUtente;
    CoreId          szTelefono;
} EASS_MsgRequestConnected;

typedef struct {
    struct {
        int         iId;
        int         iStatus;
        CoreId      szUtente;
        CoreId      szTelefono;
        CoreId      szCliente;
    } lstRequests[EASS_MAX_REQUESTS];
} EASS_MsgRequestList;

typedef struct {
    int             iRequestId;
} EASS_MsgCloseRequest;

typedef struct {
    int             iRequestId;
    time_t          timCloseTime;
} EASS_MsgRequestClosed;

typedef struct {
    char            szChatText[EASS_MSG_PAYLOAD_SIZE];
} EASS_MsgChatText;

typedef struct {
    int             iResult;
    char            szDescription[EASS_MSG_PAYLOAD_SIZE - 4];
} EASS_MsgResult;

typedef struct {
    int             iResult;
    int             iNumberToCall;
} EASS_MsgCallOpened;

typedef struct {
    FixString       szPassword;
} EASS_MsgOpenRemoteDesktop;

typedef struct {
    int             iResult;
    FixString       szPassword;
} EASS_MsgRemoteDesktopOpened;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/
#ifdef ANSI_C

#define in
#define out


#undef in
#undef out

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_EASS_INTERNAL_H_ */


 