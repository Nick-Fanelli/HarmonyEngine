#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class Project {

    public:
        Project() = default;
        Project(const std::filesystem::path& projectDirectory) : m_ProjectDirectory(projectDirectory) {
            if(!FileUtils::FileExists(GetProjectConfigurationFile())) // Create Project Configuration File
                FileUtils::CreateFile(GetProjectConfigurationFile());

            if(!FileUtils::FileExists(GetProjectAssetsDirectory())) // Create Assets Directory
                std::filesystem::create_directories(GetProjectAssetsDirectory());
        }

        static bool IsProject(const std::filesystem::path& filepath);

        bool IsAssigned() const { return m_ProjectDirectory != ""; }

        const std::filesystem::path& GetProjectDirectory() const { return m_ProjectDirectory; }

        const std::filesystem::path& GetProjectConfigurationFile() const {
            static const std::filesystem::path file = GetProjectDirectory() / "ProjectConfiguration.hyproj";
            return file;
        }

        const std::filesystem::path& GetProjectAssetsDirectory() const {
            static const std::filesystem::path directory = GetProjectDirectory() / "Assets";
            return directory;
        }

    private:
        std::filesystem::path m_ProjectDirectory = "";

    };

}