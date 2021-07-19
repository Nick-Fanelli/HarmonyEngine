#pragma once

#include <harmonypch.h>

#include "../Render/Camera.h"
#include "../Scene/Scene.h"

namespace HarmonyEngine {

    class RenderLayer {

    public:
        RenderLayer() = default;
        RenderLayer(Camera* cameraPtr, Scene* scenePtr) : m_CameraPtr(cameraPtr), m_ScenePtr(scenePtr) {}

        void OnCreate();
        void Render();
        void OnDestroy();

        const GLuint* GetRenderTexture() const { return m_RenderTexture; }

    private:
        Camera* m_CameraPtr;
        Scene* m_ScenePtr;

        GLuint* m_RenderTexture; 

    };

}