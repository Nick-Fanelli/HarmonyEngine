#include "EditorScene.h"

#include <Render/Renderer2D.h>
#include <Render/Camera.h>

#include "../Layers/RenderLayer.h"

static PerspectiveCamera s_Camera;

void EditorScene::OnCreate() {
    
    s_Camera = PerspectiveCamera();

    RenderLayer::OnCreate(&s_Camera);

}

void EditorScene::OnUpdate(float deltaTime) {
    RenderLayer::OnUpdate(deltaTime);
}

void EditorScene::OnDestroy() {

    RenderLayer::OnDestroy();

    m_Registry.clear(); // Last!
}