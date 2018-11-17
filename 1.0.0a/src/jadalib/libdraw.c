#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"

typedef struct {
    int             X;
    int             Y;
} myCoordinate;


/******************************************************************************
 *
 *      D R A W
 *
 *****************************************************************************/
DllExport JADA_Draw __stdcall JADA_DrwNew(char *szLine, int iLine)
{
    JADA_DrwSelf    *drwPtr;

    drwPtr = JADA_Calloc(szLine, iLine, 1, sizeof(JADA_DrwSelf));
    CORE_ReturnValIfFail(drwPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di draw");

    drwPtr->lstCurves = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Curve), NULL);
    CORE_ReturnValIfFail(drwPtr->lstCurves != NULL, NULL, ;, "Errore generico");
    drwPtr->iMagic = JADA_MAGIC_DRAW;

    return((JADA_Draw) drwPtr);
}

DllExport int __stdcall JADA_DrwDestroy(char *szLine, int iLine, JADA_Draw drwSelf)
{
    int             iRet;
    JADA_DrwSelf    *drwPtr = (JADA_DrwSelf *) drwSelf;
    JADA_Curve      crvSelf;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidDraw(drwPtr, -1);  

    iRet = JADA_LstGetFirst(drwPtr->lstCurves, &crvSelf, sizeof(crvSelf), &curC);
    while (iRet == 0) {
        iRet = JADA_CrvDestroy(__FILE__, __LINE__, crvSelf);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella distruzione della lista di coordinate");
        iRet = JADA_LstGetNext(drwPtr->lstCurves, &crvSelf, sizeof(crvSelf), &curC);
    }
    iRet = JADA_LstDestroy(__FILE__, __LINE__, drwPtr->lstCurves);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella distruzione della lista di curve");

    return(0);
}

DllExport int __stdcall JADA_DrwGetCurveCount(JADA_Draw drwSelf, int *piCount)
{
    JADA_DrwSelf    *drwPtr = (JADA_DrwSelf *) drwSelf;

    JADA_ReturnValIfInvalidDraw(drwPtr, -1);  

    *piCount = JADA_LstGetCount(drwPtr->lstCurves);

    return(0);
}

DllExport int __stdcall JADA_DrwGetCurve(JADA_Draw drwSelf, int iIndex, JADA_Curve *crvSelf)
{
    int             iRet;
    JADA_DrwSelf    *drwPtr = (JADA_DrwSelf *) drwSelf;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidDraw(drwPtr, -1);  

    iRet = JADA_LstGet(drwPtr->lstCurves, iIndex, crvSelf, sizeof(*crvSelf), &curC);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura della curva");

    return(0);
}

