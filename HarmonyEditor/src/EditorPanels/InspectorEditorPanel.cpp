#include "InspectorEditorPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Layers/ImGuiLayer.h"
#include "AssetsEditorPanel.h"

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const char* label, Entity& selectedEntity, UIFunction uiFunction) {

    HARMONY_PROFILE_FUNCTION();

    ImGui::PushID(label);
            
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

            if(ImGui::Button("\uf068", ImVec2{ lineHeight, lineHeight })) {
                selectedEntity.RemoveComponent<ComponentType>();
            }
        }

        if(open) {
            uiFunction(component);
            ImGui::TreePop();
        }
    }

    ImGui::PopID();
}

template<typename ComponentType>
static void DrawAddComponentMenuItem(const std::string& componentType, Entity& entity) {
    if(!entity.ContainsComponent<ComponentType>()) {
        if(ImGui::MenuItem(componentType.c_str())) {
            entity.AddComponent<ComponentType>();
            ImGui::CloseCurrentPopup();
        }
    }
}

static void DrawAddComponentButton(Entity& entity) {
    ImGui::Separator();
    if(ImGui::Button("Add Component")) 
        ImGui::OpenPopup("AddComponent");

    if(ImGui::BeginPopup("AddComponent")) {
        DrawAddComponentMenuItem<TransformComponent>("Transform", entity);
        DrawAddComponentMenuItem<QuadRendererComponent>("Quad Renderer", entity);
        DrawAddComponentMenuItem<MeshRendererComponent>("Mesh Renderer", entity);
        DrawAddComponentMenuItem<SpriteRendererComponent>("Sprite Renderer", entity);

        ImGui::EndPopup();
    }

}

void InspectorEditorPanel::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Components");

    auto& selectedEntity = ImGuiLayer::GetSelectedEntity();

    if(selectedEntity.IsCreated()) {

        DrawComponent<TagComponent>("Entity Tag", selectedEntity, [](TagComponent& component) {
            ImGuiLayer::DrawInput("Name", component.Name);
        });

        DrawComponent<TransformComponent>("Transform", selectedEntity, [](TransformComponent& component) {
            ImGuiLayer::DrawVector3("Position", component.Transform.Position);
            ImGuiLayer::DrawVector3("Rotation", component.Transform.Rotation);
            ImGuiLayer::DrawVector3("Scale", component.Transform.Scale, 1.0f);
            ImGui::Spacing();
        });

        DrawComponent<QuadRendererComponent>("Quad Renderer", selectedEntity, [](QuadRendererComponent& component) {
            ImGuiLayer::DrawColorControl("Color", component.Color);
            ImGuiLayer::DrawTextureInputControl("Texture", component.TextureHandle);
        });

        DrawComponent<MeshRendererComponent>("Mesh Renderer", selectedEntity, [](MeshRendererComponent& component) {
            ImGuiLayer::DrawColorControl("Tint", component.Color);
            ImGuiLayer::DrawMeshInputControl("Mesh", component.MeshHandle);
            ImGuiLayer::DrawTextureInputControl("Texture", component.TextureHandle);
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", selectedEntity, [](SpriteRendererComponent& component) {
            ImGuiLayer::DrawColorControl("Tint", component.Color);
            ImGuiLayer::DrawTextureInputControl("Texture", component.TextureHandle);
            ImGuiLayer::DrawVector2("Top Left Coords", component.TopLeftCoord, 0.0f, 0.0f, 1.0f, 150.0f);
            ImGuiLayer::DrawVector2("Bottom Right Coords", component.BottomRightCoord, 1.0f, 0.0f, 1.0f, 150.0f);
        });


        DrawAddComponentButton(selectedEntity);

    }

    ImGui::End();

}