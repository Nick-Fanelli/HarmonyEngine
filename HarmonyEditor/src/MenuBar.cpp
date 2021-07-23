#include "MenuBar.h"

#include <imguipch.h>

#include <Scene/SceneSerialization.h>

#include "Application.h"
#include "EditorScene.h"

using namespace HarmonyEditor;

void MenuBar::OnImGuiRender() {

    if(ImGui::BeginMainMenuBar()) {

        if(ImGui::BeginMenu("File")) {

            if(ImGui::MenuItem("Open Scene")) {
                Application::OpenFileDialog({ "Harmony Scene File", "hyscene" }, [&](const std::filesystem::path& path) {
                    m_EditorScenePtr->OpenScene(path);
                });
            }

            ImGui::Separator();

            if(ImGui::MenuItem("Save Scene")) {
                m_EditorScenePtr->SaveScene();
            }

            if(ImGui::MenuItem("Save Scene As")) {
                Application::SaveFileDialog({ "Harmony Scene File", "hyscene" }, [&](const std::filesystem::path& path) {
                    FileUtils::CreateFile(path);

                    m_EditorScenePtr->GetSelectedScene().SetSceneName(path.stem());

                    SceneSerializer serializer = SceneSerializer(&m_EditorScenePtr->GetSelectedScene(), path);
                    serializer.SerializeYAML();

                    m_EditorScenePtr->OpenScene(path);
                });
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}