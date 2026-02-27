#include "hooks.hxx"
#include "interfaces.hxx"

#include <iostream>
#include <print>

void __fastcall hooks::applyMouseHk(CInput* _this, void* edx, Vector* viewAngles, CUserCmd* cmd, float mouseX, float mouseY) {
    C_CSPlayer* localPlayer = C_CSPlayer::GetLocalCSPlayer();

    if (localPlayer == nullptr || hooks::keySpeed == 0.f) {
        std::cout << "happened" << std::endl;
        applyMouse.thiscall(_this, viewAngles, cmd, mouseX, mouseY);
        return;
    }

    std::int32_t buttons = _this->GetButtonBits(0);
    std::int32_t flags = localPlayer->m_fFlags();

    const bool aligned = ((cmd->sidemove < 0.f && mouseX < 0.f) || (cmd->sidemove > 0.f && mouseX > 0.f));

    if (!(buttons & IN_JUMP) || !aligned)
        return applyMouse.thiscall(_this, viewAngles, cmd, mouseX, mouseY);

    // magic with the help of dex & toast
    // fuck optibricker
    static Vector3<float> newVelocity {};
    static Vector3<float> lastOrigin{};

    if (cmd->command_number && cmd->tick_count) {
        auto oldSim = localPlayer->m_flOldSimulationTime();
        auto newSim = localPlayer->m_flSimulationTime();

        auto timeDelta = newSim - oldSim;

        if (timeDelta > 1e-6f)
            newVelocity = (localPlayer->m_vecNetworkOrigin() - lastOrigin) / interfaces::gpGlobals->interval_per_tick;
    }

    float twoDVel = newVelocity.twoDVel();

    spdlog::info("{}", twoDVel);

    static float degTurned{};

    const float power = 1.f; // 100% power
    const float mYaw = 0.022f;
    const float sensitivity = 2.8f; // you wouldn't hardcode either of these in a real cheat, but this was just a poc
    // see https://github.com/lilac1337/gensokyo/blob/master/src/gensokyo/cheats/movement.cxx#L16 for how to do this stuff properly
    
    const float targetDelta = math::rad2deg(std::asin(30.f / twoDVel));
    const float smoothedDelta = (cmd->tick_count || cmd->command_number) ? targetDelta - std::abs(degTurned) : targetDelta * (interfaces::gpGlobals->absoluteframetime / 0.01f); //hooks::keySpeed
    //const float smoothedDelta = targetDelta * (interfaces::gpGlobals->absoluteframetime / interfaces::gpGlobals->interval_per_tick);
    const float targetMx = std::copysignf(smoothedDelta / mYaw, mouseX);
    const float adjustedMx = ((1.f - power) * mouseX) + (power * targetMx);
    const float smoothedMx = round(targetMx / sensitivity) * sensitivity;

    //spdlog::info("{}", interfaces::gpGlobals->absoluteframetime);

    if (std::isfinite(targetMx))
        mouseX = smoothedMx;

    degTurned = (cmd->tick_count || cmd->command_number) ? 0.f : degTurned + (mouseX * 0.022f);
    if (cmd->command_number && cmd->tick_count)
        lastOrigin = localPlayer->m_vecNetworkOrigin();

    applyMouse.thiscall(_this, viewAngles, cmd, mouseX, mouseY);
}

void __fastcall hooks::controllerMoveHk(CInput* _this, void* edx,  float frameTime, CUserCmd *cmd) {
    hooks::keySpeed = frameTime;

     controllerMove.thiscall(_this, frameTime, cmd);
}

void hooks::init() {
    const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
    applyMouse = safetyhook::create_inline(reinterpret_cast<uintptr_t*>(client + 0x149830), applyMouseHk);
    controllerMove = safetyhook::create_inline(reinterpret_cast<uintptr_t*>(client + 0x147120), controllerMoveHk);
    // EndScene = safetyhook::create_inline(reinterpret_cast<uintptr_t*>(util::vFunc(gui::renderer::device, 42)), EndScene_hk);

    /*determineKeySpeed = safetyhook::create_mid(client + 0x146A6C, [](safetyhook::Context& ctx) {
        keySpeed = *reinterpret_cast<float*>(ctx.ebp + 0x8);
        //eySpeed = *reinterpret_cast<float*>(ctx.eax);
    });*/

    spdlog::info("all functions hooked");
}

void hooks::unhook() {
    applyMouse.reset();
    controllerMove.reset();
    determineKeySpeed.reset();

}
