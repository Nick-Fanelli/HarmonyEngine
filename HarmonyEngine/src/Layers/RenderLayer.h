#pragma once

#include <harmonypch.h>

#include "../Core/Assets.h"
#include "../Render/Camera.h"
#include "../Scene/Scene.h"

namespace HarmonyEngine {

    class RenderLayer {

    public:
        RenderLayer() = default;
        RenderLayer(Camera* cameraPtr, Scene* scenePtr) : m_CameraPtr(cameraPtr), m_ScenePtr(scenePtr) {}

        void OnCreate();
        void Begin();
        void Render();
        void End();
        void OnDestroy();

        const GLuint* GetRenderTexture() const { return m_RenderTexture; }

    private:    
        struct QuadRenderData {

            Transform* Transform;
            glm::vec4* Color;

            AssetHandle<Texture>* TextureHandle;
            
            QuadRenderData(struct Transform& transform, glm::vec4& color, AssetHandle<Texture>& textureHandle) : Transform(&transform), Color(&color), TextureHandle(&textureHandle) {} 
            QuadRenderData(const QuadRenderData&) = default;

        };

        static bool CompareQuadRenderData(const QuadRenderData& lhs, const QuadRenderData& rhs) {
            return lhs.Transform->Position.z < rhs.Transform->Position.z;
        }
    
    private:
        Camera* m_CameraPtr;
        Scene* m_ScenePtr;

        GLuint* m_RenderTexture; 
        std::vector<QuadRenderData> m_QuadRenderData;

    };

}