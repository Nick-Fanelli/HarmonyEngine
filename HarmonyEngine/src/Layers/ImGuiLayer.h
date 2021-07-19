#pragma once

#include "harmonypch.h"
#include "imguipch.h"

#include "Scene/Scene.h"

namespace HarmonyEngine {

    class ImGuiLayer {

    public:
        ImGuiLayer() = default;
        ImGuiLayer(Scene* scene) { m_ScenePtr = scene; }

        void OnCreate(const std::string& saveLocation);
        void Begin();
        void End();
        void OnDestroy();

    protected:
        Scene* m_ScenePtr;

    };

}