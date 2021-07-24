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

    DeserializeSetting(root, Settings::EditorMovementSensitivity, "EditorMovementSensitivity");
}

void SettingsManager::SaveSettings() {
    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    for(auto& entry : Settings::AllShowPanelSettings)
        SerializeSetting(out, *entry.second, entry.first.c_str());

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
    if(ImGui::ButtonEx("\uf0e2", { lineHeight, lineHeight })) {
        setting.Reset();
    }
}

void SettingsManager::OnImGuiRender() {
    if(Settings::ShowSettingsPanel) {
        ImGui::Begin("Settings", &Settings::ShowSettingsPanel.CurrentValue);

        if(ImGui::CollapsingHeader("General")) {
            if(ImGui::TreeNodeEx("Viewport Settings", ImGuiTreeNodeFlags_DefaultOpen)) {

                DrawSetting(Settings::EditorMovementSensitivity, []() {
                    ImGuiDefaults::DrawFloat("Editor Movement Sensitivity", Settings::EditorMovementSensitivity.CurrentValue, 0.001f, 0.0f);
                });

                ImGui::TreePop();
            }
        }

        ImGui::End();
    }
}