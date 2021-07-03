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

    std::ifstream in(GetProjectFilepath());
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());

    if(root["Project Name"]) {
        m_ProjectName = root["Project Name"].as<std::string>();
    }

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

bool ProjectManager::s_CreateProjectPromptOpen = false;
bool ProjectManager::s_CreateScenePromptOpen = false;

static std::string s_TempCreateProjectName;
static std::string s_TempCreateProjectSaveLocation;

Project ProjectManager::m_CurrentProject;

void ProjectManager::CreateProjectPopupRender() {
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

        ImGui::PopStyleColor(fileExists ? 2 : 0);

        ImGui::End();
    }
}

static std::string s_TempSceneName;

void ProjectManager::CreateScenePopupRender() {
    static constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking
                                            | ImGuiWindowFlags_NoCollapse;

    if(s_CreateScenePromptOpen) {
        ImGui::Begin("Create Scene", &s_CreateScenePromptOpen, flags);

        ImGuiLayer::DrawInput("Scene Name", s_TempSceneName);

        if(!s_TempSceneName.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2039215686, 0.5019607843, 0.9803921569, 1});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.09803921569, 0.4431372549, 1, 1});
        }

        if(ImGui::ButtonEx("Create", {}, s_TempSceneName.empty() ? ImGuiButtonFlags_Disabled : 0)) {
            ProjectManager::CreateScene(s_TempSceneName);
            s_TempSceneName = "";
            ImGui::PopStyleColor(2);
        }

        ImGui::PopStyleColor(!s_TempSceneName.empty() ? 2 : 0);

        ImGui::End();
    }
}

void ProjectManager::LoadProject(const std::filesystem::path& path) {
    if(m_CurrentProject.m_IsAssigned)
        m_CurrentProject.Save();

    m_CurrentProject = Project(path, "Unassigned");
    m_CurrentProject.m_IsAssigned = true;
    m_CurrentProject.Load();
}

void ProjectManager::PromptOpenProject() {
    Application::OpenFileDialog({ "Harmony Project", "hyproj" }, [&](const char* path) {
        std::filesystem::path projectPath = path;
        LoadProject(projectPath.parent_path());
    });
}

void ProjectManager::OnImGuiRender() {
    CreateProjectPopupRender();
    CreateScenePopupRender();

    ImGui::Begin("Project Info");

    ImGui::LabelText("Is Assigned", "%d", m_CurrentProject.m_IsAssigned);

    if(m_CurrentProject.m_IsAssigned) {
        ImGui::LabelText("Project Name", "%s", m_CurrentProject.m_ProjectName.c_str());
        ImGui::LabelText("Project Location", "%s", m_CurrentProject.GetProjectFilepath().c_str());
        ImGui::LabelText("Assets Location", "%s", m_CurrentProject.GetAssetsPath().c_str());
    }

    ImGui::End();
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

void ProjectManager::CreateScene(const std::string& name) {

    if(!m_CurrentProject.m_IsAssigned) {
        Log::Warn("Must create project first!");
        return;
    }

    Log::FormatInfo("Creating Scene: ", name.c_str());

    std::filesystem::path scenePath = m_CurrentProject.GetAssetsPath().string() + "/" + name + ".yaml";

    if(!FileUtils::FileExists(scenePath)) {
        FileUtils::CreateFile(scenePath);
    } else {
        Log::Warn("File already exists!");
    }

    m_CurrentProject.s_SceneFiles.push_back(scenePath);
}