#include "HierarchyEditorPanel.h"

#include <imguipch.h>

#include <Scene/Entity.h>

#include "../Settings.h"
#include "../ImGuiDefaults.h"

#include "../Windows/CommonWindows.h"

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
        m_IsSceneSelected = false;
    }

    if(ImGui::BeginPopupContextItem(std::to_string((uint32_t) entity.GetEntityID()).c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            if(m_SelectedEntity == entity)
                m_SelectedEntity = {};
            SceneManager::GetActiveScenePtr()->DeleteEntity(entity);
        }

        ImGui::EndPopup();
    }

    if(open)
        ImGui::TreePop();
}

void HierarchyEditorPanel::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    if(Settings::ShowHierarchyPanel) {

        ImGui::Begin("Hierarchy", &Settings::ShowHierarchyPanel.CurrentValue);

        if(ImGui::Button("New Entity")) {
            SceneManager::GetActiveScenePtr()->CreateEntity("Untitled Entity");
        }

        static constexpr auto flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

        if(ImGui::TreeNodeEx(SceneManager::GetActiveScenePtr()->GetSceneName().c_str(), flags | (m_IsSceneSelected ? ImGuiTreeNodeFlags_Selected : 0))) {

            if(ImGui::IsItemClicked()) {
                m_SelectedEntity = {};
                m_IsSceneSelected = true;
            }

            SceneManager::GetActiveScenePtr()->ForEachEntity([&](auto entityID) {
                Entity entity = { SceneManager::GetActiveScenePtr(), entityID };
                AddToHierarchy(entity);
            });

            ImGui::TreePop();
        }

        ImGui::End();
    }

}