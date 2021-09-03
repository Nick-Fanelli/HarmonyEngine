#include "Settings.h"

#include <imguipch.h>
#include <yaml-cpp/yaml.h>

#include "HarmonyEditor.h"
#include "ImGuiDefaults.h"
#include "Theme.h"

using namespace HarmonyEditor;

// Non-Displayed
Setting<bool> Settings::ShowViewportPanel = true;
Setting<bool> Settings::ShowSettingsPanel = false;
Setting<bool> Settings::ShowHierarchyPanel = true;
Setting<bool> Settings::ShowComponentsPanel = true;
Setting<bool> Settings::ShowAssetsPanel = true;
Setting<bool> Settings::ShowRendererStats = false;

std::unordered_map<std::string, Setting<bool>*> Settings::AllShowPanelSettings = {

    { "ShowViewportPanel", &Settings::ShowViewportPanel },
    { "ShowSettingsPanel", &Settings::ShowSettingsPanel },
    { "ShowHierarchyPanel", &Settings::ShowHierarchyPanel },
    { "ShowComponentsPanel", &Settings::ShowComponentsPanel },
    { "ShowAssetsPanel", &Settings::ShowAssetsPanel },
    { "ShowRendererStats", &Settings::ShowRendererStats }

};

// Displayed
Setting<bool> Settings::VSyncEnabled = true;

Setting<int> Settings::EditorTheme = 1; // Dark

Setting<int> Settings::EditorInputStyle = EditorCamera::InputStyle::InputStyleDefault;
Setting<float> Settings::EditorMovementSensitivity = 3.0f;

static const std::filesystem::path& GetSettingsFilepath() {
    static const std::filesystem::path path = HarmonyEditorApplication::GetApplicationSupportDirectory() / "user-settings.yaml";
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

    DeserializeSetting(root, Settings::VSyncEnabled, "VSyncEnabled");

    DeserializeSetting(root, Settings::EditorTheme, "EditorTheme");

    DeserializeSetting(root, Settings::EditorInputStyle, "EditorInputStyle");
    DeserializeSetting(root, Settings::EditorMovementSensitivity, "EditorMovementSensitivity");
}

void SettingsManager::SaveSettings() {
    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    for(auto& entry : Settings::AllShowPanelSettings)
        SerializeSetting(out, *entry.second, entry.first.c_str());

    SerializeSetting(out, Settings::VSyncEnabled, "VSyncEnabled");

    SerializeSetting(out, Settings::EditorTheme, "EditorTheme");

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
            DrawSetting(Settings::VSyncEnabled, []() {
                if(ImGuiDefaults::DrawBool("Enable V-Sync", Settings::VSyncEnabled.CurrentValue)) {
                    Display::SetVSync(Settings::VSyncEnabled);
                }
            });
        }

        if(ImGui::CollapsingHeader("Appearance")) {
            DrawSetting(Settings::EditorTheme, []() {
                ImGuiDefaults::DrawComboSelection("Theme", Settings::EditorTheme.CurrentValue, Theme::ThemePreset, Theme::ThemePresetCount);
            });

            if(ImGui::TreeNodeEx("Viewport Settings", ImGuiTreeNodeFlags_DefaultOpen)) {

                DrawSetting(Settings::EditorInputStyle, []() {
                    ImGuiDefaults::DrawComboSelection("Input Style", Settings::EditorInputStyle.CurrentValue, EditorCamera::InputStyleFields, EditorCamera::InputStyleCount);
                });

                DrawSetting(Settings::EditorMovementSensitivity, []() {
                    ImGuiDefaults::DrawFloat("Movement Sensitivity", Settings::EditorMovementSensitivity.CurrentValue, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.1f");
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
    std::ifstream in(HarmonyEditorApplication::GetApplicationCacheFilepath());
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

    std::ofstream outStream(HarmonyEditorApplication::GetApplicationCacheFilepath());
    outStream << out.c_str();
}