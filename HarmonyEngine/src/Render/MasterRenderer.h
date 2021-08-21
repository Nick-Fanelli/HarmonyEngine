#pragma once

#include "harmonypch.h"

#include "Framebuffer.h"
#include "Camera.h"

namespace HarmonyEngine {

    class MasterRenderer {
    public:
        MasterRenderer() = delete;
        ~MasterRenderer() = delete;

        MasterRenderer(const MasterRenderer&) = delete;

        static Camera* CameraPtr;

        static void SetClearColor(const glm::vec4& color);

        static void OnCreate(Camera* cameraPtr);
        static void OnDestroy();

        static void SetUseFramebuffer(bool value);
        static bool GetUseFramebuffer();

        static Framebuffer* GetFramebuffer();

        static void Begin();
        static void End();
    };

}