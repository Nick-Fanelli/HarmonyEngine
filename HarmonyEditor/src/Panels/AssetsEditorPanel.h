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

    public:
        enum AssetType {
            AssetTypeUnknown,
            AssetTypeDirectory,
            AssetTypeTexture,
            AssetTypeObject,
            AssetTypeHarmonyScene,
            AssetTypeLuaScript
        };

        struct AssetFile {

            std::filesystem::path Filepath;
            std::vector<AssetFile> Children;
            AssetType AssetType;

            AssetFile() = default;
            AssetFile(const std::filesystem::path& path, enum AssetType assetType) : Filepath(path), AssetType(assetType) {}

            const std::filesystem::path& GetRelativePath(const std::filesystem::path& parentPath) {
                if(m_RelativePath.empty())  
                    m_RelativePath = std::filesystem::relative(Filepath, parentPath);

                return m_RelativePath;
            }

        private:
            std::filesystem::path m_RelativePath;

        };

    private:
        void DrawFileImGui(const std::filesystem::path& parentPath, AssetFile& child);

        EditorScene* m_EditorScenePtr;

    };

}