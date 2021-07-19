#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

#include "../EditorScene.h"

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class HierarchyEditorPanel {

    public:
        HierarchyEditorPanel() = default;
        HierarchyEditorPanel(EditorScene* editorScene) : m_EditorScenePtr(editorScene) {}

        void OnImGuiRender();

        const Entity& GetSelectedEntity() const { return m_SelectedEntity; }
        Entity& GetSelectedEntity() { return m_SelectedEntity; }

    private:
        void AddToHierarchy(Entity& entity);

    private:
        EditorScene* m_EditorScenePtr;
        Entity m_SelectedEntity{};

    };

}