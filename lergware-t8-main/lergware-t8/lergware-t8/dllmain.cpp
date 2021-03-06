#include "includes.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        MH_Initialize();
        PresentHook::initalize(D3D11);

        Hook::initalize();
    }
    return TRUE;
}