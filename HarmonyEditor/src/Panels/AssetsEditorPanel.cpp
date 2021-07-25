#include "AssetsEditorPanel.h"

#include <imguipch.h>

#include <time.h>

#include "../EditorScene.h"
#include "../Settings.h"

using namespace HarmonyEditor;

enum AssetType {
    AssetTypeUnknown,
    AssetTypeDirectory,
    AssetTypeImage,
    AssetTypeObject,
    AssetTypeHarmonyScene
};

struct AssetFile {

    std::filesystem::path Filepath;
    std::vector<AssetFile> Children;
    AssetType AssetType;
    bool ShouldShow = true;

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

static AssetFile s_RootFile;
static time_t s_Timer = time(0);

AssetsEditorPanel::AssetsEditorPanel(EditorScene* editorScenePtr) : m_EditorScenePtr(editorScenePtr) {
    SyncAssets();
}

static void LoadFile(AssetFile& parent) {
    HARMONY_PROFILE_FUNCTION();

    for(const auto& childEntry : std::filesystem::directory_iterator(parent.Filepath)) {
        if(childEntry.is_directory()) {
            auto& childAssetFile = parent.Children.emplace_back(childEntry.path(), AssetTypeDirectory);
            LoadFile(childAssetFile);
        } else {

            static const std::regex imageRegex("[^\\s]+(.*?)\\.(jpg|jpeg|png|gif|bmp|tga|psd|hdr|pic|pnm|JPG|JPEG|PNG|GIF|BMP|TGA|PSD|HDR|PIC|PNM)$");

            if(childEntry.path().extension() == ".hyscene") {
                parent.Children.emplace_back(childEntry.path(), AssetTypeHarmonyScene);
            } else if(childEntry.path().extension() == ".obj") {
                parent.Children.emplace_back(childEntry.path(), AssetTypeObject);
            } else if(std::regex_match(childEntry.path().c_str(), imageRegex)) {
                parent.Children.emplace_back(childEntry.path(), AssetTypeImage);
            } else {
                parent.Children.emplace_back(childEntry.path(), AssetTypeUnknown);
            }
        }
    }
}

static void DrawFileImGui(const std::filesystem::path& parentPath, AssetFile& child) {
    HARMONY_PROFILE_FUNCTION();

    static constexpr auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if(!child.ShouldShow)
        return;

    switch(child.AssetType) {
        case AssetTypeDirectory:
            if(ImGui::TreeNode(child.Filepath.c_str(), "\uf07b %s", child.GetRelativePath(parentPath).c_str())) {
                for(auto& file : child.Children)
                    DrawFileImGui(child.Filepath, file);
                
                ImGui::TreePop();
            }
            
            break;
        case AssetTypeHarmonyScene:
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf466 %s", child.GetRelativePath(parentPath).c_str());
            break;
        case AssetTypeImage:
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf1c5 %s", child.GetRelativePath(parentPath).c_str());
            break;
        case AssetTypeObject:   
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf1b2 %s", child.GetRelativePath(parentPath).c_str());
            break;
        case AssetTypeUnknown:
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf15b %s", child.GetRelativePath(parentPath).c_str());
            break;
    }

    if(ImGui::BeginPopupContextItem(child.Filepath.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            remove(child.Filepath);
            child.ShouldShow = false;
        }

        ImGui::EndPopup();
    }
}

void AssetsEditorPanel::SyncAssets() {
    HARMONY_PROFILE_FUNCTION();
    
    if(m_EditorScenePtr->GetActiveProject().IsAssigned()) {

        s_RootFile.Children.clear();

        s_RootFile = { m_EditorScenePtr->GetActiveProject().GetProjectAssetsDirectory(), AssetTypeDirectory };
        LoadFile(s_RootFile);

    }
}

void AssetsEditorPanel::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    if(Settings::ShowAssetsPanel) {

        if(m_EditorScenePtr->GetActiveProject().IsAssigned()) {
            if(difftime(time(0), s_Timer) >= 1) { // TODO: Configure as setting
                SyncAssets();
                s_Timer = time(0);
            }
        }

        ImGui::Begin("Assets Browser", &Settings::ShowAssetsPanel.CurrentValue);

        for(auto& child : s_RootFile.Children) {
            DrawFileImGui(m_EditorScenePtr->GetActiveProject().GetProjectAssetsDirectory(), child);
        }

        ImGui::End();
    }
}