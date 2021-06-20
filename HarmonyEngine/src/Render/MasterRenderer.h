#pragma once

#include "harmonypch.h"

#include "Framebuffer.h"
#include "Camera.h"

namespace HarmonyEngine::MasterRenderer {

    void OnCreate(Camera* cameraPtr);
    void OnDestroy();

    void SetUseFramebuffer(bool value);
    bool GetUseFramebuffer();

    Framebuffer* GetFramebuffer();

    void Begin();
    void End();

};