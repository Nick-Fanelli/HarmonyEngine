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

        inline bool IsSceneSelected() const { return m_IsSceneSelected; }

    private:
        void AddToHierarchy(Entity& entity);

    private:
        EditorLayer* m_EditorLayerPtr;
        Entity m_SelectedEntity{};

        bool m_IsSceneSelected = false;

        friend class EditorLayer;

    };

}