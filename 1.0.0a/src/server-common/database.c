#include "jada.h"
#include "jada_internal.h"
#include "server.h"
#include "mysql/mysql.h"

static MYSQL mySqlSelf;


/******************************************************************************
 *
 *   S Q L     U T I L I T I E S
 *
 ******************************************************************************/
static char *ASRV_DbLogError(char *szStmt)
{
    static LongString   myDbMessage;

    sprintf(myDbMessage, "%s (%s)", mysql_error(&mySqlSelf), szStmt);
    return(myDbMessage);
}

static int ASRV_SqlGetJadaId(char *szSql, void *piJadaId)
{
    int         iRet, iResult = -1, iFields, iIdentifier = -1;
    long        iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;

    iRet = mysql_real_query(&mySqlSelf, szSql, strlen(szSql));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szSql));

    ptrResult = mysql_store_result(&mySqlSelf);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szSql));

    iRows = (long)mysql_affected_rows(&mySqlSelf);
    if (iRows > 0) {
        iFields = mysql_num_fields(ptrResult);
        CORE_ReturnValIfFail(iFields == 1, -1, ;, ASRV_DbLogError(szSql));

        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szSql));
        iIdentifier = atoi(currRow[0]); 
        mysql_free_result(ptrResult);

        memcpy(piJadaId, &iIdentifier, sizeof(iIdentifier));
        iResult = 0;
    }

    return(iResult);
}

static int ASRV_SqlGetString(char *szSql, char *szString, int iLength)
{
    int         iRet, iFields, iIdentifier = -1;
    long        iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;

    iRet = mysql_real_query(&mySqlSelf, szSql, strlen(szSql));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szSql));

    ptrResult = mysql_store_result(&mySqlSelf);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szSql));

    iRows = (long)mysql_affected_rows(&mySqlSelf);
    CORE_ReturnValIfFail(iRows == 1, -1, ;, ASRV_DbLogError(szSql));
    iFields = mysql_num_fields(ptrResult);
    CORE_ReturnValIfFail(iFields == 1, -1, ;, ASRV_DbLogError(szSql));

    currRow = mysql_fetch_row(ptrResult);
    CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szSql));
    strcpy(szString, currRow[0]); 
    mysql_free_result(ptrResult);

    return(0);
}

static MYSQL_RES *ASRV_SqlSelect(char *szSql, int *piRows)
{
    int         iRet;
    long        iRows;
    MYSQL_RES   *ptrResult;

    /* Leggi i dati della query di input */
    *piRows = 0;
    iRet = mysql_real_query(&mySqlSelf, szSql, strlen(szSql));
    CORE_ReturnValIfFail(iRet == 0, NULL, ;, ASRV_DbLogError(szSql));

    ptrResult = mysql_store_result(&mySqlSelf);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, ;, ASRV_DbLogError(szSql));

    iRows = (long)mysql_affected_rows(&mySqlSelf);
    *piRows = iRows;
    return(ptrResult);
}

static int ASRV_SqlInsert(char *szSql, void *piId)
{
    int             iRet, iId;
    //long            iRows;
    my_ulonglong    ulId;

    iRet = mysql_real_query(&mySqlSelf, szSql, strlen(szSql));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szSql));

    ulId = mysql_insert_id(&mySqlSelf);
    iId = (int) ulId;
    if (piId != NULL) {
        memcpy(piId, &iId, sizeof(iId));
    }

    return(0);
}

static int ASRV_SqlUpdate(char *szSql)
{
    int         iRet;
    //long        iRows;

    iRet = mysql_real_query(&mySqlSelf, szSql, strlen(szSql));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szSql));

    //iRows = (long)mysql_affected_rows(&mySqlSelf);
    //CORE_ReturnValIfFail(iRows == 1, -1, ;, ASRV_DbLogError(szSql));

    return(0);
}

/******************************************************************************
 *
 *   D A T A B A S E
 *
 ******************************************************************************/
int ASRV_DbStart(void)
{
    int         iRet, iPort;
    FixString   szHost, szUsername, szPassword, szDatabase;
    MYSQL       *sqlPtr;

    sqlPtr = mysql_init(&mySqlSelf);
    CORE_ReturnValIfFail(sqlPtr != NULL, -1, ;, "Errore generico");

    iRet = JADA_IniGetString("database", "host", szHost, sizeof(szHost));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError("GET HOST"));
    iRet = JADA_IniGetInt("database", "port", &iPort);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError("GET HOST"));
    iRet = JADA_IniGetString("database", "username", szUsername, sizeof(szUsername));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError("GET USERNAME"));
    iRet = JADA_IniGetString("database", "password", szPassword, sizeof(szPassword));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError("GET PASSWORD"));
    iRet = JADA_IniGetString("database", "database", szDatabase, sizeof(szDatabase));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError("GET DATABASE"));

    sqlPtr = mysql_real_connect(&mySqlSelf, szHost, szUsername, szPassword, szDatabase, iPort, NULL, 0);
    CORE_ReturnValIfFail(sqlPtr != NULL, -1, ;, ASRV_DbLogError("CONNECT TO DB"));

    return(0);
}

int ASRV_DbStop(void)
{
    mysql_close(&mySqlSelf);
    
    return(0);
}

/******************************************************************************
 *
 *   A G E N T
 *
 ******************************************************************************/
int ASRV_DbIsEnabledCliente(char *szCliente, char *szRegione)
{
    int                 iRet, iIdRegione, iIdCliente;
    LongString          szStmt;
    char                *szSqlRead1 = "\
SELECT ID FROM REGIONE WHERE NOME = '%s'";
    char                *szSqlRead2 = "\
SELECT ID FROM CLIENTE WHERE NOME = '%s' AND ID_REGIONE = %d";

    /* Leggi l'ID della regione */
    sprintf(szStmt, szSqlRead1, szRegione);
    iRet = ASRV_SqlGetJadaId(szStmt, &iIdRegione);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    /* Leggi l'ID del cliente */
    sprintf(szStmt, szSqlRead2, szCliente, iIdRegione);
    iRet = ASRV_SqlGetJadaId(szStmt, &iIdCliente);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    if (iIdCliente > 0) {
        return(0);
    }
    return(-1);
}

