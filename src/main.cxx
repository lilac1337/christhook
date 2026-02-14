#include <iostream>

#include "framework.h"
#include "hooks.hxx"
#include "spdlog/spdlog.h"
#include "interfaces.hxx"

void __stdcall mainThread(HMODULE h_module) noexcept {
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    spdlog::info("christhook loaded.");

    hooks::init();
    interfaces::init();

    while (!GetAsyncKeyState(VK_END)) Sleep(100);

    hooks::unhook();

    fclose((_iobuf*)stdout);
    FreeConsole();

    FreeLibraryAndExitThread(h_module, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}