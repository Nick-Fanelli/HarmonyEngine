#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor::ImGuiDefaults {

    static constexpr float ColumnWidth = 75.0f;

    void DrawTextInput(const std::string& label, std::string& value);
    void DrawVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f);
    void DrawColorControl(const std::string& label, glm::vec4& values);

}