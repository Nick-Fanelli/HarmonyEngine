#pragma once

#include <harmonypch.h>

#include <filesystem>

// ================================================================
// Project
// ================================================================

class Project {

    friend class ProjectManager;

public:
    Project() = default;
    Project(const std::filesystem::path& projectFilepath, const std::string& projectName);

    const std::filesystem::path& GetProjectFilepath() const;
    const std::filesystem::path& GetAssetsPath() const;

    void Load();
    void Save();

private:
    std::filesystem::path m_ProjectPath;
    std::string m_ProjectName;

    bool m_IsAssigned = false;
};

// ================================================================
// Project Manager
// ================================================================

class ProjectManager {

    static bool s_CreateProjectPromptOpen;
    static Project m_CurrentProject;

public:
    static const char* CreateProjectPopupID;

    static void OnImGuiRender();
    static void CreateProject(const std::string& name, const std::filesystem::path& path);

    static void PromptCreateProject() { s_CreateProjectPromptOpen = true; }

    static void SaveCurrentProject() { m_CurrentProject.Save(); }

    static const Project& GetCurrentProject() { return m_CurrentProject; }
};