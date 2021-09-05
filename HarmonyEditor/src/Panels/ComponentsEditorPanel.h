#pragma once

#include <harmonypch.h>

#include "HierarchyEditorPanel.h"

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class ComponentsEditorPanel {

    public:
        ComponentsEditorPanel() = default;
        ComponentsEditorPanel(HierarchyEditorPanel* hierarchyEditorPanelPtr) : m_HierarchyEditorPanelPtr(hierarchyEditorPanelPtr) {}

        ComponentsEditorPanel(const ComponentsEditorPanel&) = delete;

        void OnImGuiRender();

    private:
        void DisplayScenePreferences();

    private:
        HierarchyEditorPanel* m_HierarchyEditorPanelPtr = nullptr;

    };

}