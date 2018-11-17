#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"
#include "jada_events.h"
#include "zlib.h"
#include "minIni.h"
#include <stdint.h>
#include <stdlib.h>

#define MILLISECS_PER_SEC           1000
#define NANOSECS_PER_MILLISEC       1000000
#define NANOSECS_PER_SEC            1000000000
#define JADA_MEMORY_BLOCKS          1000

typedef struct {
        ShortString      szFile;
        int         iLine;
        void        *szPtr;
} JADA_MemoryBlock;

static int              myWhoAmI;
static FixString        myMemFileName = "";
static FixString        myIniFileName = "";
static FixString        myLogFileName = "";
static FixString        myLogDirName = "";
//static FixString        mySettingsFileName = "";
static FixString        myServerFileName = "";
static FixString        myHostName = "";
static int              myOption = -1;
static FixString        myMenuLevel = "";
static JADA_MemoryBlock lstMemory[JADA_MEMORY_BLOCKS];
static FILE             *myLogFP = (FILE *) 0xFFFFFFFFFFFFFFF0;
static int              myMemoryDebug = 0;
//static int              mySocketVNC = -1;


/******************************************************************************/
/*                                                                            */
/*   I N I T I A L I Z A T I O N     S E R V I C E S                          */
/*                                                                            */
/******************************************************************************/
char *JADA_GetLogDir(void)
{
    return(myLogDirName);
}

//int JADA_GetSocketVNC(void)
//{
//    return(mySocketVNC);
//}
//
int JADA_SetTimeSpec(struct timespec *Timespec, int iMsecsToWait)
{
    int             iNsec;
    struct timeb    Now;


    ftime(&Now);
    Timespec->tv_sec = (int) Now.time;
    Timespec->tv_nsec = Now.millitm * NANOSECS_PER_MILLISEC;

    iNsec = (iMsecsToWait % MILLISECS_PER_SEC) * NANOSECS_PER_MILLISEC;
    Timespec->tv_sec += (iMsecsToWait / MILLISECS_PER_SEC);
    Timespec->tv_nsec += iNsec;
    while (Timespec->tv_nsec >= NANOSECS_PER_SEC) {
        Timespec->tv_sec++;
        Timespec->tv_nsec -= NANOSECS_PER_SEC;
    }

    return(0);
}

static int JADA_Trim(char *szString)
{
    int     iLength;

    iLength = strlen(szString);
    szString[iLength] = '\0';

    while (--iLength >= 0) {
        if ((szString[iLength] != '\0') && (!isspace(szString[iLength]))) {
            break;
        }
    }
    szString[iLength + 1] = '\0';

    return(0);
}

int JADA_ChangeStringChar(char *szString, char chFrom, char chTo)
{
    int     iLength;

    iLength = strlen(szString);

    while (--iLength >= 0) {
        if (szString[iLength] == chFrom) {
            szString[iLength] = chTo;
        }
    }

    return(0);
}

int JADA_FileExists(char *szFile)
{
    int     iRet = -1;
    FILE    *fp = NULL;

    fp = fopen(szFile, "r");
    if (fp != NULL) {
        fclose(fp);
        iRet = 0;
    }

    return(iRet);
}

int JADA_FileRemove(char *szFile)
{
    int     iRet;


    if (JADA_FileExists(szFile) == 0) {
#ifdef UNIX
        iRet = unlink(szFile);
#else
        if (szFile[0] == '/') {
            iRet = _unlink(szFile + 1);
        }
        else {
            iRet = _unlink(szFile);
        }
#endif
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella cancellazione del file");
    }
    
    return(0);
}

int JADA_FileDelayed(char *szDelayedFile)
{
    int         iRet;
    FixString   szCurrFile;
    char        *szPtr;

    iRet = JADA_FileExists(szDelayedFile);
    if (iRet == 0) {
        strcpy(szCurrFile, szDelayedFile);
        szPtr = strrchr(szCurrFile, '.');
        if (szPtr != NULL) {
            *szPtr = '\0';
        }
        rename(szDelayedFile, szCurrFile); 
    }

    return(0);
}

int JADA_FileWrite(char *szFullPath, void *szData, int iLength)
{
    int     iRet = -1;
    FILE    *fp = NULL;

    fp = fopen(szFullPath, "w+b");
    if (fp != NULL) {
        fwrite(szData, iLength, 1, fp);
        fclose(fp);
        iRet = 0;
    }

    return(iRet);
}

/******************************************************************************
 *
 *   L O G
 *
 ******************************************************************************/
DllExport int __stdcall JADA_LogStart(void)
{
    myLogFP = fopen(myLogFileName, "a+");
    if (myLogFP != NULL) {
        fprintf(myLogFP, "*************************************************\n");
    }

    return(0);
}

DllExport void __stdcall JADA_LogError(char *szFormat, ...)
{
    va_list     argPtr;
    LongString  szMessage;
    char        *szCurrDate;

    szCurrDate = JADA_TimeToString(time(0), NULL, 0);

    va_start(argPtr, szFormat);
    vsprintf(szMessage, szFormat, argPtr);
    va_end(argPtr);
#ifdef UNIX
    if (ftell(stdout) >= 0)
#endif
        printf("%s: %s", szCurrDate, szMessage);

    if (myLogFP != NULL) {
        fprintf(myLogFP, "%s: %s", szCurrDate, szMessage);
        fflush(myLogFP);
    }
}

