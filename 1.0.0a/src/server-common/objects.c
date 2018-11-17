#include "jada.h"
#include "jada_internal.h"
#include "server.h"

#define ASRV_MAX_PASSWORDS_DaButtare          1

typedef struct {
    char                *szPassword_DaButtare;
    char                *szVncPassword_DaButtare;
} ASRV_Password_DaButtare;

static JADA_ReqId myStaticRequestId;

static ASRV_Password_DaButtare    lstPasswords_DaButtare[] = {
    //"bianco21",     "4B5E1FC9483F087797",
    //"rosso24",      "3A3DE03F31413B488B",
    //"verde17",      "58B349A5AE53DBAA7F",
    //"giallo41",     "71BA3D5730ABC9FF62",
    //"marrone68",    "DB74CD30E2795EE5EA",
    //"nero43",       "E9430FCF58A9B72DEF",
    //"azzurro98",    "E1BCC5D7D286BDA6F4",
    //"celeste54",    "21F0B8FB17A93FF7BA",
    "1234",         "EE5B0E48C8FE97716D",
    NULL,           NULL
};

/******************************************************************************
 *
 *      V N C     P A S S W O R D S
 *
 *****************************************************************************/
static int ASRV_SelectPassword_DaButtare(int *piPassword)
{
    int         iPassword;
    JADA_Timet  timNow = time(0);

    iPassword = (int) timNow % ASRV_MAX_PASSWORDS_DaButtare;
    CORE_ReturnValIfFail((iPassword >= 0) && (iPassword < ASRV_MAX_PASSWORDS_DaButtare), -1, ;, "Errore nella generazione della password temporanea");
    *piPassword = iPassword;
    return(0);
}

int ASRV_GetPassword_DaButtare(int iIndex, char *szPassword)
{
    CORE_ReturnValIfFail((iIndex >= 0) && (iIndex < ASRV_MAX_PASSWORDS_DaButtare), -1, ;, "Errore nella lettura della password temporanea");
    strcpy(szPassword, lstPasswords_DaButtare[iIndex].szPassword_DaButtare);

    return(0);
}

int ASRV_GetVncPassword_DaButtare(int iIndex, char *szPassword)
{
    iIndex = 0;
    CORE_ReturnValIfFail((iIndex >= 0) && (iIndex < ASRV_MAX_PASSWORDS_DaButtare), -1, ;, "Errore nella lettura della password temporanea");
    strcpy(szPassword, lstPasswords_DaButtare[iIndex].szVncPassword_DaButtare);

    return(0);
}

/******************************************************************************
 *
 *      G E N E R A L
 *
 *****************************************************************************/
