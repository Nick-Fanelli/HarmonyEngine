#include "HierarchyEditorPanel.h"

#include <imguipch.h>

#include <Scene/Entity.h>

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

    // TODO: Context Popup

    if(open)
        ImGui::TreePop();
}

void HierarchyEditorPanel::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    static float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    m_EditorScenePtr->GetSelectedScene().GetRegistry().each([&](auto entityID) {
        Entity entity = { &m_EditorScenePtr->GetSelectedScene(), entityID };
        AddToHierarchy(entity);
    });

    ImGui::End();
}