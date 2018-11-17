#include <sys/types.h>
#include <sys/timeb.h>

#include "jada.h"
#include "jada_internal.h"

/* Single atom size */
#define OVLS_LISTENTRYBLOCK          10

/* Memory block type */
#define OVLS_PRIVATE                 201
#define OVLS_SHARED                  102

/* List operations */
#define OVLS_COPY                    304
#define OVLS_CLEAR                   405
#define OVLS_PRESERVE                506

/* The list descripotor */
typedef struct {
    int                    iMagic;
#ifndef NO_PTHREAD
    pthread_mutex_t        Mutex;
    pthread_cond_t         CondVar;
#endif
    char                  *szBase;
    int                    iSize;
    int                    iAccessMode;
    int                    iEntries;
    int                    iEntrySize;
    CORE_FunctionPtr       pfnCompare;
} JADA_LstDescr;

#define MAX_THRESH 4

typedef struct {
    char    *lo;
    char    *hi;
} stack_node;

#define STACK_SIZE	(sizeof(size_t))
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))
#define	STACK_NOT_EMPTY	(stack < top)

/* Timing constants */
#define MILLISECS_PER_SEC               1000
#define NANOSECS_PER_MILLISEC           1000000
#define NANOSECS_PER_SEC                1000000000


/******************************************************************************
 *
 *     S T A T I C     S E R V I C E S
 *
 ******************************************************************************/
static int CORE_SetTimeSpec(struct timespec *Timespec, int iMsecsToWait)
{
    int             iNsec;
    struct timeb    Now;


    ftime(&Now);
    Timespec->tv_sec = (int) Now.time;
    Timespec->tv_nsec = Now.millitm * NANOSECS_PER_MILLISEC;

    iNsec = (iMsecsToWait % MILLISECS_PER_SEC) * NANOSECS_PER_MILLISEC;
    Timespec->tv_sec += (iMsecsToWait / MILLISECS_PER_SEC);
    Timespec->tv_nsec += iNsec;
    while (Timespec->tv_nsec >= NANOSECS_PER_SEC) {
        Timespec->tv_sec++;
        Timespec->tv_nsec -= NANOSECS_PER_SEC;
    }

    return(0);
}

static int JADA_LstLock(JADA_LstDescr *dscList)
{
    int     iRet = 0;


#ifndef NO_PTHREAD
    iRet = pthread_mutex_lock(&dscList->Mutex);
#endif
    return(iRet);
}


static int JADA_LstUnlock(JADA_LstDescr *dscList)
{
    int     iRet = 0;


#ifndef NO_PTHREAD
    iRet = pthread_mutex_unlock(&dscList->Mutex);
#endif
    return(iRet);
}


static int __stdcall JADA_LstAppendEntry(void *pchPtr1, void *pchPtr2)
{
    /*- Ritorna sempre 1 */
    return(1);
}


static void *JADA_LstLfind(void *pvKey, void *pvBase, int *piNmemb, int iSize,
                         CORE_FunctionPtr pfnCompar)
{
    unsigned char *puchResult = pvBase;
    int iCnt = 0;

    while (iCnt < *piNmemb && (*pfnCompar) (pvKey, puchResult) != 0) {
        puchResult += iSize;
        ++iCnt;
    }

    return(iCnt < *piNmemb ? (void *) puchResult : NULL);
}


static void *JADA_LstLsearch(void *pvKey, void *pvBase, int *piNmemb, int iSize,
                         CORE_FunctionPtr pfnCompar)
{
    unsigned char *puchResult;


    if (pfnCompar == JADA_LstAppendEntry) {
        /* Append requested: insert at the end.  */
        puchResult = (unsigned char *)pvBase + (*piNmemb) * iSize;
        memcpy(puchResult, pvKey, iSize);
        ++(*piNmemb);
    }
    else {
        /* Try to find it.  */
        puchResult = JADA_LstLfind(pvKey, pvBase, piNmemb, iSize, pfnCompar);
        if (puchResult == NULL) {
            /* Not available.  Insert at the end.  */
            puchResult = (unsigned char *)pvBase + (*piNmemb) * iSize;
            memcpy(puchResult, pvKey, iSize);
            ++(*piNmemb);
        }
    }

    return(puchResult);
}


