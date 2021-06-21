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

    // Quad Renderer
    auto quadRendererGroup = s_EditorScenePtr->GetRegistry().group<QuadRendererComponent>(entt::get<TransformComponent>);
    for(auto& entity : quadRendererGroup) {
        auto[renderer, transform] = quadRendererGroup.get<QuadRendererComponent, TransformComponent>(entity);
        // TODO: Remove/Find better solution
        if(renderer.TextureHandle.IsAssigned())
            Renderer2D::DrawQuad(transform.Transform, renderer.Color, renderer.TextureHandle);
        else
            Renderer2D::DrawQuad(transform.Transform, renderer.Color);
    }

    Renderer2D::EndBatch();

    Renderer::StartBatch();

    // Mesh Renderer
    auto meshRendererGroup = s_EditorScenePtr->GetRegistry().group<MeshRendererComponent>(entt::get<TransformComponent>);
    for(auto& entity : meshRendererGroup) {
        auto[renderer, transform] = meshRendererGroup.get<MeshRendererComponent, TransformComponent>(entity);
        // TODO: Remove/Find better solution
        if(renderer.TextureHandle.IsAssigned())
            Renderer::DrawMesh(transform.Transform, renderer.MeshHandle, renderer.TextureHandle, renderer.Color);
        else
            Renderer::DrawMesh(transform.Transform, renderer.MeshHandle, renderer.Color);
    }

    Renderer::EndBatch();

    MasterRenderer::End();
}

void RenderLayer::OnDestroy() {
    MasterRenderer::OnDestroy();
}