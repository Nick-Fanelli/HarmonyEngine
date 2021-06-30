#pragma once

#include <harmonypch.h>

#include <filesystem>

// ================================================================
// Project
// ================================================================

class Project {

public:
    Project() = default;
    Project(const std::string& projectFilepath);

private:
    std::filesystem::path m_ProjectPath;

};

// ================================================================
// Project Manager
// ================================================================

class ProjectManager {

    static bool s_CreateProjectPromptOpen;

public:
    static const char* CreateProjectPopupID;

    static void OnImGuiRender();
    static void CreateProject(const std::filesystem::path& path);

    static void PromptCreateProject() { s_CreateProjectPromptOpen = true; }
};