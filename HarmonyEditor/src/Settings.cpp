#include "Settings.h"

#include <imguipch.h>
#include <yaml-cpp/yaml.h>

#include "Application.h"
#include "ImGuiDefaults.h"

using namespace HarmonyEditor;

Setting<float> Settings::EditorMovementSensitivity = 0.003;

static const std::filesystem::path& GetSettingsFilepath() {
    static const std::filesystem::path path = Application::GetApplicationSupportDirectory() / "user-settings.yaml";
    return path;
}

template<typename T>
static void SerializeSetting(YAML::Emitter& out, const Setting<T>& setting) {
    out << YAML::Key << typeid(setting).name() << YAML::Value << setting;
}

template<typename T>
static void DeserializeSetting(YAML::Node& node, const Setting<T>& setting) {
    if(node[typeid(setting).name()]) 
        setting.CurrentValue = node[typeid(setting).name()].as<T>();
}

void SettingsManager::LoadSettings() {
    std::ifstream in(GetSettingsFilepath());
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());

    DeserializeSetting(root, Settings::EditorMovementSensitivity);
}

void SettingsManager::SaveSettings() {
    YAML::Emitter out;

    out << YAML::BeginMap; // Root

    SerializeSetting(out, Settings::EditorMovementSensitivity);

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
    ImGui::Begin("Settings");

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