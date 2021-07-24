#include "MenuBar.h"

#include <imguipch.h>

#include <Core/Input.h>
#include <Scene/SceneSerialization.h>

#include "Application.h"
#include "EditorScene.h"
#include "Settings.h"

using namespace HarmonyEditor;

static std::string GetCommandString(const std::string& identifiers) {
#ifdef HARMONY_PLATFORM_MACOS
    static const std::string metaIdentifer = "Command+";
#else
    static const std::string metaIdentifer = "Ctrl+";
#endif

    return std::string(metaIdentifer) + identifiers;
}

static void HandleKeyboardShortcuts() {
    if(Input::IsCommandDown(HARMONY_KEY_1)) 
        Settings::ShowViewportPanel.CurrentValue = !Settings::ShowViewportPanel.CurrentValue;
        
    if(Input::IsCommandDown(HARMONY_KEY_2)) 
        Settings::ShowSettingsPanel.CurrentValue = !Settings::ShowSettingsPanel.CurrentValue;

    if(Input::IsCommandDown(HARMONY_KEY_LEFT_SHIFT, HARMONY_KEY_LEFT_BRACKET))
        Settings::ShowHierarchyPanel.CurrentValue = !Settings::ShowHierarchyPanel.CurrentValue; 
        
    if(Input::IsCommandDown(HARMONY_KEY_LEFT_SHIFT, HARMONY_KEY_RIGHT_BRACKET))
        Settings::ShowComponentsPanel.CurrentValue = !Settings::ShowComponentsPanel.CurrentValue;
}

void MenuBar::OnImGuiRender() {

    HandleKeyboardShortcuts();

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

        if(ImGui::BeginMenu("Edit")) {
            if(ImGui::MenuItem("Open Settings"))
                Settings::ShowSettingsPanel.CurrentValue = true; 

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Window")) {

            if(ImGui::BeginMenu("Editor Panels")) {

                static const std::string gameViewportCmd = GetCommandString("1");
                ImGui::MenuItem("Game Viewport", gameViewportCmd.c_str(), &Settings::ShowViewportPanel.CurrentValue);

                static const std::string settingsCmd = GetCommandString("2");
                ImGui::MenuItem("Settings", settingsCmd.c_str(), &Settings::ShowSettingsPanel.CurrentValue);

                static const std::string hierarchyCmd = GetCommandString("Shift+[");
                ImGui::MenuItem("Hierarchy", hierarchyCmd.c_str(), &Settings::ShowHierarchyPanel.CurrentValue);
                
                static const std::string componentsCmd = GetCommandString("Shift+]");
                ImGui::MenuItem("Components", componentsCmd.c_str(), &Settings::ShowComponentsPanel.CurrentValue);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}