#include "HierarchyEditorPanel.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

#include <Scene/Component.h>

#include "../Project.h"

#include "../Layers/ImGuiLayer.h"

static char s_SearchBuffer[256];

void HierarchyEditorPanel::OnUpdate() {
    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    if(m_ScenePtr->GetCurrentSceneFile() != "no-path") {
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImGui::InputTextEx("##HierarchySearch", "Search Entity", s_SearchBuffer, IM_ARRAYSIZE(s_SearchBuffer), 
        { ImGui::GetContentRegionAvailWidth() - lineHeight - GImGui->Style.FramePadding.x * 2.0f, lineHeight}, 0);
        ImGui::SameLine();

        if(ImGui::Button("\uf067", { lineHeight, lineHeight })) { 
            m_ScenePtr->CreateEntity("Unassigned");
        }

        m_ScenePtr->GetRegistry().each([&](auto entityID) {
            Entity entity = Entity(m_ScenePtr, entityID);
            AddToHierarchy(entity);
        });

        if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            Entity nullEntity = Entity();
            ImGuiLayer::SetSelectedEntity(nullEntity);
        }
    } else {
        if(ImGui::Button("Create Scene")) {
            ProjectManager::PromptCreateScene();
        }
    }

    ImGui::End();
}

void HierarchyEditorPanel::AddToHierarchy(Entity& entity) {

    HARMONY_PROFILE_FUNCTION();

    if(!entity.ContainsComponent<TagComponent>())
        return;

    auto& entityName = entity.GetComponent<TagComponent>().Name;

    ImGuiTreeNodeFlags flags = ((ImGuiLayer::GetSelectedEntity() == entity) ?   ImGuiTreeNodeFlags_Selected : 0) | 
                                                                                ImGuiTreeNodeFlags_OpenOnArrow | 
                                                                                ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags | ImGuiTreeNodeFlags_Leaf, "%s", entityName.c_str());

    if(ImGui::IsItemClicked()) {
        ImGuiLayer::SetSelectedEntity(entity);
    }

    if(ImGui::BeginPopupContextItem(std::to_string((uint32_t) entity.GetEntityID()).c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            m_ScenePtr->DeleteEntity(entity);

            Entity nullEntity;
            ImGuiLayer::SetSelectedEntity(nullEntity);
        }

        ImGui::EndPopup();
    }

    if(opened) {
        ImGui::TreePop();        
    }

}