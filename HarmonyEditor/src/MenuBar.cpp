#include "MenuBar.h"

#include <Core/Input.h>
#include <Scene/SceneSerialization.h>

#include "HarmonyEditor.h"
#include "EditorScene.h"
#include "Settings.h"
#include "Project.h"

using namespace HarmonyEditor;

static std::string GetCommandString(const std::string& identifiers) {
#ifdef HARMONY_PLATFORM_MACOS
    static const std::string metaIdentifier = "Command+";
#else
    static const std::string metaIdentifier = "Ctrl+";
#endif

    return std::string(metaIdentifier) + identifiers;
}

static void HandleKeyboardShortcuts() {
    if(Input::IsCommandDown(HARMONY_KEY_1)) 
        Settings::ShowViewportPanel.CurrentValue = !Settings::ShowViewportPanel.CurrentValue;
        
    if(Input::IsCommandDown(HARMONY_KEY_2)) 
        Settings::ShowSettingsPanel.CurrentValue = !Settings::ShowSettingsPanel.CurrentValue;

    if(Input::IsCommandDown(HARMONY_KEY_3)) 
        Settings::ShowAssetsPanel.CurrentValue = !Settings::ShowAssetsPanel.CurrentValue;

    if(Input::IsCommandDown(HARMONY_KEY_LEFT_SHIFT, HARMONY_KEY_LEFT_BRACKET))
        Settings::ShowHierarchyPanel.CurrentValue = !Settings::ShowHierarchyPanel.CurrentValue; 
        
    if(Input::IsCommandDown(HARMONY_KEY_LEFT_SHIFT, HARMONY_KEY_RIGHT_BRACKET))
        Settings::ShowComponentsPanel.CurrentValue = !Settings::ShowComponentsPanel.CurrentValue;
}