int ASRV_DbCreateAgent(ASRV_DbAgent *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlRead1 = "\
SELECT ID FROM REGIONE WHERE NOME = '%s'";
    char                *szSqlRead2 = "\
SELECT ID FROM CLIENTE WHERE NOME = '%s' AND ID_REGIONE = %d";
    char                *szSqlRead3 = "\
SELECT ID FROM AGENT WHERE NOME = '%s' AND ID_CLIENTE = %d";
    char                *szSqlInsert = "\
INSERT INTO AGENT (NOME, MAIL, TELEFONO, CELLULARE, USA_AUDIO, ID_CLIENTE) VALUES ('%s', '%s', '%s', '%s', %d, %d)";
    char                *szSqlUpdate = "\
UPDATE AGENT SET MAIL = '%s', TELEFONO = '%s', CELLULARE = '%s', USA_AUDIO = %d WHERE ID = %d";

    /* Leggi l'ID della regione */
    sprintf(szStmt, szSqlRead1, mySelf->szRegione);
    iRet = ASRV_SqlGetJadaId(szStmt, &mySelf->iRegioneId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    /* Leggi l'ID del cliente */
    sprintf(szStmt, szSqlRead2, mySelf->szCliente, mySelf->iRegioneId);
    iRet = ASRV_SqlGetJadaId(szStmt, &mySelf->iClienteId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    /* Leggi l'ID dell'agent */
    sprintf(szStmt, szSqlRead3, mySelf->szNome, mySelf->iClienteId);
    iRet = ASRV_SqlGetJadaId(szStmt, &mySelf->iId);

    /* Se l'agent e' gia' presente in tabella... */
    if (iRet == 0) {
        /* Aggiorna le informazioni dell'agent */
        sprintf(szStmt, szSqlUpdate, mySelf->szMail, mySelf->szTelefono, mySelf->szCellulare, mySelf->iUsaAudio, mySelf->iId);
        iRet = ASRV_SqlUpdate(szStmt);
    }
    else {
        /* Inserisci il nuovo agent in tabella */
        sprintf(szStmt, szSqlInsert, mySelf->szNome, mySelf->szMail, mySelf->szTelefono, mySelf->szCellulare, mySelf->iUsaAudio, mySelf->iClienteId);
        iRet = ASRV_SqlInsert(szStmt, &mySelf->iId);
    }

    return(0);
}

int ASRV_DbGetAgent(ASRV_DbAgent *mySelf)
{
    long        i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlRead = "\
SELECT A.ID, A.NOME, A.MAIL, A.TELEFONO, A.CELLULARE, A.USA_AUDIO, A.ID_CLIENTE, C.NOME, C.ID_REGIONE, R.NOME FROM AGENT A, CLIENTE C, REGIONE R \
WHERE A.ID_CLIENTE = C.ID AND C.ID_REGIONE = R.ID AND A.ID = %d";

    /* Leggi i dati del manager */
    sprintf(szStmt, szSqlRead, mySelf->iId);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, -1, ;);

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToAgtId(currRow[0], &mySelf->iId);
        strcpy(mySelf->szNome, currRow[1]);
        strcpy(mySelf->szMail, currRow[2]);
        strcpy(mySelf->szTelefono, currRow[3]);
        strcpy(mySelf->szCellulare, currRow[4]);
        mySelf->iUsaAudio = atoi(currRow[5]); 
        mySelf->iClienteId = atoi(currRow[6]); 
        strcpy(mySelf->szCliente, currRow[7]);
        mySelf->iRegioneId = atoi(currRow[8]); 
        strcpy(mySelf->szRegione, currRow[9]);
    }

    mysql_free_result(ptrResult);

    return(0);
}

int ASRV_DbGetAgentByConnection(ASRV_DbAgent *mySelf, JADA_ConnId iConnId)
{
    long        i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlRead = "\
SELECT A.ID, A.NOME, A.MAIL, A.TELEFONO, A.CELLULARE, A.USA_AUDIO, A.ID_CLIENTE, C.NOME, C.ID_REGIONE, R.NOME FROM AGENT A, CLIENTE C, REGIONE R, CONNESSIONE CC \
WHERE A.ID_CLIENTE = C.ID AND C.ID_REGIONE = R.ID AND A.ID = CC.ID_AGENT AND CC.ID = %d";

    /* Leggi i dati dell'agent */
    sprintf(szStmt, szSqlRead, iConnId);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, -1, ;);

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToAgtId(currRow[0], &mySelf->iId);
        strcpy(mySelf->szNome, currRow[1]);
        strcpy(mySelf->szMail, currRow[2]);
        strcpy(mySelf->szTelefono, currRow[3]);
        strcpy(mySelf->szCellulare, currRow[4]);
        mySelf->iUsaAudio = atoi(currRow[5]); 
        mySelf->iClienteId = atoi(currRow[6]); 
        strcpy(mySelf->szCliente, currRow[7]);
        mySelf->iRegioneId = atoi(currRow[8]); 
        strcpy(mySelf->szRegione, currRow[9]);
    }

    mysql_free_result(ptrResult);

    return(0);
}

/******************************************************************************
 *
 *   M A N A G E R
 *
 ******************************************************************************/
