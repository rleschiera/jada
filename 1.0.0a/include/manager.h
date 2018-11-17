#ifndef _MANAGER_H_
#define _MANAGER_H_

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
extern JADA_Connection  AMGR_Connection;
extern JADA_Support     AMGR_Support;
extern JADA_Call        AMGR_Call;
extern JADA_VNC         AMGR_VNC;
extern JADA_FTP         AMGR_FTP;

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/

#define in
#define out

/* callback.c */
CORE_Callback AMGR_MnuOpenConnection(int iAutoLogin);
CORE_Callback AMGR_MnuCloseConnection(void);
CORE_Callback AMGR_MnuListRequests(void);      
CORE_Callback AMGR_MnuListPendingRequests(void);
CORE_Callback AMGR_MnuListParameters(void);
CORE_Callback AMGR_MnuGetCombo(void);
CORE_Callback AMGR_MnuGetRequestCount(void);
CORE_Callback AMGR_MnuAcceptSupport(void);      
CORE_Callback AMGR_MnuLoadSupport(void);      
CORE_Callback AMGR_MnuSendMessages(void);
CORE_Callback AMGR_MnuCloseSupport(void);
CORE_Callback AMGR_MnuRequestCall(void);
CORE_Callback AMGR_MnuRequestVnc(void);
CORE_Callback AMGR_MnuRequestFtp(void);

/* threads */
int JADA_ManageEvents(JADA_Connection connSelf);

#undef in
#undef out

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_MANAGER_H_ */
