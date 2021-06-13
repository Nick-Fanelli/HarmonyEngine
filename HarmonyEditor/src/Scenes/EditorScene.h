#pragma once

#include <harmonypch.h>

#include <Scene/Scene.h>

#include <Render/Camera.h>

using namespace HarmonyEngine;

class EditorScene : public Scene {

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;
    void OnDestroy() override; 

private:
    PerspectiveCamera m_Camera;

public:
    Camera* GetGenericCameraPtr() { return &m_Camera; }

};