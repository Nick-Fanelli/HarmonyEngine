#pragma once

#include <harmonypch.h>

#include <Core/Assets.h>

using namespace HarmonyEngine;

namespace HarmonyEditor::ImGuiDefaults {

    static constexpr float DefaultColumnWidth = 75.0f;

    void PushColumnWidth(float width);
    void PopColumnWidth();

    void DrawFloat(const std::string& label, float& value, float speed = 1.0f, float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max(), const char* format = "%.3f");

    void DrawTextInput(const std::string& label, std::string& value);
    void DrawVector2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max());
    void DrawVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f);

    void DrawColorControl(const std::string& label, glm::vec4& values);
    void DrawTextureControl(const std::string& label, AssetHandle<Texture>& textureHandle);
    void DrawMeshControl(const std::string& label, AssetHandle<Mesh>& meshHandle);

    void DrawComboSelection(const std::string& label, int& value, const char* items[], uint32_t itemCount);
}