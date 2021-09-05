#include "RenderLayer.h"

#include "../Render/MasterRenderer.h"
#include "../Render/Renderer2D.h"

#include "../Scene/Component.h"

using namespace HarmonyEngine;

void RenderLayer::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    MasterRenderer::OnCreate(m_CameraPtr);

    m_RenderTexture = MasterRenderer::GetFramebuffer()->GetTexturePtr();
}

template<typename ComponentType, typename RenderFunction>
static void RenderComponent(Scene* scene, RenderFunction renderFunction) {

    scene->ForEachEntityWithTransform<ComponentType>([&](TransformComponent& transform, ComponentType& component) {
        renderFunction(component, transform.Transform);
    });

}

void RenderLayer::Begin() {
    HARMONY_PROFILE_FUNCTION();

    MasterRenderer::Begin();
    Renderer2D::StartBatch();
}

void RenderLayer::Render() {
    HARMONY_PROFILE_FUNCTION();

    // Quad Renderer
    RenderComponent<QuadRendererComponent>(m_ScenePtr, [&](QuadRendererComponent& renderer, Transform& transform) {
        m_QuadRenderData.push_back({transform, renderer.Color, renderer.TextureHandle});
    });

    // Sprite Renderer
    RenderComponent<SpriteRendererComponent>(m_ScenePtr, [&](SpriteRendererComponent& renderer, Transform& transform) {
        if(renderer.TextureHandle.IsAssigned())
            Renderer2D::DrawQuad(transform, renderer.Color, renderer.TextureHandle, renderer.TopLeftCoord, renderer.BottomRightCoord);
        else
            Renderer2D::DrawQuad(transform, renderer.Color);
    });

    
    {
        HARMONY_PROFILE_SCOPE("RenderLayer::QuadSortingAlgorithm");
        std::sort(m_QuadRenderData.begin(), m_QuadRenderData.end(), CompareQuadRenderData);
    }

    for(auto& renderData : m_QuadRenderData) {
        if(renderData.TextureHandle->IsAssigned())
            Renderer2D::DrawQuad(*renderData.Transform, *renderData.Color, *renderData.TextureHandle);
        else
            Renderer2D::DrawQuad(*renderData.Transform, *renderData.Color);
    }

    m_QuadRenderData.clear();
}

void RenderLayer::End() {
    HARMONY_PROFILE_FUNCTION();

    Renderer2D::EndBatch();
    MasterRenderer::End();
}

void RenderLayer::OnDestroy() {
    MasterRenderer::OnDestroy();
}