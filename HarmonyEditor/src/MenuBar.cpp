#include "MenuBar.h"

#include <imguipch.h>

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

            if(ImGui::MenuItem("Save Scene")) {
                m_EditorScenePtr->SaveScene();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}