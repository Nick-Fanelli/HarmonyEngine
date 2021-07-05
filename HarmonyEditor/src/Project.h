#pragma once

#include <harmonypch.h>

#include <filesystem>

#include "Scenes/EditorScene.h"

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

    bool IsAssigned() const { return m_IsAssigned; }

private:
    std::filesystem::path m_ProjectPath;
    std::string m_ProjectName;

    bool m_IsAssigned = false;
};

// ================================================================
// Project Manager
// ================================================================

class ProjectManager {

    static EditorScene* m_ScenePtr;

    static bool s_CreateProjectPromptOpen;
    static bool s_CreateScenePromptOpen;

    static Project m_CurrentProject;

private:
    static void CreateProjectPopupRender();
    static void CreateScenePopupRender();

public:
    static const char* CreateProjectPopupID;

    static void Initialize(EditorScene* editorScene) { m_ScenePtr = editorScene; }

    static EditorScene* GetScenePtr() { return m_ScenePtr; }

    static void OnImGuiRender();

    static void CreateProject(const std::string& name, const std::filesystem::path& path);
    static void CreateScene(const std::string& name);

    static void LoadProject(const std::filesystem::path& path);

    static void PromptCreateProject() { s_CreateScenePromptOpen = false; s_CreateProjectPromptOpen = true; }
    static void PromptCreateScene() { s_CreateProjectPromptOpen = false; s_CreateScenePromptOpen = true; }

    static void OpenScene(const std::filesystem::path& path);

    static void PromptOpenProject();

    static void SaveCurrentProject() { m_CurrentProject.Save(); }

    static const Project& GetCurrentProject() { return m_CurrentProject; }
};