#include "AssetsEditorPanel.h"

#include <time.h>
#include <filesystem>

#include <Core/Assets.h>

#include "../Project.h"
#include "../Settings.h"

using namespace HarmonyEngine;

const char* AssetsEditorPanel::TextureDragDropID = "ASSET_TEXTURE";
const char* AssetsEditorPanel::MeshDragDropID = "ASSET_MESH";

static time_t s_Timer = time(0);

enum AssetType {
    AssetTypeUnknown,
    AssetTypeDirectory, 
    AssetTypeImage, 
    AssetTypeObject, 
    AssetTypeHarmonyScene
};

struct AssetFile {

    std::filesystem::path Path;
    std::vector<AssetFile> Children;
    AssetType AssetType;
    bool IsDirectory = false;
    bool ShouldDraw = true;

    AssetFile() : AssetType(AssetTypeDirectory) {}
    AssetFile(const std::filesystem::path& path, bool isDirectory, enum AssetType assetType) : Path(path),  AssetType(assetType), IsDirectory(isDirectory) {}

    const std::filesystem::path& GetRelativePath(const std::filesystem::path& parentPath) {
        if(m_RelativePath.empty())
            m_RelativePath = std::filesystem::relative(Path, parentPath);
        return m_RelativePath;
    }

private:
    std::filesystem::path m_RelativePath = "";
};

static AssetFile s_RootFile;
static std::filesystem::path s_SelectedPath;

static void LoadFile(AssetFile& parent) {
    for(const auto& childEntry : std::filesystem::directory_iterator(parent.Path)) {
        if(childEntry.is_directory()) {
            auto& childAssetFile = parent.Children.emplace_back(childEntry.path(), true, AssetTypeDirectory);
            LoadFile(childAssetFile);
        }
        else {
            static const std::regex imageRegex("[^\\s]+(.*?)\\.(jpg|jpeg|png|gif|bmp|tga|psd|hdr|pic|pnm|JPG|JPEG|PNG|GIF|BMP|TGA|PSD|HDR|PIC|PNM)$");

            if(childEntry.path().extension() == ".hyscene") {
                parent.Children.emplace_back(childEntry.path(), false, AssetTypeHarmonyScene);
            } else if(childEntry.path().extension() == ".obj") {
                parent.Children.emplace_back(childEntry.path(), false, AssetTypeObject);
            } else if(std::regex_match(childEntry.path().c_str(), imageRegex)) {
                parent.Children.emplace_back(childEntry.path(), false, AssetTypeImage);
            } else {
                parent.Children.emplace_back(childEntry.path(), false, AssetTypeUnknown);
            }
        }
    }
}

static void DrawFileImGui(const std::filesystem::path& parentPath, AssetFile& child) {

    HARMONY_PROFILE_FUNCTION();

    if(!child.ShouldDraw)
        return;

    static ImGuiIO& io = ImGui::GetIO();

    ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ChildBg);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); // Align Button Text

    static constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

    switch(child.AssetType) {
        case AssetTypeDirectory:
            if(ImGui::TreeNode(child.Path.c_str(), "\uf07b %s", child.GetRelativePath(parentPath).c_str())) {
                for(auto& file : child.Children) 
                    DrawFileImGui(child.Path, file);

                ImGui::TreePop();
            }

            break;
        case AssetTypeHarmonyScene:
            ImGui::TreeNodeEx(child.Path.c_str(), flags, "\uf466 %s", child.GetRelativePath(parentPath).c_str());

            if( ImGui::IsItemActive()) {
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    ProjectManager::OpenScene(child.Path);
                }
            }

            break;
        case AssetTypeImage:
            ImGui::TreeNodeEx(child.Path.c_str(), flags, "\uf1c5 %s", child.GetRelativePath(parentPath).c_str());

            if(ImGui::IsItemActive()) {

                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if(ImGui::BeginDragDropSource(flags)) {

                    auto textureHandle = AssetHandle<Texture>(AssetManager::QueueOrGetTexture(child.Path));

                    ImGui::SetDragDropPayload(AssetsEditorPanel::TextureDragDropID, &textureHandle, sizeof(textureHandle));
                    ImGui::EndDragDropSource();
                }
            }

            break;
        case AssetTypeObject:

            ImGui::TreeNodeEx(child.Path.c_str(), flags, "\uf1b2 %s", child.GetRelativePath(parentPath).c_str());

            if( ImGui::IsItemActive()) {

                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if(ImGui::BeginDragDropSource(flags)) {
                    
                    auto meshHandle = AssetHandle<Mesh>(AssetManager::QueueOrGetMesh(child.Path));

                    ImGui::SetDragDropPayload(AssetsEditorPanel::MeshDragDropID, &meshHandle, sizeof(meshHandle));
                    ImGui::EndDragDropSource();
                }
            }
            break;
        case AssetTypeUnknown:
            ImGui::TreeNodeEx(child.Path.c_str(), flags, "\uf15b %s", child.GetRelativePath(parentPath).c_str());
            break;
    }

    if(ImGui::BeginPopupContextItem(child.Path.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            if(remove(child.Path)) {
                child.ShouldDraw = false;
            } else {
                Log::FormatWarn("File at: %s, not deleted", child.Path.c_str());
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(); // Align Button Text
    ImGui::PopStyleColor();

}

void AssetsEditorPanel::OnCreate(EditorScene* scene) {
    m_ScenePtr = scene;
    if(ProjectManager::GetCurrentProject().IsAssigned()) {
        s_RootFile = { ProjectManager::GetCurrentProject().GetAssetsPath(), true, AssetTypeDirectory };
        LoadFile(s_RootFile);
    }
}

void AssetsEditorPanel::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Asset Dock");

    if(ProjectManager::GetCurrentProject().IsAssigned()) {
        if(difftime(time(0), s_Timer) >= Settings::GetAssetsUpdateSecond()) {
            s_RootFile.Children.clear();

            s_RootFile = { ProjectManager::GetCurrentProject().GetAssetsPath(), true, AssetTypeDirectory };
            LoadFile(s_RootFile);

            s_Timer = time(0);
        }

        for(auto& child : s_RootFile.Children) {
            DrawFileImGui(ProjectManager::GetCurrentProject().GetAssetsPath(), child);
        }
    }

    ImGui::End();  
}