DllExport void __stdcall JADA_LogMessage(char *szFormat, ...)
{
    va_list     argPtr;
    LongString  szMessage;
    char        *szCurrDate;

    szCurrDate = JADA_TimeToString(time(0), NULL, 0);

    va_start(argPtr, szFormat);
    vsprintf(szMessage, szFormat, argPtr);
    va_end(argPtr);
#ifdef UNIX
    if (ftell(stdout) >= 0)
#endif
        printf("%s: %s", szCurrDate, szMessage);

    if (myLogFP != NULL) {
        fprintf(myLogFP, "%s: %s", szCurrDate, szMessage);
        fflush(myLogFP);
    }
}

DllExport void __stdcall JADA_LogToFile(int iWhoAmI, char *szFile, unsigned char *szBuffer, int iLength)
{
    int         i;
    FILE        *fp;
    FixString   szFileName;

    sprintf(szFileName, "%s.%s", szFile, JADA_WhoAmIToString(iWhoAmI, NULL, 0));
    fp = fopen(szFileName, "a+");
    if (fp != NULL) {
        for (i = 0; i < iLength; i++) {
            if (i > 0) fprintf(fp, ":");
            fprintf(fp, "%02X", szBuffer[i]);
        }
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }
}

char *StringFormat(char *szFormat, ...)
{
    va_list             argPtr;
    static LongString   myLocalMessage;

    va_start(argPtr, szFormat);
    vsprintf(myLocalMessage, szFormat, argPtr);
    va_end(argPtr);

    return(myLocalMessage);
}

/******************************************************************************
 *
 *   S T A R T / S T O P
 *
 ******************************************************************************/
void JADA_ExitProcess(void)
{
    int     iRet;

    iRet = JADA_Stop();
}

DllExport int __stdcall JADA_Start(int iWhoAmI, char *myHomeDir)
{
    int         iRet;
    FixString   szWhoAmI, szHomeDir, szCurrDir, szDelayedFile;


#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    /* Winsock needs additional startup activities */
    WSADATA wsadata;
    WSAStartup(MAKEWORD(1,1), &wsadata);
#endif

    atexit(JADA_ExitProcess);

    memset(lstMemory, 0, sizeof(lstMemory));
    if (myHomeDir != NULL) {
        strcpy(szHomeDir, myHomeDir);
    }
    else {
#ifdef UNIX
        strcpy(szHomeDir, "/home/jada/jada");
#else
        strcpy(szHomeDir, "C:\\jada");
#endif
    }

    switch (iWhoAmI) {
    case JADA_WHOAMI_AGENT:
        strcpy(szWhoAmI, "agent");
#ifndef SERVER
        //iRet = SetVNCServer();
        //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
#endif /* SERVER */
        break;
    case JADA_WHOAMI_MANAGER:
        strcpy(szWhoAmI, "manager");
        break;
    case JADA_WHOAMI_SERVER:
        strcpy(szWhoAmI, "server");
        break;
    default:
        CORE_ReturnValIfFail(0, -1, ;, "Errore generico");
    }

    myWhoAmI = iWhoAmI;
#ifdef UNIX
    sprintf(myIniFileName, "%s/config/%s.ini", szHomeDir, szWhoAmI);
    sprintf(myMemFileName, "%s/tmp/%s.mem.txt", szHomeDir, szWhoAmI);
    sprintf(myLogFileName, "%s/log/%s.log", szHomeDir, szWhoAmI);
    sprintf(myLogDirName, "%s/log", szHomeDir);
#else
    sprintf(myIniFileName, "%s\\config\\%s.ini", szHomeDir, szWhoAmI);
    sprintf(myMemFileName, "%s\\tmp\\%s.mem.txt", szHomeDir, szWhoAmI);
    sprintf(myLogFileName, "%s\\log\\%s.log", szHomeDir, szWhoAmI);
    sprintf(myLogDirName, "%s\\log", szHomeDir);
#endif

    switch (iWhoAmI) {
    case JADA_WHOAMI_AGENT:
    case JADA_WHOAMI_MANAGER:
        break;
    case JADA_WHOAMI_SERVER:
        sprintf(szCurrDir, "%s\\connections", myLogDirName); _mkdir(szCurrDir);
        sprintf(szCurrDir, "%s\\requests", myLogDirName); _mkdir(szCurrDir);
        sprintf(szCurrDir, "%s\\sessions", myLogDirName); _mkdir(szCurrDir);
        break;
    default:
        CORE_ReturnValIfFail(0, -1, ;, "Errore generico");
    }

    iRet = JADA_LogStart();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_FileExists(myIniFileName);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

//#ifdef UNIX
//    sprintf(mySettingsFileName, "%s/config/settings.ini", szHomeDir);
//#else
//    sprintf(mySettingsFileName, "%s\\config\\settings.ini", szHomeDir);
//#endif
    //iRet = JADA_FileExists(mySettingsFileName);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

#ifdef UNIX
    sprintf(myServerFileName, "%s/config/server.ini", szHomeDir);
#else
    sprintf(myServerFileName, "%s\\config\\server.ini", szHomeDir);
#endif
    iRet = JADA_FileExists(myServerFileName);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_IniGetInt("debug", "memory", &myMemoryDebug);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    if (strlen(myHostName) == 0) {
        gethostname(myHostName, sizeof(myHostName));
    }

#ifndef SERVER
    //if (iWhoAmI == JADA_WHOAMI_MANAGER) {
    //    mySocketVNC = JADA_TcpCreateListenPort("127.0.0.1", JADA_REMOTE_DESKTOP_PORT);
    //    CORE_ReturnValIfFail(mySocketVNC > 0, -1, ;, "Errore generico");
    //}
#endif /* SERVER */

    sprintf(szDelayedFile, "%s\\bin\\winagent.exe.delayed", szHomeDir);
    iRet = JADA_FileDelayed(szDelayedFile);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    sprintf(szDelayedFile, "%s\\bin\\winmanager.exe.delayed", szHomeDir);
    iRet = JADA_FileDelayed(szDelayedFile);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    JADA_LogMessage("Processo %s attivato su %s\n", szWhoAmI, myHostName);
    return(0);
}