void MenuBar::OnImGuiRender() {

    HARMONY_PROFILE_FUNCTION();

    HandleKeyboardShortcuts();

    if(ImGui::BeginMainMenuBar()) {

        static const float menuBarHeight = ImGui::GetWindowHeight();
        // const float menuBarWidth = ImGui::GetWindowWidth();

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, (ImGui::GetWindowWidth() / 2.0f) - menuBarHeight - 8.0f);

        if(ImGui::BeginMenu("File")) {

            if(ImGui::MenuItem("New Project")) {
                HarmonyEditorApplication::OpenFolderDialog([&](const std::filesystem::path& path) {
                    m_EditorLayerPtr->SetActiveProject({ path });
                });
            }

            ImGui::Separator();

            if(ImGui::MenuItem("Open Project")) {
                HarmonyEditorApplication::OpenFolderDialog([&](const std::filesystem::path& path) {
                    if(!Project::IsProject(path)) {
                        Log::Warn("Warning not a project; Status: returning!"); // TODO: Display with ImGui or some kind-of popup
                        return;
                    }
                    m_EditorLayerPtr->SetActiveProject({ path });
                });
            }

            if(ImGui::MenuItem("Open Scene")) {
                HarmonyEditorApplication::OpenFileDialog({ "Harmony Scene File", "hyscene" }, [&](const std::filesystem::path& path) {
                    m_EditorLayerPtr->OpenScene(path);
                });
            }

            ImGui::Separator();

            if(ImGui::MenuItem("Save Scene")) {
                m_EditorLayerPtr->SaveScene();
            }

            if(ImGui::MenuItem("Save Scene As")) {
                HarmonyEditorApplication::SaveFileDialog({ "Harmony Scene File", "hyscene" }, [&](const std::filesystem::path& path) {
                    FileUtils::CreateFile(path);

                    SceneManager::GetActiveScenePtr()->SetSceneName(path.stem());

                    SceneSerializer serializer = SceneSerializer(SceneManager::GetActiveScenePtr(), path);
                    serializer.SerializeYAML(m_EditorLayerPtr->GetActiveProject().GetProjectDirectory());

                    m_EditorLayerPtr->OpenScene(path);
                });
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Edit")) {
            if(ImGui::MenuItem("Open Settings"))
                Settings::ShowSettingsPanel.CurrentValue = true; 

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View")) {
            if(ImGui::MenuItem("Reset Camera View", "0"))
                m_EditorLayerPtr->GetEditorCamera().ResetView();

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Window")) {

            if(ImGui::BeginMenu("Editor Panels")) {

                static const std::string gameViewportCmd = GetCommandString("1");
                ImGui::MenuItem("Game Viewport", gameViewportCmd.c_str(), &Settings::ShowViewportPanel.CurrentValue);

                static const std::string settingsCmd = GetCommandString("2");
                ImGui::MenuItem("Settings", settingsCmd.c_str(), &Settings::ShowSettingsPanel.CurrentValue);

                static const std::string assetsCmd = GetCommandString("3");
                ImGui::MenuItem("Assets", assetsCmd.c_str(), &Settings::ShowAssetsPanel.CurrentValue);

                static const std::string hierarchyCmd = GetCommandString("Shift+[");
                ImGui::MenuItem("Hierarchy", hierarchyCmd.c_str(), &Settings::ShowHierarchyPanel.CurrentValue);
                
                static const std::string componentsCmd = GetCommandString("Shift+]");
                ImGui::MenuItem("Components", componentsCmd.c_str(), &Settings::ShowComponentsPanel.CurrentValue);

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Debug Panels")) {
                ImGui::MenuItem("Renderer Stats", NULL, &Settings::ShowRendererStats.CurrentValue);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::NextColumn();


        // const ImGuizmo::OPERATION& currentOperation = m_EditorScenePtr->GetCurrentOperation();

        // Play and Stop Button Controls

        ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

        if(SceneRuntime::IsRunning())
            ImGui::PushStyleColor(ImGuiStyleVar_Alpha, { 1.0f, 1.0f, 1.0f, 0.25f });

        if(ImGui::ButtonEx("\uf04b", { menuBarHeight, menuBarHeight }, SceneRuntime::IsRunning() ? ImGuiItemFlags_Disabled : 0)) { // Play Button
            m_EditorLayerPtr->StartRuntime();
            if(SceneRuntime::IsRunning())
                ImGui::PushStyleColor(ImGuiStyleVar_Alpha, { 1.0f, 1.0f, 1.0f, 0.25f });
        }

        if(!SceneRuntime::IsRunning())
            ImGui::PushStyleColor(ImGuiStyleVar_Alpha, { 1.0f, 1.0f, 1.0f, 0.25f });
        else
            ImGui::PopStyleColor();

        if(ImGui::ButtonEx("\uf04d", { menuBarHeight, menuBarHeight }, SceneRuntime::IsRunning() ? 0 : ImGuiItemFlags_Disabled)) { // Stop Button
            m_EditorLayerPtr->StopRuntime();
            if(!SceneRuntime::IsRunning())
                ImGui::PushStyleColor(ImGuiStyleVar_Alpha, { 1.0f, 1.0f, 1.0f, 0.25f });
        }

        ImGui::PopStyleColor(SceneRuntime::IsRunning() ? 1 : 2);

        // if(currentOperation == ImGuizmo::OPERATION::TRANSLATE)
        //     ImGui::PopStyleColor();
        // if(ImGui::Button("T", { menuBarHeight, menuBarHeight }))
        //     m_EditorScenePtr->SetCurrentOperation(ImGuizmo::OPERATION::TRANSLATE);
        // else if(currentOperation == ImGuizmo::OPERATION::TRANSLATE)
        //     ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

        // if(currentOperation == ImGuizmo::OPERATION::ROTATE)
        //     ImGui::PopStyleColor();
        // if(ImGui::Button("R", { menuBarHeight, menuBarHeight }))
        //     m_EditorScenePtr->SetCurrentOperation(ImGuizmo::OPERATION::ROTATE);
        // else if(currentOperation == ImGuizmo::OPERATION::ROTATE)
        //     ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

        // if(currentOperation == ImGuizmo::OPERATION::SCALE)
        //     ImGui::PopStyleColor();
        // if(ImGui::Button("S", { menuBarHeight, menuBarHeight }))
        //     m_EditorScenePtr->SetCurrentOperation(ImGuizmo::OPERATION::SCALE);
        // else if(currentOperation == ImGuizmo::OPERATION::SCALE)
        //     ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

        ImGui::Columns(1);

        ImGui::EndMainMenuBar();
    }

}