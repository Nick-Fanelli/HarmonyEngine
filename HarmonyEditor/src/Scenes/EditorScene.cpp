#include "EditorScene.h"

#include <Render/Renderer2D.h>

#include <Scene/Entity.h>
#include <Scene/Component.h>
#include <Scene/SceneSerialization.h>

#include <Core/Input.h>

#include "../Settings.h"

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"
#include "../Scenes/EditorCamera.h"

static Entity s_Entity;
static SceneSerializer s_SceneSerializer;
static Settings s_Settings;

void EditorScene::OnCreate() {

    HARMONY_PROFILE_FUNCTION();

    this->m_SceneName = "Editor Scene";
    
    m_Camera = EditorCamera();

    s_SceneSerializer = SceneSerializer(this, "scene.yaml");
    s_SceneSerializer.DeserializeYAML();

    s_Settings = Settings();
    s_Settings.LoadSettings();

    AssetManager::CreateAll();

    RenderLayer::OnCreate(this);
    ImGuiLayer::OnCreate(this, &s_Settings);

    // AssetManager::QueueTexture("assets/textures/stallTexture.png");
    // AssetManager::QueueTexture("assets/textures/grass.jpeg");

    // AssetManager::QueueMesh("assets/objects/stall.obj");

    // Entity stall = CreateEntity("Stall"); 
    // stall.AddComponent<MeshRendererComponent>(stallMesh);

    // CreateEntity("Quad", Transform({-8.0f, 0.0f, 0.0f})).AddComponent<QuadRendererComponent>();
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

    s_SceneSerializer.SerializeYAML();
    s_Settings.SaveSettings();

    RenderLayer::OnDestroy();
    ImGuiLayer::OnDestroy();
    
    AssetManager::DestroyAll();

    m_Registry.clear(); // Last!
}