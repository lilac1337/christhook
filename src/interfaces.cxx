#include <Windows.h>
#include <iostream>
#include <spdlog/spdlog.h>

#include "interfaces.hxx"

uintptr_t interfaces::getInterface(std::string_view dll, std::string_view interfaceName) {
    using createInterfacePrototype = void* (__cdecl* )(const char* name, int* returnCode);

    HINSTANCE__* moduleHandle = GetModuleHandle(dll.data());

    if (moduleHandle == nullptr) {
        spdlog::critical("couldn't find module: {}", dll);
        return 0;
    }

    createInterfacePrototype createInterface = reinterpret_cast<createInterfacePrototype>(GetProcAddress(moduleHandle, "CreateInterface"));

    std::int32_t returnCode{};
    void* foundInterface = createInterface(interfaceName.data(), &returnCode);

    if (foundInterface == nullptr) {
        spdlog::critical("couldn't find interface: {}", interfaceName);
        return 0;
    }

    spdlog::info("{} found at 0x{:X}", interfaceName, reinterpret_cast<uintptr_t>(foundInterface));

    return reinterpret_cast<uintptr_t>(foundInterface);
}

void interfaces::init() {
    EntityList = getInterface<IEntityList>("client.dll", "VClientEntityList003");
    PlayerInfoManger = getInterface<IPlayerInfoManager>("server.dll", "PlayerInfoManager002");

    gpGlobals = PlayerInfoManger->GetGlobalVars();
}