#ifndef _EASS_COMMON_H_
#define _EASS_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/******************************************************************************/
/*                                                                            */
/*   G e n e r a l     I n c l u d e     F i l e s                            */
/*                                                                            */
/******************************************************************************/
#include <assert.h>

/******************************************************************************/
/*                                                                            */
/*   M a c r o s     D e f i n i t i o n                                      */
/*                                                                            */
/******************************************************************************/
/* Assert if the condition fails */
#define CORE_AssertIfFail(expr, cmds, msg)                       \
        { if (!(expr)) {                                         \
            printf("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            assert(0);                                           \
            cmds;                                                \
            return;                                              \
        } }
#define CORE_AssertValIfFail(expr, val, cmds, msg)               \
        { if (!(expr)) {                                         \
            printf("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            assert(0);                                           \
            cmds;                                                \
            return(val);                                         \
        } }

/* Return if the condition fails */
#define CORE_ReturnIfFail(expr, cmds, msg)                       \
        { if (!(expr)) {                                         \
            printf("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                \
            return;                                              \
        } }
#define CORE_SilentReturnIfFail(expr, cmds)                      \
        { if (!(expr)) {                                         \
            cmds;                                                \
            return;                                              \
        } }
#define CORE_ReturnValIfFail(expr, val, cmds, msg)               \
        { if (!(expr)) {                                         \
            printf("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                \
            return(val);                                         \
        } }
#define CORE_SilentReturnValIfFail(expr, val, cmds)              \
        { if (!(expr)) {                                         \
            cmds;                                                \
            return(val);                                         \
        } }

/* Execute statements if the condition fails */
#define CORE_ExecuteIfFail(expr, cmds, msg)                      \
        { if (!(expr)) {                                         \
            printf("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                \
        } }
#define CORE_SilentExecuteIfFail(expr, cmds)                     \
        { if (!(expr)) {                                         \
            cmds;                                                \
        } }

/* Goto label if the condition fails */
#define CORE_GotoIfFail(expr, label, cmds, msg)                  \
        { if (!(expr)) {                                         \
            printf("%s, %d: %s\n", __FILE__, __LINE__, msg);     \
            cmds;                                                \
            goto label;                                          \
        } }
#define CORE_SilentGotoIfFail(expr, label, cmds)                 \
        { if (!(expr)) {                                         \
            cmds;                                                \
            goto label;                                          \
        } }

#define CORE_Max(a,b)               (((a) > (b)) ? (a) : (b))
#define CORE_Min(a,b)               (((a) < (b)) ? (a) : (b))

/* Evaluate the condition and set the value accordlingly */
#define CORE_Evaluate(cond,yes,no)  ((cond) ? (yes) : (no))

/******************************************************************************/
/*                                                                            */
/*   C o n s t a n t s     D e f i n i t i o n                                */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*   M  e  s  s  a  g  e  s                                                   */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*   T y p e s     D e f i n i t i o n                                        */
/*                                                                            */
/******************************************************************************/
typedef char FixString[260];
typedef char CoreId[64];

#define CORE_Callback       int __stdcall
typedef int (__stdcall *CORE_CallbackPtr)();

typedef struct {
    char                *szMenuLevel;
    int                 iOption;
    char                *szDescription;
    CORE_CallbackPtr    cbkFunction;
    int                 iPressKeyWhenFinished;
} ECOM_Command;

/******************************************************************************/
/*                                                                            */
/*   V a r i a b l e s                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*   M e t h o d s     D e f i n i t i o n                                    */
/*                                                                            */
/******************************************************************************/
#ifdef ANSI_C

#define in
#define out

/* Generic utilities */
int __stdcall ECOM_Sleep(int iMilliSecs);

/* TCP communications */
int __stdcall ECOM_TcpCreateListenPort(char *szHostName, int iPort);
int __stdcall ECOM_TcpConnectToPort(char *szHostName, int iPort);
int __stdcall ECOM_TcpSendMessage(int iSocket, char *msgOut, int iLength);
int __stdcall ECOM_TcpReceiveMessage(int iSocket, char *msgIn, int iLength);

/* Ini file management */
int __stdcall ECOM_Initialize(int iWhoAmI);
int __stdcall ECOM_IniGetString(char *szSection, char *szKey, char *szOut, int iOutLength);
int __stdcall ECOM_IniGetInt(char *szSection, char *szKey, int *piOut);
int __stdcall ECOM_IniSetString(char *szSection, char *szKey, char *szIn);
int __stdcall CORE_GetString(char *szPrompt, char *szString, int iLength);
int __stdcall CORE_GetInteger(char *szPrompt, int *piInteger);
int __stdcall CORE_GetTrueFalse(char *szPrompt);

/* Message communications */
char * __stdcall ECOM_OpcodeToString(int iOpcode);
char * __stdcall ECOM_GetServerName(void);
char * __stdcall ECOM_GetMyName(void);
char * __stdcall ECOM_GetMyIP(void);
int __stdcall ECOM_ConnectToServer(void);
int __stdcall ECOM_SendSessionMessage(int iOpcode, void *pPayload, int iSize);
int __stdcall ECOM_SendRequestMessage(int iOpcode, void *pPayload, int iSize);

/* Menu management */
CORE_Callback ECOM_MnuExit(void);
CORE_Callback ECOM_MnuNext(void);
CORE_Callback ECOM_MnuBack(void);
int __stdcall ECOM_WaitKey(char *szPrompt);
int __stdcall ECOM_StartMenu(char *szCmdName, ECOM_Command *Command);

/* Processes management */
int __stdcall ECOM_ProcExists(char *szProcName);
int __stdcall ECOM_ProcKill(char *szProcName);
int __stdcall ECOM_ProcExecute(char *szProcName, char *szProcParams);

#undef in
#undef out

#endif /* ANSI_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_EASS_COMMON_H_ */
