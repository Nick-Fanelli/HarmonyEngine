#include "InspectorEditorPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "../Layers/ImGuiLayer.h"

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

static void DrawColorControl(const std::string& label, glm::vec4& values) {
    ImGui::ColorEdit4(label.c_str(), glm::value_ptr(values));
}

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const char* label, Entity& selectedEntity, UIFunction uiFunction) {

    HARMONY_PROFILE_FUNCTION();
            
    static constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if(selectedEntity.ContainsComponent<ComponentType>()) {
        auto& component = selectedEntity.GetComponent<ComponentType>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2, 2 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImGui::Separator();

        bool open = ImGui::TreeNodeEx(label, treeNodeFlags);

        ImGui::PopStyleVar();

        if(!std::is_same<ComponentType, TagComponent>()) {

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

            if(ImGui::Button("-", ImVec2{ lineHeight, lineHeight })) {
                selectedEntity.RemoveComponenet<ComponentType>();
            }
        }

        if(open) {
            uiFunction(component);
            ImGui::TreePop();
        }
    }
}

static void DrawAddComponentButton(Entity& entity) {
    ImGui::Separator();
    ImGui::Button("Add Component");
}

void InspectorEditorPanel::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Components");

    auto& selectedEntity = ImGuiLayer::GetSelectedEntity();

    if(selectedEntity.IsCreated()) {

        DrawComponent<TagComponent>("Entity Tag", selectedEntity, [](TagComponent& component) {
            ImGui::InputText("Name", &component.Name);
        });

        DrawComponent<TransformComponent>("Transform", selectedEntity, [](TransformComponent& component) {
            DrawVec3Control("Position", component.Transform.Position);
            DrawVec3Control("Rotation", component.Transform.Rotation);
            DrawVec3Control("Scale", component.Transform.Scale, 1.0f);
        });

        DrawComponent<QuadRendererComponent>("Quad Renderer", selectedEntity, [](QuadRendererComponent& component) {
            DrawColorControl("Color", component.Color);
        });

        DrawComponent<MeshRendererComponent>("Mesh Renderer", selectedEntity, [](MeshRendererComponent& component) {
            DrawColorControl("Tint", component.Color);
        });

        DrawAddComponentButton(selectedEntity);

    }

    ImGui::End();

}