#include "eass.h"

extern int              EASS_SessionId;
extern int              EASS_RequestId;

static int              ECOM_iWhoAmI;
static FixString        ECOM_szIniFileName = "";
static FixString        ECOM_szMyHostName = "";
static FixString        ECOM_szMyHostIP = "";
static FixString        ECOM_szServerName = "";
static SOCKET           ECOM_iSessSocket = -1;
static int              ECOM_Option = -1;
static FixString        ECOM_MenuLevel = "";


#ifndef EASS_SERVER
static void *ECOM_TcpSessionThread(void *FuncArg);
int __stdcall EASS_HandleSessionMessage(EASS_Message *msgIn);
#endif /* ! EASS_SERVER */

/******************************************************************************/
/*                                                                            */
/*   T C P     S E R V I C E S                                                */
/*                                                                            */
/******************************************************************************/
int __stdcall ECOM_TcpCreateListenPort(char *szHostName, int iPort)
{
    int                 iRet;
    struct hostent     *n;
    struct sockaddr_in  myAddress;
    int                 iSocket;

    memset(&myAddress, 0, sizeof(struct sockaddr_in));
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(iPort);
    myAddress.sin_addr.s_addr = inet_addr(szHostName);
    if (myAddress.sin_addr.s_addr == INADDR_NONE) {
        n = gethostbyname(szHostName);
        CORE_ReturnValIfFail(n != NULL, -1, ;, "Errore generico");
        memcpy(n->h_addr, (char *) &myAddress.sin_addr, n->h_length);
    }
    strcpy(ECOM_szMyHostName, szHostName);
    strcpy(ECOM_szMyHostIP, inet_ntoa(myAddress.sin_addr));

    /* Crea il listener socket */
    iSocket = socket(AF_INET, SOCK_STREAM, 0);
    CORE_ReturnValIfFail(iSocket >= 0, -1, ;, "Errore generico");
    iRet = bind(iSocket, (struct sockaddr *)&myAddress, sizeof(myAddress));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = listen(iSocket, 5);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iSocket);
}

int __stdcall ECOM_TcpConnectToPort(char *szHostName, int iPort)
{
    int                 iRet;
    struct hostent     *n;
    struct sockaddr_in  myAddress;
    int                 iLength;
    int                 iSocket;

    /* XXX */
    memset(&myAddress, 0, sizeof(struct sockaddr_in));
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(iPort);
    myAddress.sin_addr.s_addr = inet_addr(szHostName);
    if (myAddress.sin_addr.s_addr == INADDR_NONE) {
        n = gethostbyname(szHostName);
        CORE_ReturnValIfFail(n != NULL, -1, ;, "Errore generico");
        memcpy(n->h_addr, (char *) &myAddress.sin_addr, n->h_length);
    }

	iSocket = socket(AF_INET, SOCK_STREAM, 0);
    CORE_ReturnValIfFail(iSocket >= 0, -1, ;, "Errore generico");
    iRet = connect(iSocket, (struct sockaddr *)&myAddress, sizeof(myAddress));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iLength = sizeof(myAddress);  
    iRet = getsockname(iSocket, (struct sockaddr *)&myAddress, &iLength);  
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
    strcpy(ECOM_szMyHostIP, inet_ntoa(myAddress.sin_addr));

    if (iSocket < 0) {
        int i = 0;
    }
  
    return(iSocket);
}

int __stdcall ECOM_TcpSendMessage(int iSocket, char *msgOut, int iLength)
{
    int     iBytes;
    int     iSent = 0, iRemaining = iLength;

    CORE_ReturnValIfFail(iSocket >= 0, -1, ;, "Errore generico");
    do {
        iBytes = send(iSocket, &msgOut[iSent], iRemaining, 0);
        CORE_SilentReturnValIfFail(iBytes >= 0, -1, ;);
        iSent += iBytes;
        iRemaining -= iBytes;
    } while (iRemaining > 0);

    return(0);
}

int __stdcall ECOM_TcpReceiveMessage(int iSocket, char *msgIn, int iLength)
{
    int     iBytes;
    int     iReceived = 0, iRemaining = iLength;

    CORE_ReturnValIfFail(iSocket >= 0, -1, ;, "Errore generico");
    do {
        iBytes = recv(iSocket, &msgIn[iReceived], iRemaining, 0);
        CORE_SilentReturnValIfFail(iBytes >= 0, -1, ;);
        iReceived += iBytes;
        iRemaining -= iBytes;
    } while (iRemaining > 0);

    return(0);
}

