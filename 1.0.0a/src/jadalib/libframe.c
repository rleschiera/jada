#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"


DllExport JADA_Frame __stdcall JADA_FrmNew(char *szLine, int iLine, int iType)
{
    JADA_FrmSelf    *frmPtr;

    frmPtr = JADA_Calloc(szLine, iLine, 1, sizeof(JADA_FrmSelf));
    CORE_ReturnValIfFail(frmPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di frame");

    frmPtr->iType = iType;
    frmPtr->iMagic = JADA_MAGIC_FRAME;

    return((JADA_Frame) frmPtr);
}

DllExport JADA_Frame __stdcall JADA_FrmNewFromMessage(char *szLine, int iLine, JADA_Message msgSelf)
{
    int             iRet;
    JADA_FrmSelf    *frmPtr;
    JADA_MsgHeader  msgHeader;

    iRet = JADA_MsgGetHeader(msgSelf, &msgHeader);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura dell'header del frame message");

    frmPtr = JADA_Calloc(szLine, iLine, 1, sizeof(JADA_FrmSelf));
    CORE_ReturnValIfFail(frmPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di frame");

    switch (msgHeader.iType) {
    case JADA_MSGTYPE_AUDIOFRAME:
        frmPtr->iType = JADA_FRAMETYPE_AUDIO;
        break;
    case JADA_MSGTYPE_VIDEOFRAME:
        frmPtr->iType = JADA_FRAMETYPE_VIDEO;
        break;
    case JADA_MSGTYPE_DRAWFRAME:
        frmPtr->iType = JADA_FRAMETYPE_DRAW;
        break;
    case JADA_MSGTYPE_DESKTOPFRAME:
        frmPtr->iType = JADA_FRAMETYPE_DESKTOP;
        break;
    case JADA_MSGTYPE_FTPFRAME:
        frmPtr->iType = JADA_FRAMETYPE_FTP;
        break;
    default:
        CORE_ReturnValIfFail(0, NULL, ;, "Il messaggio non e' di tipo frame");
        break;
    }
    frmPtr->iMagic = JADA_MAGIC_FRAME;

    frmPtr->iLength = msgHeader.iLength;
    frmPtr->szPayload = JADA_Calloc(szLine, iLine, 1, frmPtr->iLength);
    CORE_ReturnValIfFail(frmPtr->szPayload != NULL, NULL, ;, "Errore nella creazione del payload del frame");

    iRet = JADA_MsgGetPayload(msgSelf, frmPtr->szPayload, frmPtr->iLength);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "Errore nella lettura del frame dal messaggio");

    return((JADA_Frame) frmPtr);
}

DllExport int __stdcall JADA_FrmDestroy(char *szLine, int iLine, JADA_Frame evtSelf)
{
    JADA_FrmSelf    *frmPtr = (JADA_FrmSelf *) evtSelf;

    JADA_ReturnValIfInvalidFrame(frmPtr, -1);  

    if (frmPtr->szPayload != NULL) {
        JADA_Free(szLine, iLine, frmPtr->szPayload, sizeof(frmPtr->iLength));
    }
    JADA_Free(szLine, iLine, frmPtr, sizeof(*frmPtr));

    return(0);
}

DllExport int __stdcall JADA_FrmGetType(JADA_Frame evtSelf, int *piType)
{
    JADA_FrmSelf    *frmPtr = (JADA_FrmSelf *) evtSelf;

    JADA_ReturnValIfInvalidFrame(frmPtr, -1);  

    *piType = frmPtr->iType;

    return(0);
}

DllExport int __stdcall JADA_FrmGetLength(JADA_Frame evtSelf, int *piLength)
{
    JADA_FrmSelf    *frmPtr = (JADA_FrmSelf *) evtSelf;

    JADA_ReturnValIfInvalidFrame(frmPtr, -1);  

    *piLength = frmPtr->iLength;

    return(0);
}

DllExport int __stdcall JADA_FrmSetData(JADA_Frame evtSelf, void *szPayload, int iLength)
{
    JADA_FrmSelf    *frmPtr = (JADA_FrmSelf *) evtSelf;

    JADA_ReturnValIfInvalidFrame(frmPtr, -1);  

    if (frmPtr->szPayload != NULL) {
        JADA_Free(__FILE__, __LINE__, frmPtr->szPayload, sizeof(frmPtr->iLength));
    }
    frmPtr->iLength = iLength;
    frmPtr->szPayload = JADA_Calloc(__FILE__, __LINE__, 1, frmPtr->iLength);
    CORE_ReturnValIfFail(frmPtr->szPayload != NULL, -1, ;, "Errore nella creazione del payload del frame");

    memcpy(frmPtr->szPayload, szPayload, frmPtr->iLength);

    return(0);
}

DllExport int __stdcall JADA_FrmGetData(JADA_Frame evtSelf, void *szPayload, int iLength)
{
    JADA_FrmSelf    *frmPtr = (JADA_FrmSelf *) evtSelf;

    JADA_ReturnValIfInvalidFrame(frmPtr, -1);  
    CORE_ReturnValIfFail(frmPtr->iLength <= iLength, -1, ;, "Buffer troppo piccolo per estrarre il payload del frame");

    memcpy(szPayload, frmPtr->szPayload, iLength);

    return(0);
}

