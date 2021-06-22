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

    AssetManager::QueueTexture("assets/textures/stallTexture.png");
    AssetManager::QueueTexture("assets/textures/grass.jpeg");

    auto stallMesh = AssetManager::QueueMesh("assets/objects/stall.obj");

    Entity stall = CreateEntity("Stall"); 
    stall.AddComponent<MeshRendererComponent>(stallMesh);

    CreateEntity("Quad", Transform({-8.0f, 0.0f, 0.0f})).AddComponent<QuadRendererComponent>();

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