#include "Settings.h"

#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "Application.h"
#include "Project.h"

#include "Layers/ImGuiLayer.h"

// ====================================================================================================
// Settings
// ====================================================================================================

// Assets Settings
Settings::Setting Settings::s_AssetsUpdateSeconds = { 2, "AssetsUpdateSeconds" };

const std::filesystem::path& Settings::GetSaveFilepath() {
#ifdef HARMONY_PLATFORM_MACOS
    static std::filesystem::path path = Application::GetApplicationSupportDirectory() + "/user-settings.yaml";
#elif defined(HARMONY_PLATFORM_WINDOWS)
    static auto path = std::filesystem::canonical("");
#endif

    return path;
}


template<typename T>
static void LoadSpecificSetting(YAML::Node& node, Settings::Setting<T>& setting) {
    if(node[setting.SettingID])
        setting.CurrentValue = node[setting.SettingID].template as<T>();
}

void Settings::LoadSettings() {

    HARMONY_PROFILE_FUNCTION();

    std::ifstream in(Settings::GetSaveFilepath());
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());

    if(root["Assets"]) {
        auto assets = root["Assets"];

        LoadSpecificSetting(assets, s_AssetsUpdateSeconds);
    }
}

template<typename T>
static void SaveSetting(YAML::Emitter& out, Settings::Setting<T>& setting) {
    out << YAML::Key << setting.SettingID << YAML::Value << setting; 
}

void Settings::SaveSettings() {

    HARMONY_PROFILE_FUNCTION();

    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    // Assets
    out << YAML::Key << "Assets" << YAML::BeginMap; // Assets
    SaveSetting(out, s_AssetsUpdateSeconds);
    out << YAML::EndMap; // Assets

    out << YAML::EndMap; // Root

    std::ofstream outStream(Settings::GetSaveFilepath());
    outStream << out.c_str();
}

void Settings::LoadCacheData() {

    HARMONY_PROFILE_FUNCTION();

    std::ifstream in(Application::GetApplicationCacheFilepath());
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());

    if(root["Current Project"]) {
        std::filesystem::path projectPath = root["Current Project"].as<std::string>();
        if(std::filesystem::exists(projectPath) && std::filesystem::is_directory(projectPath.parent_path())) {
            ProjectManager::LoadProject(projectPath.parent_path());
        }
    }
}

void Settings::SaveCacheData() {

    HARMONY_PROFILE_FUNCTION();

    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    if(ProjectManager::GetCurrentProject().IsAssigned())
        out << YAML::Key << "Current Project" << YAML::Value << ProjectManager::GetCurrentProject().GetProjectFilepath();

    out << YAML::EndMap; // Root

    std::ofstream outStream(Application::GetApplicationCacheFilepath());
    outStream << out.c_str();

}

template<typename SettingType, typename UIFunction>
static void DrawSetting(Settings::Setting<SettingType>& setting, UIFunction uiFunction) {
    uiFunction();

    ImGui::SameLine();
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::PushFont(ImGuiLayer::GetFontAwesome());

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        // TODO: Push to undo buffer
        setting.CurrentValue = setting.DefaultValue;
    }

    ImGui::PopFont();
}

void Settings::OnImGuiRender() {
    ImGui::Begin("Global Settings");

    if(ImGui::CollapsingHeader("General")) {

    }

    if(ImGui::CollapsingHeader("Assets")) {
        DrawSetting(s_AssetsUpdateSeconds, []() {
            ImGuiLayer::DrawInteger("Assets Update Seconds", s_AssetsUpdateSeconds, 0.05f, 0);
        });
    }

    ImGui::End();
}