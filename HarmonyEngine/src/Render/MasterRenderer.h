#pragma once

#include "harmonypch.h"

#include "Framebuffer.h"
#include "Camera.h"

namespace HarmonyEngine::MasterRenderer {

    void SetClearColor(const glm::vec4& color);

    void OnCreate(Camera* cameraPtr);
    void OnDestroy();

    void SetUseFramebuffer(bool value);
    bool GetUseFramebuffer();

    Framebuffer* GetFramebuffer();

    void Begin();
    void End();

};