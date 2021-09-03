#include "ImGuiDefaults.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imguipch.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

using namespace HarmonyEditor;

static float ColumnWidth = ImGuiDefaults::DefaultColumnWidth;
static bool s_IsInputFocused = false;

const char* ImGuiDefaults::TexturePathDragDropID       = "_!_TEXTURE_PATH_DND_ID_!_"   ;
const char* ImGuiDefaults::ObjectPathDragDropID        = "_!_OBJECT_PATH_DND_ID_!_"    ;
const char* ImGuiDefaults::HarmonyScenePathDragDropID  = "_!_HYSCENE_PATH_DND_ID_!_"   ;
const char* ImGuiDefaults::LuaScriptPathDragDropID     = "_!_LUA_PATH_DND_ID_!_"       ;

void ImGuiDefaults::PushColumnWidth(float width) { ColumnWidth = width; }
void ImGuiDefaults::PopColumnWidth() { ColumnWidth = ImGuiDefaults::DefaultColumnWidth; }

void ImGuiDefaults::ResetIsInputFocused() {
    s_IsInputFocused = false;
}

bool ImGuiDefaults::IsInputFocused() {
    return s_IsInputFocused;
}

void ImGuiDefaults::DrawFloat(const std::string& label, float& value, float speed, float min, float max, const char* format) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth * 3.0f);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    ImGui::DragFloat("", &value, speed, min, max, format);
    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawBool(const std::string& label, bool& value) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth * 3.0f);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Checkbox("", &value);
    ImGui::Columns(1);

    ImGui::PopID();
}


void ImGuiDefaults::DrawTextInput(const std::string& label, std::string& value) {
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::InputText("", &value);

    s_IsInputFocused |= ImGui::IsItemFocused();

    ImGui::PopID();
}

void ImGuiDefaults::DrawVector2(const std::string& label, glm::vec2& values, float resetValue, float min, float max) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, min, max, "%.2f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, min, max, "%.2f");
    s_IsInputFocused |= ImGui::IsItemFocused();

    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawVector3(const std::string& label, glm::vec3& values, float resetValue) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawColorControl(const std::string& label, glm::vec4& values) {
    static const float resetValue = 1.0f;

    ImGui::PushID(label.c_str());

    bool changed = false;

    glm::vec4 tempColor = glm::vec4(values) * 255.0f;

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    
    ImGui::PushMultiItemsWidths(5, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    // ImGui::PushFont(boldFont);
    if(ImGui::Button("R", buttonSize)) {
        values.r = resetValue;
    }
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##R", &tempColor.r, 1.0f, 0.0f, 255.0f, "%.0f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("G", buttonSize)) {
        values.g = resetValue;
    }
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##G", &tempColor.g, 1.0f, 0.0f, 255.0f, "%.0f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("B", buttonSize)) {
        values.b = resetValue;
    }
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##B", &tempColor.b, 1.0f, 0.0f, 255.0f, "%.0f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("A", buttonSize)) {
        values.a = resetValue;
    }
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##A", &tempColor.a, 1.0f, 0.0f, 255.0f, "%.0f");
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::PopItemWidth();
    
    ImGui::PopStyleVar();

    if(changed)
        values = tempColor / 255.0f;

    ImGui::SameLine();
    if(ImGui::ColorButton("##ColorButton", ImVec4{ values.r, values.g, values.b, values.a })) {
        ImGui::OpenPopup("picker");
    }

    if (ImGui::BeginPopup("picker")) {
        ImGui::ColorPicker4("##picker", glm::value_ptr(values));
        ImGui::EndPopup();
    }
    
    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawTextureControl(const std::string& label, AssetHandle<Texture>& textureHandle) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::Button(textureHandle.IsAssigned() ? textureHandle.GetAssetBinding()->GetAssetName().c_str() : "[Unattached]");

    if(ImGui::BeginDragDropTarget()) {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(TexturePathDragDropID)) {
            auto path = *(const std::filesystem::path*) payload->Data;
            textureHandle = AssetManager::CreateTexture(path);
        }
    }

    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::SameLine();

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        textureHandle = {};
    }

    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawLuaScriptControl(const std::string& label, LuaScript& script) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    
    ImGui::Button(script.IsAssigned() ? script.GetFilepath().filename().c_str() : "[Unattached]");

    if(ImGui::BeginDragDropTarget()) {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(LuaScriptPathDragDropID)) {
            auto path = *(const std::filesystem::path*) payload->Data;
            script.LoadGlobalScript(path);
        }
    }

    ImGui::SameLine();

    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        script = {};
    }

    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawComboSelection(const std::string& label, int& value, const char* items[], uint32_t itemCount) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Combo("", &value, items, itemCount);
    s_IsInputFocused |= ImGui::IsItemFocused();
    ImGui::Columns(1);

    ImGui::PopID();
}