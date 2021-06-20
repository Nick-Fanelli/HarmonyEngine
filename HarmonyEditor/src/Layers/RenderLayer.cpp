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

    auto quadRendererGroup = s_EditorScenePtr->GetRegistry().group<QuadRenderer>(entt::get<Transform>);
    for(auto& entity : quadRendererGroup) {
        auto[renderer, transform] = quadRendererGroup.get<QuadRenderer, Transform>(entity);
        Renderer2D::DrawQuad(transform.Position, renderer.Scale, renderer.Color);
    }

    Renderer2D::EndBatch();

    Renderer::StartBatch();

    // Mesh Renderer
    auto meshRendererGroup = s_EditorScenePtr->GetRegistry().group<MeshRenderer>(entt::get<Transform>);
    for(auto& entity : meshRendererGroup) {
        auto[renderer, transform] = meshRendererGroup.get<MeshRenderer, Transform>(entity);
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