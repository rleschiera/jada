#include "jada.h"


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
    BOOL    result = TRUE;

    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return (result);
}

int  DllRegisterServer(void)
{
    return(S_OK);
}

int  DllUnregisterServer(void)
{
    return(S_OK);
}
