/******************************************************************************/
/*                                                                            */
/*   File    : ses.h                                                          */
/*   Scope   : The Session Manager main include file                          */
/*   Module  :                                                                */
/*   Author  : Arel Team                                                      */
/*   Release : 5                                                              */
/*   Version : 0                                                              */
/*   Data    : 11/10/2001                                                     */
/*   Note    :                                                                */
/*                                                                            */
/******************************************************************************/

#ifndef _AGENT_H_
#define _AGENT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/*                                                                            */
/*   G e n e r a l     I n c l u d e     F i l e s                            */
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                                                                            */
/*   V a r i a b l e s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/
extern FixString            EASS_TmpFile;
extern int                  EASS_SessionId;
extern int                  EASS_RequestId;
extern FixString            EASS_UserName;
extern FixString            EASS_AgentName;
extern struct sockaddr_in   EASS_AgentAddress;
extern FixString            EASS_SoftPhone;
extern FixString            EASS_VncServer;
extern FixString            EASS_AudioVideo;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/

#define in
#define out

/* callback.c */
CORE_Callback EASS_MnuOpenSession(void);
CORE_Callback EASS_MnuCloseSession(void);
CORE_Callback EASS_MnuCreateRequest(void);      
CORE_Callback EASS_MnuConnectRequest(void);      
CORE_Callback EASS_MnuSendMessages(void);
CORE_Callback EASS_MnuCloseRequest(void);

/* main.c */
int __stdcall SetVNCPassword(char *szNewPassword);

/* session.c */
int __stdcall EASS_OpenSession(char *szUtente, char *szTelefono, char *szCliente);
int __stdcall EASS_CloseSession(void);
int __stdcall EASS_CreateRequest(char *szArea, char *szDescription);
int __stdcall EASS_ConnectRequest(int iRequestId);
int __stdcall EASS_CloseRequest(void);
int __stdcall EASS_HandleSessionMessage(EASS_Message *msgIn);

/* tcp.c */
int __stdcall ECOM_ConnectToRemoteDesktopServer(void);

#undef in
#undef out

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_AGENT_H_ */
