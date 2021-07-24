#pragma once

#include <harmonypch.h>

#include <ImGuizmo.h>

#include <Scene/Scene.h>

namespace HarmonyEditor {

    using namespace HarmonyEngine;

    class EditorScene : public Scene {

    public:
        void OnCreate() override;
        void OnUpdate(float deltaTime) override;
        void OnDestroy() override;  

        void OpenScene(const std::filesystem::path& filepath);
        void SaveScene();

        const Scene& GetSelectedScene() const { return m_SelectedScene; }
        Scene& GetSelectedScene() { return m_SelectedScene; }

        const ImGuizmo::OPERATION& GetCurrentOperation();
        void SetCurrentOperation(const ImGuizmo::OPERATION& operation);

    private:
        Scene m_SelectedScene = Scene{"Untitled Scene"};

    };
}