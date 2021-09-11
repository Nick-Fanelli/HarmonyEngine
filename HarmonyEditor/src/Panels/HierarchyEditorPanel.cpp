#include "HierarchyEditorPanel.h"

#include <imguipch.h>

#include <Scene/Entity.h>

#include "../Settings.h"
#include "../ImGuiDefaults.h"

#include "../Windows/CommonWindows.h"

using namespace HarmonyEditor;

static std::vector<Entity> s_Entities;

void HierarchyEditorPanel::AddToHierarchy(Entity& entity) {
    HARMONY_PROFILE_FUNCTION();

    if(!entity.ContainsComponent<TagComponent>())
        return; 

    auto& entityName = entity.GetComponent<TagComponent>().Name;

    ImGui::PushID((void*)(uint64_t)(uint32_t) entity);
    ImGui::Selectable(entityName.c_str(), m_SelectedEntity == entity);
    ImGui::PopID();

    if(ImGui::BeginPopupContextItem(std::to_string((uint32_t) entity.GetEntityID()).c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            if(m_SelectedEntity == entity)
                m_SelectedEntity = {};
            SceneManager::GetActiveScenePtr()->DeleteEntity(entity);
        }

        ImGui::EndPopup();
    }

    if(ImGui::IsItemClicked()) {
        m_SelectedEntity = entity; 
        m_IsSceneSelected = false;
    }
}

void HierarchyEditorPanel::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    if(Settings::ShowHierarchyPanel) {

        ImGui::Begin("Hierarchy", &Settings::ShowHierarchyPanel.CurrentValue);

        SceneManager::GetActiveScenePtr()->ForEachEntity([&](auto entityID) {

            Entity entity{ SceneManager::GetActiveScenePtr(), entityID };

            // Push back entity to found entity vector and if there's any left over remove them?
            if(std::find(s_Entities.begin(), s_Entities.end(), entity) == s_Entities.end()) {
                s_Entities.push_back(entity);
            }
        });

        if(ImGui::Button("New Entity")) {
            SceneManager::GetActiveScenePtr()->CreateEntity("Untitled Entity");
        }

        static constexpr auto flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

        if(ImGui::TreeNodeEx(SceneManager::GetActiveScenePtr()->GetSceneName().c_str(), flags | (m_IsSceneSelected ? ImGuiTreeNodeFlags_Selected : 0))) {

            for(size_t i = 0; i < s_Entities.size(); i++) {

                if(!SceneManager::GetActiveScenePtr()->IsValid(s_Entities[i])) {
                    s_Entities.erase(s_Entities.begin() + i);
                    continue;
                }

                AddToHierarchy(s_Entities[i]);

                if(ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
                    // Log::Warn("Active");
                    size_t next = i + (ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).y < 0.0f ? -1 : 1);

                    if(next >= 0 && next < s_Entities.size()) {
                        std::swap(s_Entities[i], s_Entities[next]);

                        ImGui::ResetMouseDragDelta();
                    }

                }
            }

            ImGui::TreePop();

        }

        ImGui::End();
    }

}