static void JADA_LstBubbleSort(void *pvBase, int iNmemb, int iSize,
                         CORE_FunctionPtr pfnCompar)
{
    int     i, j, iRet;
    int     iSorted;
    char    *pchBase = pvBase, *pchPtr1, *pchPtr2;


    for(i = iNmemb - 1; i > 0; i--) {
        iSorted = TRUE;
        for(j = 0; j < i; j++) {

            pchPtr1 = pchBase + (j) * iSize;
            pchPtr2 = pchBase + (j + 1) * iSize;
	        iRet = (*pfnCompar)(pchPtr1, pchPtr2);
            if (iRet > 0) {
        	    CORE_Swap (pchPtr1, pchPtr2, iSize);
            }
            iSorted = FALSE;
        }

        if (iSorted == TRUE) {
            break;
        }
    }

    return;
}


static void JADA_LstQsort(void *pvBase, int iNmemb, int iSize,
                         CORE_FunctionPtr pfnCompar)
{
  register char *base_ptr = (char *) pvBase;
  char *pivot_buffer;
  const size_t max_thresh = MAX_THRESH * iSize;;

  if (iNmemb == 0)
  /* Avoid lossage with unsigned arithmetic below.  */
  return;

  /* Allocating SIZE bytes for a pivot buffer facilitates a better */
  /*   algorithm below since we can do comparisons directly on the pivot. */ 
  pivot_buffer = JADA_Calloc(__FILE__, __LINE__, 1, iSize);
  

  if (iNmemb > MAX_THRESH)
    {
      char *lo = base_ptr;
      char *hi = &lo[iSize * (iNmemb - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack + 1;

      while (STACK_NOT_EMPTY)
        {
          char *left_ptr;
          char *right_ptr;

	  char *pivot = pivot_buffer;

	  /* Select median value from among LO, MID, and HI. Rearrange
	     LO and HI so the three values are sorted. This lowers the
	     probability of picking a pathological pivot value and
	     skips a comparison for both the LEFT_PTR and RIGHT_PTR in
	     the while loops. */

	  char *mid = lo + iSize * ((hi - lo) / iSize >> 1);

	  if ((*pfnCompar) ((void *) mid, (void *) lo) < 0)
	    CORE_Swap (mid, lo, iSize);
	  if ((*pfnCompar) ((void *) hi, (void *) mid) < 0)
	    CORE_Swap (mid, hi, iSize);
	  else
	    goto jump_over;
	  if ((*pfnCompar) ((void *) mid, (void *) lo) < 0)
	    CORE_Swap (mid, lo, iSize);
	jump_over:;
	  memcpy(pivot, mid, iSize);
	  pivot = pivot_buffer;

	  left_ptr  = lo + iSize;
	  right_ptr = hi - iSize;

	  /* Here's the famous ``collapse the walls'' section of quicksort.
	     Gotta like those tight inner loops!  They are the main reason
	     that this algorithm runs much faster than others. */
	  do
	    {
	      while ((*pfnCompar) ((void *) left_ptr, (void *) pivot) < 0)
		left_ptr += iSize;

	      while ((*pfnCompar) ((void *) pivot, (void *) right_ptr) < 0)
		right_ptr -= iSize;

	      if (left_ptr < right_ptr)
		{
		  CORE_Swap (left_ptr, right_ptr, iSize);
		  left_ptr += iSize;
		  right_ptr -= iSize;
		}
	      else if (left_ptr == right_ptr)
		{
		  left_ptr += iSize;
		  right_ptr -= iSize;
		  break;
		}
	    }
	  while (left_ptr <= right_ptr);

          /* Set up pointers for next iteration.  First determine whether
             left and right partitions are below the threshold size.  If so,
             ignore one or both.  Otherwise, push the larger partition's
             bounds on the stack and continue sorting the smaller one. */

          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
		/* Ignore both small partitions. */
                POP (lo, hi);
              else
		/* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
	    /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
	      /* Push larger left partition indices. */
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
	      /* Push larger right partition indices. */
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }

  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */

  {
    char *const end_ptr = &base_ptr[iSize * (iNmemb - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = CORE_Min(end_ptr, base_ptr + max_thresh);
    register char *run_ptr;

    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */

    for (run_ptr = tmp_ptr + iSize; run_ptr <= thresh; run_ptr += iSize)
      if ((*pfnCompar) ((void *) run_ptr, (void *) tmp_ptr) < 0)
        tmp_ptr = run_ptr;

    if (tmp_ptr != base_ptr)
      CORE_Swap (tmp_ptr, base_ptr, iSize);

    /* Insertion sort, running from left-hand-side up to right-hand-side.  */

    run_ptr = base_ptr + iSize;
    while ((run_ptr += iSize) <= end_ptr)
      {
	tmp_ptr = run_ptr - iSize;
	while ((*pfnCompar) ((void *) run_ptr, (void *) tmp_ptr) < 0)
	  tmp_ptr -= iSize;

	tmp_ptr += iSize;
        if (tmp_ptr != run_ptr)
          {
            char *trav;

	    trav = run_ptr + iSize;
	    while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;

                for (hi = lo = trav; (lo -= iSize) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
          }
      }
  }

  /*- CORE_Free the allocated pivot buffer */
  JADA_Free(__FILE__, __LINE__, pivot_buffer, 0);
}


static void JADA_LstMySort(void *pvBase, int iNmemb, int iSize,
                         CORE_FunctionPtr pfnCompar)
{
    JADA_LstBubbleSort(pvBase, iNmemb, iSize, pfnCompar);
}


/******************************************************************************
 *
 *     B A S I C     S E R V I C E S
 *
 ******************************************************************************/
DllExport CORE_List __stdcall JADA_LstNew(char *szFile, int iLine, int iEntrySize, CORE_FunctionPtr pfnCompare)
{
    int             iRet;
    CORE_List       *lstDesc;
    JADA_LstDescr   *dscList;


    /*- Allocate and initialize the list descriptor */
    lstDesc = JADA_Calloc(szFile, iLine, 1, sizeof(JADA_LstDescr));
    CORE_ReturnValIfFail(lstDesc != NULL, NULL, ;, "");

    /*- Get the list handle */
    dscList = (JADA_LstDescr *) lstDesc;

    /*- Allocate space to contain OVLS_LISTENTRYBLOCK entries */
    dscList->szBase = JADA_Calloc(szFile, iLine, OVLS_LISTENTRYBLOCK, iEntrySize);
    CORE_ReturnValIfFail(dscList->szBase != NULL, NULL, ;, "generic error");

    /*- Create the synchronization objects */
#ifndef NO_PTHREAD
    iRet = pthread_mutex_init(&dscList->Mutex, NULL);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "generic error");
    iRet = pthread_cond_init(&dscList->CondVar, NULL);
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, "generic error");
#endif

    /*- Set the information in the list descriptor */
    dscList->iAccessMode = OVLS_PRIVATE;
    dscList->iSize       = OVLS_LISTENTRYBLOCK;
    dscList->iEntries    = 0;
    dscList->iEntrySize  = iEntrySize;
    if (pfnCompare != NULL) {
        dscList->pfnCompare  = pfnCompare;
    }
    else {
        dscList->pfnCompare  = JADA_LstAppendEntry;
    }
    dscList->iMagic = JADA_MAGIC_LIST;

    /*- Ritorna con esito positivo */
    return((CORE_List)lstDesc);
}


DllExport int __stdcall JADA_LstIsValid(CORE_List lstSelf)
{
    JADA_LstDescr  *dscList;


    /*- Get and check the list handle */
    if (lstSelf == NULL) {
        return(-1);
    }
    dscList = (JADA_LstDescr *) lstSelf;
    if (dscList->iMagic != JADA_MAGIC_LIST) {
        return(-1);
    }

    /*- Ritorna con esito positivo */
    return(0);
}


DllExport int __stdcall JADA_LstDestroy(char *szFile, int iLine, CORE_List lstSelf)
{
    int             iRet;
    JADA_LstDescr   *dscList;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");

    /*- Lock the list
    JADA_LstLock(dscList); altrimenti fallisce la pthread_mutex_destroy */

    /*- Destroy the synchronization objects */
#ifndef NO_PTHREAD
    iRet = pthread_mutex_destroy(&dscList->Mutex);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "generic error");
    iRet = pthread_cond_destroy(&dscList->CondVar);
    // CORE_ReturnValIfFail(iRet == 0, -1, ;, "generic error");
#endif

    /*- Release the allocated memory block */
    if ((dscList->szBase != NULL) && (dscList->iAccessMode == OVLS_PRIVATE)) {
        JADA_Free(szFile, iLine, dscList->szBase, 0);
    }

    /*- Release the allocated list handle */
    JADA_Free(szFile, iLine, dscList, sizeof(*dscList));

    /*- Ritorna con esito positivo */
    return(0);
}


DllExport int __stdcall JADA_LstClear(CORE_List lstSelf)
{
    JADA_LstDescr  *dscList;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Reinizializza il descrittore di lista passato come parametro */
    dscList->iEntries = 0;

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(0);
}


/******************************************************************************
 *
 *     E N T R Y     M A N A G E M E N T     S E R V I C E S
 *
 ******************************************************************************/
DllExport int __stdcall JADA_LstAdd(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC)
{
    int             iRet;
    char            *pchPtr;
    JADA_LstDescr   *dscList;
    int             iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se nella lista non c'e' spazio sufficiente per il nuovo entry... */
    if(dscList->iEntries >= dscList->iSize) {

        /*- Se la lista e' gestita in memoria centrale... */
        if (dscList->iAccessMode == OVLS_PRIVATE) {

            /*- Alloca nuovo spazio per contenere il nuovo entry */
            dscList->iSize = dscList->iSize + OVLS_LISTENTRYBLOCK;
            dscList->szBase = (char *) realloc(dscList->szBase, dscList->iSize * dscList->iEntrySize);
            if (dscList->szBase != NULL) {
				/*- Aggiungi fisicamente l'entry alla lista */
                pchPtr = (char *)JADA_LstLsearch(pvEntry, dscList->szBase, 
                                        &(dscList->iEntries), dscList->iEntrySize, 
                                        dscList->pfnCompare);
                iRetCode = 0;
            }
        }
    }
	else {
		/*- Altrimenti aggiungi fisicamente l'entry alla lista */
        pchPtr = (char *)JADA_LstLsearch(pvEntry, dscList->szBase, 
                                &(dscList->iEntries), dscList->iEntrySize, 
                                dscList->pfnCompare);
        iRetCode = 0;
    }

    /*- Determina l'indirizzo in memoria dell'entry aggiunto */
    if (iRetCode == 0) {
        if (curC != NULL) {
            *curC = (pchPtr - dscList->szBase) / dscList->iEntrySize;
        }
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Notify the new entry */
#ifndef NO_PTHREAD
    if (iRetCode == 0) {
        iRet = pthread_cond_broadcast(&dscList->CondVar);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "generic error");
    }
#endif

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstDelete(CORE_List lstSelf, CORE_Currency *curC)
{
    JADA_LstDescr  *dscList;
    char  *pchCurr;
    char  *pchNext;
    int    i;
    int    iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se un currency entry non e' definito per la lista in ingresso... */
    if ((*curC >= 0) && (*curC < dscList->iEntries)) {

        /*- Per ciascun entry che segue quello da cancellare... */
        for (i = *curC; i < dscList->iEntries - 1; i++) {

            /*- Determina l'indirizzo in memoria dell'entry correntemente gestito */
            pchCurr = dscList->szBase + (i * dscList->iEntrySize);

            /*- Determina l'indirizzo in memoria dell'entry successivo */
            pchNext = dscList->szBase + ((i + 1) * dscList->iEntrySize);

            /*- Sposta l'entry successivo su quello correntemente gestito */
            memcpy(pchCurr, pchNext, dscList->iEntrySize);
        }

        /*- Aggiorna il numero di entry e il currency entry nel descrittore */
        (dscList->iEntries)--;
        (*curC)--;
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstUpdate(CORE_List lstSelf, void *pvEntry, int iLength, CORE_Currency *curC)
{
    char   *pchPtr;
    JADA_LstDescr  *dscList;
    int    iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se un currency entry non e' definito per la lista in ingresso... */
    if ((*curC >= 0) && (*curC < dscList->iEntries)) {

        /*- Determina l'indirizzo in memoria dell'entry da aggiornare */
        pchPtr = dscList->szBase + ((*curC) * dscList->iEntrySize);

        /*- Copia la nuova definizione nell'entry da aggiornare */
        memcpy(pchPtr, pvEntry, dscList->iEntrySize);
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


/******************************************************************************
 *
 *     E N T R Y     N A V I G A T I O N     S E R V I C E S
 *
 ******************************************************************************/
DllExport int __stdcall JADA_LstFind(CORE_List lstSelf, CORE_FunctionPtr pfnCompare, 
                                void *pvEntry, int iLength, CORE_Currency *curC)
{
    char  *pchPtr;
    JADA_LstDescr  *dscList;
    int    iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Cerca l'entry desiderato nella lista */
    if (pfnCompare == NULL) {
        pchPtr = (char *) JADA_LstLfind(pvEntry, dscList->szBase, &(dscList->iEntries),
                             dscList->iEntrySize, dscList->pfnCompare);
    }
    else {
        pchPtr = (char *) JADA_LstLfind(pvEntry, dscList->szBase, &(dscList->iEntries),
                             dscList->iEntrySize, pfnCompare);
    }

    if (pchPtr != NULL) {

        /*- Determina l'indirizzo in memoria dell'entry trovato */
        *curC = (pchPtr - dscList->szBase) / dscList->iEntrySize;

        /*- Copia il valore dell'entry trovato nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGet(CORE_List lstSelf, int iEntryNo, void *pvEntry, 
                                int iLength, CORE_Currency *curC)
{
    char    *pchPtr;
    JADA_LstDescr  *dscList;
    int    iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se alla posizione definita in ingresso non corrisponde alcun entry... */
    if ((iEntryNo >= 0) && (iEntryNo < dscList->iEntries)) {

        /*- Determina l'indirizzo in memoria dell'entry cercato */
        pchPtr =  dscList->szBase + (iEntryNo * dscList->iEntrySize);

        /*- Aggiorna il currency entry nel descrittore */
        *curC = iEntryNo;

        /*- Copia il valore dell'entry cercato nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGetCurrent(CORE_List lstSelf, void *pvEntry, int iLength, 
                                CORE_Currency *curC)
{
    char            *pchPtr;
    JADA_LstDescr   *dscList;
    int             iRetCode = -1;
    int             iEntryNo;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Leggi il currency entry */
    iEntryNo = *curC;

    /*- Se alla posizione corrente corrisponde un entry... */
    if ((iEntryNo >= 0) && (iEntryNo < dscList->iEntries)) {

        /*- Determina l'indirizzo in memoria dell'entry cercato */
        pchPtr =  dscList->szBase + (iEntryNo * dscList->iEntrySize);

        /*- Copia il valore dell'entry cercato nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGetIndex(CORE_List lstSelf, CORE_Currency *curC, int *piIndex)
{
    JADA_LstDescr *dscList;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Ritorna con esito positivo */
    *piIndex = *curC;
    return(0);
}


DllExport int __stdcall JADA_LstSetIndex(CORE_List lstSelf, int iIndex, CORE_Currency *curC)
{
    JADA_LstDescr *dscList;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Ritorna con esito positivo */
    *curC = iIndex;
    return(0);
}


DllExport int __stdcall JADA_LstGetFirst(CORE_List lstSelf, void *pvEntry, int iLength, 
                                CORE_Currency *curC)
{
    char            *pchPtr;
    JADA_LstDescr   *dscList;
    int              iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se non ci sono entry nella lista... */
    if (dscList->iEntries > 0) {

        /*- Definisce il primo entry della lista come currency */
        *curC = 0;

        /*- Determina l'indirizzo in memoria del currency entry */
        pchPtr = dscList->szBase + ((*curC) * dscList->iEntrySize);

        /*- Copia il valore del currency entry nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);
        iRetCode = 0;   
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGetNext(CORE_List lstSelf, void *pvEntry, int iLength, 
                                CORE_Currency *curC)
{
    char            *pchPtr;
    int             iTableLimit;
    JADA_LstDescr   *dscList;
    int             iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se non ci sono entry nella lista... */
    if (dscList->iEntries > 0) {

        /*- Calcola il numero di entry attualmente nella lista */
        iTableLimit = dscList->iEntries - 1;
        if (*curC < iTableLimit) {

            /*- Definisce il prossimo entry della lista come currency */
            (*curC)++;

            /*- Determina l'indirizzo in memoria del currency entry */
            pchPtr = dscList->szBase + ((*curC) * dscList->iEntrySize);

            /*- Copia il valore del currency entry nel relativo parametro di output */
            memcpy(pvEntry, pchPtr, dscList->iEntrySize);
            iRetCode = 0;
        }
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGetPrev(CORE_List lstSelf, void *pvEntry, int iLength, 
                                CORE_Currency *curC)
{
    char            *pchPtr;
    JADA_LstDescr   *dscList;
    int             iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Definisce l'entry precedente della lista come currency */
    if (*curC > 0) {
        (*curC)--;
    }

    /*- Determina l'indirizzo in memoria del currency entry */
    pchPtr = dscList->szBase + ((*curC) * dscList->iEntrySize);

    /*- Copia il valore del currency entry nel relativo parametro di output */
    memcpy(pvEntry, pchPtr, dscList->iEntrySize);
    iRetCode = 0;

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGetLast(CORE_List lstSelf, void *pvEntry, int iLength, 
                                CORE_Currency *curC)
{
    char            *pchPtr;
    JADA_LstDescr   *dscList;
    int             iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se non ci sono entry nella lista... */
    if (dscList->iEntries > 0) {

        /*- Definisce l'ultimo entry della lista come currency */
        *curC = dscList->iEntries - 1;

        /*- Determina l'indirizzo in memoria del currency entry */
        pchPtr = dscList->szBase + ((*curC) * dscList->iEntrySize);

        /*- Copia il valore del currency entry nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}


DllExport int __stdcall JADA_LstGetRandom(CORE_List lstSelf, void *pvEntry, int iLength, 
                                CORE_Currency *curC)
{
    int             iRetCode = -1;
    char            *pchPtr;
    JADA_LstDescr   *dscList;
    int             iIndex;
    float           fTmp;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");
    CORE_ReturnValIfFail(curC != NULL, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se ci sono entry nella lista... */
    if (dscList->iEntries > 0) {

        /*- Create a random number to select the entry */
        fTmp = (float)rand() / (float)RAND_MAX;
        iIndex = (int)(fTmp * (float)dscList->iEntries);

        /*- Get the selected entry entry */
        *curC = iIndex;

        /*- Determina l'indirizzo in memoria del currency entry */
        pchPtr = dscList->szBase + ((*curC) * dscList->iEntrySize);

        /*- Copia il valore del currency entry nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);
        iRetCode = 0;   
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}

DllExport int __stdcall JADA_LstWait(CORE_List lstSelf, void *pvEntry, int iLength, 
                                int iMsecsToWait)
{
    int             iRet, i;
    int             iRetCode = -1;
    JADA_LstDescr   *dscList;
    char            *pchPtr;
    char            *pchCurr;
    char            *pchNext;
    struct timespec TimeSpec;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");
    CORE_ReturnValIfFail(dscList->iEntrySize == iLength, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Se non ci sono entry nella lista... */
    if (dscList->iEntries == 0) {

        /*- Setto il tempo di attesa */
        iRet = CORE_SetTimeSpec(&TimeSpec, iMsecsToWait);

        /*- Aspetta per il tempo richiesto che ne arrivi uno */
#ifndef NO_PTHREAD
       iRet = pthread_cond_timedwait(&dscList->CondVar, &dscList->Mutex, 
                                &TimeSpec);
#endif
    }

    /*- Se ci sono entry nella lista... */
    if (dscList->iEntries > 0) {

        /*- Determina l'indirizzo in memoria del primo entry */
        pchPtr = dscList->szBase;

        /*- Copia il primo entry nel relativo parametro di output */
        memcpy(pvEntry, pchPtr, dscList->iEntrySize);

        //JADA_LogMessage("popping up %d entries...\n", 
        //                dscList->iEntries);

        /*- Per ciascun entry che segue il primo... */
        for (i = 0; i < dscList->iEntries - 1; i++) {

            /*- Determina l'indirizzo in memoria dell'entry correntemente gestito */
            pchCurr = dscList->szBase + (i * dscList->iEntrySize);

            /*- Determina l'indirizzo in memoria dell'entry successivo */
            pchNext = dscList->szBase + ((i + 1) * dscList->iEntrySize);

            /*- Sposta l'entry successivo su quello correntemente gestito */
            memcpy(pchCurr, pchNext, dscList->iEntrySize);
        }

        /*- Aggiorna il numero di entry nel descrittore */
        (dscList->iEntries)--;
        iRetCode = 0;
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(iRetCode);
}

/******************************************************************************
 *
 *     M A N A G E M E N T     S E R V I C E S
 *
 ******************************************************************************/
DllExport int __stdcall JADA_LstSort(CORE_List lstSelf, CORE_FunctionPtr pfnSort)
{
    JADA_LstDescr  *dscList;
    int    iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Ordina la lista */
    if (pfnSort == NULL) {
       JADA_LstMySort(dscList->szBase, dscList->iEntries, dscList->iEntrySize,
             dscList->pfnCompare);
    }
    else {
       JADA_LstMySort(dscList->szBase, dscList->iEntries, dscList->iEntrySize, pfnSort);
    }

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(0);
}


DllExport int __stdcall JADA_LstGetCount(CORE_List lstSelf)
{
    JADA_LstDescr  *dscList;
    int    iRetCode = -1;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");

    /*- Ritorna il numero di entry in lista */
    return(dscList->iEntries);
}


DllExport int __stdcall JADA_LstPrintDescr(CORE_List lstSelf)
{
    JADA_LstDescr  *dscList;


    /*- Get and check the list handle */
    CORE_ReturnValIfFail(lstSelf != NULL, -1, ;, "generic error");
    dscList = (JADA_LstDescr *) lstSelf;
    CORE_ReturnValIfFail(dscList->iMagic == JADA_MAGIC_LIST, -1, ;, "generic error");

    /*- Lock the list */
    JADA_LstLock(dscList);

    /*- Stampa i vari campi del descrittore */
    printf("szBase     : %p\n", dscList->szBase);
    printf("iSize      : %d\n", dscList->iSize);
    printf("iEntries   : %d\n", dscList->iEntries);
    printf("iEntrySize : %d\n", dscList->iEntrySize);
    printf("pfnCompare : %p\n", dscList->pfnCompare);

    /*- Unlock the list */
    JADA_LstUnlock(dscList);

    /*- Ritorna con esito positivo */
    return(0);
}
