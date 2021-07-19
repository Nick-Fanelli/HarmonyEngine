#include "RenderLayer.h"

#include "../Render/MasterRenderer.h"
#include "../Render/Renderer.h"
#include "../Render/Renderer2D.h"

#include "../Scene/Component.h"

using namespace HarmonyEngine;

void RenderLayer::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    MasterRenderer::OnCreate(m_CameraPtr);
    MasterRenderer::SetUseFramebuffer(true);

    m_RenderTexture = MasterRenderer::GetFramebuffer()->GetTexturePtr();
}

template<typename ComponentType, typename RenderFunction>
static void RenderComponent(Scene* scene, RenderFunction renderFunction) {
    auto rendererGroup = scene->GetRegistry().group<ComponentType>(entt::get<TransformComponent>);
    for(auto& entity : rendererGroup) {
        auto[renderer, transform] = rendererGroup.template get<ComponentType, TransformComponent>(entity);
        renderFunction(renderer, transform.Transform);
    }
}

void RenderLayer::Render() {
    HARMONY_PROFILE_FUNCTION();

    MasterRenderer::Begin(); // Begin the master renderer

    Renderer2D::StartBatch(); // 2D Rendering Begin

    // Quad Renderer
    RenderComponent<QuadRendererComponent>(m_ScenePtr, [&](QuadRendererComponent& renderer, Transform& transform) {
        if(renderer.TextureHandle.IsAssigned())
            Renderer2D::DrawQuad(transform, renderer.Color, renderer.TextureHandle);
        else
            Renderer2D::DrawQuad(transform, renderer.Color);
    });

    // Sprite Renderer
    RenderComponent<SpriteRendererComponent>(m_ScenePtr, [&](SpriteRendererComponent& renderer, Transform& transform) {
        if(renderer.TextureHandle.IsAssigned())
            Renderer2D::DrawQuad(transform, renderer.Color, renderer.TextureHandle, renderer.TopLeftCoord, renderer.BottomRightCoord);
        else
            Renderer2D::DrawQuad(transform, renderer.Color);
    });

    Renderer2D::EndBatch(); // 2D Rendering End

    Renderer::StartBatch(); // 3D Rendering Start

    RenderComponent<MeshRendererComponent>(m_ScenePtr, [&](MeshRendererComponent& renderer, Transform& transform) {
        if(!renderer.MeshHandle.IsAssigned())
            return;
        if(renderer.TextureHandle.IsAssigned())
            Renderer::DrawMesh(transform, renderer.MeshHandle, renderer.TextureHandle, renderer.Color);
        else
            Renderer::DrawMesh(transform, renderer.MeshHandle, renderer.Color);
    });

    Renderer::EndBatch(); // 3D Rendering End

    MasterRenderer::End(); // End the master renderer
}

void RenderLayer::OnDestroy() {
    MasterRenderer::OnDestroy();
}