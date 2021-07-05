#include "Settings.h"

#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "Application.h"
#include "Project.h"

#include "Layers/ImGuiLayer.h"
#include "Layers/MenuBarLayer.h"

// ====================================================================================================
// Settings
// ====================================================================================================

// Cache Settings
Settings::Setting Settings::s_ShouldCacheCurrentProject = { true, "ShouldCacheCurrentProject" };
Settings::Setting Settings::s_ShouldCacheCurrentScene = { true, "ShouldCacheCurrentScene" };

// Panel Settings
Settings::Setting Settings::s_ShouldShowRendererStats = { false, "ShouldShowRendererStats" };
Settings::Setting Settings::s_ShouldShowEnvironmentSettings = { false, "ShouldShowEnvironmentSettings" };
Settings::Setting Settings::s_ShouldShowAssetsStats = { false, "ShouldShowAssetsStats" };
Settings::Setting Settings::s_ShouldShowProjectInfo = { false, "ShouldShowProjectInfo" };
Settings::Setting Settings::s_ShouldShowGlobalSettings = { false, "ShouldShowGlobalSettings" };

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

    if(root["Cache"]) {
        auto cache = root["Cache"];

        LoadSpecificSetting(cache, s_ShouldCacheCurrentProject);
        LoadSpecificSetting(cache, s_ShouldCacheCurrentScene);
    }

    if(root["Panels"]) {
        auto panels = root["Panels"];

        LoadSpecificSetting(panels, s_ShouldShowRendererStats);
        LoadSpecificSetting(panels, s_ShouldShowEnvironmentSettings);
        LoadSpecificSetting(panels, s_ShouldShowAssetsStats);
        LoadSpecificSetting(panels, s_ShouldShowProjectInfo);
        LoadSpecificSetting(panels, s_ShouldShowGlobalSettings);
    }

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

    // Cache
    out << YAML::Key << "Cache" << YAML::BeginMap; // Cache

    SaveSetting(out, s_ShouldCacheCurrentProject);
    SaveSetting(out, s_ShouldCacheCurrentScene);

    out << YAML::EndMap; // Cache

    // Panels
    out << YAML::Key << "Panels" << YAML::BeginMap;

    SaveSetting(out, s_ShouldShowRendererStats);
    SaveSetting(out, s_ShouldShowEnvironmentSettings);
    SaveSetting(out, s_ShouldShowAssetsStats);
    SaveSetting(out, s_ShouldShowProjectInfo);
    SaveSetting(out, s_ShouldShowGlobalSettings);

    out << YAML::EndMap;

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

    if(Settings::ShouldCacheCurrentProject() && root["Current Project"]) {
        std::filesystem::path projectPath = root["Current Project"].as<std::string>();
        if(std::filesystem::exists(projectPath) && std::filesystem::is_directory(projectPath.parent_path())) {
            ProjectManager::LoadProject(projectPath.parent_path());
        }
    }

    if(Settings::ShouldCacheCurrentScene() && root["Current Scene"]) {
        if(ProjectManager::GetCurrentProject().IsAssigned()) {
            std::string pathString = root["Current Scene"].as<std::string>();
            ProjectManager::OpenScene(std::filesystem::path{pathString});
        }
    }
}

void Settings::SaveCacheData() {

    HARMONY_PROFILE_FUNCTION();

    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    if(Settings::ShouldCacheCurrentProject() && ProjectManager::GetCurrentProject().IsAssigned())
        out << YAML::Key << "Current Project" << YAML::Value << ProjectManager::GetCurrentProject().GetProjectFilepath();

    if(Settings::ShouldCacheCurrentScene() && ProjectManager::GetCurrentProject().IsAssigned() && ProjectManager::GetScenePtr()->GetCurrentSceneFile() != "no-path")
        out << YAML::Key << "Current Scene" << YAML::Value << ProjectManager::GetScenePtr()->GetCurrentSceneFile();
 
    out << YAML::EndMap; // Root

    std::ofstream outStream(Application::GetApplicationCacheFilepath());
    outStream << out.c_str();

}

template<typename SettingType, typename UIFunction>
static void DrawSetting(Settings::Setting<SettingType>& setting, UIFunction uiFunction) {
    uiFunction();

    ImGui::SameLine();
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::PushID(setting.SettingID.c_str());

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        // TODO: Push to undo buffer
        setting.CurrentValue = setting.DefaultValue;
    }

    ImGui::PopID();
}

void Settings::OnImGuiRender() {

    if(Settings::ShouldShowGlobalSettings()) {
        ImGui::Begin("Global Settings", &s_ShouldShowGlobalSettings.CurrentValue);

        if(ImGui::CollapsingHeader("Cache")) {
            DrawSetting(s_ShouldCacheCurrentProject, [&]() {
                ImGuiLayer::DrawBool("Should Cache Current Project", s_ShouldCacheCurrentProject);
            });

            DrawSetting(s_ShouldCacheCurrentScene, [&]() {
                ImGuiLayer::DrawBool("Should Cache Current Scene", s_ShouldCacheCurrentScene);
            }); 
        }

        if(ImGui::CollapsingHeader("Assets")) {
            DrawSetting(s_AssetsUpdateSeconds, [&]() {
                ImGuiLayer::DrawInteger("Assets Update Seconds", s_AssetsUpdateSeconds, 0.05f, 0);
            });
        }

        ImGui::End();
    }
}