#include "EditorScene.h"

#include <Render/Renderer2D.h>

#include <Scene/Entity.h>
#include <Scene/Component.h>

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"
#include "../Scenes/EditorCamera.h"

static Entity s_Entity;

void EditorScene::OnCreate() {

    HARMONY_PROFILE_FUNCTION();

    this->m_SceneName = "Editor Scene";
    
    m_Camera = EditorCamera();

    RenderLayer::OnCreate(this);
    ImGuiLayer::OnCreate(this);

    Entity entity1 = CreateEntity("Entity 1");
    Entity entity2 = CreateEntity("Entity 2", Transform({4, 0, 0}));

    auto cubeMesh = AssetManager::QueueMesh("assets/objects/cube.obj");
    auto stallMesh = AssetManager::QueueMesh("assets/objects/stall.obj");

    entity1.AddComponent<MeshRendererComponent>(cubeMesh);
    entity2.AddComponent<MeshRendererComponent>(stallMesh);

    AssetManager::CreateAll();
}

void EditorScene::OnUpdate(float deltaTime) {

    HARMONY_PROFILE_FUNCTION();

    if(ImGuiLayer::GetIsEditorSelected())
        m_Camera.OnUpdate(deltaTime);

    RenderLayer::OnUpdate();
    ImGuiLayer::OnUpdate();
}

void EditorScene::OnDestroy() {

    HARMONY_PROFILE_FUNCTION();

    RenderLayer::OnDestroy();
    ImGuiLayer::OnDestroy();
    
    AssetManager::DestroyAll();

    m_Registry.clear(); // Last!
}