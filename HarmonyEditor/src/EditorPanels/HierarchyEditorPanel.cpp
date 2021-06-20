#include "HierarchyEditorPanel.h"

#include "../Layers/ImGuiLayer.h"

#include <Scene/Component.h>

void HierarchyEditorPanel::OnUpdate() {
    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    m_ScenePtr->GetRegistry().each([&](auto entityID) {
        Entity entity = Entity(m_ScenePtr, entityID);
        AddToHierarchy(entity);
    });

    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
        Entity nullEntity = Entity();
        ImGuiLayer::SetSelectedEntity(nullEntity);
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
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, "%s", entityName.c_str());
    if(ImGui::IsItemClicked()) {
        ImGuiLayer::SetSelectedEntity(entity);
    }

    if(opened) {
        ImGui::TreePop();        
    }

}