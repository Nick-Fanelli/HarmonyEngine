#include "RenderLayer.h"

#include <Render/Renderer2D.h>
#include <Render/Renderer.h>

#include <Scene/Component.h>

static EditorScene* s_EditorScenePtr;

void RenderLayer::OnCreate(EditorScene* editorScene) {
    s_EditorScenePtr = editorScene;

    Renderer2D::OnCreate(s_EditorScenePtr->GetGenericCameraPtr());
    Renderer::OnCreate(s_EditorScenePtr->GetGenericCameraPtr());
}

void RenderLayer::OnUpdate() {
    // Renderer2D::StartBatch();
    // Renderer2D::EndBatch();

    Renderer::StartBatch();

    // Mesh Renderer
    auto meshRendererGroup = s_EditorScenePtr->GetRegistry().group<MeshRenderer>(entt::get<Transform>);
    for(auto& entity : meshRendererGroup) {
        auto[renderer, transform] = meshRendererGroup.get<MeshRenderer, Transform>(entity);
        Renderer::DrawMesh(renderer.MeshHandle, transform.Position);
    }

    Renderer::EndBatch();
}

void RenderLayer::OnDestroy() {
    Renderer2D::OnDestroy();
    Renderer::OnDestroy();
}