#include "jada.h"
#include "jada_internal.h"

#ifndef UNIX
#include <windows.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#endif

static int ProcessIsRunning(DWORD processID, wchar_t *szProcessToCheck)
{
    int     iRet, iFound = -1;
    wchar_t szProcessName[MAX_PATH] = TEXT("<unknown>");
    HMODULE hMod;
    DWORD   cbNeeded;
    HANDLE  hProcess;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL) {
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
            iRet = wcscmp(szProcessName, szProcessToCheck);
            if (iRet == 0) {
                iFound = 0;
            }
        }
        CloseHandle(hProcess);
    }

    return(iFound);
}

static int KillProcess(DWORD processID)
{
    HANDLE  hProcess;

    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess != NULL) {
        TerminateProcess(hProcess, 9);
        CloseHandle(hProcess);
    }

    return(0);
}

int JADA_ProcExists(char *szProcName)
{
#ifndef UNIX
    int             iRet;
    DWORD           aProcesses[1024], cbNeeded, cProcesses;
    wchar_t         vProcName[256];
    unsigned int    i;

    mbstowcs_s(NULL, vProcName, strlen(szProcName) + 1, szProcName, strlen(szProcName));

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return(-1);
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            iRet = ProcessIsRunning(aProcesses[i], vProcName);
            if (iRet == 0) {
                return(0); 
            }
        }
    }

    return(-1);
#else
    return(-1);
#endif
}

int  JADA_ProcKill(char *szProcName)
{
#ifndef UNIX
    int             iRet;
    DWORD           aProcesses[1024], cbNeeded, cProcesses;
    wchar_t         vProcName[256];
    unsigned int    i;

    mbstowcs_s(NULL, vProcName, strlen(szProcName) + 1, szProcName, strlen(szProcName));

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return(-1);
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            iRet = ProcessIsRunning(aProcesses[i], vProcName);
            if (iRet == 0) {
                iRet = KillProcess(aProcesses[i]);
                return(0); 
            }
        }
    }

    return(-1);
#else
    return(-1);
#endif
}

int  JADA_ProcExecute(char *szProcName, char *szProcParams)
{
#ifndef UNIX
    wchar_t             szCmdLine[256];
    wchar_t             szWorkingDir[256];
    STARTUPINFO         si;
    PROCESS_INFORMATION pi;
    wchar_t             vProcName[256];
    wchar_t             vProcParams[256];

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    memset(&pi, 0, sizeof(pi));

    mbstowcs_s(NULL, vProcName, strlen(szProcName) + 1, szProcName, strlen(szProcName));
    mbstowcs_s(NULL, vProcParams, strlen(szProcParams) + 1, szProcParams, strlen(szProcParams));
    swprintf(szCmdLine, sizeof(szCmdLine), L"C:/jada/bin/%s %s", vProcName, vProcParams);
    swprintf(szWorkingDir, sizeof(szWorkingDir), L"C:/jada/bin");

    CreateProcess(NULL, szCmdLine,
                  NULL,               // Process handle not inheritable
                  NULL,               // Thread handle not inheritable
                  FALSE,              // Set handle inheritance to FALSE
                  0,                  // No creation flags
                  NULL,               // Use parent's environment block
                  szWorkingDir,       // Use parent's starting directory 
                  &si,                // Pointer to STARTUPINFO structure
                  &pi);               // Pointer to PROCESS_INFORMATION structure
#else
#endif

    return(0);
}
