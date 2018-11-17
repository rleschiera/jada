#include "jada.h"
#define extern
#include "server.h"
#undef extern
#ifdef UNIX
#include <signal.h>
#endif

static int       myDebugMode = FALSE;
static int       myDaemonMode = FALSE;
#ifdef UNIX
static FixString myHomeDir = "/home/jada/jada";
#else
static FixString myHomeDir = "C:\\jada";
#endif
static FixString myPidFile = "";

extern JADA_Command Command[];


/* Per risolvere il simbolo in jadalib */
int  JADA_ProcKill(char *szProcName)
{
    return(0);
}

void ASRV_ExitProcess(void)
{
    int     iRet;

    iRet = ASRV_DbStop();
}


static int ScanInputParameters(int argc, char *argv[])
{
    int         i;

    /*- Verifica i parametri di input e li elabora */
    for (i = 1; i < argc && argv[i][0] == '-'; i++) {
        if (strcmp(&argv[i][1], "debug") == 0) {
            myDebugMode = TRUE;
		}
		else if (strcmp(&argv[i][1], "home") == 0) {
            strcpy(myHomeDir, argv[++i]);
		}
		else if (strcmp(&argv[i][1], "pid") == 0) {
            strcpy(myPidFile, argv[++i]);
		}
		else if (strcmp(&argv[i][1], "daemon") == 0) {
            myDaemonMode = TRUE;
		}
    }

    return(0);
}

static int StartAsDaemon(void)
{
#ifdef UNIX
    pid_t   pid, sid;
    FILE    *fp;
        
    /* Fork off the parent process */
    pid = fork();
    CORE_AssertIfFail(pid >= 0, ;, "Errore generico");

    if (pid > 0) {
        if (strlen(myPidFile) > 0) {
            fp = fopen(myPidFile, "w");
            if (fp != NULL) {
                fprintf(fp, "%d", pid);
                fclose(fp);
            }
        }
        exit(0);
    }

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(-1);
    }
        
    /* Change the current working directory */
    if ((chdir(myHomeDir)) < 0) {
        exit(-1);
    }
        
    /* Close out the standard file descriptors */
    close(0); // stdin
    close(1); // stdout
    close(2); // stderr
#endif

    return(0);        
}

int main(int argc, char *argv[])
{
    int         iRet;

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    /* Winsock needs additional startup activities */
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2), &wsadata);
#endif

    iRet = ScanInputParameters(argc, argv);

    atexit(ASRV_ExitProcess);
    // signal(SIGFPE, SIG_IGN);
    // signal(SIGILL, SIG_IGN);
    // signal(SIGINT, SIG_IGN);
    // signal(SIGSEGV, SIG_IGN);
    // signal(SIGTERM, SIG_IGN);
#ifdef UNIX
    // signal(SIGHUP, SIG_IGN);
    // signal(SIGQUIT, SIG_IGN);
    // signal(SIGKILL, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    // signal(SIGCHLD, SIG_IGN);
#endif

    if (myDaemonMode == TRUE) {
        StartAsDaemon();
    }

    iRet = JADA_Start(ASRV_WHOAMI, myHomeDir);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "Errore generico");
    JADA_WhoAmIToString(ASRV_WHOAMI, JADA_WhoAmI, sizeof(JADA_WhoAmI));
    iRet = ASRV_DbStart();
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "Errore generico");

    /*- Initialize the internal lists */
    lstConnections = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Connection *), NULL);
    CORE_ReturnValIfFail(lstConnections != NULL, -1, JADA_WaitKey("");, "Errore generico");
    lstSessions = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Session *), NULL);
    CORE_ReturnValIfFail(lstSessions != NULL, -1, JADA_WaitKey("");, "Errore generico");

    /*- Crea i meccanismi di ascolto */
    iRet = ASRV_WaitConnections();
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore generico");

    if (myDebugMode == TRUE) {
        iRet = JADA_StartMenu("JADA (Just Another Desktop Assistant)", JADA_WhoAmI, &Command[0]);
        CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore generico");
    }
    else {
        while(1)
            JADA_Sleep(60000);
    }

    /*- Return the result */
    return(0);
}