int ASRV_DbCheckManager(ASRV_DbManager *mySelf, char *szUsername, char *szPassword, int *piResult)
{
    long        iRet, i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlCheckUser = "\
SELECT ID, PASSWORD FROM MANAGER WHERE USERNAME = '%s'";
    char        *szSqlCheckPassword = "\
SELECT ID, NOME, USERNAME, MAIL, TELEFONO, CELLULARE, AREA FROM MANAGER WHERE USERNAME = '%s' AND PASSWORD = '%s'";
    char        *szSqlSetPassword = "\
UPDATE MANAGER SET PASSWORD = '%s' WHERE USERNAME = '%s'";

    /* Controlla se il manager esiste */
    *piResult = JADA_CREDENTIAL_UNKNOWN_USER;
    sprintf(szStmt, szSqlCheckUser, szUsername);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, 0, ;);

    /* Se il manager accede per la prima volta, setta la password con quella passata */
    currRow = mysql_fetch_row(ptrResult);
    CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));
    if (strcmp(currRow[1], "*") == 0) {
        sprintf(szStmt, szSqlSetPassword, szPassword, szUsername);
        iRet = ASRV_SqlUpdate(szStmt);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));
    }
    mysql_free_result(ptrResult);

    /* Controlla se la password del manager e' corretta */
    *piResult = JADA_CREDENTIAL_WRONG_PASSWORD;
    sprintf(szStmt, szSqlCheckPassword, szUsername, szPassword);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, 0, ;);

    /* Leggi i dati del manager */
    *piResult = JADA_CREDENTIAL_OK;
    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToMgrId(currRow[0], &mySelf->iId);
        strcpy(mySelf->szNome, currRow[1]); 
        strcpy(mySelf->szUsername, currRow[2]); 
        strcpy(mySelf->szMail, currRow[3]); 
        strcpy(mySelf->szTelefono, currRow[4]); 
        strcpy(mySelf->szCellulare, currRow[5]); 
        strcpy(mySelf->szArea, currRow[6]);
    }

    mysql_free_result(ptrResult);

    return(0);
}

int ASRV_DbSetManager(ASRV_DbManager *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE MANAGER SET NOME = '%s', MAIL = '%s', TELEFONO = '%s', CELLULARE = '%s', USA_AUDIO = %d WHERE ID = %d";

    /* Aggiorna le informazioni del manager */
    sprintf(szStmt, szSqlUpdate, mySelf->szNome, mySelf->szMail, mySelf->szTelefono, mySelf->szCellulare, mySelf->iUsaAudio, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);

    return(0);
}

int ASRV_DbGetManager(ASRV_DbManager *mySelf)
{
    long        i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlRead = "\
SELECT ID, NOME, USERNAME, MAIL, TELEFONO, CELLULARE, USA_AUDIO, AREA FROM MANAGER WHERE ID = %d";

    /* Leggi i dati del manager */
    sprintf(szStmt, szSqlRead, mySelf->iId);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, -1, ;);

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToMgrId(currRow[0], &mySelf->iId);
        strcpy(mySelf->szNome, currRow[1]);
        strcpy(mySelf->szUsername, currRow[2]);
        strcpy(mySelf->szMail, currRow[3]);
        strcpy(mySelf->szTelefono, currRow[4]);
        strcpy(mySelf->szCellulare, currRow[5]);
        mySelf->iUsaAudio = atoi(currRow[6]);
        strcpy(mySelf->szArea, currRow[7]);
    }

    mysql_free_result(ptrResult);

    return(0);
}

int ASRV_DbGetManagerByConnection(ASRV_DbManager *mySelf, JADA_ConnId iConnId)
{
    long        i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlRead = "\
SELECT M.ID, M.NOME, M.USERNAME, M.MAIL, M.TELEFONO, M.CELLULARE, M.USA_AUDIO, M.AREA FROM MANAGER M, CONNESSIONE CC WHERE M.ID = CC.ID_MANAGER AND CC.ID = %d";

    /* Leggi i dati del manager */
    sprintf(szStmt, szSqlRead, iConnId);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, -1, ;);

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToMgrId(currRow[0], &mySelf->iId);
        strcpy(mySelf->szNome, currRow[1]);
        strcpy(mySelf->szUsername, currRow[2]);
        strcpy(mySelf->szMail, currRow[3]);
        strcpy(mySelf->szTelefono, currRow[4]);
        strcpy(mySelf->szCellulare, currRow[5]);
        mySelf->iUsaAudio = atoi(currRow[6]);
        strcpy(mySelf->szArea, currRow[7]);
    }

    mysql_free_result(ptrResult);

    return(0);
}

/******************************************************************************
 *
 *   C O N N E S S I O N E
 *
 ******************************************************************************/
