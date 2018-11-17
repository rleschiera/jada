#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"

static int JADA_StreamId = 1;

DllExport JADA_Stream __stdcall JADA_StmNew(char *szLine, int iLine, JADA_Connection connSelf)
{
    int             iRet;
    JADA_StmSelf    *stmPtr;

    stmPtr = JADA_Calloc(szLine, iLine, 1, sizeof(JADA_StmSelf));
    CORE_ReturnValIfFail(stmPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di stream");

    stmPtr->lstLines = JADA_LstNew(__FILE__, __LINE__, sizeof(FixString), NULL);
    CORE_ReturnValIfFail(stmPtr->lstLines != NULL, NULL, ;, "Errore nella creazione della lista di eventi");
    stmPtr->iId = JADA_StreamId++;
    stmPtr->iMagic = JADA_MAGIC_STREAM;

    iRet = JADA_ConnAddStream(connSelf, (JADA_Stream) stmPtr);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella creazione del payload del stream");

    return((JADA_Stream) stmPtr);
}

DllExport int __stdcall JADA_StmDestroy(char *szLine, int iLine, JADA_Stream stmSelf)
{
    int             iRet;
    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;

    JADA_ReturnValIfInvalidStream(stmPtr, -1);  

    iRet = JADA_LstDestroy(__FILE__, __LINE__, stmPtr->lstLines);

    JADA_Free(szLine, iLine, stmPtr, sizeof(*stmPtr));

    return(0);
}

DllExport int __stdcall JADA_StmGetId(JADA_Stream stmSelf, int *piId)
{
    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;

    JADA_ReturnValIfInvalidStream(stmPtr, -1);  

    *piId = stmPtr->iId;

    return(0);
}

//DllExport int __stdcall JADA_StmGetLength(JADA_Stream stmSelf, int *piLength)
//{
//    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;
//
//    JADA_ReturnValIfInvalidStream(stmPtr, -1);  
//
//    *piLength = stmPtr->iLength;
//
//    return(0);
//}
//
//DllExport int __stdcall JADA_StmSetData(JADA_Stream stmSelf, char *szPayload, int iLength)
//{
//    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;
//
//    JADA_ReturnValIfInvalidStream(stmPtr, -1);  
//
//    if (stmPtr->szPayload != NULL) {
//        JADA_Free(__FILE__, __LINE__, stmPtr->szPayload, stmPtr->iLength);
//    }
//    stmPtr->iLength = iLength;
//    stmPtr->szPayload = JADA_Calloc(__FILE__, __LINE__, 1, stmPtr->iLength);
//    CORE_ReturnValIfFail(stmPtr->szPayload != NULL, -1, ;, "Errore nella creazione del payload del stream");
//
//    memcpy(stmPtr->szPayload, szPayload, stmPtr->iLength);
//
//    return(0);
//}
//
//DllExport int __stdcall JADA_StmGetData(JADA_Stream stmSelf, char *szPayload, int iLength)
//{
//    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;
//
//    JADA_ReturnValIfInvalidStream(stmPtr, -1);  
//    CORE_ReturnValIfFail(stmPtr->iLength <= iLength, -1, ;, "Buffer troppo piccolo per estrarre il payload del stream");
//
//    memcpy(szPayload, stmPtr->szPayload, iLength);
//
//    return(0);
//}

DllExport int __stdcall JADA_StmAddLine(JADA_Stream stmSelf, char *szLine)
{
    int             iRet;
    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;
    FixString       myLine;

    JADA_ReturnValIfInvalidStream(stmPtr, -1);  

    memset(myLine, 0, sizeof(myLine));
    strncpy(myLine, szLine, sizeof(myLine) - 1);

    iRet = JADA_LstAdd(stmPtr->lstLines, myLine, sizeof(myLine), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Buffer troppo piccolo per estrarre il payload del stream");

    return(0);
}

DllExport int __stdcall JADA_StmGetFirstLine(JADA_Stream stmSelf, char *szLine, int iLength)
{
    int             iRet;
    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;
    FixString       myLine;

    JADA_ReturnValIfInvalidStream(stmPtr, -1);  

    iRet = JADA_LstGetFirst(stmPtr->lstLines, myLine, sizeof(myLine), &stmPtr->curL);
    if (iRet == 0) {
        memset(szLine, 0, iLength);
        strncpy(szLine, myLine, iLength - 1);
    }

    return(iRet);
}

DllExport int __stdcall JADA_StmGetNextLine(JADA_Stream stmSelf, char *szLine, int iLength)
{
    int             iRet;
    JADA_StmSelf    *stmPtr = (JADA_StmSelf *) stmSelf;
    FixString       myLine;

    JADA_ReturnValIfInvalidStream(stmPtr, -1);  

    iRet = JADA_LstGetNext(stmPtr->lstLines, myLine, sizeof(myLine), &stmPtr->curL);
    if (iRet == 0) {
        memset(szLine, 0, iLength);
        strncpy(szLine, myLine, iLength - 1);
    }

    return(iRet);
}

