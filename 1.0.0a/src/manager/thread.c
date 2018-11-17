#include "jada.h"
#include "jada_internal.h"
#include "jada_events.h"

static void *JADA_ManageEventThread(void *FuncArg);


int JADA_ManageEvents(JADA_Connection connSelf)
{
    int             iRet;
    pthread_t       pThreadId;
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&pThreadId, &attr, JADA_ManageEventThread, connSelf);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

static char *JADA_WhoamiToString(int iWhoAmI)
{
    static ShortString   myWhoAmIString;

    switch (iWhoAmI) {
    case JADA_WHOAMI_AGENT:
        strcpy(myWhoAmIString, "Agent");
        break;
    case JADA_WHOAMI_MANAGER:
        strcpy(myWhoAmIString, "Manager");
        break;
    case JADA_WHOAMI_SERVER_FTP:
        strcpy(myWhoAmIString, "FtpServer");
        break;
    case JADA_WHOAMI_SERVER_MEDIA:
        strcpy(myWhoAmIString, "CallServer");
        break;
    case JADA_WHOAMI_SERVER_SUPPORT:
        strcpy(myWhoAmIString, "SupportServer");
        break;
    case JADA_WHOAMI_SERVER_VNC:
        strcpy(myWhoAmIString, "VncServer");
        break;
    default:
        strcpy(myWhoAmIString, "???");
        break;
    }

    return(myWhoAmIString);
}


/******************************************************************************/
/*                                                                            */
/*   T H R E A D S                                                            */
/*                                                                            */
/******************************************************************************/
static void *JADA_ManageEventThread(void *FuncArg)
{
    int             iRet;
    JADA_Event      evtRaised;
    JADA_Connection connSelf = (JADA_Connection) FuncArg;
    int             iType;

    while (TRUE) {
        iRet = JADA_ConnGetNextEvent(connSelf, 1000, &evtRaised);
        CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);

        iRet = JADA_EvtGetType(evtRaised, &iType);
        CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);

        JADA_LogMessage("***** Evento ricevuto: %s\n", JADA_EvtTypeToString(iType));

        switch (iType) {
        case JADA_EVT_CONNECTION_CONNECTED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La connessione %d e' stata connessa il %s\n",
                     evtParams.iRequestId, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_CONNECTION_OPENED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La connessione %d e' stata aperta il %s\n",
                     evtParams.iRequestId, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_SUPPORT_OPENED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La richiesta %d e' stata aperta il %s\n",
                     evtParams.iRequestId, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_SUPPORT_ACCEPTED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      Richiesta %d accettata il %s: sei connesso a %s\n",
                     evtParams.iRequestId, JADA_TimeToString(evtParams.timTime, NULL, 0), evtParams.szUtente);
            }
            break;

        case JADA_EVT_SUPPORT_LOADED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La richiesta %d di %s e' stata presa in carico il %s\n",
                     evtParams.iRequestId, evtParams.szUtente, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_SUPPORT_CONNECTED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La richiesta %d di %s e' stata connessa il %s\n",
                     evtParams.iRequestId, evtParams.szUtente, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_SUPPORT_SOLICITED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La richiesta %d e' stata sollecitata il %s\n",
                     evtParams.iRequestId, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_SUPPORT_CLOSED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La richiesta %d e' stata chiusa il %s\n",
                     evtParams.iRequestId, JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_MSG_RESULT:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      L'operazione e' terminata con esito %d (%s)\n",
                     evtParams.iResult, evtParams.szDescription);
            }
            break;

        case JADA_EVT_SESSIONS_LIST:
        case JADA_EVT_REQUESTS_LIST:
            {
            JADA_EventData  evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("Ricevuta lista tramite lo stream %d.\n", evtParams.iStreamId);
            }
            break;

        case JADA_EVT_LIST_COUNTERS:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      Ci sono attualmente %d richieste in coda (%d totali), %d richieste pendenti (%d totali).\n",
                     evtParams.iAreaRequests, evtParams.iTotalRequests,
                     evtParams.iAreaPendingRequests, evtParams.iTotalPendingRequests);
            }
            break;

        case JADA_EVT_CHATTEXT_RECEIVED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      %s >> %s\n", JADA_WhoamiToString(evtParams.iWhoAmI), evtParams.szChatText);
            }
            break;

        case JADA_EVT_COMBO_VALUES:
            {
            JADA_EventData  evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("Ricevuta lista valori di combo nello stream %d.\n", evtParams.iStreamId);
            }
            break;

        case JADA_EVT_CALL_OPENED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      La chiamata audio/video e' stata aperta il %s\n",
                     JADA_TimeToString(evtParams.timTime, NULL, 0));
            }
            break;

        case JADA_EVT_DESKTOP_REQUESTED:
            {
            JADA_EventData evtParams;
            JADA_VNC vncSelf;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      Desktop remoto richiesto il %s, la password e' %s\n",
                     JADA_TimeToString(evtParams.timTime, NULL, 0), evtParams.szPassword);

            vncSelf = JADA_VncNew(__FILE__, __LINE__, connSelf);
            CORE_SilentGotoIfFail(vncSelf != NULL, lblNext, ;);
            iRet = JADA_VncAcceptDesktop(vncSelf, TRUE);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            }

        case JADA_EVT_DESKTOP_OPENED:
            {
            JADA_EventData evtParams;

            iRet = JADA_EvtGetData(evtRaised, &evtParams);
            CORE_SilentGotoIfFail(iRet == 0, lblNext, ;);
            JADA_LogMessage("      Desktop remoto aperto il %s, la password e' %s\n",
                     JADA_TimeToString(evtParams.timTime, NULL, 0), evtParams.szPassword);
            }
            break;

        default:
            JADA_LogMessage("      Evento non previsto (%s)\n", JADA_EvtTypeToString(iType));
            break;
        }
        JADA_LogMessage("      Premi Invio per fare refresh...\n");

lblNext:
        ;
    }

    JADA_LogMessage("ManageEventThread e' stato chiuso a causa di un errore.\n");

    return(NULL);
}