/******************************************************************************/
/*                                                                            */
/*   I N I T I A L I Z A T I O N     S E R V I C E S                          */
/*                                                                            */
/******************************************************************************/
static int __stdcall ECOM_FileExists(char *szFullPath)
{
    int     iRet = -1;
    FILE    *fp = NULL;


    fp = fopen(szFullPath, "r");
    if (fp != NULL) {
        fclose(fp);
        iRet = 0;
    }

    return(iRet);
}

int __stdcall ECOM_Initialize(int iWhoAmI)
{
    int         iRet;
    FixString   szWhoAmI;

    switch (iWhoAmI) {
    case EASS_WHOAMI_AGENT:
        strcpy(szWhoAmI, "agent");
        break;
    case EASS_WHOAMI_MANAGER:
        strcpy(szWhoAmI, "manager");
        break;
    case EASS_WHOAMI_SERVER:
        strcpy(szWhoAmI, "server");
        break;
    default:
        CORE_ReturnValIfFail(0, -1, ;, "Errore generico");
    }

    ECOM_iWhoAmI = iWhoAmI;
    sprintf(ECOM_szIniFileName, "C:/engassist/config/%s.ini", szWhoAmI);
    iRet = ECOM_FileExists(ECOM_szIniFileName);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    if (strlen(ECOM_szMyHostName) == 0) {
        gethostname(ECOM_szMyHostName, sizeof(ECOM_szMyHostName));
    }

    return(0);
}

int __stdcall ECOM_IniGetString(char *szSection, char *szKey, char *szOut, int iOutLength)
{
    DWORD       dwRet;

    dwRet = GetPrivateProfileString(szSection, szKey, "", szOut, iOutLength, ECOM_szIniFileName);
    CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall ECOM_IniGetInt(char *szSection, char *szKey, int *piOut)
{
    DWORD       dwRet;
    CoreId      szOut;

    dwRet = GetPrivateProfileString(szSection, szKey, "", szOut, sizeof(szOut), ECOM_szIniFileName);
    CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "Errore generico");
    *piOut = atoi(szOut);

    return(0);
}

