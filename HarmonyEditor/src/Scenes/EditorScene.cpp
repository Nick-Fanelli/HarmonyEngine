#include "EditorScene.h"

#include <Render/Renderer2D.h>
#include <Render/ObjectLoader.h>

#include <Scene/Entity.h>
#include <Scene/Component.h>
#include <Scene/SceneSerialization.h>

#include <Core/Input.h>

#include "../Settings.h"
#include "../Project.h"

#include "../Layers/RenderLayer.h"
#include "../Layers/ImGuiLayer.h"
#include "../Scenes/EditorCamera.h"

static Entity s_Entity;
static Settings s_Settings;

void EditorScene::OnCreate() {

    HARMONY_PROFILE_FUNCTION();

    this->m_SceneName = "Editor Scene";

    ProjectManager::Initialize(this);
    
    m_Camera = EditorCamera();

    s_Settings = Settings();
    s_Settings.LoadSettings();
    s_Settings.LoadCacheData();

    // FBXLoader::LoadFBXFile("");

    AssetManager::CreateAll();

    RenderLayer::OnCreate(this);
    ImGuiLayer::OnCreate(this, &s_Settings);
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

    s_Settings.SaveSettings();
    s_Settings.SaveCacheData();

    if(m_CurrentSceneFile != "no-path") {
        SceneSerializer serializer{this, m_CurrentSceneFile};
        serializer.SerializeYAML();
    }

    RenderLayer::OnDestroy();
    ImGuiLayer::OnDestroy();
    
    AssetManager::DestroyAll();

    m_Registry.clear(); // Last!
}