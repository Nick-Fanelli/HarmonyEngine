#include "RenderLayer.h"

#include <Render/MasterRenderer.h>
#include <Render/Renderer2D.h>
#include <Render/Renderer.h>
#include <Render/Framebuffer.h>

#include <Scene/Component.h>

static EditorScene* s_EditorScenePtr;

void RenderLayer::OnCreate(EditorScene* editorScene) {

    HARMONY_PROFILE_FUNCTION();

    s_EditorScenePtr = editorScene;

    MasterRenderer::OnCreate(s_EditorScenePtr->GetGenericCameraPtr());
    MasterRenderer::SetUseFramebuffer(true);

    s_EditorScenePtr->SetRenderTexture(MasterRenderer::GetFramebuffer()->GetTexturePtr());
}

void RenderLayer::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    MasterRenderer::Begin();

    Renderer2D::StartBatch();

    auto quadRendererGroup = s_EditorScenePtr->GetRegistry().group<QuadRendererComponent>(entt::get<TransformComponent>);
    for(auto& entity : quadRendererGroup) {
        auto[renderer, transform] = quadRendererGroup.get<QuadRendererComponent, TransformComponent>(entity);
        Renderer2D::DrawQuad(transform.Position, renderer.Scale, renderer.Color);
    }

    Renderer2D::EndBatch();

    Renderer::StartBatch();

    // Mesh Renderer
    auto meshRendererGroup = s_EditorScenePtr->GetRegistry().group<MeshRendererComponent>(entt::get<TransformComponent>);
    for(auto& entity : meshRendererGroup) {
        auto[renderer, transform] = meshRendererGroup.get<MeshRendererComponent, TransformComponent>(entity);
        if(renderer.TextureHandle.IsAssigned())
            Renderer::DrawMesh(renderer.MeshHandle, renderer.TextureHandle, transform.Position);
        else
            Renderer::DrawMesh(renderer.MeshHandle, transform.Position);
    }

    Renderer::EndBatch();

    MasterRenderer::End();
}

void RenderLayer::OnDestroy() {
    MasterRenderer::OnDestroy();
}