int ASRV_DbCreateConnessione(ASRV_DbConnessione *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlInsert = "\
INSERT INTO CONNESSIONE (INDIRIZZO_IP, INDIRIZZO_IP_HOST, STATO) VALUES ('%s', '%s', '%c')";

    /* Aggiorna lo stato */
    mySelf->iStato = ASRV_CONNSTATUS_OPENED;

    /* Salva le informazioni di connessione dell'agent */
    sprintf(szStmt, szSqlInsert, mySelf->szRealIP, mySelf->szHostIP, mySelf->iStato);
    iRet = ASRV_SqlInsert(szStmt, &mySelf->iId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetConnessione(ASRV_DbConnessione *mySelf)
{
    int                 iRet = -1;
    LongString          szStmt;
    char                *szSqlUpdateAgent = "\
UPDATE CONNESSIONE SET INDIRIZZO_IP = '%s', INDIRIZZO_IP_HOST = '%s', \
    ID_AGENT = %d, STATO = '%c', USA_AUDIO = %d WHERE ID = %d";
    char                *szSqlUpdateManager = "\
UPDATE CONNESSIONE SET INDIRIZZO_IP = '%s', INDIRIZZO_IP_HOST = '%s', \
    ID_MANAGER = %d, STATO = '%c', USA_AUDIO = %d WHERE ID = %d";

    /* Aggiorna lo stato */
    mySelf->iStato = ASRV_CONNSTATUS_RUNNING;

    /* Aggiorna le informazioni di connessione */
    if (JADA_IsValidJadaId(&mySelf->iIdAgent)) {
        sprintf(szStmt, szSqlUpdateAgent, mySelf->szRealIP, mySelf->szHostIP, mySelf->iIdAgent, mySelf->iStato, mySelf->iUsaAudio, mySelf->iId);
        iRet = ASRV_SqlUpdate(szStmt);
    }
    else if (JADA_IsValidJadaId(&mySelf->iIdManager)) {
        sprintf(szStmt, szSqlUpdateManager, mySelf->szRealIP, mySelf->szHostIP, mySelf->iIdManager, mySelf->iStato, mySelf->iUsaAudio, mySelf->iId);
        iRet = ASRV_SqlUpdate(szStmt);
    }
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbDestroyConnessione(ASRV_DbConnessione *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE CONNESSIONE SET STATO = '%c', DATA_FINE = NOW() WHERE ID = %d";

    /* Aggiorna le informazioni di connessione */
    mySelf->iStato = ASRV_CONNSTATUS_CLOSED;
    sprintf(szStmt, szSqlUpdate, mySelf->iStato, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetParametro(ASRV_DbConnessione *mySelf, char *szSection, char *szName, char *szValue)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSql = "\
INSERT INTO PARAMETRO(ID_CONNESSIONE, SEZIONE, NOME, VALORE) VALUES ('%d', '%s', '%s', '%s') \
ON DUPLICATE KEY UPDATE VALORE = '%s'";

    /* Aggiorna le informazioni di connessione */
    sprintf(szStmt, szSql, mySelf->iId, szSection, szName, szValue, szValue);
    iRet = ASRV_SqlInsert(szStmt, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));
    // JADA_LogMessage("Aggiornato parametro per %d: %s:%s = %s\n", mySelf->iId, szSection, szName, szValue);

    return(0);
}

CORE_List ASRV_DbListParametri(ASRV_DbConnessione *mySelf, CORE_List lstSelf, char *szFilter)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstParametri;
    ASRV_Parameter      myParametro;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT P.SEZIONE, P.NOME, P.VALORE FROM PARAMETRO P WHERE P.ID_CONNESSIONE = %d AND (P.SEZIONE = '%s' OR '%s' = '')";

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, mySelf->iId, szFilter, szFilter);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, ;, ASRV_DbLogError(szStmt));

    if (lstSelf == NULL) {
        lstParametri = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_Parameter), NULL);
        CORE_ReturnValIfFail(lstParametri != NULL, NULL, ;, "Errore generico");
    }
    else {
        lstParametri = lstSelf;
    }

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstParametri);, ASRV_DbLogError(szStmt));

        memset(&myParametro, 0, sizeof(myParametro));
        strcpy(myParametro.szSection, currRow[0]); 
        strcpy(myParametro.szName, currRow[1]); 
        strcpy(myParametro.szValue, currRow[2]); 

        iRet = JADA_LstAdd(lstParametri, &myParametro, sizeof(ASRV_Parameter), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nella add alla lista delle richieste");
    }

    mysql_free_result(ptrResult);

    return(lstParametri);
}


/******************************************************************************
 *
 *   R I C H I E S T A
 *
 ******************************************************************************/