int __stdcall ECOM_IniSetString(char *szSection, char *szKey, char *szIn)
{
    DWORD       dwRet;

    dwRet = WritePrivateProfileString(szSection, szKey, szIn, ECOM_szIniFileName);
    CORE_ReturnValIfFail(dwRet >= 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall CORE_GetString(char *szPrompt, char *szString, int iLength)
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

int __stdcall CORE_GetInteger(char *szPrompt, int *piInteger)
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

int __stdcall CORE_GetTrueFalse(char *szPrompt)
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
char * __stdcall ECOM_OpcodeToString(int iOpcode)
{
    static FixString    myOpcodeString;

    switch (iOpcode) {
    case EASS_MSG_OPEN_SESSION:
        strcpy(myOpcodeString, "MSG_OPEN_SESSION");
        break;
    case EASS_MSG_SESSION_OPENED:
        strcpy(myOpcodeString, "MSG_SESSION_OPENED");
        break;
    case EASS_MSG_RESULT:
        strcpy(myOpcodeString, "MSG_RESULT");
        break;
    case EASS_MSG_OPEN_REQUEST:
        strcpy(myOpcodeString, "MSG_OPEN_REQUEST");
        break;
    case EASS_MSG_REQUEST_OPENED:
        strcpy(myOpcodeString, "MSG_REQUEST_OPENED");
        break;
    case EASS_MSG_ACCEPT_REQUEST:
        strcpy(myOpcodeString, "MSG_ACCEPT_REQUEST");
        break;
    case EASS_MSG_REQUEST_ACCEPTED:
        strcpy(myOpcodeString, "MSG_REQUEST_ACCEPTED");
        break;
    case EASS_MSG_LOAD_REQUEST:
        strcpy(myOpcodeString, "MSG_LOAD_REQUEST");
        break;
    case EASS_MSG_REQUEST_LOADED:
        strcpy(myOpcodeString, "MSG_REQUEST_LOADED");
        break;
    case EASS_MSG_CONNECT_REQUEST:
        strcpy(myOpcodeString, "MSG_CONNECT_REQUEST");
        break;
    case EASS_MSG_REQUEST_CONNECTED:
        strcpy(myOpcodeString, "MSG_REQUEST_CONNECTED");
        break;
    case EASS_MSG_LIST_REQUESTS:
        strcpy(myOpcodeString, "MSG_LIST_REQUESTS");
        break;
    case EASS_MSG_REQUESTS_LIST:
        strcpy(myOpcodeString, "MSG_REQUESTS_LIST");
        break;
    case EASS_MSG_LIST_PENDING_REQUESTS:
        strcpy(myOpcodeString, "");
        break;
    case EASS_MSG_PENDING_REQUESTS_LIST:
        strcpy(myOpcodeString, "MSG_PENDING_REQUESTS_LIST");
        break;
    case EASS_MSG_SEND_CHAT_TEXT:
        strcpy(myOpcodeString, "MSG_SEND_CHAT_TEXT");
        break;
    case EASS_MSG_OPEN_AUDIOCALL:
        strcpy(myOpcodeString, "MSG_OPEN_AUDIOCALL");
        break;
    case EASS_MSG_AUDIOCALL_OPENED:
        strcpy(myOpcodeString, "MSG_AUDIOCALL_OPENED");
        break;
    case EASS_MSG_OPEN_AUDIOVIDEOCALL:
        strcpy(myOpcodeString, "MSG_OPEN_AUDIOVIDEOCALL");
        break;
    case EASS_MSG_AUDIOVIDEOCALL_OPENED:
        strcpy(myOpcodeString, "MSG_AUDIOVIDEOCALL_OPENED");
        break;
    case EASS_MSG_OPEN_REMOTEDESKTOP:
        strcpy(myOpcodeString, "MSG_OPEN_REMOTEDESKTOP");
        break;
    case EASS_MSG_REMOTEDESKTOP_OPENED:
        strcpy(myOpcodeString, "MSG_REMOTEDESKTOP_OPENED");
        break;
    default:
        strcpy(myOpcodeString, "Opcode sconosciuto");
        break;
    }

    return(myOpcodeString);

}

char * __stdcall ECOM_GetMyName(void)
{
    return(ECOM_szMyHostName);
}

char * __stdcall ECOM_GetMyIP(void)
{
    return(ECOM_szMyHostIP);
}

char * __stdcall ECOM_GetServerName(void)
{
    return(ECOM_szServerName);
}

#ifndef EASS_SERVER
int __stdcall ECOM_ConnectToServer(void)
{
    int                 iRet;
    pthread_t           pThreadId;
    pthread_attr_t      attr;

    iRet = ECOM_IniGetString("server", "ip", ECOM_szServerName, sizeof(ECOM_szServerName));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    ECOM_iSessSocket = ECOM_TcpConnectToPort(ECOM_szServerName, EASS_SERVER_PORT);
    CORE_ReturnValIfFail(ECOM_iSessSocket >= 0, -1, ;, "Errore generico");

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&pThreadId, &attr, ECOM_TcpSessionThread, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, closesocket(ECOM_iSessSocket);, "Errore generico");

    return(0);
}
#endif /* ! EASS_SERVER */

#ifndef EASS_SERVER
int __stdcall ECOM_SendSessionMessage(int iOpcode, void *pPayload, int iSize)
{
    int             iRet;
    EASS_Message    msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iWhoAmI = ECOM_iWhoAmI;
    msgOut.iId = EASS_SessionId;
    msgOut.iOpcode = iOpcode;
    if (pPayload != NULL) {
        memcpy(&msgOut.szPayload, pPayload, CORE_Min(sizeof(msgOut.szPayload), iSize));
    }

    CORE_ReturnValIfFail(ECOM_iSessSocket >= 0, -1, ;, "Errore generico");
    iRet = ECOM_TcpSendMessage(ECOM_iSessSocket, (char *)&msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int __stdcall ECOM_SendRequestMessage(int iOpcode, void *pPayload, int iSize)
{
    int             iRet;
    EASS_Message    msgOut;

    memset(&msgOut, 0, sizeof(msgOut));
    msgOut.iWhoAmI = ECOM_iWhoAmI;
    msgOut.iId = EASS_RequestId;
    msgOut.iOpcode = iOpcode;
    if (pPayload != NULL) {
        memcpy(&msgOut.szPayload, pPayload, CORE_Min(sizeof(msgOut.szPayload), iSize));
    }

    CORE_ReturnValIfFail(ECOM_iSessSocket >= 0, -1, ;, "Errore generico");
    iRet = ECOM_TcpSendMessage(ECOM_iSessSocket, (char *)&msgOut, sizeof(msgOut));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

static void *ECOM_TcpSessionThread(void *FuncArg)
{
    int             iRet;
    fd_set          fdSet;
    struct timeval  tv = {5,0};
    EASS_Message    msgIn;
    int             iError;

    while (TRUE) {
        FD_ZERO(&fdSet);
        FD_SET(ECOM_iSessSocket, &fdSet);

        iRet = select(1, &fdSet, NULL, NULL, &tv);
        CORE_GotoIfFail(iRet >= 0, lblError, iError = EASS_ERR_TCP_SELECT;, "Errore generico");

	    if (FD_ISSET(ECOM_iSessSocket, &fdSet)) {
            iRet = ECOM_TcpReceiveMessage(ECOM_iSessSocket, (char *)&msgIn, sizeof(msgIn));
            CORE_GotoIfFail(iRet == 0, lblError, iError = EASS_ERR_TCP_RECV;, "Errore generico");

            iRet = EASS_HandleSessionMessage(&msgIn);
            CORE_GotoIfFail(iRet == 0, lblNext, ;, "Errore generico");
        } 

lblNext:
        ;
    }

lblError:
    closesocket(ECOM_iSessSocket);
    ECOM_iSessSocket = -1;

    printf("EASS_SessionThread e' stato chiuso a causa di un errore.\n");

    return(NULL);
}
#endif /* ! EASS_SERVER */

/******************************************************************************/
/*                                                                            */
/*   M E N U     M A N A G E M E N T                                          */
/*                                                                            */
/******************************************************************************/
CORE_Callback ECOM_MnuExit(void)
{
    exit(0);
}

CORE_Callback ECOM_MnuNext(void)
{
    FixString   szNext;

    sprintf(szNext, ".%d", ECOM_Option);
    strcat(ECOM_MenuLevel, szNext);

    return(1);
}

CORE_Callback ECOM_MnuBack(void)
{
    char    *szPtr;

    szPtr = strrchr(ECOM_MenuLevel, '.');
    if (szPtr != NULL) {
        *szPtr = '\0';
    }

    return(1);
}

int __stdcall ECOM_WaitKey(char *szPrompt)
{
    char    szBuffer[32];

    if (strlen(szPrompt) > 0) {
        printf("%s ", szPrompt);
    }
    printf("Press enter to continue...\n");
    fgets(szBuffer, sizeof(szBuffer), stdin);

    return(0);
}

int __stdcall ECOM_StartMenu(char *szCmdName, ECOM_Command *Command)
{
    int         iRet, i;
    FixString   szBuffer;

    /*- Initialize the internal variables */
    strcpy(ECOM_MenuLevel, "1");

    do {
#ifdef ON_WINDOWS
        system("cls");
#else
        system("clear");
#endif
        printf("%s\n\n", szCmdName);
        for (i = 0; Command[i].szMenuLevel != NULL; i++) {
            if (strcmp(Command[i].szMenuLevel, ECOM_MenuLevel) == 0) {
                printf("    %2d. %s\n", Command[i].iOption, Command[i].szDescription);
            }
        }
        printf("\n");
        printf("Scegli l'opzione: ");
        fgets(szBuffer, sizeof(szBuffer), stdin);

        ECOM_Option = atoi(szBuffer);
        for (i = 0; Command[i].szMenuLevel != NULL; i++) {
            if (strcmp(Command[i].szMenuLevel, ECOM_MenuLevel) == 0) {
                if (Command[i].iOption == ECOM_Option) {
                    if (Command[i].cbkFunction != NULL) {
                        iRet = (*Command[i].cbkFunction)();
                        if (Command[i].iPressKeyWhenFinished != FALSE) {
                            ECOM_WaitKey("Comando eseguito.");
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

int __stdcall ECOM_Sleep(int iMilliSecs)
{
#ifdef ON_WINDOWS
    Sleep (iMilliSecs);
#else
    usleep (iMilliSecs);
#endif

    return(0);
}

