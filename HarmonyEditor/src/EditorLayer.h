#pragma once

#include <harmonypch.h>

#include <ImGuizmo.h>

#include <Scene/Scene.h>

#include "Project.h"
#include "EditorCamera.h"

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorScene;

    class EditorLayer {

    public:
        EditorLayer() = default;
        EditorLayer(EditorScene* editorScenePtr) : m_EditorScenePtr(editorScenePtr) {}

        void OnCreate();
        void OnUpdate(float deltaTime);
        void OnDestroy();  

        void DrawGameViewport();

        void OpenScene(const std::filesystem::path& filepath);
        void SaveScene();

        void StartRuntime();
        void StopRuntime();

        const ImGuizmo::OPERATION& GetCurrentOperation();
        void SetCurrentOperation(const ImGuizmo::OPERATION& operation);

        const Project& GetActiveProject() const { return m_ActiveProject; }
        Project& GetActiveProject() { return m_ActiveProject; }

        void SetActiveProject(const Project& project);

        EditorCamera& GetEditorCamera();

    private:
        Project m_ActiveProject = {};
        EditorScene* m_EditorScenePtr = nullptr;

    };

};