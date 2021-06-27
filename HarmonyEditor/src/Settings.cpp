#include "Settings.h"

#include "Layers/ImGuiLayer.h"

template<typename DataType>
struct Setting {

    DataType DefaultValue;

    Setting() = default;
    Setting(const DataType& dataType) : DefaultValue(dataType) {}

    operator DataType() const { return DefaultValue; }
};

// ====================================================================================================
// Settings
// ====================================================================================================

// Assets Settings
static const Setting<int> DefaultAssetsUpdateSeconds = 2; 
int Settings::s_AssetsUpdateSeconds = DefaultAssetsUpdateSeconds;

void Settings::LoadSettings() {

}

void Settings::SaveSettings() {

}

template<typename SettingType, typename UIFunction>
static void DrawSetting(const Setting<SettingType>& setting, SettingType& settingBuffer, UIFunction uiFunction) {
    uiFunction();

    ImGui::SameLine();
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::PushFont(ImGuiLayer::GetFontAwesome());

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        // TODO: Push to undo buffer
        settingBuffer = setting;
    }

    ImGui::PopFont();
}

void Settings::OnImGuiRender() {
    ImGui::Begin("Global Settings");

    if(ImGui::CollapsingHeader("General")) {

    }

    if(ImGui::CollapsingHeader("Assets")) {
        DrawSetting(DefaultAssetsUpdateSeconds, s_AssetsUpdateSeconds, []() {
            ImGuiLayer::DrawInteger("Assets Seconds", s_AssetsUpdateSeconds, 0.05f, 0);
        });
    }

    ImGui::End();
}