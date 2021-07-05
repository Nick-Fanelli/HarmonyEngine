#include "MenuBarLayer.h"

#include <imguipch.h>

#include "../Project.h"

#include "ImGuiLayer.h"

bool MenuBarLayer::s_ShowRendererStats = false;
bool MenuBarLayer::s_ShowEnvironmentSettings = false;
bool MenuBarLayer::s_ShowAssetsStats = false;
bool MenuBarLayer::s_ShowProjectInfo = false;
bool MenuBarLayer::s_ShowGlobalSettings = false;

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
                ImGui::MenuItem("Renderer Stats", "", &s_ShowRendererStats);
                ImGui::MenuItem("Environment Settings", "", &s_ShowEnvironmentSettings);
                ImGui::MenuItem("Assets Stats", "", &s_ShowAssetsStats);
                ImGui::MenuItem("Project Info", "", &s_ShowProjectInfo);
                ImGui::MenuItem("Global Settings", "", &s_ShowGlobalSettings);

                ImGui::EndMenu();
            }
        });

    }

    ImGui::EndMainMenuBar();

}