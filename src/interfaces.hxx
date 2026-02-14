#include <string_view>
#include "sdk.hxx"

namespace interfaces {
    inline IEntityList* EntityList = nullptr;
    inline IPlayerInfoManager* PlayerInfoManger = nullptr;

    inline CGlobalVarsBase* gpGlobals = nullptr;

    uintptr_t getInterface(std::string_view dll, std::string_view interfaceName);

    template <class T>
    inline T* getInterface(std::string_view dll, std::string_view interfaceName) { return reinterpret_cast<T*>(getInterface(dll, interfaceName)); }

    void init();
};