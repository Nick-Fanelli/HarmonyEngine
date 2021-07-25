#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorScene;

    class AssetsEditorPanel {

    public: 
        AssetsEditorPanel() = default;
        AssetsEditorPanel(EditorScene* editorScenePtr);

        void OnImGuiRender();
        void SyncAssets();

    private:
        EditorScene* m_EditorScenePtr;

    };

}