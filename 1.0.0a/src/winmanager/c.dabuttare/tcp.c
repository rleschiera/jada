#include "eass.h"
#include "agent.h"

static void *EASS_RemoteDesktopThread(void *FuncArg);

int __stdcall ECOM_ConnectToRemoteDesktopServer(void)
{
    int                 iRet;
    pthread_t           pThreadId;
    pthread_attr_t      attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&pThreadId, &attr, EASS_RemoteDesktopThread, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

static void *EASS_RemoteDesktopThread(void *FuncArg)
{
	int					        iRet;
    int                         iBytesRecv, iBytesSent;
	int                         iSocketServer = -1, iSocketVNC = -1;
	fd_set				        fdSet;
    struct timeval		        tv = {15,0};
    int					        iError;
    unsigned char               buf[1024];
    int                         *piSessionId = FuncArg;
    FixString                   szServerName;
    EASS_MsgRemoteDesktopOpened msgOut;

    printf("\nNuova gestione desktop remoto per la richiesta %d", EASS_RequestId);
    iRet = ECOM_IniGetString("server", "ip", szServerName, sizeof(szServerName));
    CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore generico");

    iSocketServer = ECOM_TcpConnectToPort(szServerName, EASS_REMOTE_DESKTOP_PORT + EASS_RequestId);
    CORE_GotoIfFail(iSocketServer >= 0, lblError, ;, "Errore generico");

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iResult = 0;
    iRet = ECOM_SendRequestMessage(EASS_MSG_REMOTEDESKTOP_OPENED, &msgOut, sizeof(msgOut));
    CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore generico");

    while (TRUE) {
        FD_ZERO(&fdSet);
        FD_SET(iSocketServer, &fdSet);
        if (iSocketVNC >= 0) {
            FD_SET(iSocketVNC, &fdSet);
        }

        iRet = select(FD_SETSIZE, &fdSet, NULL, NULL, &tv);
        CORE_GotoIfFail(iRet >= 0, lblError, iError = EASS_ERR_TCP_SELECT;, "Errore generico");

        if (FD_ISSET(iSocketVNC, &fdSet)) {
            iBytesRecv = recv(iSocketVNC, buf, sizeof(buf), 0);
            // printf("\nNuovi dati ricevuti da VNC (%d)", iBytesRecv);
            CORE_GotoIfFail(iBytesRecv > 0, lblError, iError = EASS_ERR_TCP_RECV;, "Errore generico");
            iBytesSent = send(iSocketServer, buf, iBytesRecv, 0);
            // printf("\nNuovi dati inviati verso il server (%d)", iBytesSent);
            CORE_GotoIfFail(iBytesSent == iBytesRecv , lblError, iError = EASS_ERR_TCP_SEND;, "Errore nella lettura dei dati RD da VNC server");
        }
        else if (FD_ISSET(iSocketServer, &fdSet)) {
            iBytesRecv = recv(iSocketServer, buf, sizeof(buf), 0);
            // printf("\nNuovi dati ricevuti dal server (%d)", iBytesRecv);
            CORE_GotoIfFail(iBytesRecv > 0, lblError, iError = EASS_ERR_TCP_RECV;, "Errore nella lettura dei dati RD dal server");
            if (iSocketVNC < 0) {
                iRet = ECOM_ProcExists(EASS_VncServer);
                if (iRet != 0) {
                    iRet = ECOM_ProcExecute(EASS_VncServer, "");
                    CORE_GotoIfFail(iRet == 0, lblError, ;, "Errore generico");
                    ECOM_Sleep(1000);
                }

                iSocketVNC = ECOM_TcpConnectToPort(ECOM_GetMyIP(), EASS_VNCSERVER_PORT);
                CORE_GotoIfFail(iSocketVNC >= 0, lblError, ;, "Errore nella connessione al VNC Server");
                printf("\nNuova connessione verso VNC, porta %d", EASS_VNCSERVER_PORT);
            }
            else {
                iBytesSent = send(iSocketVNC, buf, iBytesRecv, 0);
                // printf("\nNuovi dati inviati verso VNC (%d)", iBytesSent);
                CORE_GotoIfFail(iBytesSent == iBytesRecv , lblError, iError = EASS_ERR_TCP_SEND;, "Errore generico");
            }
        }
	}

lblError:
    if (iSocketServer >= 0) {
        closesocket(iSocketServer);
    }
    if (iSocketVNC >= 0) {
        closesocket(iSocketVNC);
    }

    iRet = ECOM_ProcExists(EASS_VncServer);
    if (iRet == 0) {
        iRet = ECOM_ProcKill(EASS_VncServer);
    }

    printf("EASS_RemoteDesktopThread e' stato chiuso.\n");

    return(NULL);
}