//int ASRV_GetParameter(CORE_List lstParameters, char *szSection, char *szName, char *szValue, int iLength)
//{
//    int             iRet;
//    ASRV_Parameter  parSelf;
//    CORE_Currency   curP;
//
//    memset(szValue, 0, iLength);
//
//    iRet = JADA_LstGetFirst(lstParameters, &parSelf, sizeof(parSelf), &curP);
//    while (iRet == 0) {
//        if (strcmp(parSelf.szName, szName) == 0) {
//            memset(szValue, 0, iLength);
//            strncpy(szValue, parSelf.szValue, iLength - 1);
//            return(0);
//        }
//
//        iRet = JADA_LstGetNext(lstParameters, &parSelf, sizeof(parSelf), &curP);
//    }
//
//    return(-1);
//}
//
//int ASRV_GetParameterInt(CORE_List lstParameters, char *szSection, char *szName, int *piValue)
//{
//    int             iRet;
//    ShortString     szTmp;
//
//    *piValue = 0;
//    iRet = ASRV_GetParameter(lstParameters, szSection, szName, szTmp, sizeof(szTmp));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del parametro (int)");
//    *piValue = atoi(szTmp);    
//
//    return(0);
//}
//
//int ASRV_SetParameter(ASRV_DbConnessione *myConnSelf, CORE_List lstParameters, char *szSection, char *szName, char *szValue)
//{
//    int             iRet;
//    ASRV_Parameter  parSelf;
//    CORE_Currency   curP;
//
//    iRet = JADA_LstGetFirst(lstParameters, &parSelf, sizeof(parSelf), &curP);
//    while (iRet == 0) {
//        if (strcmp(parSelf.szName, szName) == 0) {
//            if (strcmp(parSelf.szValue, szValue) != 0) {
//                strncpy(parSelf.szValue, szValue, sizeof(parSelf.szValue) - 1);
//                iRet = JADA_LstUpdate(lstParameters, &parSelf, sizeof(parSelf), &curP);
//                CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");
//
//                iRet = ASRV_DbUpdateParametro(myConnSelf, szSection, szName, szValue);
//                CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");
//            }
//
//            return(0);
//        }
//
//        iRet = JADA_LstGetNext(lstParameters, &parSelf, sizeof(parSelf), &curP);
//    }
//
//    memset(&parSelf, 0, sizeof(parSelf));
//    strcpy(parSelf.szSection, szSection);
//    strcpy(parSelf.szName, szName);
//    strcpy(parSelf.szValue, szValue);
//    iRet = JADA_LstAdd(lstParameters, &parSelf, sizeof(parSelf), NULL);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, StringFormat("errore nell'aggiunta in lista del parametro %s/%s", szSection, szName));
//
//    iRet = ASRV_DbCreateParametro(myConnSelf, szSection, szName, szValue);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nell'aggiornamento in lista del parametro");
//
//    return(0);
//}
//
//int ASRV_SetParameterInt(CORE_List lstParameters, char *szSection, char *szName, int iValue)
//{
//    int             iRet;
//    ShortString     szTmp;
//
//    sprintf(szTmp, "%d", iValue);
//    iRet = ASRV_SetParameter(lstParameters, szSection, szName, szTmp);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura del parametro (int)");
//
//    return(0);
//}
//
//int ASRV_CopyParameter(CORE_List lstFrom, CORE_List lstTo, char *szSection, char *szName)
//{
//    int             iRet;
//    FixString       szValue;
//
//    iRet = ASRV_GetParameter(lstFrom, szSection, szName, szValue, sizeof(szValue));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, StringFormat("errore nella lettura del parametro %s/%s", szSection, szName));
//    iRet = ASRV_SetParameter(lstTo, szSection, szName, szValue);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, StringFormat("errore nella scrittura del parametro %s/%s", szSection, szName));
//
//    return(0);
//}

/******************************************************************************
 *
 *      C O N N E C T I O N
 *
 *****************************************************************************/
ASRV_Connection *ASRV_CreateConnection(char *szFile, int iLine, char *szIP, int iSocket)
{
    int             iRet;
    ASRV_Connection *connPtr;

    connPtr = JADA_Calloc(szFile, iLine, 1, sizeof(ASRV_Connection));
    CORE_ReturnValIfFail(connPtr != NULL, NULL, ;, "Errore generico");

	strcpy(connPtr->dbConn.szHostIP, szIP);

    connPtr->timLastActivity = time(0);
    connPtr->iMagic = ASRV_MAGIC_CONNECTION;
    //connPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Parameter), NULL);
    //CORE_ReturnValIfFail(connPtr->lstParameters != NULL, NULL, ;, "Errore nella creazione della lista di parametri");
    iRet = ASRV_TcpCreateSocket(&connPtr->sokConn, iSocket);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");

    iRet = JADA_LstAdd(lstConnections, &connPtr, sizeof(connPtr), NULL);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");

    return(connPtr);
}

