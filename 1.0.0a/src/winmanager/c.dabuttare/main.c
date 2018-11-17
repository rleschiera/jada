/******************************************************************************/
/*                                                                            */
/*   File    : main.c                                                         */
/*   Scope   : FLOW Framework System Manager main program                      */
/*   Module  :                                                                */
/*   Author  :                                                                */
/*   Release : 5                                                              */
/*   Version : 0                                                              */
/*   Data    : 11/10/2001                                                     */
/*   Note    :                                                                */
/*                                                                            */
/******************************************************************************/
#include "eass.h"
#define extern
#include "agent.h"
#undef extern

static ECOM_Command Command[] = {
//    "1",    1, "Apri sessione",                EASS_MnuOpenSession,        TRUE,
    "1",    1, "Use Case 1",                   ECOM_MnuNext,               FALSE,                 
    "1",    2, "Use Case 2",                   ECOM_MnuNext,               FALSE,                 
//    "1",    4, "Chiudi sessione",              EASS_MnuCloseSession,       TRUE,
    "1",   99, "Esci",                         ECOM_MnuExit,               FALSE, 
    "1.1",  1, "Collegati ad una richiesta",   EASS_MnuConnectRequest,     TRUE,                 
    "1.1",  3, "Invia messaggi in chat",       EASS_MnuSendMessages,       TRUE,                 
    "1.1",  6, "Chiudi richiesta",             EASS_MnuCloseRequest,       TRUE,                 
    "1.1", 99, "Indietro",                     ECOM_MnuBack,               FALSE,                 
    "1.2",  1, "Crea una richiesta",           EASS_MnuCreateRequest,      TRUE,                 
    "1.2",  4, "Invia messaggi in chat",       EASS_MnuSendMessages,        TRUE,                 
    "1.2",  7, "Chiudi richiesta",             EASS_MnuCloseRequest,       TRUE,                 
    "1.2", 99, "Indietro",                     ECOM_MnuBack,               FALSE,                 
    NULL,   0, NULL,                           NULL,                       -1
};

typedef struct {
    char        *szSession;
    char        *szKey;
    char        *szValue;
    int         iIsPassword;
} EASS_IniEntry;

static EASS_IniEntry VNCServerIni[] = {
    "UltraVNC",         "passwd",                   "01F7BC7B8F7C3F9811",       1,
    "UltraVNC",         "passwd2",                  "01F7BC7B8F7C3F9811",       1,
    "admin",            "UseRegistry",              "0",                        0,
    "admin",            "MSLogonRequired",          "0",                        0,
    "admin",            "NewMSLogon",               "0",                        0,
    "admin",            "DebugMode",                "0",                        0,
    "admin",            "Avilog",                   "0",                        0,
    "admin",            "path",                     "C:\\engassist\\bin",       0,
    "admin",            "accept_reject_mesg",       "",                         0,
    "admin",            "DebugLevel",               "8",                        0,
    "admin",            "DisableTrayIcon",          "0",                        0,
    "admin",            "LoopbackOnly",             "0",                        0,
    "admin",            "UseDSMPlugin",             "0",                        0,
    "admin",            "AllowLoopback",            "1",                        0,
    "admin",            "AuthRequired",             "1",                        0,
    "admin",            "ConnectPriority",          "0",                        0,
    "admin",            "DSMPlugin",                "No Plugin Detected",       0,
    "admin",            "AuthHosts",                "",                         0,
    "admin",            "DSMPluginConfig",          "",                         0,
    "admin",            "AllowShutdown",            "1",                        0,
    "admin",            "AllowProperties",          "1",                        0,
    "admin",            "AllowEditClients",         "1",                        0,
    "admin",            "FileTransferEnabled",      "1",                        0,
    "admin",            "FTUserImpersonation",      "1",                        0,
    "admin",            "BlankMonitorEnabled",      "1",                        0,
    "admin",            "BlankInputsOnly",          "0",                        0,
    "admin",            "DefaultScale",             "1",                        0,
    "admin",            "primary",                  "1",                        0,
    "admin",            "secondary",                "0",                        0,
    "admin",            "SocketConnect",            "1",                        0,
    "admin",            "HTTPConnect",              "1",                        0,
    "admin",            "AutoPortSelect",           "1",                        0,
    "admin",            "PortNumber",               "5900",                     0,
    "admin",            "HTTPPortNumber",           "5800",                     0,
    "admin",            "IdleTimeout",              "0",                        0,
    "admin",            "IdleInputTimeout",         "0",                        0,
    "admin",            "RemoveWallpaper",          "0",                        0,
    "admin",            "RemoveAero",               "0",                        0,
    "admin",            "QuerySetting",             "2",                        0,
    "admin",            "QueryTimeout",             "10",                       0,
    "admin",            "QueryAccept",              "0",                        0,
    "admin",            "QueryIfNoLogon",           "1",                        0,
    "admin",            "InputsEnabled",            "1",                        0,
    "admin",            "LockSetting",              "0",                        0,
    "admin",            "LocalInputsDisabled",      "0",                        0,
    "admin",            "EnableJapInput",           "0",                        0,
    "admin",            "kickrdp",                  "0",                        0,
    "admin",            "clearconsole",             "0",                        0,
    "admin",            "service_commandline",      "",                         0,
    "admin",            "FileTransferTimeout",      "1",                        0,
    "admin",            "KeepAliveInterval",        "5",                        0,
    "admin_auth",       "group1",                   "",                         0,
    "admin_auth",       "group2",                   "",                         0,
    "admin_auth",       "group3",                   "",                         0,
    "admin_auth",       "locdom1",                  "0",                        0,
    "admin_auth",       "locdom2",                  "0",                        0,
    "admin_auth",       "locdom3",                  "0",                        0,
    "poll",             "TurboMode",                "1",                        0,
    "poll",             "PollUnderCursor",          "0",                        0,
    "poll",             "PollForeground",           "0",                        0,
    "poll",             "PollFullScreen",           "1",                        0,
    "poll",             "OnlyPollConsole",          "0",                        0,
    "poll",             "OnlyPollOnEvent",          "0",                        0,
    "poll",             "MaxCpu",                   "40",                       0,
    "poll",             "EnableDriver",             "0",                        0,
    "poll",             "EnableHook",               "1",                        0,
    "poll",             "EnableVirtual",            "0",                        0,
    "poll",             "SingleWindow",             "0",                        0,
    "poll",             "SingleWindowName",         "",                         0,
    NULL,               NULL,                       NULL,                       0
};

