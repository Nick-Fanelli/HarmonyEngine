#include "MenuBarLayer.h"

#include <imguipch.h>

#include "../Project.h"

#include "ImGuiLayer.h"

bool MenuBarLayer::s_ShowRendererStats = false;
bool MenuBarLayer::s_ShowEnvironmentSettings = false;

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

    if(ImGui::BeginMainMenuBar()) {

        DrawMenu("File", []() {

            if(ImGui::MenuItem("Open Project")) {
                ProjectManager::PromptOpenProject();
            }

            if(ImGui::MenuItem("New Project")) {
                ProjectManager::PromptCreateProject();
            }

            if(ImGui::MenuItem("New Scene")) {
                ProjectManager::PromptCreateScene();
            }
        });

        DrawMenu("Window", [](){
            ImGui::MenuItem("Renderer Stats", "", &s_ShowRendererStats);
            ImGui::MenuItem("Environment Settings", "", &s_ShowEnvironmentSettings);
        });

    }

    ImGui::EndMainMenuBar();

}