int ASRV_DestroyConnection(ASRV_Connection *connPtr)
{
    int             iRet;
    ASRV_Connection *conTmp;
    CORE_Currency   curCC;

    iRet = JADA_LstGetFirst(lstConnections, &conTmp, sizeof(conTmp), &curCC);
    while (iRet == 0) {
        if (JADA_CompareJadaId(&connPtr->dbConn.iId, &conTmp->dbConn.iId)) {
            iRet = JADA_LstDelete(lstConnections, &curCC);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;
        }
        iRet = JADA_LstGetNext(lstConnections, &conTmp, sizeof(conTmp), &curCC);
    }

    iRet = ASRV_DbDestroyConnessione(&connPtr->dbConn); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

int ASRV_RealDestroyConnection(char *szFile, int iLine, ASRV_Connection *connPtr)
{
    int             iRet;

    iRet = ASRV_TcpDestroySocket(&connPtr->sokConn);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    //iRet = JADA_LstDestroy(__FILE__, __LINE__, connPtr->lstParameters);
    //CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    JADA_Free(szFile, iLine, connPtr, sizeof(*connPtr));

    return(0);
}

ASRV_Connection *ASRV_GetConnectionById(JADA_ConnId iConnectionId)
{
    int             iRet;
    ASRV_Connection *conTmp;
    CORE_Currency   curC;

    iRet = JADA_LstGetFirst(lstConnections, &conTmp, sizeof(conTmp), &curC);
    while (iRet == 0) {
        if (JADA_CompareJadaId(&conTmp->dbConn.iId, &iConnectionId)) {
            return(conTmp);
        }
        iRet = JADA_LstGetNext(lstConnections, &conTmp, sizeof(conTmp), &curC);
    }

    return(NULL);
}

ASRV_Connection *ASRV_GetConnectionByAgentId(JADA_AgtId iAgentId)
{
    int             iRet;
    ASRV_Connection *conTmp;
    CORE_Currency   curC;

    iRet = JADA_LstGetFirst(lstConnections, &conTmp, sizeof(conTmp), &curC);
    while (iRet == 0) {
        if (JADA_CompareJadaId(&conTmp->dbConn.iIdAgent, &iAgentId)) {
            return(conTmp);
        }
        iRet = JADA_LstGetNext(lstConnections, &conTmp, sizeof(conTmp), &curC);
    }

    return(NULL);
}

/******************************************************************************
 *
 *      S E S S I O N
 *
 *****************************************************************************/
ASRV_Session *ASRV_CreateSession(char *szFile, int iLine, ASRV_Connection *connPtr, int iSessionType, JADA_ReqId iRequestId)
{
    int             iRet;
    ASRV_Session   *sesPtr;

    sesPtr = JADA_Calloc(szFile, iLine, 1, sizeof(ASRV_Session));
    CORE_ReturnValIfFail(sesPtr != NULL, NULL, ;, "Errore generico");

    sesPtr->iType = iSessionType;
    sesPtr->timLastActivity = time(0);
    sesPtr->dbSess.iIdRichiesta = iRequestId;
    sesPtr->dbReq.iId = iRequestId;
    sesPtr->lstInMessages = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_IncomingMsg), NULL);
    CORE_ReturnValIfFail(sesPtr->lstInMessages != NULL, NULL, ;, "Errore generico");
    sesPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Parameter), NULL);
    CORE_ReturnValIfFail(sesPtr->lstParameters != NULL, NULL, ;, "Errore generico");
    if (connPtr->dbConn.iWhoAmI == JADA_WHOAMI_AGENT) {
        ASRV_SetSessionAgent(sesPtr, connPtr);
    }
    else {
        ASRV_SetSessionManager(sesPtr, connPtr); 
    }
    sesPtr->iMagic = ASRV_MAGIC_SESSION;

    iRet = ASRV_DbCreateSessione(&sesPtr->dbSess);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
    iRet = ASRV_DbGetRichiesta(&sesPtr->dbReq);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");

    connPtr->iIdSession = sesPtr->dbSess.iId;

    return(sesPtr);
}

int ASRV_DestroySession(ASRV_Session *sesPtr)
{
    int                 iRet;
    ASRV_Session        *sesTmp;
    ASRV_IncomingMsg    msgTmp;
    CORE_Currency       curSS;

    sesPtr->iExitThread = TRUE;

    iRet = JADA_LstGetFirst(lstSessions, &sesTmp, sizeof(sesTmp), &curSS);
    while (iRet == 0) {
        if (JADA_CompareJadaId(&sesPtr->dbSess.iId, &sesTmp->dbSess.iId)) {
            iRet = JADA_LstDelete(lstSessions, &curSS);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
            break;
        }
        iRet = JADA_LstGetNext(lstSessions, &sesTmp, sizeof(sesTmp), &curSS);
    }

    iRet = JADA_LstGetFirst(sesPtr->lstInMessages, &msgTmp, sizeof(msgTmp), &curSS);
    while (iRet == 0) {
        iRet = JADA_MsgDestroy(__FILE__, __LINE__, msgTmp.msgIn);
        iRet = JADA_LstGetNext(sesPtr->lstInMessages, &msgTmp, sizeof(msgTmp), &curSS);
    }

    return(0);
}

int ASRV_RealDestroySession(char *szFile, int iLine, ASRV_Session *sesPtr)
{
    int                 iRet;

    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstInMessages);
    iRet = JADA_LstDestroy(__FILE__, __LINE__, sesPtr->lstParameters);

    JADA_Free(szFile, iLine, sesPtr, sizeof(*sesPtr));

    return(0);
}

