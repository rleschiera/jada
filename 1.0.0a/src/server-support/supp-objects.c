#include "jada.h"
#include "server.h"

/******************************************************************************
 *
 *      R E Q U E S T S
 *
 *****************************************************************************/
//ASRV_Request *ASRV_CreateRequest(char *szFile, int iLine, ASRV_Session *sesPtr)
//{
//    int             iRet;
//    ASRV_Request    *reqPtr;
//
//    reqPtr = JADA_Calloc(szFile, iLine, 1, sizeof(ASRV_Request));
//    CORE_ReturnValIfFail(reqPtr != NULL, NULL, ;, "Errore generico");
//
//    reqPtr->iId = ASRV_GetNewRequestId();
//    CORE_ReturnValIfFail(JADA_IsValidJadaId(reqPtr->iId), NULL, ;, "Errore generico");
//    reqPtr->iStatus = STATUS_REQUESTED;
//    iRet = ASRV_SelectPassword(&reqPtr->iPassword);
//    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
//    reqPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Parameter), NULL);
//    CORE_ReturnValIfFail(reqPtr->lstParameters != NULL, NULL, ;, "Errore nella creazione della lista di parametri");
//    reqPtr->lstChatMsgs = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_MsgChatText), NULL);
//    CORE_ReturnValIfFail(reqPtr->lstChatMsgs!= NULL, NULL, ;, "Errore nella creazione della lista di chat msgs");
//    reqPtr->iMagic = ASRV_MAGIC_REQUEST;
//
//    iRet = JADA_LstAdd(lstRequests, &reqPtr, sizeof(reqPtr), NULL);
//    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
//
//    return(reqPtr);
//}
//
//ASRV_Request *ASRV_CreateRequestFromPending(char *szFile, int iLine, ASRV_Session *sesPtr, ASRV_PendingRequest *penPtr)
//{
//    int             iRet;
//    ASRV_Request    *reqPtr;
//
//    reqPtr = JADA_Calloc(szFile, iLine, 1, sizeof(ASRV_Request));
//    CORE_ReturnValIfFail(reqPtr != NULL, NULL, ;, "Errore generico");
//
//    reqPtr->iId = penPtr->iId;
//    reqPtr->iStatus = STATUS_LOADED;
//    iRet = ASRV_SelectPassword(&reqPtr->iPassword);
//    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
//    reqPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Parameter), NULL);
//    CORE_ReturnValIfFail(reqPtr->lstParameters != NULL, NULL, ;, "Errore nella creazione della lista di parametri");
//    reqPtr->lstChatMsgs = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_MsgChatText), NULL);
//    CORE_ReturnValIfFail(reqPtr->lstChatMsgs!= NULL, NULL, ;, "Errore nella creazione della lista di chat msgs");
//    reqPtr->iMagic = ASRV_MAGIC_REQUEST;
//
//    if (sesPtr->iWhoAmI == JADA_WHOAMI_AGENT) {
//        reqPtr->iAgentSession = sesPtr->iId;
//        iRet = ASRV_CopyParameter(sesPtr->lstParameters, penPtr->lstParameters, "sessione", "utente"); 
//        iRet = ASRV_CopyParameter(sesPtr->lstParameters, penPtr->lstParameters, "sessione", "cliente"); 
//        iRet = ASRV_CopyParameter(sesPtr->lstParameters, penPtr->lstParameters, "sessione", "telefono"); 
//        iRet = ASRV_CopyParameter(sesPtr->lstParameters, penPtr->lstParameters, "sessione", "mail"); 
//        iRet = ASRV_CopyParameter(sesPtr->lstParameters, penPtr->lstParameters, "sessione", "audio"); 
//    }
//    else if (sesPtr->iWhoAmI == JADA_WHOAMI_MANAGER) {
//        reqPtr->iManagerSession = sesPtr->iId;
//    }
//    iRet = ASRV_CopyParameter(penPtr->lstParameters, reqPtr->lstParameters, "richiesta", "area"); 
//    iRet = ASRV_CopyParameter(penPtr->lstParameters, reqPtr->lstParameters, "richiesta", "descrizione"); 
//
//    iRet = JADA_LstAdd(lstRequests, &reqPtr, sizeof(reqPtr), NULL);
//    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore generico");
//
//    return(reqPtr);
//}
//
//int ASRV_DestroyRequest(char *szFile, int iLine, ASRV_Request *reqPtr, CORE_Currency *curR)
//{
//    int             iRet;
//    ASRV_Request   *reqTmp;
//    CORE_Currency   curRR;
//
//
//    iRet = JADA_LstDestroy(__FILE__, __LINE__, reqPtr->lstParameters);
//    iRet = JADA_LstDestroy(__FILE__, __LINE__, reqPtr->lstChatMsgs);
//
//    if (curR != NULL) {
//        iRet = JADA_LstDelete(lstRequests, curR);
//        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//    }
//    else {
//        iRet = JADA_LstGetFirst(lstRequests, &reqTmp, sizeof(reqTmp), &curRR);
//        while (iRet == 0) {
//            if (JADA_CompareJadaId(reqPtr->iId, reqTmp->iId)) {
//                iRet = JADA_LstDelete(lstRequests, &curRR);
//                CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//                break;
//            }
//            iRet = JADA_LstGetNext(lstRequests, &reqTmp, sizeof(reqTmp), &curRR);
//        }
//    }
//    
//    memset(reqPtr, 0, sizeof(*reqPtr));
//    JADA_Free(szFile, iLine, reqPtr);
//
//    return(0);
//}
//
//ASRV_Request *ASRV_GetRequestById(char *szFile, int iLine, JADA_Id iRequestId)
//{
//    int             iRet;
//    ASRV_Request   *reqPtr;
//    CORE_Currency   curC;
//
//    iRet = JADA_LstGetFirst(lstRequests, &reqPtr, sizeof(reqPtr), &curC);
//    while (iRet == 0) {
//        if (JADA_CompareJadaId(reqPtr->iId, iRequestId)) {
//            return(reqPtr);
//        }
//
//        iRet = JADA_LstGetNext(lstRequests, &reqPtr, sizeof(reqPtr), &curC);
//    }
//
//    return (NULL);
//}
//
//ASRV_Request *ASRV_GetRequestByPendingId(char *szFile, int iLine, JADA_Id iRequestId)
//{
//    int             iRet;
//    ASRV_Request   *reqPtr;
//    CORE_Currency   curC;
//
//    iRet = JADA_LstGetFirst(lstRequests, &reqPtr, sizeof(reqPtr), &curC);
//    while (iRet == 0) {
//        if (JADA_CompareJadaId(reqPtr->iId, iRequestId)) {
//            return(reqPtr);
//        }
//
//        iRet = JADA_LstGetNext(lstRequests, &reqPtr, sizeof(reqPtr), &curC);
//    }
//
//    return (NULL);
//}
//
//int ASRV_MoveRequestToPending(char *szFile, int iLine, ASRV_Request *reqPtr)
//{
//    int                     iRet;
//    ASRV_PendingRequest     *penPtr;
//    ASRV_Session            *sesAgent;
//
//    iRet = ASRV_DbExistsPendingRequest(reqPtr->iId);
//    if (iRet != 0) {
//        penPtr = ASRV_CreatePendingRequest(szFile, iLine);
//        CORE_ReturnValIfFail(penPtr != NULL, -1, ;, "Errore generico");
//
//        penPtr->iId = reqPtr->iId;
//        iRet = ASRV_CopyParameter(reqPtr->lstParameters, penPtr->lstParameters, "richiesta", "area"); 
//        iRet = ASRV_CopyParameter(reqPtr->lstParameters, penPtr->lstParameters, "richiesta", "descrizione"); 
//        sesAgent = ASRV_GetSessionById(__FILE__, __LINE__, reqPtr->iAgentSession);
//        if (sesAgent != NULL) {
//            iRet = ASRV_CopyParameter(sesAgent->lstParameters, penPtr->lstParameters, "sessione", "utente");
//            iRet = ASRV_CopyParameter(sesAgent->lstParameters, penPtr->lstParameters, "sessione", "telefono");
//            iRet = ASRV_CopyParameter(sesAgent->lstParameters, penPtr->lstParameters, "sessione", "mail");
//            iRet = ASRV_CopyParameter(sesAgent->lstParameters, penPtr->lstParameters, "sessione", "cliente");
//        }
//
//        iRet = ASRV_DbSavePendingRequest(penPtr);
//        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//        iRet = ASRV_DestroyPendingRequest(__FILE__, __LINE__, penPtr, FALSE);
//        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//    }
//
//    return(0);
//}
//
///******************************************************************************
// *
// *      P E N D I N G     R E Q U E S T S
// *
// *****************************************************************************/
//ASRV_PendingRequest *ASRV_CreatePendingRequest(char *szFile, int iLine)
//{
//    //int                 iRet;
//    ASRV_PendingRequest *reqPtr;
//
//    reqPtr = JADA_Calloc(szFile, iLine, 1, sizeof(ASRV_PendingRequest));
//    CORE_ReturnValIfFail(reqPtr != NULL, NULL, ;, "Errore generico");
//
//    reqPtr->iStatus = STATUS_REQUESTED;
//    reqPtr->lstParameters = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Parameter), NULL);
//    CORE_ReturnValIfFail(reqPtr->lstParameters != NULL, NULL, ;, "Errore nella creazione della lista di parametri");
//    reqPtr->iMagic = ASRV_MAGIC_PENDING_REQUEST;
//
//    return(reqPtr);
//}
//
//int ASRV_DestroyPendingRequest(char *szFile, int iLine, ASRV_PendingRequest *reqPtr, int iFromDb)
//{
//    int                 iRet;
//    
//    if (iFromDb == TRUE) {
//        iRet = ASRV_DbDeletePendingRequest(reqPtr->iId);
//        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//    }
//
//    iRet = JADA_LstDestroy(__FILE__, __LINE__, reqPtr->lstParameters);
//    memset(reqPtr, 0, sizeof(*reqPtr));
//    JADA_Free(szFile, iLine, reqPtr);
//
//    return(0);
//}
//
//ASRV_PendingRequest *ASRV_GetPendingRequestById(char *szFile, int iLine, JADA_Id iRequestId)
//{
//    ASRV_PendingRequest *reqPtr;
//
//    reqPtr = ASRV_DbLoadPendingRequest(__FILE__, __LINE__, iRequestId);
//    CORE_ReturnValIfFail(reqPtr != NULL, NULL, ;, "Errore generico");
//
//    return (reqPtr);
//}
