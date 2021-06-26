#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

#include "../Scenes/EditorScene.h"

using namespace HarmonyEngine;

namespace ImGuiLayer {

    static bool s_IsEditorSelected = false;
    static Entity s_SelectedEntity;

    static ImFont* s_FontAwesomePtr = nullptr;

    void OnCreate(EditorScene* editorScenePtr);
    void OnUpdate();
    void OnDestroy();

    bool GetIsEditorSelected();

    Entity& GetSelectedEntity();
    void SetSelectedEntity(Entity& entity);

    ImFont* GetFontAwesome();

    void DrawVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    void DrawColorControl(const std::string& label, glm::vec4& values, float resetValue = 255.0f);

    // TODO: to be removed
    void ShowGameViewport();

};