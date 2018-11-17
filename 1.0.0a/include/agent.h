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
extern JADA_Connection  AAGT_Connection;
extern JADA_Support     AAGT_Support;
extern JADA_Call        AAGT_Call;
extern JADA_VNC         AAGT_VNC;
extern JADA_FTP         AAGT_FTP;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/

#define in
#define out

/* callback.c */
CORE_Callback AAGT_MnuOpenConnection(int iAutoLogin);
CORE_Callback AAGT_MnuCloseConnection(void);
CORE_Callback AAGT_MnuCreateSupport(void);      
CORE_Callback AAGT_MnuConnectSupport(void);      
CORE_Callback AAGT_MnuSendMessages(void);
CORE_Callback AAGT_MnuCloseSupport(void);
CORE_Callback AAGT_MnuAcceptCall(void);
CORE_Callback AAGT_MnuAcceptVnc(void);
CORE_Callback AAGT_MnuAcceptFtp(void);

/* main.c */
int AAGT_SetVNCPassword(char *szNewPassword);

/* threads */
int JADA_ManageEvents(JADA_Connection connSelf);

#undef in
#undef out

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_AGENT_H_ */
