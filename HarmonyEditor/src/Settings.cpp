#include "Settings.h"

#include <imguipch.h>
#include <yaml-cpp/yaml.h>

#include "Application.h"
#include "ImGuiDefaults.h"

using namespace HarmonyEditor;

// Non-Displayed
Setting<bool> Settings::ShowViewportPanel = true;
Setting<bool> Settings::ShowSettingsPanel = true;
Setting<bool> Settings::ShowHierarchyPanel = true;
Setting<bool> Settings::ShowComponentsPanel = true;

std::unordered_map<std::string, Setting<bool>*> Settings::AllShowPanelSettings = {

    { "ShowViewportPanel", &Settings::ShowViewportPanel },
    { "ShowSettingsPanel", &Settings::ShowSettingsPanel },
    { "ShowHierarchyPanel", &Settings::ShowHierarchyPanel },
    { "ShowComponentsPanel", &Settings::ShowComponentsPanel }

};

// Displayed
Setting<int> Settings::EditorInputStyle = EditorCamera::InputStyle::InputStyleDefault;
Setting<float> Settings::EditorMovementSensitivity = 0.003;

static const std::filesystem::path& GetSettingsFilepath() {
    static const std::filesystem::path path = Application::GetApplicationSupportDirectory() / "user-settings.yaml";
    return path;
}

template<typename T>
static void SerializeSetting(YAML::Emitter& out, const Setting<T>& setting, const char* id) {
    out << YAML::Key << id << YAML::Value << setting;
}

template<typename T>
static void DeserializeSetting(YAML::Node& node, const Setting<T>& setting, const char* id) {
    if(node[id]) 
        setting.CurrentValue = node[id].as<T>();
}

void SettingsManager::LoadSettings() {
    std::ifstream in(GetSettingsFilepath());
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());

    for(auto& entry : Settings::AllShowPanelSettings)
        DeserializeSetting(root, *entry.second, entry.first.c_str());

    DeserializeSetting(root, Settings::EditorInputStyle, "EditorInputStyle");
    DeserializeSetting(root, Settings::EditorMovementSensitivity, "EditorMovementSensitivity");
}

void SettingsManager::SaveSettings() {
    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    for(auto& entry : Settings::AllShowPanelSettings)
        SerializeSetting(out, *entry.second, entry.first.c_str());

    SerializeSetting(out, Settings::EditorInputStyle, "EditorInputStyle");
    SerializeSetting(out, Settings::EditorMovementSensitivity, "EditorMovementSensitivity");

    out << YAML::EndMap; // Root

    std::ofstream outStream(GetSettingsFilepath());
    outStream << out.c_str();
}

template<typename T, typename UIFunction>
static void DrawSetting(const Setting<T>& setting, UIFunction uiFunction) {

    uiFunction();

    ImGui::SameLine();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::PushID(&setting);

    if(ImGui::ButtonEx("\uf0e2", { lineHeight, lineHeight })) {
        setting.Reset();
    }

    ImGui::PopID();
}

void SettingsManager::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    if(Settings::ShowSettingsPanel) {
        ImGui::Begin("Settings", &Settings::ShowSettingsPanel.CurrentValue);

        if(ImGui::CollapsingHeader("General")) {
            if(ImGui::TreeNodeEx("Viewport Settings", ImGuiTreeNodeFlags_DefaultOpen)) {

                DrawSetting(Settings::EditorInputStyle, []() {
                    static const char* items[] = { "Default", "Reversed", "Modern" };
                    ImGuiDefaults::DrawComboSelection("Input Style", Settings::EditorInputStyle.CurrentValue, items, 3);
                });

                DrawSetting(Settings::EditorMovementSensitivity, []() {
                    ImGuiDefaults::DrawFloat("Movement Sensitivity", Settings::EditorMovementSensitivity.CurrentValue, 0.001f, 0.0f);
                });

                ImGui::TreePop();
            }
        }

        ImGui::End();
    }
}

// Cache Manager

std::string CacheManager::LastOpenProject;

template<typename T>
static void DeserializeValue(YAML::Node& node, T& value, const char* id) {
    if(node[id])
        value = node[id].as<T>();
}

void CacheManager::LoadCache() {
    std::ifstream in(Application::GetApplicationCacheFilepath());
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());

    DeserializeValue(root, CacheManager::LastOpenProject, "LastOpenedProject");
}

void CacheManager::SaveCache() {
    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    out << YAML::Key << "LastOpenedProject" << YAML::Value << CacheManager::LastOpenProject;

    out << YAML::EndMap; // Root

    std::ofstream outStream(Application::GetApplicationCacheFilepath());
    outStream << out.c_str();
}