#include "jada.h"
#define extern
#include "manager.h"
#undef extern

#ifdef UNIX
static FixString myHomeDir = "/home/jada/jada";
#else
static FixString myHomeDir = "C:\\jada";
#endif


static JADA_Command Command[] = {
    "1",    1, "Riprendi una richiesta",                JADA_MnuNext,               FALSE,                 
    "1",    2, "Accetta una richiesta",                 JADA_MnuNext,               FALSE,                 
    "1",    4, "Gestisci la richiesta",                 JADA_MnuNext,               FALSE,                 
    "1",    5, "Chiudi la richiesta",                   JADA_MnuNext,               FALSE,                 
    "1",    6, "Funzioni varie",                        JADA_MnuNext,               FALSE,                 
    "1",   99, "Esci",                                  JADA_MnuExit,               FALSE, 
    /* Use Case: M.3 */
    "1.1",  1, "Elenca le richieste pending",           AMGR_MnuListPendingRequests,TRUE,                 
    "1.1",  2, "Riprendi una richiesta di supporto",    AMGR_MnuLoadSupport,        TRUE,                 
    "1.1", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: S.1 */
    "1.2",  1, "Elenca le richieste attive",            AMGR_MnuListRequests,       TRUE,                 
    "1.2",  2, "Accetta una richiesta di supporto",     AMGR_MnuAcceptSupport,      TRUE,                 
    "1.2", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: S.2 */
    "1.4",  1, "Invia messaggi in chat",                AMGR_MnuSendMessages,       TRUE,                 
    "1.4",  2, "Richiedi una chiamata",                 AMGR_MnuRequestCall,        TRUE,                 
    "1.4",  3, "Richiedi accesso al un desktop remoto", AMGR_MnuRequestVnc,         TRUE,                 
    "1.4",  4, "Richiedi accesso FTP",                  AMGR_MnuRequestFtp,         TRUE,                 
    "1.4", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: S.7 */
    "1.5",  1, "Chiudi la richiesta",                   AMGR_MnuCloseSupport,       TRUE,                 
    "1.5", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Funzioni varie */
    "1.6",  1, "Leggi i contatori",                     AMGR_MnuGetRequestCount,    TRUE, 
    "1.6",  2, "Leggi i valori delle combo",            AMGR_MnuGetCombo,           TRUE, 
    "1.6",  3, "Leggi i parametri dell'agent",          AMGR_MnuListParameters,     TRUE, 
    "1.6", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    NULL,   0, NULL,                                    NULL,                       -1
};

void AMGR_ExitProcess(void)
{
    int     iRet;

    iRet = JADA_Stop();
}

int main(int argc, char *argv[])
{
    int         iRet;

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    /* Winsock needs additional startup activities */
    WSADATA wsadata;
    WSAStartup(MAKEWORD(1,1), &wsadata);
#endif

    atexit(AMGR_ExitProcess);

    iRet = JADA_Start(JADA_WHOAMI_MANAGER, myHomeDir);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "Errore generico");

    iRet = AMGR_MnuOpenConnection(TRUE);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore nella connessione al server");

    iRet = JADA_StartMenu("JADA (AREAS Customer Assistant) Manager", "manager", &Command[0]);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore generico");    

    /*- Return the result */
    return(0);
}
