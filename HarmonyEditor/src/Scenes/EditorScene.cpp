#include "EditorScene.h"

#include <Render/Renderer2D.h>

#include <Scene/Entity.h>
#include <Scene/Component.h>

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"
#include "../Scenes/EditorCamera.h"

static Entity s_Entity;

void EditorScene::OnCreate() {

    this->m_SceneName = "Editor Scene";
    
    m_Camera = EditorCamera();

    RenderLayer::OnCreate(this);
    ImGuiLayer::OnCreate(this);

    auto mesh = AssetManager::QueueMesh("assets/objects/stall.obj");

    s_Entity = Entity(this);
    s_Entity.AddComponent<EntityTag>("Stall");
    s_Entity.AddComponent<Transform>();
    s_Entity.AddComponent<MeshRenderer>(mesh);

    AssetManager::CreateAll();
}

void EditorScene::OnUpdate(float deltaTime) {
    m_Camera.OnUpdate(deltaTime);

    RenderLayer::OnUpdate();
    ImGuiLayer::OnUpdate();
}

void EditorScene::OnDestroy() {

    RenderLayer::OnDestroy();
    ImGuiLayer::OnDestroy();
    
    AssetManager::DestroyAll();

    m_Registry.clear(); // Last!
}