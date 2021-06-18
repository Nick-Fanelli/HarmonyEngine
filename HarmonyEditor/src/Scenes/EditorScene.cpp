#include "EditorScene.h"

#include <Render/Renderer2D.h>

#include <Scene/Entity.h>
#include <Scene/Component.h>

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"
#include "../Scenes/EditorCamera.h"

static Entity s_Entity;
static Entity s_QuadEntity;

void EditorScene::OnCreate() {

    this->m_SceneName = "Editor Scene";
    
    m_Camera = EditorCamera();

    RenderLayer::OnCreate(this);
    ImGuiLayer::OnCreate(this);

    auto mesh = AssetManager::QueueMesh("assets/objects/stall.obj");
    auto texture = AssetManager::QueueTexture("assets/textures/stallTexture.png");

    s_Entity = Entity(this);
    s_Entity.AddComponent<EntityTag>("Stall");
    s_Entity.AddComponent<Transform>();
    s_Entity.AddComponent<MeshRenderer>(mesh, texture);

    s_QuadEntity = Entity(this);
    s_QuadEntity.AddComponent<EntityTag>("Quad");
    s_QuadEntity.AddComponent<Transform>(glm::vec3{5, 10, 0});
    s_QuadEntity.AddComponent<QuadRenderer>(glm::vec2{5, 5}, glm::vec4{1, 0, 0, 1});

    AssetManager::CreateAll();
}

void EditorScene::OnUpdate(float deltaTime) {
    if(ImGuiLayer::GetIsEditorSelected())
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