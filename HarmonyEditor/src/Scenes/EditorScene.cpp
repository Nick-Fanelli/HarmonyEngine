#include "EditorScene.h"

#include <Render/Renderer2D.h>
#include <Render/Camera.h>

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"

static PerspectiveCamera s_Camera;

void EditorScene::OnCreate() {
    
    s_Camera = PerspectiveCamera();

    RenderLayer::OnCreate(&s_Camera);
    ImGuiLayer::OnCreate();

}

void EditorScene::OnUpdate(float deltaTime) {

    ImGuiLayer::OnUpdate();
    RenderLayer::OnUpdate(deltaTime);

}

void EditorScene::OnDestroy() {

    RenderLayer::OnDestroy();
    ImGuiLayer::OnDestroy();

    m_Registry.clear(); // Last!
}