#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor::DebugPanels {

    class AssetsDebugPanel {

    public: 
        AssetsDebugPanel() = delete;
        AssetsDebugPanel(const AssetsDebugPanel&) = delete;

        static void OnImGuiRender();

    };

    void OnImGuiRender();

}