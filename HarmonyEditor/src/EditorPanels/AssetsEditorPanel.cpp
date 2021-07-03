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

struct AssetFile {

    std::filesystem::path Path;
    std::vector<AssetFile> Children;
    bool IsDirectory = false;
    bool ShouldDraw = true;

    AssetFile() = default;
    AssetFile(const std::filesystem::path& path, bool isDirectory) : Path(path), IsDirectory(isDirectory) {}

};

static AssetFile s_RootFile;

static void LoadFile(AssetFile& parent) {
    for(const auto& childEntry : std::filesystem::directory_iterator(parent.Path)) {
        auto& childAssetFile = parent.Children.emplace_back(childEntry.path(), childEntry.is_directory());
        if(childAssetFile.IsDirectory)
            LoadFile(childAssetFile);
    }
}

static void DrawFileImGui(const std::filesystem::path& parentPath, AssetFile& child) {

    if(!child.ShouldDraw)
        return;

    if(child.IsDirectory) {
        if(ImGui::TreeNode(std::filesystem::relative(child.Path, parentPath).c_str())) {
            for(auto& file : child.Children) 
                DrawFileImGui(child.Path, file);

            ImGui::TreePop();
        }
    } else {
        static ImGuiIO& io = ImGui::GetIO();

        float height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y;
        float width = ImGui::GetContentRegionAvailWidth();

        ImVec2 buttonSize = { width, height };

        ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ChildBg);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); // Align Button Text

        ImGui::ButtonEx(std::filesystem::relative(child.Path, parentPath).c_str(), buttonSize, ImGuiButtonFlags_AlignTextBaseLine);

        if(ImGui::IsItemActive()) {

            static const std::regex imageRegex(".jpeg|.png|.jpg|.tga|.bmp|.psd|.gif|.hdr|.pic|.pnm");
            static const std::regex meshRegex(".obj");
            if(std::regex_match(child.Path.extension().c_str(), imageRegex)) { // Supported Texture Files

                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if(ImGui::BeginDragDropSource(flags)) {

                    auto textureHandle = AssetHandle<Texture>(AssetManager::QueueOrGetTexture(child.Path));

                    ImGui::SetDragDropPayload(AssetsEditorPanel::TextureDragDropID, &textureHandle, sizeof(textureHandle));
                    ImGui::EndDragDropSource();
                }

            } else if(std::regex_match(child.Path.extension().c_str(), meshRegex)) {

                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if(ImGui::BeginDragDropSource(flags)) {
                    
                    auto meshHandle = AssetHandle<Mesh>(AssetManager::QueueOrGetMesh(child.Path));

                    ImGui::SetDragDropPayload(AssetsEditorPanel::MeshDragDropID, &meshHandle, sizeof(meshHandle));
                    ImGui::EndDragDropSource();
                }

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
        }

        ImGui::PopStyleVar(); // Align Button Text
        ImGui::PopStyleColor();
    }

}

void AssetsEditorPanel::OnCreate(Scene* scene) {
    m_ScenePtr = scene;
    if(ProjectManager::GetCurrentProject().IsAssigned()) {
        s_RootFile = { ProjectManager::GetCurrentProject().GetAssetsPath(), true };
        LoadFile(s_RootFile);
    }
}

void AssetsEditorPanel::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Asset Dock");

    if(ProjectManager::GetCurrentProject().IsAssigned()) {
        if(difftime(time(0), s_Timer) >= Settings::GetAssetsUpdateSecond()) {
            s_RootFile.Children.clear();

            s_RootFile = { ProjectManager::GetCurrentProject().GetAssetsPath(), true };
            LoadFile(s_RootFile);

            s_Timer = time(0);
        }

        for(auto& child : s_RootFile.Children) {
            DrawFileImGui(ProjectManager::GetCurrentProject().GetAssetsPath(), child);
        }
    }

    ImGui::End();

}