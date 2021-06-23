#pragma once

#include <harmonypch.h>

#include "EditorPanel.h"

class AssetsEditorPanel : public EditorPanel {

private:

    enum AssetType {
        AssetTypeNone, AssetTypeTexture, AssetTypeMesh
    };

    AssetType m_AssetType = AssetType::AssetTypeNone;

public:

    static const char* TextureDragDropID;
    static const char* MeshDragDropID;

    void OnCreate(HarmonyEngine::Scene* scene) override;
    void OnUpdate() override;

};