#include "Settings.h"

#include <yaml-cpp/yaml.h>

#include "Application.h"
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

    if(!std::filesystem::exists(path.parent_path()))
        std::filesystem::create_directories(path.parent_path());

    return path;
}


template<typename T>
static void LoadSpecificSetting(YAML::Node& node, Settings::Setting<T>& setting) {
    if(node[setting.SettingID])
        setting.CurrentValue = node[setting.SettingID].template as<T>();
}

void Settings::LoadSettings() {
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