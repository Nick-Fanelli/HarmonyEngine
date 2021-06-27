#include "Settings.h"

#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "Layers/ImGuiLayer.h"

static const std::filesystem::path SaveFilepath = "user-setting.yaml";

// ====================================================================================================
// Settings
// ====================================================================================================

// Assets Settings
Settings::Setting Settings::s_AssetsUpdateSeconds = { 2, "AssetsUpdateSeconds" };

template<typename T>
static void LoadSpecificSetting(YAML::Node& node, Settings::Setting<T>& setting) {
    if(node[setting.SettingID])
        setting.CurrentValue = node[setting.SettingID].template as<T>();
}

void Settings::LoadSettings() {
    std::ifstream in(SaveFilepath);
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

    std::ofstream outStream(SaveFilepath);
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