DllExport int __stdcall JADA_DrwAddCurve(JADA_Draw drwSelf, JADA_Curve crvSelf)
{
    int             iRet;
    JADA_DrwSelf    *drwPtr = (JADA_DrwSelf *) drwSelf;

    JADA_ReturnValIfInvalidDraw(drwPtr, -1);  

    iRet = JADA_LstAdd(drwPtr->lstCurves, &crvSelf, sizeof(crvSelf), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella scrittura della curva");

    return(0);
}

/******************************************************************************
 *
 *      C U R V E
 *
 *****************************************************************************/
DllExport JADA_Curve __stdcall JADA_CrvNew(char *szLine, int iLine)
{
    JADA_CrvSelf    *crvPtr;

    crvPtr = JADA_Calloc(szLine, iLine, 1, sizeof(JADA_CrvSelf));
    CORE_ReturnValIfFail(crvPtr != NULL, NULL, ;, "Errore nell'allocazione del descrittore di curve");

    crvPtr->lstCoordinates = JADA_LstNew(__FILE__, __LINE__, sizeof(myCoordinate), NULL);
    CORE_ReturnValIfFail(crvPtr->lstCoordinates != NULL, NULL, ;, "Errore generico");
    crvPtr->iMagic = JADA_MAGIC_CURVE;

    return((JADA_Curve) crvPtr);
}

DllExport int __stdcall JADA_CrvDestroy(char *szLine, int iLine, JADA_Curve crvSelf)
{
    int             iRet;
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    iRet = JADA_LstDestroy(__FILE__, __LINE__, crvPtr->lstCoordinates);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella distruzione della lista di curve");

    return(0);
}

DllExport int __stdcall JADA_CrvSetColor(JADA_Curve crvSelf, int iAlpha, int iRed, int iGreen, int iBlue)
{
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    crvPtr->iAlpha = iAlpha;
    crvPtr->iRed = iRed;
    crvPtr->iGreen = iGreen;
    crvPtr->iBlue = iBlue;

    return(0);
}

DllExport int __stdcall JADA_CrvGetColor(JADA_Curve crvSelf, int *piAlpha, int *piRed, int *piGreen, int *piBlue)
{
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    *piAlpha = crvPtr->iAlpha;
    *piRed = crvPtr->iRed;
    *piGreen = crvPtr->iGreen;
    *piBlue = crvPtr->iBlue;

    return(0);
}

DllExport int __stdcall JADA_CrvSetWidth(JADA_Curve crvSelf, int iWidth)
{
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    crvPtr->iWidth = iWidth;

    return(0);
}

DllExport int __stdcall JADA_CrvGetWidth(JADA_Curve crvSelf, int *piWidth)
{
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    *piWidth = crvPtr->iWidth;

    return(0);
}

DllExport int __stdcall JADA_CrvGetCoordinatesCount(JADA_Curve crvSelf, int *piCount)
{
    int             iCount;
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    iCount = JADA_LstGetCount(crvPtr->lstCoordinates);
    *piCount = iCount;

    return(0);
}

DllExport int __stdcall JADA_CrvGetCoordinate(JADA_Curve crvSelf, int iIndex, int *pX, int *pY)
{
    int             iRet;
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;
    myCoordinate    myCoord;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    iRet = JADA_LstGet(crvPtr->lstCoordinates, iIndex, &myCoord, sizeof(myCoord), &curC);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella distruzione della lista di curve");

    *pX = myCoord.X;
    *pY = myCoord.Y;

    return(0);
}


DllExport int __stdcall JADA_CrvAddCoordinate(JADA_Curve crvSelf, int X, int Y)
{
    int             iRet;
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;
    myCoordinate    myCoord;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    memset(&myCoord, 0, sizeof(myCoord));
    myCoord.X = X; myCoord.Y = Y;
    iRet = JADA_LstAdd(crvPtr->lstCoordinates, &myCoord, sizeof(myCoord), NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella distruzione della lista di curve");

    return(0);
}

DllExport int __stdcall JADA_DrwSerialize(JADA_Draw drwSelf, void *szBuffer, int iLength, int *piOutLength)
{
    int             iRet, iOffset = 0;
    int             i, iCount;
    int             j, jCount;
    JADA_DrwSelf    *drwPtr = (JADA_DrwSelf *) drwSelf;
    JADA_CrvSelf    *crvPtr;
    myCoordinate    myCoord;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidDraw(drwPtr, -1);  

    /*- Put the fields in the target buffer */
    iRet = JADA_BufSetInt32(szBuffer, iLength, drwPtr->iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iCount = JADA_LstGetCount(drwPtr->lstCurves);
    iRet = JADA_BufSetInt32(szBuffer, iLength, iCount, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    for (i = 0; i < iCount; i++) {
        iRet = JADA_LstGet(drwPtr->lstCurves, i, &crvPtr, sizeof(crvPtr), &curC);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iAlpha, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iRed, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iGreen, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iBlue, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iWidth, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        jCount = JADA_LstGetCount(crvPtr->lstCoordinates);
        iRet = JADA_BufSetInt32(szBuffer, iLength, jCount, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        for (j = 0; j < jCount; j++) {
            iRet = JADA_LstGet(crvPtr->lstCoordinates, j, &myCoord, sizeof(myCoord), &curC);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
            iRet = JADA_BufSetInt32(szBuffer, iLength, myCoord.X, &iOffset);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
            iRet = JADA_BufSetInt32(szBuffer, iLength, myCoord.Y, &iOffset);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        }
    }

    *piOutLength = iOffset;

    return(0);
}

DllExport JADA_Draw __stdcall JADA_DrwDeserialize(void *szBuffer, int iLength)
{
    int             iRet, iOffset = 0;
    int             iMagic, iWidth, X, Y;
    int             iAlpha, iRed, iGreen, iBlue;
    int             i, iCount;
    int             j, jCount;
    JADA_Draw       drwSelf;
    JADA_Curve      crvSelf;

    /*- Get the fields from the source buffer */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    CORE_ReturnValIfFail(iMagic == JADA_MAGIC_DRAW, NULL, ;, "errore generico");
    drwSelf = JADA_DrwNew(__FILE__, __LINE__);
    CORE_ReturnValIfFail(drwSelf != NULL, NULL, ;, "errore generico");

    iRet = JADA_BufGetInt32(szBuffer, iLength, &iCount, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    for (i = 0; i < iCount; i++) {
        crvSelf = JADA_CrvNew(__FILE__, __LINE__);
        CORE_ReturnValIfFail(crvSelf != NULL, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &iAlpha, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &iRed, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &iGreen, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &iBlue, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_CrvSetColor(crvSelf, iAlpha, iRed, iGreen, iBlue);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &iWidth, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_CrvSetWidth(crvSelf, iWidth);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &jCount, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        for (j = 0; j < jCount; j++) {
            iRet = JADA_BufGetInt32(szBuffer, iLength, &X, &iOffset);
            CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
            iRet = JADA_BufGetInt32(szBuffer, iLength, &Y, &iOffset);
            CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
            iRet = JADA_CrvAddCoordinate(crvSelf, X, Y);
            CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        }
        iRet = JADA_DrwAddCurve(drwSelf, crvSelf);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    }

    return(drwSelf);
}


DllExport int __stdcall JADA_CrvSerialize(JADA_Curve crvSelf, void *szBuffer, int iLength, int *piOutLength)
{
    int             iRet, iOffset = 0;
    int             i, iCount;
    JADA_CrvSelf    *crvPtr = (JADA_CrvSelf *) crvSelf;
    myCoordinate    myCoord;
    CORE_Currency   curC;

    JADA_ReturnValIfInvalidCurve(crvPtr, -1);  

    /*- Put the fields in the target buffer */
    iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iRed, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iGreen, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iBlue, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_BufSetInt32(szBuffer, iLength, crvPtr->iWidth, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iCount = JADA_LstGetCount(crvPtr->lstCoordinates);
    iRet = JADA_BufSetInt32(szBuffer, iLength, iCount, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    for (i = 0; i < iCount; i++) {
        iRet = JADA_LstGet(crvPtr->lstCoordinates, i, &myCoord, sizeof(myCoord), &curC);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, myCoord.X, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_BufSetInt32(szBuffer, iLength, myCoord.Y, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    *piOutLength = iOffset;

    return(0);
}

DllExport JADA_Curve __stdcall JADA_CrvDeserialize(void *szBuffer, int iLength)
{
    int             iRet, iOffset = 0;
    int             iMagic, iWidth, X, Y;
    int             iAlpha, iRed, iGreen, iBlue;
    int             i, iCount;
    JADA_Curve      crvSelf;

    /*- Get the fields from the source buffer */
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iMagic, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    CORE_ReturnValIfFail(iMagic == JADA_MAGIC_CURVE, NULL, ;, "errore generico");
    crvSelf = JADA_CrvNew(__FILE__, __LINE__);
    CORE_ReturnValIfFail(crvSelf != NULL, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iAlpha, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iRed, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iGreen, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iBlue, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_CrvSetColor(crvSelf, iAlpha, iRed, iGreen, iBlue);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iWidth, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_CrvSetWidth(crvSelf, iWidth);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    iRet = JADA_BufGetInt32(szBuffer, iLength, &iCount, &iOffset);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    for (i = 0; i < iCount; i++) {
        iRet = JADA_BufGetInt32(szBuffer, iLength, &X, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_BufGetInt32(szBuffer, iLength, &Y, &iOffset);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
        iRet = JADA_CrvAddCoordinate(crvSelf, X, Y);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore generico");
    }

    return(crvSelf);
}

/******************************************************************************
 *
 *      T E S T
 *
 *****************************************************************************/
DllExport int __stdcall JADA_DrwTest(void)
{
    int         iRet;
    int         i, j, iLength;
    JADA_Draw   drwSelf1, drwSelf2;
    JADA_Curve  crvSelf1, crvSelf2;
    int         iCount1, iCount2;
    int         jCount1, jCount2;
    int         iAlpha1, iAlpha2;
    int         iRed1, iRed2;
    int         iGreen1, iGreen2;
    int         iBlue1, iBlue2;
    int         iWidth1, iWidth2;
    int         X1, X2, Y1, Y2;
    char        szBuffer[JADA_DRAW_FRAME_LENGTH];

    drwSelf1 = JADA_DrwNew(__FILE__, __LINE__);
    CORE_ReturnValIfFail(drwSelf1 != NULL, -1, ;, "errore generico");

    for (i = 0; i < 32; i++) {
        crvSelf1 = JADA_CrvNew(__FILE__, __LINE__);
        CORE_ReturnValIfFail(crvSelf1 != NULL, -1, ;, "errore generico");
        iRet = JADA_CrvSetColor(crvSelf1, 126 + i, 127 + i, 128 + i, 129 + i);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_CrvSetWidth(crvSelf1, i);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        for (j = 0; j < 16; j++) {
            iRet = JADA_CrvAddCoordinate(crvSelf1, i, j);
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        }
        iRet = JADA_DrwAddCurve(drwSelf1, crvSelf1);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    iRet = JADA_DrwSerialize(drwSelf1, szBuffer, sizeof(szBuffer), &iLength); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    drwSelf2 = JADA_DrwDeserialize(szBuffer, sizeof(szBuffer)); 
    CORE_ReturnValIfFail(drwSelf2 != NULL, -1, ;, "errore generico");

    iRet = JADA_DrwGetCurveCount(drwSelf1, &iCount1); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_DrwGetCurveCount(drwSelf2, &iCount2); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    CORE_ReturnValIfFail(iCount1 == iCount2, -1, ;, "errore generico");

    for (i = 0; i < iCount1; i++) {
        iRet = JADA_DrwGetCurve(drwSelf1, i, &crvSelf1); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_DrwGetCurve(drwSelf2, i, &crvSelf2); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

        iRet = JADA_CrvGetColor(crvSelf1, &iAlpha1, &iRed1, &iGreen1, &iBlue1); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_CrvGetColor(crvSelf2, &iAlpha2, &iRed2, &iGreen2, &iBlue2); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        CORE_ReturnValIfFail(iAlpha1 == iAlpha2, -1, ;, "errore generico");
        CORE_ReturnValIfFail(iRed1 == iRed2, -1, ;, "errore generico");
        CORE_ReturnValIfFail(iGreen1 == iGreen2, -1, ;, "errore generico");
        CORE_ReturnValIfFail(iBlue1 == iBlue2, -1, ;, "errore generico");

        iRet = JADA_CrvGetWidth(crvSelf1, &iWidth1); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_CrvGetWidth(crvSelf2, &iWidth2); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        CORE_ReturnValIfFail(iWidth1 == iWidth2, -1, ;, "errore generico");

        iRet = JADA_CrvGetCoordinatesCount(crvSelf1, &jCount1); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_CrvGetCoordinatesCount(crvSelf2, &jCount2); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        for (j = 0; j < jCount1; j++) {
            iRet = JADA_CrvGetCoordinate(crvSelf1, j, &X1, &Y1); 
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
            iRet = JADA_CrvGetCoordinate(crvSelf2, j, &X2, &Y2); 
            CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
            CORE_ReturnValIfFail(X1 == X2, -1, ;, "errore generico");
            CORE_ReturnValIfFail(Y1 == Y2, -1, ;, "errore generico");
        }
    }

    return(0);
}

DllExport int __stdcall JADA_CrvTest(void)
{
    int         iRet;
    int         i, iLength;
    JADA_Curve  crvSelf1, crvSelf2;
    int         iCount1, iCount2;
    int         iAlpha1, iAlpha2;
    int         iRed1, iRed2;
    int         iGreen1, iGreen2;
    int         iBlue1, iBlue2;
    int         iWidth1, iWidth2;
    int         X1, X2, Y1, Y2;
    char        szBuffer[JADA_DRAW_FRAME_LENGTH];

    crvSelf1 = JADA_CrvNew(__FILE__, __LINE__);
    CORE_ReturnValIfFail(crvSelf1 != NULL, -1, ;, "errore generico");
    iRet = JADA_CrvSetColor(crvSelf1, 126, 127, 128, 129);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_CrvSetWidth(crvSelf1, 10);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");

    for (i = 0; i < 16; i++) {
        iRet = JADA_CrvAddCoordinate(crvSelf1, 31, i);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    }

    iRet = JADA_CrvSerialize(crvSelf1, szBuffer, sizeof(szBuffer), &iLength); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    crvSelf2 = JADA_CrvDeserialize(szBuffer, sizeof(szBuffer)); 
    CORE_ReturnValIfFail(crvSelf2 != NULL, -1, ;, "errore generico");

    iRet = JADA_CrvGetColor(crvSelf1, &iAlpha1, &iRed1, &iGreen1, &iBlue1); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_CrvGetColor(crvSelf2, &iAlpha2, &iRed2, &iGreen2, &iBlue2); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    CORE_ReturnValIfFail(iAlpha1 == iAlpha2, -1, ;, "errore generico");
    CORE_ReturnValIfFail(iRed1 == iRed2, -1, ;, "errore generico");
    CORE_ReturnValIfFail(iGreen1 == iGreen2, -1, ;, "errore generico");
    CORE_ReturnValIfFail(iBlue1 == iBlue2, -1, ;, "errore generico");

    iRet = JADA_CrvGetWidth(crvSelf1, &iWidth1); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_CrvGetWidth(crvSelf2, &iWidth2); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    CORE_ReturnValIfFail(iWidth1 == iWidth2, -1, ;, "errore generico");

    iRet = JADA_CrvGetCoordinatesCount(crvSelf1, &iCount1); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    iRet = JADA_CrvGetCoordinatesCount(crvSelf2, &iCount2); 
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
    for (i = 0; i < iCount1; i++) {
        iRet = JADA_CrvGetCoordinate(crvSelf1, i, &X1, &Y1); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        iRet = JADA_CrvGetCoordinate(crvSelf2, i, &X2, &Y2); 
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore generico");
        CORE_ReturnValIfFail(X1 == X2, -1, ;, "errore generico");
        CORE_ReturnValIfFail(Y1 == Y2, -1, ;, "errore generico");
    }

    return(0);
}
