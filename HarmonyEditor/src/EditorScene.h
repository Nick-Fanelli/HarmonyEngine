#pragma once

#include <harmonypch.h>

#include <ImGuizmo.h>

#include <Scene/Scene.h>

#include "Project.h"
#include "EditorCamera.h"

namespace HarmonyEditor {

    using namespace HarmonyEngine;

    class EditorScene : public Scene {

    public:
        void OnCreate() override;
        void OnUpdate(float deltaTime) override;
        void OnDestroy() override;  

        void DrawGameViewport();

        void OpenScene(const std::filesystem::path& filepath);
        void SaveScene();

        void StartRuntime();
        void StopRuntime();

        const Scene& GetSelectedScene() const { return m_SelectedScene; }
        Scene& GetSelectedScene() { return m_SelectedScene; }

        const ImGuizmo::OPERATION& GetCurrentOperation();
        void SetCurrentOperation(const ImGuizmo::OPERATION& operation);

        const Project& GetActiveProject() const { return m_ActiveProject; }
        Project& GetActiveProject() { return m_ActiveProject; }

        void SetActiveProject(const Project& project);

        bool IsRunning() const { return m_IsRunning; }

        EditorCamera& GetEditorCamera();

    private:
        Project m_ActiveProject = {};
        Scene m_SelectedScene = Scene{"Untitled Scene"};

        bool m_IsRunning = false;

    };
}