#pragma once

#include <harmonypch.h>

#include "../Scenes/EditorScene.h"

using namespace HarmonyEngine;

namespace ImGuiLayer {

    void OnCreate(EditorScene* editorScenePtr);
    void OnUpdate();
    void OnDestroy();

};