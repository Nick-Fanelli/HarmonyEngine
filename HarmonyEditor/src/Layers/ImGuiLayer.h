#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

#include "../Scenes/EditorScene.h"

using namespace HarmonyEngine;

namespace ImGuiLayer {

    static bool s_IsEditorSelected = false;
    static Entity s_SelectedEntity;

    void OnCreate(EditorScene* editorScenePtr);
    void OnUpdate();
    void OnDestroy();

    bool GetIsEditorSelected();

    Entity& GetSelectedEntity();
    void SetSelectedEntity(Entity& entity);

    // TODO: to be removed
    void ShowGameViewport();

};