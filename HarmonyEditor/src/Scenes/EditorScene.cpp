#include "EditorScene.h"

#include <Core/Assets.h>

#include <Render/Renderer2D.h>

#include <Scene/Entity.h>
#include <Scene/Component.h>

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"

static PerspectiveCamera s_Camera;
static Entity s_Entity;

void EditorScene::OnCreate() {
    
    m_Camera = PerspectiveCamera();

    RenderLayer::OnCreate(this);
    ImGuiLayer::OnCreate();

    auto mesh = AssetManager::QueueMesh("assets/objects/stall.obj");

    s_Entity = Entity(this);
    s_Entity.AddComponent<Transform>();
    s_Entity.AddComponent<MeshRenderer>(mesh);

    AssetManager::CreateAll();
}

void EditorScene::OnUpdate(float deltaTime) {
    ImGuiLayer::OnUpdate();
    RenderLayer::OnUpdate();
}

void EditorScene::OnDestroy() {

    RenderLayer::OnDestroy();
    ImGuiLayer::OnDestroy();
    
    AssetManager::DestroyAll();

    m_Registry.clear(); // Last!
}