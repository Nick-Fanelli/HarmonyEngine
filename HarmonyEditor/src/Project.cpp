#include "Project.h"

#include <stdio.h>
#include <stdlib.h>

#include <imguipch.h>

#include <yaml-cpp/yaml.h>

#include "Application.h"
#include "Layers/ImGuiLayer.h"

using namespace HarmonyEngine;

// ================================================================
// Project
// ================================================================

Project::Project(const std::filesystem::path& projectFilepath, const std::string& projectName) : m_ProjectPath(projectFilepath), m_ProjectName(projectName) {}

const std::filesystem::path& Project::GetProjectFilepath() const {
    static const std::filesystem::path projectFilepath = m_ProjectPath.string() + "/project-settings.hyproj";
    return projectFilepath;
}

const std::filesystem::path& Project::GetAssetsPath() const {
    static const std::filesystem::path assetsPath = m_ProjectPath.string() + "/assets";
    return assetsPath;
}

void Project::Load() {
    if(!m_IsAssigned)
        return;
}

void Project::Save() {
    if(!m_IsAssigned)
        return;

    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    out << YAML::Key << "Project Name" << YAML::Value << m_ProjectName;

    out << YAML::EndMap; // Root

    std::ofstream outStream(Project::GetProjectFilepath());
    outStream << out.c_str();
    outStream.close();
}

// ================================================================
// Project Manager
// ================================================================

const char* ProjectManager::CreateProjectPopupID = "CreateProjectPopup-ProjectManager";
bool ProjectManager::s_CreateProjectPromptOpen = true;

static std::string s_TempCreateProjectName;
static std::string s_TempCreateProjectSaveLocation;

Project ProjectManager::m_CurrentProject;

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
    if(name.empty() || path.empty()) {
        Log::Warn("Can not create a new project without a name and path!\n\tStatus: Returning");
        return;
    }

    if(!std::filesystem::exists(path)) {
        Log::Warn("Can not create a project with a non-existant path!\n\tStatus: Returning");
        return;
    }

    m_CurrentProject = Project(path, name);
    m_CurrentProject.m_IsAssigned = true;

    auto& projectFile = m_CurrentProject.GetProjectFilepath();
    auto& assetsDirectory = m_CurrentProject.GetAssetsPath();
    Log::FormatInfo("Creating project: %s", projectFile.c_str());

    // Create Project File
    if(!FileUtils::FileExists(projectFile))
        FileUtils::CreateFile(projectFile);
    else
        Log::Warn("Project File Already Exists! Do you really want to continue?");
        // TODO: Display Dialog

    // Create Assets Directory
    if(!std::filesystem::exists(assetsDirectory)) {
        std::filesystem::create_directories(assetsDirectory);
    }
    
    m_CurrentProject.Save();
    s_CreateProjectPromptOpen = false;
}