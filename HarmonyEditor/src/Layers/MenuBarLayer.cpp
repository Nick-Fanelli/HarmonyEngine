#include "MenuBarLayer.h"

#include <imguipch.h>

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

        DrawMenu("Window", [](){
            ImGui::MenuItem("Renderer Stats", "", &s_ShowRendererStats);
            ImGui::MenuItem("Environment Settings", "", &s_ShowEnvironmentSettings);
        });

    }

    ImGui::EndMainMenuBar();

}