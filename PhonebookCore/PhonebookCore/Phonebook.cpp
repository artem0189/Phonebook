// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "Phonebook.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#ifdef __cplusplus   
extern "C" {
#endif

    PHONEBOOKCORE_API void __cdecl GetPhonebook()
    {

    }

    PHONEBOOKCORE_API void __cdecl Search(PhonebookRecord searchParam)
    {

    }

#ifdef __cplusplus
}
#endif
