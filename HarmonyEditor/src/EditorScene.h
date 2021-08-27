#pragma once

#include <harmonypch.h>

#include <Scene/Scene.h>

#include <Layers/RenderLayer.h>

#include "EditorLayer.h"

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorScene : public Scene {

    public:
        void OnCreate() override;
        void OnUpdate(float deltaTime) override;
        void OnDestroy() override;  

        const RenderLayer& GetRenderLayer() const { return m_RenderLayer; }
        RenderLayer& GetRenderLayer() { return m_RenderLayer; }
 
    private:
        EditorLayer m_EditorLayer;
        RenderLayer m_RenderLayer;

    };
}