int ASRV_DbCreateRichiesta(ASRV_DbRichiesta *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlReadCoda = "\
SELECT ID FROM CODA WHERE NOME = '%s'";
    char                *szSqlInsert = "\
INSERT INTO RICHIESTA (STATO, ID_CODA, ID_AGENT, DESCRIZIONE) VALUES ('%c', %d, %d, '%s')";

    /* Leggi le informazioni sulla coda */
    if (! JADA_IsValidJadaId(&mySelf->iIdCoda)) {
        sprintf(szStmt, szSqlReadCoda, mySelf->szCoda);
        iRet = ASRV_SqlGetJadaId(szStmt, &mySelf->iIdCoda);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));
    }

    /* Aggiorna lo stato */
    mySelf->iStato = ASRV_REQSTATUS_OPENED;

    /* Salva le informazioni di richiesta */
    sprintf(szStmt, szSqlInsert, mySelf->iStato, mySelf->iIdCoda, mySelf->iIdAgent, mySelf->szDescrizione);
    iRet = ASRV_SqlInsert(szStmt, &mySelf->iId);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetRichiesta(ASRV_DbRichiesta *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE RICHIESTA SET STATO = '%c', ID_CODA = %d, DESCRIZIONE = '%s' WHERE ID = %d";

    /* Salva le informazioni di richiesta */
    sprintf(szStmt, szSqlUpdate, mySelf->iStato, mySelf->iIdCoda, mySelf->szDescrizione, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetStatusRichiesta(ASRV_DbRichiesta *mySelf, int iStato)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE RICHIESTA SET STATO = '%c' WHERE ID = %d AND STATO <> '%c'";

    /* Salva le informazioni di richiesta */
    mySelf->iStato = iStato;
    sprintf(szStmt, szSqlUpdate, mySelf->iStato, mySelf->iId, ASRV_REQSTATUS_CLOSED);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbGetRichiesta(ASRV_DbRichiesta *mySelf)
{
    int         iRet;
    long        i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlRead1 = "\
SELECT ID, STATO, ID_CODA, ID_AGENT, DESCRIZIONE, DATA_INIZIO, DATA_FINE FROM RICHIESTA WHERE ID = %d";
    char        *szSqlRead2 = "\
SELECT NOME FROM CODA WHERE ID = %d";

    /* Leggi i dati del manager */
    sprintf(szStmt, szSqlRead1, mySelf->iId);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_SilentReturnValIfFail(iRows == 1, -1, ;);

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToReqId(currRow[0], &mySelf->iId);
        mySelf->iStato = currRow[1][0]; 
        JADA_StringToQueueId(currRow[2], &mySelf->iIdCoda);
        JADA_StringToAgtId(currRow[3], &mySelf->iIdAgent); 
        strcpy(mySelf->szDescrizione, currRow[4]); 

        sprintf(szStmt, szSqlRead2, mySelf->iIdCoda);
        iRet = ASRV_SqlGetString(szStmt, mySelf->szCoda, sizeof(mySelf->szCoda));
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    }

    mysql_free_result(ptrResult);

    return(0);
}

int ASRV_DbDestroyRichiesta(ASRV_DbRichiesta *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE RICHIESTA SET STATO = '%c', DATA_FINE = NOW() WHERE ID = %d";

    /* Salva le informazioni di richiesta */
    mySelf->iStato = ASRV_REQSTATUS_CLOSED;
    sprintf(szStmt, szSqlUpdate, mySelf->iStato, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

CORE_List ASRV_DbListRichiesteAperte(char *szCoda)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstRichieste;
    ASRV_DbRichiesta    myRichiesta;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT \
    R.ID, R.STATO, R.ID_CODA, C.NOME, R.ID_AGENT, R.DESCRIZIONE, R.DATA_INIZIO \
FROM \
    RICHIESTA R, CODA C \
WHERE \
    R.ID_CODA = C.ID AND R.STATO IN ('%c') AND (C.NOME = '%s' OR '%s' = '') ORDER BY R.ID";

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, ASRV_REQSTATUS_OPENED, szCoda, szCoda);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, ;, ASRV_DbLogError(szStmt));

    lstRichieste = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_DbRichiesta), NULL);
    CORE_ReturnValIfFail(lstRichieste != NULL, NULL, ;, "Errore generico");

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);, ASRV_DbLogError(szStmt));

        memset(&myRichiesta, 0, sizeof(myRichiesta));
        JADA_StringToReqId(currRow[0], &myRichiesta.iId);
        myRichiesta.iStato = currRow[1][0]; 
        JADA_StringToQueueId(currRow[2], &myRichiesta.iIdCoda); 
        strcpy(myRichiesta.szCoda, currRow[3]); 
        JADA_StringToAgtId(currRow[4], &myRichiesta.iIdAgent); 
        strcpy(myRichiesta.szDescrizione, currRow[5]); 
        //strcpy(myRichiesta.timDataInizio, currRow[6]); 

        iRet = JADA_LstAdd(lstRichieste, &myRichiesta, sizeof(myRichiesta), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nella add alla lista delle richieste");
    }

    mysql_free_result(ptrResult);

    return(lstRichieste);
}

CORE_List ASRV_DbCountRichiesteAperte(JADA_MgrId *piIdManager)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstRichieste;
    ASRV_DbRichiesta    myRichiesta;
    LongString          szStmt;
    char                *szSqlAll = "\
SELECT DISTINCT \
    R.ID, R.ID_AGENT \
FROM \
    RICHIESTA R \
WHERE \
    R.STATO = '%c' ORDER BY R.ID";
    char                *szSqlManager = "\
SELECT DISTINCT \
    R.ID, R.ID_AGENT \
FROM \
    RICHIESTA R, MANAGER_CODA MC \
WHERE \
    R.STATO = '%c' AND R.ID_CODA = MC.ID_CODA AND (MC.ID_MANAGER = %s) ORDER BY R.ID";


    /* Leggi i dati del manager */
    if (piIdManager != NULL) {
        sprintf(szStmt, szSqlManager, ASRV_REQSTATUS_OPENED, JADA_JadaIdToString(piIdManager, NULL));
    }
    else {
        sprintf(szStmt, szSqlAll, ASRV_REQSTATUS_OPENED);
    }

    /* Leggi la lista delle richieste */
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, ;, ASRV_DbLogError(szStmt));

    lstRichieste = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_DbRichiesta), NULL);
    CORE_ReturnValIfFail(lstRichieste != NULL, NULL, ;, "Errore generico");

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);, ASRV_DbLogError(szStmt));

        memset(&myRichiesta, 0, sizeof(myRichiesta));
        JADA_StringToReqId(currRow[0], &myRichiesta.iId);
        JADA_StringToAgtId(currRow[1], &myRichiesta.iIdAgent); 

        iRet = JADA_LstAdd(lstRichieste, &myRichiesta, sizeof(myRichiesta), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, ;, "errore nella add alla lista delle richieste");
    }

    mysql_free_result(ptrResult);

    return(lstRichieste);
}

