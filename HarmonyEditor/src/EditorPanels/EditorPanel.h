#pragma once

#include <harmonypch.h>

#include "../Project.h"
#include "../Scenes/EditorScene.h"

using namespace HarmonyEngine;

class EditorPanel {

protected:
    EditorScene* m_ScenePtr;

public:
    virtual void OnCreate(EditorScene* scene) { m_ScenePtr = scene; }
    virtual void OnUpdate() = 0;

};