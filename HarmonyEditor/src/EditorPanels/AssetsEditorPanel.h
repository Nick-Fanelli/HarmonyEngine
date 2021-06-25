#pragma once

#include <harmonypch.h>

#include "EditorPanel.h"

class AssetsEditorPanel : public EditorPanel {

public:

    static const char* TextureDragDropID;
    static const char* MeshDragDropID;

    void OnCreate(HarmonyEngine::Scene* scene) override;
    void OnUpdate() override;

};