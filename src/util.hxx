#pragma once

namespace util {
    template <typename Return, typename ... Arguments>
    constexpr Return callVfunc(void* vmt, const std::uint32_t index, Arguments ... args) noexcept {
        using Function = Return(__thiscall*)(void*, decltype(args)...);
        return (*static_cast<Function**>(vmt))[index](vmt, args...);
    }

    constexpr void* vFunc(void* _this, size_t index) noexcept {
        return (*static_cast<void***>(_this))[index];
    }

};