DllExport int __stdcall JADA_Stop(void)
{
#ifndef SERVER
    int         iRet;
#endif

    switch (myWhoAmI) {
#ifndef SERVER
    case JADA_WHOAMI_AGENT:
        iRet = JADA_ProcKill(JADA_PROC_VNCSERVER);
        iRet = JADA_ProcKill(JADA_PROC_CALLAGENT);
        /* TODO: ammazzare i thread interni */
        break;
    case JADA_WHOAMI_MANAGER:
        iRet = JADA_ProcKill(JADA_PROC_VNCVIEWER);
        iRet = JADA_ProcKill(JADA_PROC_CALLMANAGER);
        /* TODO: ammazzare i thread interni */
        break;
#endif
    case JADA_WHOAMI_SERVER:
        break;
    default:
        CORE_ReturnValIfFail(0, -1, ;, "Errore generico");
    }
    JADA_LogMessage("Processo terminato\n");

    return(0);
}

/******************************************************************************
 *
 *  I N I     F I L E     S E R V I C E S
 *
 *****************************************************************************/
int JADA_MyIniGetString(char *szSection, char *szKey, char *szOut, int iOutLength, char *szFileName)
{
#ifdef UNIX
    int         iRet;
#else
    DWORD       dwRet;
#endif

    memset(szOut, 0, iOutLength);

#ifdef UNIX
    iRet = ini_gets(szSection, szKey, "", szOut, iOutLength, szFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore nella lettura dal file ini");
#else
    dwRet = GetPrivateProfileStringA(szSection, szKey, "", szOut, iOutLength, szFileName);
    CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "Errore generico");
#endif

    return(0);
}

int JADA_MyIniSetString(char *szSection, char *szKey, char *szIn, char *szFileName)
{
#ifdef UNIX
    int         iRet;
#else
    DWORD       dwRet;
#endif

#ifdef UNIX
    iRet = ini_puts(szSection, szKey, szIn, szFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore nella scrittura nel file ini");
#else
    dwRet = WritePrivateProfileStringA(szSection, szKey, szIn, szFileName);
    CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "Errore generico");
#endif

    return(0);
}

