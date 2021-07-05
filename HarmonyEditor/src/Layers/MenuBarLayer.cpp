#include "MenuBarLayer.h"

#include <imguipch.h>

#include "../Project.h"

#include "ImGuiLayer.h"

void MenuBarLayer::OnCreate() {
    // Load Menubar Saved Settings
}

template<typename UIFunction>
static void DrawMenu(const char* title, UIFunction uiFunction) {
    if(ImGui::BeginMenu(title)) {
        uiFunction();
        ImGui::EndMenu();
    }
}

void MenuBarLayer::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    if(ImGui::BeginMainMenuBar()) {

        DrawMenu("File", []() {

            if(ImGui::MenuItem("Open Project")) {
                ProjectManager::PromptOpenProject();
            }

            ImGui::Separator();

            if(ImGui::MenuItem("New Project")) {
                ProjectManager::PromptCreateProject();
            }

            if(ImGui::MenuItem("New Scene")) {
                ProjectManager::PromptCreateScene();
            }
        });

        DrawMenu("Window", []() {
            if(ImGui::BeginMenu("Panels")) {
                ImGui::MenuItem("Environment Settings", "", &Settings::s_ShouldShowEnvironmentSettings.CurrentValue);
                ImGui::MenuItem("Global Settings", "", &Settings::s_ShouldShowGlobalSettings.CurrentValue);

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Debug Panels")) {
                ImGui::MenuItem("Assets Stats", "", &Settings::s_ShouldShowAssetsStats.CurrentValue);
                ImGui::MenuItem("Project Info", "", &Settings::s_ShouldShowProjectInfo.CurrentValue);
                ImGui::MenuItem("Renderer Stats", "", &Settings::s_ShouldShowRendererStats.CurrentValue);

                ImGui::EndMenu();
            }
        });

    }

    ImGui::EndMainMenuBar();

}