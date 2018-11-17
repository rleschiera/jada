#include "jada.h"

FixString   szInFile = "";
FixString   szOutFile = "";
FixString   szNewPatchUrl = "";
FixString   szOldPatchUrl = "http://AAA.BBB.CCC.DDD/jada/aggiornare_url_con_il_valore_specifico_della_installazione";
FixString   szNewPatchCustomer = "";
FixString   szOldPatchCustomer = "EFGHIJKLMNOPQRSTUVWXYZ0123456789";
int         iPatchLength = 0;

static int ScanInputParameters(int argc, char *argv[])
{
    int         i;

    /*- Verifica i parametri di input e li elabora */
    for (i = 1; i < argc && argv[i][0] == '-'; i++) {
		if (strcmp(&argv[i][1], "in") == 0) {
            strcpy(szInFile, argv[++i]);
		}
		else if (strcmp(&argv[i][1], "out") == 0) {
            strcpy(szOutFile, argv[++i]);
		}
		else if (strcmp(&argv[i][1], "url") == 0) {
            strcpy(szNewPatchUrl, argv[++i]);
		}
		else if (strcmp(&argv[i][1], "customer") == 0) {
            strcpy(szNewPatchCustomer, argv[++i]);
		}
    }

    return(0);
}

int main(int argc, char *argv[])
{
    int         iRet;
    FILE        *fpIn, *fpOut;
    char        *szBuffer;
    int         i, iRead, iWritten, iLength; 

    iRet = JADA_LogStart();
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nell'inizializzazione del log");

    iRet = ScanInputParameters(argc, argv);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nei parametri di ingresso");

    //if (strlen(szInFile) == 0) {
    //    strcpy(szInFile, "C:\\jada\\bin\\winagent.template.exe");
    //}
    //if (strlen(szOutFile) == 0) {
    //    strcpy(szOutFile, "C:\\jada\\bin\\winagent.exe");
    //}
    //if (strlen(szNewPatchUrl) == 0) {
    //    strcpy(szNewPatchUrl, "http://areas-demo.eng.it/jadabum/");
    //}
    //if (strlen(szNewPatchCustomer) == 0) {
    //    strcpy(szNewPatchCustomer, "bumsrl");
    //}

    CORE_ReturnValIfFail(strlen(szInFile) > 0, -1, ;, "File di input non specificato");
    CORE_ReturnValIfFail(strlen(szOutFile) > 0, -1, ;, "File di output non specificato");
    CORE_ReturnValIfFail(strlen(szNewPatchUrl) > 0, -1, ;, "Stringa di patch 'URL' non specificata");
    CORE_ReturnValIfFail(strlen(szNewPatchCustomer) > 0, -1, ;, "Stringa di patch 'Customer' non specificata");

    iPatchLength = strlen(szOldPatchUrl);
    for (i = 0; i < iPatchLength; i++) {
        strcat(szNewPatchUrl, " ");
    }
    szNewPatchUrl[iPatchLength] = '\0';

    iPatchLength = strlen(szOldPatchCustomer);
    for (i = 0; i < iPatchLength; i++) {
        strcat(szNewPatchCustomer, " ");
    }
    szNewPatchCustomer[iPatchLength] = '\0';

    fpIn = fopen(szInFile, "rb");
    CORE_ReturnValIfFail(fpIn != NULL, -1, ;, "Errore nell'apertura del file di input");

    fseek (fpIn, 0, SEEK_END);
    iLength = ftell(fpIn);
    rewind(fpIn);

    szBuffer = calloc(iLength, sizeof(char));
    CORE_ReturnValIfFail(szBuffer != NULL, -1, ;, "Errore nell'allocazione del buffer di lavoro");

    iRead = fread(szBuffer, 1, iLength, fpIn);
    CORE_ReturnValIfFail(iRead == iLength, -1, ;, "Errore nella lettura del file di input nel buffer di lavoro");

    fclose(fpIn);

    for (i = 0; i < iLength; i++) {
        iRet = memcmp(&szBuffer[i], szOldPatchUrl, strlen(szOldPatchUrl));
        if (iRet == 0) {
            memcpy(&szBuffer[i], szNewPatchUrl, strlen(szNewPatchUrl));
        }
        iRet = memcmp(&szBuffer[i], szOldPatchCustomer, strlen(szOldPatchCustomer));
        if (iRet == 0) {
            memcpy(&szBuffer[i], szNewPatchCustomer, strlen(szNewPatchCustomer));
        }
    }

    fpOut = fopen(szOutFile, "w+b");
    CORE_ReturnValIfFail(fpOut != NULL, -1, ;, "Errore nell'apertura del file di output");

    iWritten = fwrite(szBuffer, 1, iLength, fpOut);
    CORE_ReturnValIfFail(iWritten == iLength, -1, ;, "Errore nella scrittura del file di output dal buffer di lavoro");

    fclose(fpOut);

    printf("Patch del programma %s eseguito correttamente.\n", szOutFile);

    return(0);
}
