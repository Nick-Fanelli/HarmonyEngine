#include "EditorScene.h"

using namespace HarmonyEditor;

static Camera s_Camera;

void EditorScene::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    m_SceneName = "Untitled Scene";

    m_RenderLayer = RenderLayer(&s_Camera, this);
    m_RenderLayer.OnCreate();

    m_EditorLayer = EditorLayer(this);
    m_EditorLayer.OnCreate();
}

void EditorScene::OnUpdate(float deltaTime) {
    HARMONY_PROFILE_FUNCTION();

    m_EditorLayer.OnUpdate(deltaTime);

    m_RenderLayer.Begin();
    m_RenderLayer.Render();
    m_RenderLayer.End();
}

void EditorScene::OnDestroy() {
    HARMONY_PROFILE_FUNCTION();

    m_RenderLayer.OnDestroy();
    m_EditorLayer.OnDestroy();
}