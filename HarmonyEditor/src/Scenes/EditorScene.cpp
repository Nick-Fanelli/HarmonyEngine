#include "EditorScene.h"

#include <Render/Renderer2D.h>
#include <Render/Camera.h>

static PerspectiveCamera s_Camera;

void EditorScene::OnCreate() {
    
    s_Camera = PerspectiveCamera();

    Renderer2D::OnCreate(&s_Camera);

}

void EditorScene::OnUpdate(float deltaTime) {

    Renderer2D::StartBatch();

    Renderer2D::DrawQuad({2, 0, 0}, {1, 1});

    Renderer2D::EndBatch();

}

void EditorScene::OnDestroy() {
    m_Registry.clear();
    Renderer2D::OnDestroy();
}