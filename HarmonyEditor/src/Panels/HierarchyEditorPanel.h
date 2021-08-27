#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

#include "../EditorScene.h"

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorLayer;

    class HierarchyEditorPanel {

    public:
        HierarchyEditorPanel() = default;
        HierarchyEditorPanel(EditorLayer* editorLayer) : m_EditorLayerPtr(editorLayer) {}

        void OnImGuiRender();

        const Entity& GetSelectedEntity() const { return m_SelectedEntity; }
        Entity& GetSelectedEntity() { return m_SelectedEntity; }

    private:
        void AddToHierarchy(Entity& entity);
        void DisplayScenePreferences();

        void SortEntities();

    private:
        EditorLayer* m_EditorLayerPtr;
        Entity m_SelectedEntity{};

        std::map<uint32_t, entt::entity> m_EntityMap;

        bool m_IsSceneSelected = false;

        friend class EditorLayer;

    };

}