//CORE_List ASRV_DbListRichiestePending(char *szCoda)
//{
//    int                 iRet;
//    long                i, iRows;
//    MYSQL_RES           *ptrResult;
//    MYSQL_ROW           currRow;
//    CORE_List           lstRichieste;
//    ASRV_DbRichiesta    myRichiesta;
//    LongString          szStmt;
//    char                *szSqlRead = "\
//SELECT \
//    R.ID, R.STATO, R.ID_CODA, C.NOME, R.ID_AGENT, R.DESCRIZIONE, R.DATA_INIZIO \
//FROM \
//    RICHIESTA R, CODA C \
//WHERE \
//    R.ID_CODA = C.ID AND R.STATO = '%c' AND (C.NOME = '%s' OR '%s' = '') ORDER BY R.ID";
//
//    lstRichieste = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_DbRichiesta), NULL);
//    CORE_ReturnValIfFail(lstRichieste != NULL, NULL, ;, "Errore generico");
//
//    /* Leggi la lista delle richieste */
//    sprintf(szStmt, szSqlRead, ASRV_REQSTATUS_PENDING, szCoda, szCoda);
//    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
//    CORE_ReturnValIfFail(ptrResult != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);, ASRV_DbLogError(szStmt));
//
//    for (i = 0; i < iRows; i++) {
//        currRow = mysql_fetch_row(ptrResult);
//        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);, ASRV_DbLogError(szStmt));
//
//        memset(&myRichiesta, 0, sizeof(myRichiesta));
//        JADA_StringToReqId(currRow[0], &myRichiesta.iId); 
//        myRichiesta.iStato = currRow[1][0]; 
//        JADA_StringToQueueId(currRow[2], &myRichiesta.iIdCoda); 
//        strcpy(myRichiesta.szCoda, currRow[3]); 
//        JADA_StringToAgtId(currRow[4], &myRichiesta.iIdAgent); 
//        strcpy(myRichiesta.szDescrizione, currRow[5]); 
//        //strcpy(myRichiesta.timDataInizio, currRow[6]); 
//
//        iRet = JADA_LstAdd(lstRichieste, &myRichiesta, sizeof(myRichiesta), NULL);
//        CORE_ReturnValIfFail(iRet == 0, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstRichieste);, "errore nella LstAdd");
//    }
//
//    mysql_free_result(ptrResult);
//
//    return(lstRichieste);
//}

//int ASRV_DbGetCountRichieste(JADA_MgrId *piIdManager, int iStato, int *piCount)
//{
//    long        i, iRows;
//    MYSQL_RES   *ptrResult;
//    MYSQL_ROW   currRow;
//    LongString  szStmt;
//    char        *szSqlManager = "\
//SELECT COUNT(R.ID) FROM RICHIESTA R, MANAGER_CODA MC WHERE (R.STATO = '%c') \
//                AND R.ID_CODA = MC.ID_CODA AND (MC.ID_MANAGER = %s)";
//    char        *szSqlAll = "\
//SELECT COUNT(R.ID) FROM RICHIESTA R WHERE (R.STATO = '%c')";
//
//    /* Leggi i dati del manager */
//    if (piIdManager != NULL) {
//        sprintf(szStmt, szSqlManager, iStato, JADA_JadaIdToString(piIdManager, NULL));
//    }
//    else {
//        sprintf(szStmt, szSqlAll, iStato);
//    }
//    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
//    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
//    CORE_ReturnValIfFail(iRows == 1, -1, ;, ASRV_DbLogError(szStmt));
//
//    for (i = 0; i < iRows; i++) {
//        currRow = mysql_fetch_row(ptrResult);
//        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));
//
//        *piCount = atoi(currRow[0]); 
//    }
//
//    mysql_free_result(ptrResult);
//
//    return(0);
//}

/******************************************************************************
 *
 *   S E S S I O N E
 *
 ******************************************************************************/
int ASRV_DbCreateSessione(ASRV_DbSessione *mySelf)
{
    int                 iRet = -1;
    LongString          szStmt = "";
    char                *szSqlInsertAgent = "\
INSERT INTO SESSIONE (ID_CONNESSIONE_AGENT, ID_CONNESSIONE_MANAGER, STATO, ID_RICHIESTA) VALUES (%d, NULL, '%c', %d)";
    char                *szSqlInsertManager = "\
INSERT INTO SESSIONE (ID_CONNESSIONE_AGENT, ID_CONNESSIONE_MANAGER, STATO, ID_RICHIESTA) VALUES (NULL, %d, '%c', %d)";

    /* Aggiorna lo stato */
    mySelf->iStato = ASRV_SESSSTATUS_OPENED;

    /* Salva le informazioni di sessione */
    if (JADA_IsValidJadaId(&mySelf->iIdConnAgent)) {
        sprintf(szStmt, szSqlInsertAgent, mySelf->iIdConnAgent, mySelf->iStato, mySelf->iIdRichiesta);
    }
    else if (JADA_IsValidJadaId(&mySelf->iIdConnManager)) {
        sprintf(szStmt, szSqlInsertManager, mySelf->iIdConnManager, mySelf->iStato, mySelf->iIdRichiesta);
    }
    if (strlen(szStmt) > 0) {
        iRet = ASRV_SqlInsert(szStmt, &mySelf->iId);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));
    }

    return(iRet);
}

