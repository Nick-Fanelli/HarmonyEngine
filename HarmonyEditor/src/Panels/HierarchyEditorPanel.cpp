#include "HierarchyEditorPanel.h"

#include <imguipch.h>

#include <Scene/Entity.h>

#include "../Settings.h"
#include "../ImGuiDefaults.h"

using namespace HarmonyEditor;

void HierarchyEditorPanel::AddToHierarchy(Entity& entity) {
    HARMONY_PROFILE_FUNCTION();

    if(!entity.ContainsComponent<TagComponent>())
        return; 

    auto& entityName = entity.GetComponent<TagComponent>().Name;

    ImGuiTreeNodeFlags flags =  ImGuiTreeNodeFlags_OpenOnArrow          | 
                                ImGuiTreeNodeFlags_SpanAvailWidth       |
                                ImGuiTreeNodeFlags_Leaf                 |
                                ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);

    bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, "%s", entityName.c_str());

    if(ImGui::IsItemClicked()) {
        m_SelectedEntity = entity; 
    }

    if(ImGui::BeginPopupContextItem(std::to_string((uint32_t) entity.GetEntityID()).c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            m_EditorScenePtr->GetSelectedScene().DeleteEntity(entity);
        }

        ImGui::EndPopup();
    }

    if(open)
        ImGui::TreePop();
}

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const char* componentName, Entity& entity, UIFunction uiFunction) {

    HARMONY_PROFILE_FUNCTION();

    ImGui::PushID(componentName);

    static constexpr auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if(entity.ContainsComponent<ComponentType>()) {
        
        ImGui::Separator();

        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        auto contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2.0f, 3.0f });
        bool opened = ImGui::TreeNodeEx(componentName, flags);
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight / 2.0f);

        if(ImGui::Button("\uf068", { lineHeight, lineHeight })) {
            entity.RemoveComponent<ComponentType>();
            if(opened) ImGui::TreePop();
            ImGui::PopID();
            return;
        }

        if(opened) {
            auto& component = entity.GetComponent<ComponentType>();
            uiFunction(component);
            ImGui::TreePop();
        }
    }

    ImGui::PopID();
}

template<typename ComponentType>
static void DrawAddComponentMenuItem(const char* componentName, Entity& entity) {
    if(!entity.ContainsComponent<ComponentType>()) {
        if(ImGui::MenuItem(componentName)) {
            entity.AddComponent<ComponentType>();
            ImGui::CloseCurrentPopup();
        }
    }
}

static void DisplayEntity(Entity& entity) {

    if(!entity.ContainsComponent<TagComponent>())
        return;

    auto& tagComponent = entity.GetComponent<TagComponent>();
    ImGuiDefaults::DrawTextInput("Entity Name", tagComponent.Name);

    DrawComponent<TransformComponent>("Transform", entity, [&](TransformComponent& component) {
        ImGuiDefaults::DrawVector3("Position", component.Transform.Position);
        ImGuiDefaults::DrawVector3("Rotation", component.Transform.Rotation, 0.0f);
        ImGuiDefaults::DrawVector3("Scale", component.Transform.Scale, 1.0f);
    });

    DrawComponent<QuadRendererComponent>("Quad Renderer", entity, [&](QuadRendererComponent& component) {  
        ImGuiDefaults::DrawColorControl("Color", component.Color);
        ImGuiDefaults::DrawTextureControl("Texture", component.TextureHandle);
    });

    DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [&](MeshRendererComponent& component) {
        ImGuiDefaults::DrawColorControl("Color", component.Color);
        ImGuiDefaults::DrawTextureControl("Texture", component.TextureHandle);
        ImGuiDefaults::DrawMeshControl("Mesh", component.MeshHandle);
    });

    // New Component Button
    ImGui::Separator();
    if(ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponentPopup");

    if(ImGui::BeginPopup("AddComponentPopup")) {
        
        DrawAddComponentMenuItem<TransformComponent>("Transform", entity);
        DrawAddComponentMenuItem<QuadRendererComponent>("Quad Renderer", entity);
        DrawAddComponentMenuItem<MeshRendererComponent>("Mesh Renderer", entity);
        DrawAddComponentMenuItem<SpriteRendererComponent>("Sprite Renderer", entity);

        ImGui::EndPopup();
    }

}

void HierarchyEditorPanel::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    if(Settings::ShowHierarchyPanel) {

        ImGui::Begin("Hierarchy", &Settings::ShowHierarchyPanel.CurrentValue);

        if(ImGui::Button("New Entity")) {
            m_EditorScenePtr->GetSelectedScene().CreateEntity("Untitled Entity");
        }

        static constexpr auto flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

        if(ImGui::TreeNodeEx(m_EditorScenePtr->GetSelectedScene().GetSceneName().c_str(), flags)) {

            m_EditorScenePtr->GetSelectedScene().GetRegistry().each([&](auto entityID) {
                Entity entity = { &m_EditorScenePtr->GetSelectedScene(), entityID };
                AddToHierarchy(entity);
            });

            ImGui::TreePop();
        }

        ImGui::End();
    }

    if(Settings::ShowComponentsPanel) {

        ImGui::Begin("Components", &Settings::ShowComponentsPanel.CurrentValue);

        if(m_SelectedEntity)
            DisplayEntity(m_SelectedEntity);

        ImGui::End();

    }

}