int JADA_MyIniGetInt(char *szSection, char *szKey, int *piOut, char *szFileName)
{
    int         iRet;
    FixString   szTmp;

    iRet = JADA_MyIniGetString(szSection, szKey, szTmp, sizeof(szTmp), szFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    *piOut = atoi(szTmp);
    return(0);
}

int JADA_MyIniSetInt(char *szSection, char *szKey, int iIn, char *szFileName)
{
    int         iRet;
    FixString   szTmp;

    sprintf(szTmp, "%d", iIn);
    iRet = JADA_MyIniSetString(szSection, szKey, szTmp, szFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

int JADA_MyIniGetJadaId(char *szSection, char *szKey, void *piOut, char *szFileName)
{
    int         iRet;
    FixString   szTmp;

    iRet = JADA_MyIniGetString(szSection, szKey, szTmp, sizeof(szTmp), szFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    JADA_StringToJadaId(szTmp, piOut);
    return(0);
}

int JADA_MyIniSetJadaId(char *szSection, char *szKey, void *piId, char *szFileName)
{
    int         iRet;
    FixString   szTmp;

    JADA_JadaIdToString(piId, szTmp);
    iRet = JADA_MyIniSetString(szSection, szKey, szTmp, szFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_IniGetString(char *szSection, char *szKey, char *szOut, int iOutLength)
{
    int         iRet;

    iRet = JADA_MyIniGetString(szSection, szKey, szOut, iOutLength, myIniFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_IniSetString(char *szSection, char *szKey, char *szIn)
{
    int         iRet;

    iRet = JADA_MyIniSetString(szSection, szKey, szIn, myIniFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_IniGetInt(char *szSection, char *szKey, int *piOut)
{
    int         iRet;
    FixString   szTmp;

    iRet = JADA_IniGetString(szSection, szKey, szTmp, sizeof(szTmp));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    *piOut = atoi(szTmp);
    return(0);
}

DllExport int __stdcall JADA_IniSetInt(char *szSection, char *szKey, int iIn)
{
    int         iRet;
    FixString   szTmp;

    sprintf(szTmp, "%d", iIn);
    iRet = JADA_IniSetString(szSection, szKey, szTmp);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_IniGetJadaId(char *szSection, char *szKey, void *piOut)
{
    int         iRet;
    FixString   szTmp;

    iRet = JADA_IniGetString(szSection, szKey, szTmp, sizeof(szTmp));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    JADA_StringToJadaId(szTmp, piOut);
    return(0);
}

DllExport int __stdcall JADA_IniSetJadaId(char *szSection, char *szKey, void *piId)
{
    int         iRet;
    FixString   szTmp;

    JADA_JadaIdToString(piId, szTmp);
    iRet = JADA_IniSetString(szSection, szKey, szTmp);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_ServerGetString(char *szSection, char *szKey, char *szOut, int iOutLength)
{
    int         iRet;

    iRet = JADA_MyIniGetString(szSection, szKey, szOut, iOutLength, myServerFileName);
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_ServerGetInt(char *szSection, char *szKey, int *piOut)
{
    int         iRet;
    FixString   szTmp;

    iRet = JADA_ServerGetString(szSection, szKey, szTmp, sizeof(szTmp));
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");

    *piOut = atoi(szTmp);
    return(0);
}

//DllExport int __stdcall JADA_SettingsGetString(char *szSection, char *szKey, char *szOut, int iOutLength)
//{
//    int         iRet;
//
//    iRet = JADA_MyIniGetString(szSection, szKey, szOut, iOutLength, mySettingsFileName);
//    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
//
//    return(0);
//}
//
//DllExport int __stdcall JADA_SettingsSetString(char *szSection, char *szKey, char *szIn)
//{
//    int         iRet;
//
//    iRet = JADA_MyIniSetString(szSection, szKey, szIn, mySettingsFileName);
//    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
//
//    return(0);
//}
//
//DllExport int __stdcall JADA_SettingsGetInt(char *szSection, char *szKey, int *piOut)
//{
//    int         iRet;
//    FixString   szTmp;
//
//    iRet = JADA_SettingsGetString(szSection, szKey, szTmp, sizeof(szTmp));
//    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
//
//    *piOut = atoi(szTmp);
//    return(0);
//}
//
//DllExport int __stdcall JADA_SettingsSetInt(char *szSection, char *szKey, int iIn)
//{
//    int         iRet;
//    FixString   szTmp;
//
//    sprintf(szTmp, "%d", iIn);
//    iRet = JADA_SettingsSetString(szSection, szKey, szTmp);
//    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
//
//    return(0);
//}

/******************************************************************************
 *
 *  I N T E R A C T I V E     S E R V I C E S
 *
 *****************************************************************************/
DllExport int __stdcall CORE_GetString(char *szPrompt, char *szString, int iLength)
{
    char    szBuffer[255];
    char    *szRet;
    char    *szPtr;

    do {
        if (strlen(szPrompt) > 0) {
           fprintf(stderr, "%s [%s]:", szPrompt, szString);
        }
        szRet = fgets(szBuffer, sizeof(szBuffer), stdin);

        szPtr = strchr(szBuffer, '\n');
        if (szPtr != NULL) {
            *szPtr = '\0';
        }
    } while (szBuffer[0] == '#');

    if (strlen(szBuffer) > 0) {
        strcpy(szString, szBuffer);
    }

    return(0);
}

DllExport int __stdcall CORE_GetInteger(char *szPrompt, int *piInteger)
{
    char    szBuffer[255];
    char   *szRet;
    char   *szPtr;


    // *piInteger = 0;
    do {
        if (strlen(szPrompt) > 0) {
           fprintf(stderr, "%s [%d]:", szPrompt, *piInteger);
        }
        szRet = fgets(szBuffer, sizeof(szBuffer), stdin);

        szPtr = strchr(szBuffer, '\n');
        if (szPtr != NULL) {
            *szPtr = '\0';
        }
    } while (szBuffer[0] == '#');

    if (strlen(szBuffer) > 0) {
        *piInteger = atoi(szBuffer);
    }

    return(0);
}

DllExport int __stdcall CORE_GetTrueFalse(char *szPrompt)
{
    char    szBuffer[255];
    char    *szRet;
    char    *szPtr;
    int     iAnswer;


    do {
        if (strlen(szPrompt) > 0) {
           fprintf(stderr, "%s", szPrompt);
        }
        szRet = fgets(szBuffer, sizeof(szBuffer), stdin);

        szPtr = strchr(szBuffer, '\n');
        if (szPtr != NULL) {
            *szPtr = '\0';
        }
        iAnswer = toupper(szBuffer[0]);
    } while ((iAnswer != 'Y') && (iAnswer != 'N'));

    iAnswer = CORE_Evaluate(iAnswer == 'Y', TRUE, FALSE);

    return(iAnswer);
}

/******************************************************************************/
/*                                                                            */
/*   S E S S I O N     S E R V I C E S                                        */
/*                                                                            */
/******************************************************************************/
char *JADA_GetMyName(void)
{
    return(myHostName);
}

int JADA_GetWhoAmI(void)
{
    return(myWhoAmI);
}

DllExport int __stdcall JADA_Sleep(int iMilliSecs)
{
#ifdef UNIX
    usleep (iMilliSecs);
#else
    Sleep (iMilliSecs);
#endif

    return(0);
}

DllExport JADA_Timet __stdcall JADA_GetNow(void)
{
    JADA_Timet  myRet = 0;
    time_t      myTime = time(0);

    if (sizeof(myTime) == 4) {
        myRet = (JADA_Timet) myTime;
    }
    else {
        myRet = (JADA_Timet) myTime;
    }

    return(myRet);
}

int JADA_MkTemp(char *szFileName, int iLength)
{
    char    *szPtr;
#ifndef UNIX
    char *_mktemp(char *szFileName);
#endif

#ifdef UNIX
    strcpy(szFileName, "/tmp/jadaXXXXXX");
    szPtr = mktemp(szFileName);
#else
    strcpy(szFileName, "C:\\jada\\tmp\\jadaXXXXXX");
    szPtr = _mktemp(szFileName);
#endif
    CORE_ReturnValIfFail(szPtr != NULL, -1, ;, "errore nella lettura del payload");
    strcpy(szFileName, szPtr);

    return(0);
}

DllExport char * __stdcall JADA_TimeToString(JADA_Timet myTime, char *szTimeString, int iLength)
{
    static FixString    myTimeString;
    struct tm           *tminfo;
    char                *szPtr;
    time_t              timSelf = (time_t) myTime;

    tminfo = localtime(&timSelf);
    if (tminfo != NULL) {
        sprintf(myTimeString, "%02.2d/%02.2d/%04.4d %02.2d:%02.2d:%02.2d", 
                tminfo->tm_mday, tminfo->tm_mon + 1, tminfo->tm_year + 1900,
                tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec);
    }
    else {
        sprintf(myTimeString, "%s", ctime(&timSelf));
    }

    szPtr = strchr(myTimeString, '\n');
    if (szPtr != NULL) {
        *szPtr = '\0';
    }
  
    if (szTimeString != NULL) {
        strcpy(szTimeString, myTimeString);
    }

    return (myTimeString);
}

DllExport char * __stdcall JADA_WhoAmIToString(int iWhoAmI, char *szString, int iLength)
{
    static FixString    myString;

    switch (iWhoAmI) {
    case JADA_WHOAMI_UNKNOWN:
        strcpy(myString, "Sconosciuto");
        break;
    case JADA_WHOAMI_AGENT:
        strcpy(myString, "Agent");
        break;
    case JADA_WHOAMI_MANAGER:
        strcpy(myString, "Manager");
        break;
    case JADA_WHOAMI_SERVER:
        strcpy(myString, "Server");
        break;
    default:
        strcpy(myString, "???");
        break;
    }
  
    if (szString != NULL) {
        strcpy(szString, myString);
    }

    return (myString);
}

DllExport char * __stdcall JADA_SessionIdToToken(JADA_SessId iSessId, char *szSessionIdString, int iLength)
{
    static FixString    mySessionIdString; /* Formato stringa: XXXX-XXXX-XXXX */
    FixString           szTmp;

    sprintf(szTmp, "%012.12d", iSessId); 
    memset(mySessionIdString, 0, sizeof(mySessionIdString));
    strncpy(&mySessionIdString[0], &szTmp[0], 4);
    mySessionIdString[4] = '-';
    strncpy(&mySessionIdString[5], &szTmp[4], 4);
    mySessionIdString[9] = '-';
    strncpy(&mySessionIdString[10], &szTmp[8], 4);

    if (szSessionIdString != NULL) {
        strcpy(szSessionIdString, mySessionIdString);
    }

    return (mySessionIdString);
}

DllExport int __stdcall JADA_IntToRequestId(int iReqId, JADA_ReqId *piReqId)
{
    piReqId->iReqId = iReqId;

    return (0);
}

DllExport int __stdcall JADA_TokenToSessionId(char *szSessionIdString, JADA_SessId *piSessId)
{
    char        *src, *dst;
    FixString   szTmp;

    strcpy(szTmp, szSessionIdString);
    for (src = dst = szTmp; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != '-')
            dst++;
    }
    *dst = '\0';

    piSessId->iSessId = atoi(szTmp);

    return (0);
}

/******************************************************************************/
/*                                                                            */
/*   M E N U     M A N A G E M E N T                                          */
/*                                                                            */
/******************************************************************************/
DllExport CORE_Callback __stdcall JADA_MnuExit(void)
{
    exit(0);
}

DllExport CORE_Callback __stdcall JADA_MnuNext(void)
{
    FixString   szNext;

    sprintf(szNext, ".%d", myOption);
    strcat(myMenuLevel, szNext);

    return(1);
}

DllExport CORE_Callback __stdcall JADA_MnuBack(void)
{
    char    *szPtr;

    szPtr = strrchr(myMenuLevel, '.');
    if (szPtr != NULL) {
        *szPtr = '\0';
    }

    return(1);
}

DllExport int __stdcall JADA_WaitKey(char *szPrompt)
{
    char    szBuffer[32];

    if (strlen(szPrompt) > 0) {
        printf("%s ", szPrompt);
    }
    printf("Press enter to continue...\n");
    fgets(szBuffer, sizeof(szBuffer), stdin);

    return(0);
}

DllExport int __stdcall JADA_StartMenu(char *szCmdName, char *szProcessName, JADA_Command *Command)
{
    int         iRet, i;
    FixString   szBuffer;

    /*- Initialize the internal variables */
    strcpy(myMenuLevel, "1");

    do {
#ifdef UNIX
        system("clear");
#else
        system("cls");
#endif
        printf("%s %s\n\n", szCmdName, szProcessName);
        for (i = 0; Command[i].szMenuLevel != NULL; i++) {
            if (strcmp(Command[i].szMenuLevel, myMenuLevel) == 0) {
                printf("    %2d. %s\n", Command[i].iOption, Command[i].szDescription);
            }
        }
        printf("\n");
        printf("Scegli l'opzione: ");
        fgets(szBuffer, sizeof(szBuffer), stdin);

        myOption = atoi(szBuffer);
        for (i = 0; Command[i].szMenuLevel != NULL; i++) {
            if (strcmp(Command[i].szMenuLevel, myMenuLevel) == 0) {
                if (Command[i].iOption == myOption) {
                    if (Command[i].cbkFunction != NULL) {
                        iRet = (*Command[i].cbkFunction)();
                        if (Command[i].iPressKeyWhenFinished != FALSE) {
                            JADA_WaitKey("Comando eseguito.");
                        }
                    }
                    break;
                }
            }
        }
    } while (1);

    /*- Return the result */
    return(0);
}

/******************************************************************************
 *
 *   M E M O R Y
 *
 ******************************************************************************/
static void JADA_DumpMemory(void)
{
    int     i;
    FILE    *fp;

    _unlink(myMemFileName);

    fp = fopen(myMemFileName, "w+");
    for (i = 0; i < JADA_MEMORY_BLOCKS; i++) {
        fprintf(fp, "%d;%x;%s;%d\n", i, lstMemory[i].szPtr,
                lstMemory[i].szFile, lstMemory[i].iLine);
    }
    fclose(fp);
}

void *JADA_Calloc(char *szFile, int iLine, int iEntries, int iSize)
{
    void    *szPtr;
    int     i;

    szPtr = calloc(iEntries, iSize);

    if (myMemoryDebug != 0) {
        for (i = 0; i < JADA_MEMORY_BLOCKS; i++) {
            if (lstMemory[i].szPtr == NULL) {
                strcpy(lstMemory[i].szFile, szFile);
                lstMemory[i].iLine = iLine;
                lstMemory[i].szPtr = szPtr;
                break;
            }
        }

        if (i == JADA_MEMORY_BLOCKS) {
            JADA_DumpMemory();
            exit(0);
        }
    }

    return(szPtr);
}

void *JADA_Realloc(void *szOldPtr, int iSize)
{
    void    *szPtr;
    int     i;

    szPtr = realloc(szOldPtr, iSize);

    if (myMemoryDebug != 0) {
        for (i = 0; i < JADA_MEMORY_BLOCKS; i++) {
            if (lstMemory[i].szPtr == szOldPtr) {
                lstMemory[i].szPtr = szPtr;
                break;
            }
        }
    }

    return(szPtr);
}

void JADA_Free(char *szFile, int iLine, void *szPtr, int iLength)
{
    int     i;

    if (iLength > 0) {
        memset(szPtr, 0, iLength);
    }
    free(szPtr);

    if (myMemoryDebug != 0) {
        for (i = 0; i < JADA_MEMORY_BLOCKS; i++) {
            if (lstMemory[i].szPtr == szPtr) {
                memset(&lstMemory[i], 0, sizeof(lstMemory[i]));
                break;
            }
        }
    }
}

/******************************************************************************
 *                                                                            
 *   B A S E 6 4     S E R V I C E S                          
 *                                                                            
 ******************************************************************************/
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

char *JADA_Base64Encode(const unsigned char *data,
                    int input_length,
                    int *output_length)
{
    char    *encoded_data;
    int     i, j;

    *output_length = 4 * ((input_length + 2) / 3);
    encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

static void build_decoding_table() 
{
    int     i;

    decoding_table = malloc(256);

    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

static void JADA_Base64Stop() 
{
    free(decoding_table);
}

unsigned char *JADA_Base64Decode(const char *data,
                             int input_length,
                             int *output_length)
{
    int             i, j;
    unsigned char   *decoded_data;

    if (decoding_table == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}

int JADA_GetTimeDelay(int iFromSecs, int iFromMsecs, int iToSecs, int iToMsecs)
{
    int     iSecs, iMsecs, iDelay;

    /* Calcola il tempo di attraversamento del pacchetto */
    iSecs = (iToSecs - iFromSecs);
    iMsecs = (iToMsecs - iFromMsecs);
    if (iMsecs < 0) {
        iMsecs += 1000;
        iSecs -= 1;
    }

    iDelay = (iSecs * 1000 + iMsecs);
    return(iDelay);
}

/******************************************************************************
 *
 *      S E R I A L I Z A T I O N
 *
 *****************************************************************************/
int JADA_BufSetInt32(char *szBuffer, int iLength, int iValue, int *piOffset)
{
    int     *piInt;
    int     iNewLength = *piOffset + sizeof(iValue);

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufSetInt32: %d vs %d\n", iLength, iNewLength));

    /*- Set the value in the buffer */
    piInt = (int *) &szBuffer[*piOffset];
    *piInt = htonl(iValue);
    *piOffset += sizeof(iValue);

    /*- Return the result code */
    return(0);
}

int JADA_BufGetInt32(char *szBuffer, int iLength, int *piValue, int *piOffset)
{
    int     *piInt;
    int     iNewLength = *piOffset + sizeof(*piValue);

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufGetInt32: %d vs %d\n", iLength, iNewLength));

    /*- Get the value from the buffer */
    piInt = (int *) &szBuffer[*piOffset];
    *piValue = ntohl(*piInt);
    *piOffset += sizeof(*piValue);
    CORE_ReturnValIfFail(*piOffset <= iLength, -1, ;, "buffer overflow");

    /*- Return the result code */
    return(0);
}

int JADA_BufSetString(char *szBuffer, int iLength, char *szValue, int iValueLength, int *piOffset)
{
    char    *szString;
    int     iNewLength = *piOffset + iValueLength;

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufSetString: %d vs %d\n", iLength, iNewLength));

    /*- Set the value in the buffer */
    szString = &szBuffer[*piOffset];
    memset(szString, 0, iValueLength);
    memcpy(szString, szValue, iValueLength);
    *piOffset += iValueLength;

    /*- Return the result code */
    return(0);
}

int JADA_BufGetString(char *szBuffer, int iLength, char *szValue, int iValueLength, int *piOffset)
{
    char    *szString;
    int     iNewLength = *piOffset + iValueLength;

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufGetString: %d vs %d\n", iLength, iNewLength));

    /*- Set the value in the buffer */
    szString = &szBuffer[*piOffset];
    memset(szValue, 0, iValueLength);
    memcpy(szValue, szString, iValueLength);
    *piOffset += iValueLength;

    /*- Return the result code */
    return(0);
}

int JADA_BufSetJadaMsgId(char *szBuffer, int iLength, JADA_MsgId iValue, int *piOffset)
{
    int     *piInt, myValue;
    int     iNewLength = *piOffset + sizeof(iValue);

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufSetInt32: %d vs %d\n", iLength, iNewLength));
    CORE_ReturnValIfFail(sizeof(iValue) == sizeof(myValue), -1, ;, StringFormat("dimensioni diverse per i tipi: %d vs %d\n", sizeof(iValue), sizeof(myValue)))

    /*- Set the value in the buffer */
    memcpy(&myValue, &iValue, sizeof(iValue));
    piInt = (int *) &szBuffer[*piOffset];
    *piInt = htonl(myValue);
    *piOffset += sizeof(iValue);

    /*- Return the result code */
    return(0);
}

int JADA_BufGetJadaMsgId(char *szBuffer, int iLength, JADA_MsgId *piValue, int *piOffset)
{
    int     *piInt, myValue;
    int     iNewLength = *piOffset + sizeof(*piValue);

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufGetInt32: %d vs %d\n", iLength, iNewLength));
    CORE_ReturnValIfFail(sizeof(*piValue) == sizeof(myValue), -1, ;, StringFormat("dimensioni diverse per i tipi: %d vs %d\n", sizeof(*piValue), sizeof(myValue)));

    /*- Get the value from the buffer */
    piInt = (int *) &szBuffer[*piOffset];
    myValue = ntohl(*piInt);
    memcpy(piValue, &myValue, sizeof(myValue));
    *piOffset += sizeof(*piValue);
    CORE_ReturnValIfFail(*piOffset <= iLength, -1, ;, "buffer overflow");

    /*- Return the result code */
    return(0);
}

int JADA_BufSetJadaSessId(char *szBuffer, int iLength, JADA_SessId iValue, int *piOffset)
{
    int     *piInt, myValue;
    int     iNewLength = *piOffset + sizeof(iValue);

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufSetInt32: %d vs %d\n", iLength, iNewLength));
    CORE_ReturnValIfFail(sizeof(iValue) == sizeof(myValue), -1, ;, StringFormat("dimensioni diverse per i tipi: %d vs %d\n", sizeof(iValue), sizeof(myValue)))

    /*- Set the value in the buffer */
    memcpy(&myValue, &iValue, sizeof(iValue));
    piInt = (int *) &szBuffer[*piOffset];
    *piInt = htonl(myValue);
    *piOffset += sizeof(iValue);

    /*- Return the result code */
    return(0);
}

int JADA_BufGetJadaSessId(char *szBuffer, int iLength, JADA_SessId *piValue, int *piOffset)
{
    int     *piInt, myValue;
    int     iNewLength = *piOffset + sizeof(*piValue);

    /*- Check if enough space is in the buffer */
    CORE_ReturnValIfFail(iLength >= iNewLength, -1, ;, StringFormat("buffer overflow nella JADA_BufGetInt32: %d vs %d\n", iLength, iNewLength));
    CORE_ReturnValIfFail(sizeof(*piValue) == sizeof(myValue), -1, ;, StringFormat("dimensioni diverse per i tipi: %d vs %d\n", sizeof(*piValue), sizeof(myValue)));

    /*- Get the value from the buffer */
    piInt = (int *) &szBuffer[*piOffset];
    myValue = ntohl(*piInt);
    memcpy(piValue, &myValue, sizeof(myValue));
    *piOffset += sizeof(*piValue);
    CORE_ReturnValIfFail(*piOffset <= iLength, -1, ;, "buffer overflow");

    /*- Return the result code */
    return(0);
}

/******************************************************************************
 *
 *      A R C A     I D
 *
 *****************************************************************************/
int JADA_StringToJadaId(char *szString, void *piId)
{
    int         *piJadaId = (int *)piId;

    *piJadaId = atoi(szString);

    return(0);
}

char *JADA_JadaIdToString(void *piId, char *szString)
{
    static Identifier   szTmp;
    int                 *piJadaId = (int *)piId;

    if (piId != NULL) {
        sprintf(szTmp, "%d", *piJadaId);
    }
    else {
        strcpy(szTmp, "0");
    }

    if (szString != NULL) {
        strcpy(szString, szTmp);
    }
    return(szTmp);
}

int JADA_SessIdToMsgId(JADA_SessId *piSessId, JADA_MsgId *piMsgId)
{
    memcpy(piMsgId, piSessId, sizeof(*piSessId));

    return(0);
}

int JADA_ReqIdToMsgId(JADA_ReqId *piReqId, JADA_MsgId *piMsgId)
{
    memcpy(piMsgId, piReqId, sizeof(*piReqId));

    return(0);
}

int JADA_ConnIdToMsgId(JADA_ConnId *piConnId, JADA_MsgId *piMsgId)
{
    memcpy(piMsgId, piConnId, sizeof(*piConnId));

    return(0);
}

int JADA_ConnIdToSessId(JADA_ConnId *piConnId, JADA_SessId *piSessId)
{
    memcpy(piSessId, piConnId, sizeof(*piConnId));

    return(0);
}

void JADA_ClearJadaId(void *piId)
{
    memset(piId, 0, sizeof(int));
}

int JADA_CompareJadaId(void *piId1, void *piId2)
{
    int     iRet;

    iRet = memcmp(piId1, piId2, sizeof(int));

    if (iRet == 0)
        return(TRUE);
    return(FALSE);
}

int JADA_IsValidJadaId(void *piId)
{
    int     iId;

    memcpy(&iId, piId, sizeof(iId));
    return(iId > 0);
}

int JADA_StringToAgtId(char *szString, JADA_AgtId *piAgtId)
{
    piAgtId->iAgtId = atoi(szString);

    return(0);
}

int JADA_StringToMgrId(char *szString, JADA_MgrId *piMgrId)
{
    piMgrId->iMgrId = atoi(szString);

    return(0);
}

int JADA_StringToConnId(char *szString, JADA_ConnId *piConnId)
{
    piConnId->iConnId = atoi(szString);

    return(0);
}

int JADA_StringToSessId(char *szString, JADA_SessId *piSessId)
{
    piSessId->iSessId = atoi(szString);

    return(0);
}

int JADA_StringToReqId(char *szString, JADA_ReqId *piReqId)
{
    piReqId->iReqId = atoi(szString);

    return(0);
}

int JADA_StringToQueueId(char *szString, JADA_QueueId *piQueueId)
{
    piQueueId->iQueueId = atoi(szString);

    return(0);
}

int JADA_StringToTimet(char *szString, time_t *ptimTime)
{
    char        *szPtr;
    FixString   myString;
    struct tm   myTime;

    if (szString != NULL) {
        if (strlen(szString) > 0) {
            strcpy(myString, szString);
            szPtr = strtok(&myString[0], "-");
            if (szPtr != NULL) 
                myTime.tm_year = atoi(szPtr);
            szPtr = strtok(NULL, "-");
            if (szPtr != NULL) 
                myTime.tm_mon = atoi(szPtr);
            szPtr = strtok(NULL, " ");
            if (szPtr != NULL) 
                myTime.tm_mday = atoi(szPtr);
            szPtr = strtok(NULL, ":");
            if (szPtr != NULL) 
                myTime.tm_hour = atoi(szPtr);
            szPtr = strtok(NULL, ":");
            if (szPtr != NULL) 
                myTime.tm_min = atoi(szPtr);
            szPtr = strtok(NULL, ":");
            if (szPtr != NULL) 
                myTime.tm_sec = atoi(szPtr);
            *ptimTime = mktime (&myTime);
        }
    } 

    return(0);
}
