#include "Project.h"

#include "Application.h"

using namespace HarmonyEditor;

bool Project::IsProject(const std::filesystem::path& filepath) {
    bool result = true;

    result |= std::filesystem::is_directory(filepath);
    result |= FileUtils::FileExists(std::filesystem::path(filepath) / "ProjectConfiguration.hyproj");
    result |= FileUtils::FileExists(std::filesystem::path(filepath) / "Assets") && std::filesystem::is_directory(std::filesystem::path(filepath) / "Assets");

    return result;
}