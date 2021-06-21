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
    GLuint* m_RenderTexture = nullptr;

public:
    Camera* GetGenericCameraPtr() { return &m_Camera; }
    const GLuint* GetRenderTexture() const { return m_RenderTexture; }

    const EditorCamera& GetEditorCamera() const { return m_Camera; }
    EditorCamera& GetEditorCamera() { return m_Camera; }

    void SetRenderTexture(GLuint* texture) { m_RenderTexture = texture; }

};