int ASRV_StartSession(ASRV_Session *sesPtr)
{
    int             iRet;
    pthread_attr_t  attr;

    iRet = JADA_LstAdd(lstSessions, &sesPtr, sizeof(sesPtr), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    sesPtr->iExitThread = FALSE;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    iRet = pthread_create(&sesPtr->pThreadId, &attr, ASRV_SessionThread, sesPtr);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

ASRV_Session *ASRV_GetSessionById(JADA_SessId iSessionId)
{
    int             iRet;
    ASRV_Session   *sesTmp;
    CORE_Currency   curS;

    iRet = JADA_LstGetFirst(lstSessions, &sesTmp, sizeof(sesTmp), &curS);
    while (iRet == 0) {
        if (JADA_CompareJadaId(&sesTmp->dbSess.iId, &iSessionId)) {
            return(sesTmp);
        }
        iRet = JADA_LstGetNext(lstSessions, &sesTmp, sizeof(sesTmp), &curS);
    }

    return(NULL);
}

ASRV_Session *ASRV_GetSessionByRequestId(JADA_ReqId iRequestId)
{
    int             iRet;
    ASRV_Session    *sesTmp;
    CORE_Currency   curS;

    iRet = JADA_LstGetFirst(lstSessions, &sesTmp, sizeof(sesTmp), &curS);
    while (iRet == 0) {
        if (JADA_CompareJadaId(&sesTmp->dbReq.iId, &iRequestId)) {
            return(sesTmp);
        }
        iRet = JADA_LstGetNext(lstSessions, &sesTmp, sizeof(sesTmp), &curS);
    }

    return(NULL);
}

int ASRV_SetSessionAgent(ASRV_Session *ptrSess, ASRV_Connection *ptrConn)
{
    int     iRet;

    if (ptrConn != NULL) {
        iRet = ASRV_DbSetAgentSessione(&ptrSess->dbSess, &ptrConn->dbConn.iId);
        ptrSess->iIdAgent = ptrConn->dbConn.iIdAgent;
        ptrConn->iIdSession = ptrSess->dbSess.iId; 
    }
    else {
        //??? iRet = ASRV_DbSetAgentSessione(&ptrSess->dbSess, NULL);
        JADA_ClearJadaId(&ptrSess->iIdAgent);
    }

    return(0);
}

int ASRV_SetSessionManager(ASRV_Session *ptrSess, ASRV_Connection *ptrConn)
{
    int     iRet;

    if (ptrConn != NULL) {
        iRet = ASRV_DbSetManagerSessione(&ptrSess->dbSess, &ptrConn->dbConn.iId);
        ptrSess->iIdManager = ptrConn->dbConn.iIdManager;
        ptrConn->iIdSession = ptrSess->dbSess.iId; 
    }
    else {
        //??? iRet = ASRV_DbSetManagerSessione(&ptrSess->dbSess, NULL);
        JADA_ClearJadaId(&ptrSess->iIdManager);
    }

    return(0);
}

int ASRV_SendCachedChat(ASRV_Session *ptrSess)
{
    int                 iRet;
    CORE_List           lstChat;
    ASRV_DbChat         myChat;
    CORE_Currency       curC;
    JADA_MsgChatText    myMsg;

    lstChat = ASRV_DbListCachedChat(ptrSess->dbReq.iId);
    CORE_ReturnValIfFail(lstChat != NULL, -1, ;, "Errore generico");

    iRet = JADA_LstGetFirst(lstChat, &myChat, sizeof(myChat), &curC);
    while (iRet == 0) {
        memset(&myMsg, 0, sizeof(myMsg));
        myMsg.iRequestId = myChat.iIdRichiesta; 
        myMsg.iWhoAmI = myChat.iMittente;
        strcpy(myMsg.szText, myChat.szTesto);  

        if (myMsg.iWhoAmI == JADA_WHOAMI_MANAGER) {
            iRet = ASRV_TcpSendSessionMessageToAgent(ptrSess, JADA_MSG_SEND_CHAT_TEXT, &myMsg, sizeof(myMsg));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        }
        else if (myMsg.iWhoAmI == JADA_WHOAMI_AGENT) {
            iRet = ASRV_TcpSendSessionMessageToManager(ptrSess, JADA_MSG_SEND_CHAT_TEXT, &myMsg, sizeof(myMsg));
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        }

        iRet = JADA_LstGetNext(lstChat, &myChat, sizeof(myChat), &curC);
    }

    iRet = ASRV_DbCachedChatSent(ptrSess->dbReq.iId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iRet = JADA_LstDestroy(__FILE__, __LINE__, lstChat);

    return(0);
}

