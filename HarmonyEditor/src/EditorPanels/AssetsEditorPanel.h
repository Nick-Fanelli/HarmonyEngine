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

    void OnUpdate() override;

};