#pragma GCC diagnostic ignored "-Wformat-security"

#include "HierarchyEditorPanel.h"

#include <Scene/Component.h>

void HierarchyEditorPanel::OnUpdate() {
    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    m_ScenePtr->GetRegistry().each([&](auto entityID) {
        Entity entity = Entity(m_ScenePtr, entityID);
        DrawEntityNode(entity);
    });

    ImGui::End();
}

void HierarchyEditorPanel::DrawEntityNode(Entity& entity) {
    const char* entityName = entity.GetComponent<EntityTag>().Name.c_str();

    ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, entityName);
    if(ImGui::IsItemClicked()) {
        m_SelectionContext = entity;
    }

    if(opened) {
        ImGui::TreePop();        
    }

}