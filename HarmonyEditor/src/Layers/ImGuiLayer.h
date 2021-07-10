#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

#include "../Settings.h"
#include "../Scenes/EditorScene.h"

using namespace HarmonyEngine;

namespace ImGuiLayer {

    static constexpr float ColumnWidth = 75.0f;

    static bool s_IsEditorSelected = false;
    static Entity s_SelectedEntity;

    void OnCreate(EditorScene* editorScenePtr, Settings* settings);
    void OnUpdate();
    void OnDestroy();

    bool GetIsEditorSelected();

    Entity& GetSelectedEntity();
    void SetSelectedEntity(Entity& entity);

    void DrawBool(const std::string& label, bool& value);

    void DrawInteger(const std::string& label, int& value, float speed = 1.0f, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max());

    void DrawFloat(const std::string& label, float& value, float speed = 1.0f, float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max());
    void DrawFloat(const std::string& label, float& value, float speed, float min, float max, const std::string& format);

    void DrawInput(const std::string& label, std::string& value);
    void DrawVector2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max(), float columnWidth = ColumnWidth);
    void DrawVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f);
    void DrawColorControl(const std::string& label, glm::vec4& values, float resetValue = 255.0f);

    void DrawTextureInputControl(const std::string& label, AssetHandle<Texture>& assetHandle);
    void DrawMeshInputControl(const std::string& label, AssetHandle<Mesh>& assetHandle);

    // TODO: to be removed
    void DrawGameViewport();

};