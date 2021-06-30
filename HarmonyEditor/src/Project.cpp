#include "Project.h"

#include <imguipch.h>

#include "Layers/ImGuiLayer.h"

using namespace HarmonyEngine;

// ================================================================
// Project
// ================================================================

Project::Project(const std::string& projectFilepath) {
    m_ProjectPath = projectFilepath;
}

// ================================================================
// Project Manager
// ================================================================

const char* ProjectManager::CreateProjectPopupID = "CreateProjectPopup-ProjectManager";
bool ProjectManager::s_CreateProjectPromptOpen = true;

static std::string s_TempCreateProjectName;
static std::string s_TempCreateProjectSaveLocation;

void ProjectManager::OnImGuiRender() {
    static constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking
                                            | ImGuiWindowFlags_NoCollapse;

    if(s_CreateProjectPromptOpen) {
        ImGui::Begin("New Project", &s_CreateProjectPromptOpen, flags);

        ImGuiLayer::DrawInput("Project Name", s_TempCreateProjectName);
        ImGuiLayer::DrawInput("Location", s_TempCreateProjectSaveLocation);

        ImGui::End();
    }

}

void ProjectManager::CreateProject(const std::filesystem::path& path) {
    
}