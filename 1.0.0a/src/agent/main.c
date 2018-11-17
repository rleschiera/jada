#include "jada.h"
#define extern
#include "agent.h"
#undef extern

#ifdef UNIX
static FixString myHomeDir = "/home/jada/jada";
#else
static FixString myHomeDir = "C:\\jada";
#endif

static JADA_Command Command[] = {
    "1",    1, "Accoda una richiesta",                  JADA_MnuNext,               FALSE,                 
    "1",    2, "Riprendi una richiesta",                JADA_MnuNext,               FALSE,                 
    "1",    3, "Apri una richiesta",                    JADA_MnuNext,               FALSE,                 
    "1",    4, "Gestisci una richiesta",                JADA_MnuNext,               FALSE,                 
    "1",    5, "Chiudi la richiesta",                   JADA_MnuNext,               FALSE,                 
    "1",   99, "Esci",                                  JADA_MnuExit,               FALSE, 
    /* Use Case: C.3 */
    "1.1",  1, "Crea una richiesta di supporto",        AAGT_MnuCreateSupport,      TRUE,                 
    "1.1",  2, "Chiudi la connessione",                 AAGT_MnuCloseConnection,    TRUE,                 
    "1.1", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: M.3 */
    "1.2",  1, "Riprendi una richiesta di supporto",    AAGT_MnuConnectSupport,     TRUE,                 
    "1.2", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: S.1 */
    "1.3",  1, "Crea una richiesta di supporto",        AAGT_MnuCreateSupport,      TRUE,                 
    "1.3", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: S.2 */
    "1.4",  1, "Invia messaggi in chat",                AAGT_MnuSendMessages,       TRUE,                 
    "1.4",  2, "Accetta una chiamata",                  AAGT_MnuAcceptCall,         TRUE,                 
    "1.4",  3, "Accetta il desktop remoto",             AAGT_MnuAcceptVnc,          TRUE,                 
    "1.4",  4, "Accetta la sessione FTP",               AAGT_MnuAcceptFtp,          TRUE,                 
    "1.4", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    /* Use Case: S.7 */
    "1.5",  1, "Chiudi la richiesta",                   AAGT_MnuCloseSupport,       TRUE,                 
    "1.5", 99, "Indietro",                              JADA_MnuBack,               FALSE,                 
    NULL,   0, NULL,                                    NULL,                       -1
};

void AAGT_ExitProcess(void)
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

    atexit(AAGT_ExitProcess);

    iRet = JADA_Start(JADA_WHOAMI_AGENT, myHomeDir);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore generico");

    iRet = AAGT_MnuOpenConnection(TRUE);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore nella connessione al server");

    iRet = JADA_StartMenu("JADA (AREAS Customer Assistant)", "agent", &Command[0]);
    CORE_ReturnValIfFail(iRet == 0, -1, JADA_WaitKey("");, "errore generico");

    /*- Return the result */
    return(0);
}
