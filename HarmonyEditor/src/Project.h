#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class Project {

    public:
        Project() = default;
        Project(const std::filesystem::path& projectDirectory)
            : m_ProjectDirectory(projectDirectory) {}

        bool IsAssigned() const { return m_ProjectDirectory != ""; }

    private:
        std::filesystem::path m_ProjectDirectory = "";

    };

}