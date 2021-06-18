#pragma once

#include <harmonypch.h>

#include "../Scenes/EditorScene.h"

using namespace HarmonyEngine;

namespace ImGuiLayer {

    static bool s_IsEditorSelected = false;

    void OnCreate(EditorScene* editorScenePtr);
    void OnUpdate();
    void OnDestroy();

    // TODO: to be removed
    void ShowGameViewport();

    bool GetIsEditorSelected();

};