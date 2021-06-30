#include "Project.h"

#include <stdio.h>
#include <stdlib.h>

#include <imguipch.h>

#include "Application.h"
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

        bool fileExists = s_TempCreateProjectSaveLocation.empty() || FileUtils::FileExists(s_TempCreateProjectSaveLocation);

        if(!fileExists)
            ImGui::PushStyleColor(ImGuiCol_Text, { 1, 0, 0, 1});
        ImGuiLayer::DrawInput("Location", s_TempCreateProjectSaveLocation);
        if(!fileExists) 
            ImGui::PopStyleColor();

        ImGui::SameLine();

        if(ImGui::Button("Choose")) {
            Application::OpenFolderDialog(nullptr, [&](const char* directoryPath) {
                s_TempCreateProjectSaveLocation = directoryPath;
            });
        }

        if(!fileExists) {
            ImGui::PushStyleColor(ImGuiCol_Text, { 1, 0, 0, 1});
            ImGui::Text("*Directory Does Not Exist*");
            ImGui::PopStyleColor();

        }

        if(fileExists) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2039215686, 0.5019607843, 0.9803921569, 1});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.09803921569, 0.4431372549, 1, 1});
        }

        if(ImGui::ButtonEx("Create Project", {}, fileExists ? 0 : ImGuiButtonFlags_Disabled))
            ProjectManager::CreateProject(s_TempCreateProjectName, s_TempCreateProjectSaveLocation);

        if(fileExists) {
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }

}

void ProjectManager::CreateProject(const std::string& name, const std::filesystem::path& path) {
    
}