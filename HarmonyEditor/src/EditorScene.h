#pragma once

#include <harmonypch.h>

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

    private:
        Scene m_SelectedScene = Scene{"Untitled Scene"};

    };
}