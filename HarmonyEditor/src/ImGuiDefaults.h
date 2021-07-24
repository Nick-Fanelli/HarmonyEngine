#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor::ImGuiDefaults {

    static constexpr float ColumnWidth = 75.0f;

    void DrawFloat(const std::string& label, float& value, float speed = 1.0f, float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max());

    void DrawTextInput(const std::string& label, std::string& value);
    void DrawVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f);
    void DrawColorControl(const std::string& label, glm::vec4& values);

}