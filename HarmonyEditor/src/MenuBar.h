#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorLayer;

    class MenuBar {

    public:
        MenuBar() = default;
        MenuBar(EditorLayer* editorLayerPtr) : m_EditorLayerPtr(editorLayerPtr) {}

        void OnImGuiRender();
    private:
        EditorLayer* m_EditorLayerPtr = nullptr;
    };

}