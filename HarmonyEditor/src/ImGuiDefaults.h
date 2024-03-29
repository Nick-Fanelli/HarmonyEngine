#pragma once

#include <harmonypch.h>

#include <Core/Assets.h>
#include <Scene/LuaScript.h>

using namespace HarmonyEngine;

namespace HarmonyEditor::ImGuiDefaults {

    extern const char* TexturePathDragDropID;
    extern const char* ObjectPathDragDropID;
    extern const char* HarmonyScenePathDragDropID;
    extern const char* LuaScriptPathDragDropID;

    static constexpr float DefaultColumnWidth = 75.0f;

    void ResetIsInputFocused();
    bool IsInputFocused();

    void PushColumnWidth(float width);
    void PopColumnWidth();

    void DrawFloat(const std::string& label, float& value, float speed = 1.0f, float min = static_cast<float>(NULL), float max = static_cast<float>(NULL), const char* format = "%.3f");
    void DrawInteger(const std::string& label, int& value, int speed = 1, int min = static_cast<int>(NULL), int max = static_cast<int>(NULL));
    bool DrawBool(const std::string& label, bool& value);

    void DrawTextInput(const std::string& label, std::string& value);
    void DrawVector2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float min = static_cast<float>(NULL), float max = static_cast<float>(NULL));
    void DrawVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f);

    void DrawColorControl(const std::string& label, glm::vec4& values);
    void DrawTextureControl(const std::string& label, AssetHandle<Texture>& textureHandle);
    void DrawLuaScriptControl(const std::string& label, LuaScript& script);

    void DrawComboSelection(const std::string& label, int& value, const char* items[], uint32_t itemCount);
}