int ASRV_DbSetSessione(ASRV_DbSessione *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE SESSIONE SET ID_CONNESSIONE_AGENT = %d, ID_CONNESSIONE_MANAGER = %d, STATO = '%c', ID_RICHIESTA = %d WHERE ID = %d";

    /* Aggiorna le informazioni di connessione */
    sprintf(szStmt, szSqlUpdate, mySelf->iIdConnAgent, mySelf->iIdConnManager, mySelf->iStato, mySelf->iIdRichiesta, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetStatusSessione(ASRV_DbSessione *mySelf, int iStato)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE SESSIONE SET STATO = '%c' WHERE ID = %d AND STATO <> '%c'";

    /* Salva le informazioni di richiesta */
    mySelf->iStato = iStato;
    sprintf(szStmt, szSqlUpdate, mySelf->iStato, mySelf->iId, ASRV_SESSSTATUS_CLOSED);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetAgentSessione(ASRV_DbSessione *mySelf, JADA_ConnId *piConnId)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE SESSIONE SET ID_CONNESSIONE_AGENT = %d WHERE ID = %d";

    /* Aggiorna le informazioni di connessione */
    if (piConnId != NULL) {
        mySelf->iIdConnAgent = *piConnId;
    }
    else {
        mySelf->iIdConnAgent.iConnId = 0;
    }
    sprintf(szStmt, szSqlUpdate, mySelf->iIdConnAgent, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetManagerSessione(ASRV_DbSessione *mySelf, JADA_ConnId *piConnId)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE SESSIONE SET ID_CONNESSIONE_MANAGER = %d WHERE ID = %d";

    /* Aggiorna le informazioni di connessione */
    if (piConnId != NULL) {
        mySelf->iIdConnManager = *piConnId;
    }
    else {
        mySelf->iIdConnManager.iConnId = 0;
    }
    sprintf(szStmt, szSqlUpdate, mySelf->iIdConnManager, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbSetRichiestaIdSessione(ASRV_DbSessione *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE SESSIONE SET ID_RICHIESTA = %d WHERE ID = %d";

    /* Aggiorna le informazioni di sessione */
    sprintf(szStmt, szSqlUpdate, mySelf->iIdRichiesta, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbDestroySessione(ASRV_DbSessione *mySelf)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE SESSIONE SET STATO = '%c', DATA_FINE = NOW() WHERE ID = %d";

    /* Aggiorna le informazioni di connessione */
    mySelf->iStato = ASRV_REQSTATUS_CLOSED;
    sprintf(szStmt, szSqlUpdate, mySelf->iStato, mySelf->iId);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbGetSessione(ASRV_DbSessione *mySelf)
{
    long        i, iRows;
    MYSQL_RES   *ptrResult;
    MYSQL_ROW   currRow;
    LongString  szStmt;
    char        *szSqlRead = "\
SELECT ID, ID_CONNESSIONE_AGENT, ID_CONNESSIONE_MANAGER, STATO, ID_RICHIESTA FROM SESSIONE WHERE ID = %d";

    /* Leggi i dati della sessione */
    sprintf(szStmt, szSqlRead, mySelf->iId);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, -1, ;, ASRV_DbLogError(szStmt));
    CORE_ReturnValIfFail(iRows == 1, -1, ;, "sessione non trovata nel DB");

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, -1, ;, ASRV_DbLogError(szStmt));

        JADA_StringToSessId(currRow[0], &mySelf->iId); 
        JADA_StringToConnId(currRow[1], &mySelf->iIdConnAgent);
        JADA_StringToConnId(currRow[2], &mySelf->iIdConnManager);
        mySelf->iStato = currRow[3][0];
        JADA_StringToReqId(currRow[4], &mySelf->iIdRichiesta);
    }

    mysql_free_result(ptrResult);

    return(0);
}

/******************************************************************************
 *
 *   C H A T
 *
 ******************************************************************************/
int ASRV_DbSendChat(JADA_ReqId iIdRichiesta, int iMittente, char *szText)
{
    int                 iRet;
    LongString          szStmt;
    ShortString         szMittente = "?";
    char                *szSqlInsert = "\
INSERT INTO CHAT (ID_RICHIESTA, MITTENTE, TESTO, DATA_RICEZIONE) VALUES (%d, '%s', '%s', CURRENT_TIMESTAMP)";

    /* Determina il mittente */
    switch (iMittente) {
    case JADA_WHOAMI_AGENT:
        strcpy(szMittente, "A");
        break;
    case JADA_WHOAMI_MANAGER:
        strcpy(szMittente, "M");
        break;
    }

    /* Salva le informazioni di chat */
    sprintf(szStmt, szSqlInsert, iIdRichiesta, szMittente, szText);
    iRet = ASRV_SqlInsert(szStmt, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbCacheChat(JADA_ReqId iIdRichiesta, int iMittente, char *szText)
{
    int                 iRet;
    LongString          szStmt;
    ShortString         szMittente = "?";
    char                *szSqlInsert = "\
INSERT INTO CHAT (ID_RICHIESTA, MITTENTE, TESTO) VALUES (%d, '%s', '%s')";

    /* Determina il mittente */
    switch (iMittente) {
    case JADA_WHOAMI_AGENT:
        strcpy(szMittente, "A");
        break;
    case JADA_WHOAMI_MANAGER:
        strcpy(szMittente, "M");
        break;
    }

    /* Salva le informazioni di chat */
    sprintf(szStmt, szSqlInsert, iIdRichiesta, szMittente, szText);
    iRet = ASRV_SqlInsert(szStmt, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

CORE_List ASRV_DbListCachedChat(JADA_ReqId iIdRichiesta)
{
    int                 iRet;
    int                 i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstChat;
    ASRV_DbChat         myChat;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT ID_RICHIESTA, MITTENTE, TESTO, DATA_INVIO, DATA_RICEZIONE FROM CHAT \
WHERE ID_RICHIESTA = %d AND DATA_RICEZIONE IS NULL ORDER BY DATA_INVIO";

    lstChat = JADA_LstNew(__FILE__, __LINE__, sizeof(ASRV_DbChat), NULL);
    CORE_ReturnValIfFail(lstChat != NULL, NULL, ;, "Errore generico");

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, iIdRichiesta);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstChat);, ASRV_DbLogError(szStmt));

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstChat);, ASRV_DbLogError(szStmt));

        memset(&myChat, 0, sizeof(myChat));
        JADA_StringToReqId(currRow[0], &myChat.iIdRichiesta);
        if (strcmp(currRow[1], "A") == 0) {
            myChat.iMittente = JADA_WHOAMI_AGENT;
        }
        else if (strcmp(currRow[1], "M") == 0) {
            myChat.iMittente = JADA_WHOAMI_MANAGER;
        }
        strcpy(myChat.szTesto, currRow[2]);
        JADA_StringToTimet(currRow[3], &myChat.timDataInvio); 
        JADA_StringToTimet(currRow[4], &myChat.timDataRicezione); 

        iRet = JADA_LstAdd(lstChat, &myChat, sizeof(myChat), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstChat);, "errore nella LstAdd");
    }

    mysql_free_result(ptrResult);

    return(lstChat);
}

