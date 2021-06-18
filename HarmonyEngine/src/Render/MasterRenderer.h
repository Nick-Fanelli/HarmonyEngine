#pragma once

#include "harmonypch.h"

#include "Framebuffer.h"

namespace HarmonyEngine::MasterRenderer {

    void SetUseFramebuffer(bool value);
    bool GetUseFramebuffer();

    Framebuffer* GetFramebuffer();

    void Begin();
    void End();

};