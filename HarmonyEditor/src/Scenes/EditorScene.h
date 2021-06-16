#pragma once

#include <harmonypch.h>

#include <Core/Assets.h>

#include <Scene/Scene.h>

#include <Render/Camera.h>

#include "EditorCamera.h"

using namespace HarmonyEngine;

class EditorScene : public Scene {

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;
    void OnDestroy() override; 

private:
    EditorCamera m_Camera;
    AssetHandle<Texture> m_Texture;

public:
    Camera* GetGenericCameraPtr() { return &m_Camera; }
    AssetHandle<Texture>& GetTexture() { return m_Texture; }

};