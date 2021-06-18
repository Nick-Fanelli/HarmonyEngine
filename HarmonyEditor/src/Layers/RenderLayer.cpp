#include "RenderLayer.h"

#include <Render/Renderer2D.h>
#include <Render/Renderer.h>
#include <Render/Framebuffer.h>

#include <Scene/Component.h>

static EditorScene* s_EditorScenePtr;
static Framebuffer s_Framebuffer;

void RenderLayer::OnCreate(EditorScene* editorScene) {
    s_EditorScenePtr = editorScene;
    s_Framebuffer.OnCreate();

    s_EditorScenePtr->SetRenderTexture(s_Framebuffer.GetTexturePtr());

    Renderer2D::OnCreate(s_EditorScenePtr->GetGenericCameraPtr());
    Renderer::OnCreate(s_EditorScenePtr->GetGenericCameraPtr(), &s_Framebuffer);
}

void RenderLayer::OnUpdate() {
    // Renderer2D::StartBatch();
    // Renderer2D::EndBatch();

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
}

void RenderLayer::OnDestroy() {
    Renderer2D::OnDestroy();
    Renderer::OnDestroy();
}