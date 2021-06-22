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

    // TODO: to be removed
    void ShowGameViewport();

};