int ASRV_DbCachedChatSent(JADA_ReqId iIdRichiesta)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE CHAT SET DATA_RICEZIONE = CURRENT_TIMESTAMP WHERE ID_RICHIESTA = %d AND DATA_RICEZIONE IS NULL";

    /* Salva le informazioni di desktop remoto */
    sprintf(szStmt, szSqlUpdate, iIdRichiesta);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

/******************************************************************************
 *
 *   V N C
 *
 ******************************************************************************/
int ASRV_DbStartVnc(JADA_SessId iIdSessione)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlInsert = "\
INSERT INTO VNC (ID_SESSIONE) VALUES (%d)";

    /* Salva le informazioni di desktop remoto */
    sprintf(szStmt, szSqlInsert, iIdSessione);
    iRet = ASRV_SqlInsert(szStmt, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

int ASRV_DbStopVnc(JADA_SessId iIdSessione)
{
    int                 iRet;
    LongString          szStmt;
    char                *szSqlUpdate = "\
UPDATE VNC SET DATA_FINE = CURRENT_TIMESTAMP WHERE ID_SESSIONE = %d AND DATA_FINE IS NULL";

    /* Salva le informazioni di desktop remoto */
    sprintf(szStmt, szSqlUpdate, iIdSessione);
    iRet = ASRV_SqlUpdate(szStmt);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, ASRV_DbLogError(szStmt));

    return(0);
}

CORE_List ASRV_DbGetComboRegione(char *szFilter)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstValues;
    ShortString         myValue;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT R.NOME FROM REGIONE R ORDER BY R.NOME";

    lstValues = JADA_LstNew(__FILE__, __LINE__, sizeof(myValue), NULL);
    CORE_ReturnValIfFail(lstValues != NULL, NULL, ;, "Errore generico");

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, szFilter);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

        memset(myValue, 0, sizeof(myValue));
        strcpy(myValue, currRow[0]); 

        iRet = JADA_LstAdd(lstValues, myValue, sizeof(myValue), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, "errore nella LstAdd");
    }

    mysql_free_result(ptrResult);

    return(lstValues);
}

CORE_List ASRV_DbGetComboCliente(char *szFilter)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstValues;
    ShortString         myValue;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT C.NOME FROM REGIONE R, CLIENTE C WHERE R.ID = C.ID_REGIONE AND R.NOME = '%s' ORDER BY C.NOME";

    lstValues = JADA_LstNew(__FILE__, __LINE__, sizeof(myValue), NULL);
    CORE_ReturnValIfFail(lstValues != NULL, NULL, ;, "Errore generico");

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, szFilter);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

        memset(myValue, 0, sizeof(myValue));
        strcpy(myValue, currRow[0]); 

        iRet = JADA_LstAdd(lstValues, myValue, sizeof(myValue), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, "errore nella LstAdd");
    }

    mysql_free_result(ptrResult);

    return(lstValues);
}

CORE_List ASRV_DbGetComboCoda(char *szFilter)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstValues;
    ShortString         myValue;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT C.NOME FROM CODA C ORDER BY C.NOME";

    lstValues = JADA_LstNew(__FILE__, __LINE__, sizeof(myValue), NULL);
    CORE_ReturnValIfFail(lstValues != NULL, NULL, ;, "Errore generico");

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, szFilter);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

        memset(myValue, 0, sizeof(myValue));
        strcpy(myValue, currRow[0]); 

        iRet = JADA_LstAdd(lstValues, myValue, sizeof(myValue), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, "errore nella LstAdd");
    }

    mysql_free_result(ptrResult);

    return(lstValues);
}

CORE_List ASRV_DbGetComboManagerCoda(char *szFilter)
{
    int                 iRet;
    long                i, iRows;
    MYSQL_RES           *ptrResult;
    MYSQL_ROW           currRow;
    CORE_List           lstValues;
    ShortString         myValue;
    LongString          szStmt;
    char                *szSqlRead = "\
SELECT C.NOME FROM CODA C, MANAGER_CODA MC, MANAGER M \
WHERE C.ID = MC.ID_CODA AND M.ID = MC.ID_MANAGER AND M.USERNAME = '%s' \
ORDER BY C.NOME";

    lstValues = JADA_LstNew(__FILE__, __LINE__, sizeof(myValue), NULL);
    CORE_ReturnValIfFail(lstValues != NULL, NULL, ;, "Errore generico");

    /* Leggi la lista delle richieste */
    sprintf(szStmt, szSqlRead, szFilter);
    ptrResult = ASRV_SqlSelect(szStmt, &iRows);
    CORE_ReturnValIfFail(ptrResult != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

    for (i = 0; i < iRows; i++) {
        currRow = mysql_fetch_row(ptrResult);
        CORE_ReturnValIfFail(currRow != NULL, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, ASRV_DbLogError(szStmt));

        memset(myValue, 0, sizeof(myValue));
        strcpy(myValue, currRow[0]); 

        iRet = JADA_LstAdd(lstValues, myValue, sizeof(myValue), NULL);
        CORE_ReturnValIfFail(iRet == 0, NULL, JADA_LstDestroy(__FILE__, __LINE__, lstValues);, "errore nella LstAdd");
    }

    mysql_free_result(ptrResult);

    return(lstValues);
}

int ASRV_DbGetManagerDefaultCoda(JADA_MgrId iIdManager, char *szCoda, int iLength)
{
    int         iRet;
    LongString  szStmt;
    char        *szSqlRead = "\
SELECT C.NOME FROM CODA C, MANAGER_CODA MC, MANAGER M \
WHERE C.ID = MC.ID_CODA AND M.ID = MC.ID_MANAGER AND M.ID = %d \
AND MC.DEFAULT <> 0";

    sprintf(szStmt, szSqlRead, iIdManager);
    iRet = ASRV_SqlGetString(szStmt, szCoda, iLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}