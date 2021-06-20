#pragma once

#include <harmonypch.h>

#include <Scene/Scene.h>

class EditorPanel {

protected:
    HarmonyEngine::Scene* m_ScenePtr;

public:
    virtual void OnCreate(HarmonyEngine::Scene* scene) { m_ScenePtr = scene; }
    virtual void OnUpdate() = 0;

};