static FixString    EASS_VncServerIniFile;

static int SetVNCServer(void)
{
    int     i;
    DWORD   dwRet;

    for (i = 0; VNCServerIni[i].szSession != NULL; i++) {
        dwRet = WritePrivateProfileString(VNCServerIni[i].szSession, VNCServerIni[i].szKey, VNCServerIni[i].szValue, EASS_VncServerIniFile);
        CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "errore nel settaggio del valore nel file INI");
    }

    return(0);
}

int __stdcall SetVNCPassword(char *szNewPassword)
{
    int     i;
    DWORD   dwRet;

    for (i = 0; VNCServerIni[i].szSession != NULL; i++) {
        if (VNCServerIni[i].iIsPassword != 0) {
            if (szNewPassword != NULL) {
                dwRet = WritePrivateProfileString(VNCServerIni[i].szSession, VNCServerIni[i].szKey, szNewPassword, EASS_VncServerIniFile);
                CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "errore nel settaggio del valore nel file INI");
            }
            else {
                dwRet = WritePrivateProfileString(VNCServerIni[i].szSession, VNCServerIni[i].szKey, VNCServerIni[i].szValue, EASS_VncServerIniFile);
                CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "errore nel settaggio del valore nel file INI");
            }
        }
    }

    return(0);
}

void EASS_ExitProcess(void)
{
    int     iRet;

    iRet = SetVNCPassword(NULL);
}

int main(int argc, char *argv[])
{
    int         iRet;
#ifndef ON_WINDOWS
    int         iPid;
#endif

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    /* Winsock needs additional startup activities */
    WSADATA wsadata;
    WSAStartup(MAKEWORD(1,1), &wsadata);
#endif

    atexit(EASS_ExitProcess);

    iRet = ECOM_Initialize(EASS_WHOAMI_AGENT);
    CORE_ReturnValIfFail(iRet == 0, -1, ECOM_WaitKey("");, "errore generico");

    strcpy(EASS_SoftPhone, "agentphone.exe");
    strcpy(EASS_VncServer, "winvnc.exe");
    strcpy(EASS_AudioVideo, "audiovideo.exe");
    strcpy(EASS_VncServerIniFile, "C:/engassist/bin/UltraVNC.ini");

    iRet = SetVNCServer();
    CORE_ReturnValIfFail(iRet == 0, -1, ECOM_WaitKey("");, "errore generico");

    /*- Initialize the internal variables */
    EASS_SessionId = -1;
    EASS_RequestId = -1;

    iRet = EASS_MnuOpenSession();
    CORE_ReturnValIfFail(iRet == 0, -1, ECOM_WaitKey("");, "errore nella connessione al server");

    iRet = ECOM_StartMenu("ENG Assist Agent", &Command[0]);
    CORE_ReturnValIfFail(iRet == 0, -1, ECOM_WaitKey("");, "errore generico");

    /*- Return the result */
    return(0);
}
