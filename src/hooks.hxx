#pragma once

#include <safetyhook.hpp>
#include <spdlog/spdlog.h>
#include "sdk.hxx"

namespace hooks {
	inline safetyhook::InlineHook applyMouse;
    inline safetyhook::InlineHook controllerMove;
    inline safetyhook::MidHook determineKeySpeed;

    inline float keySpeed{};

	void __fastcall applyMouseHk(CInput* _this, void* edx, Vector* viewAngles, CUserCmd* cmd, float mouseX, float mouseY);
    void __fastcall controllerMoveHk(CInput* _this, void* edx,  float frameTime, CUserCmd *cmd);

	void init();
	void unhook();
};
