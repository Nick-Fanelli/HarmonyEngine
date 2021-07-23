#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorScene;

    class MenuBar {

    public:
        MenuBar() = default;
        MenuBar(EditorScene* editorScenePtr) : m_EditorScenePtr(editorScenePtr) {}

        void OnImGuiRender();
    private:
        EditorScene* m_EditorScenePtr = nullptr;
    };

}