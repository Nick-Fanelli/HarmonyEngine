#include "AssetsEditorPanel.h"

#include <time.h>
#include <filesystem>

#include <Core/Assets.h>

using namespace HarmonyEngine;

const char* AssetsEditorPanel::TextureDragDropID = "ASSET_TEXTURE";
const char* AssetsEditorPanel::MeshDragDropID = "ASSET_MESH";

static const std::filesystem::path AssetsPath = "assets/";
static std::filesystem::path s_CurrentPath = AssetsPath;

// static const uint32_t AssetUpdateSeconds = 2;
// static time_t s_Timer = time(0);

// static void UpdateFile(const std::filesystem::directory_entry& entry) {
//     if(entry.is_directory()) {
//         for(const auto& file : std::filesystem::directory_iterator(entry.path())) {
//             UpdateFile(file);
//         }
//     } else {

//         // static const std::regex imageRegex(".jpeg|.png|.jpg|.tga|.bmp|.psd|.gif|.hdr|.pic|.pnm");
//         // static const std::regex meshRegex(".obj");

//         // auto extension = entry.path().extension();
//         // if(std::regex_match(extension.c_str(), imageRegex)) {
//         //     AssetManager::UpdateTextureRegistry(entry.path());
//         // } else if(std::regex_match(extension.c_str(), meshRegex)) {
//         //     AssetManager::UpdateMeshRegistry(entry.path());
//         // }
//     }
// }

// static void DrawAssetTextures(ImGuiIO& io) {
//     for(auto& texture : AssetManager::GetTextureRegistry()) {
//         ImGui::Button(texture.GetFilepath().c_str());
//         if(ImGui::IsItemActive()) {
//             ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

//             static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
//             if(ImGui::BeginDragDropSource(flags)) {

//                 auto textureHandle = AssetHandle<Texture>(&texture);

//                 ImGui::SetDragDropPayload(AssetsEditorPanel::TextureDragDropID, &textureHandle, sizeof(texture));
//                 ImGui::EndDragDropSource();
//             }
//         }
//     }
// }

// static void DrawAssetMeshes(ImGuiIO& io) {
//     for(auto& mesh : AssetManager::GetMeshRegistry()) {
//         ImGui::Button(mesh.Filepath.c_str());
//         if(ImGui::IsItemActive()) {
//             ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);
            
//             static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
//             if(ImGui::BeginDragDropSource(flags)) {

//                 auto meshHandle = AssetHandle<Mesh>(&mesh);

//                 ImGui::SetDragDropPayload(AssetsEditorPanel::MeshDragDropID, &meshHandle, sizeof(mesh));
//                 ImGui::EndDragDropSource();
//             }
//         }
//     }
// }

static void LoadFile(const std::filesystem::path& rootPath, const std::filesystem::directory_entry& entry) {
    static const ImGuiIO& io = ImGui::GetIO();

    if(entry.is_directory()) {
        if(ImGui::TreeNode(std::filesystem::relative(entry.path(), rootPath).c_str())) {

            for(const auto& childEntry : std::filesystem::directory_iterator(entry.path())) {
                LoadFile(entry.path(), childEntry);
            }

            ImGui::TreePop();
        }
    } else {
        float height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y;
        float width = ImGui::GetContentRegionAvailWidth();

        ImVec2 buttonSize = { width, height };

        ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ChildBg);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); // Align Button Text

        ImGui::ButtonEx(std::filesystem::relative(entry.path(), rootPath).c_str(), buttonSize, ImGuiButtonFlags_AlignTextBaseLine);

        if(ImGui::IsItemActive()) {

            static const std::regex imageRegex(".jpeg|.png|.jpg|.tga|.bmp|.psd|.gif|.hdr|.pic|.pnm");
            static const std::regex meshRegex(".obj");
            if(std::regex_match(entry.path().extension().c_str(), imageRegex)) { // Supported Texture Files

                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if(ImGui::BeginDragDropSource(flags)) {

                    auto textureHandle = AssetHandle<Texture>(AssetManager::QueueOrGetTexture(entry.path()));

                    ImGui::SetDragDropPayload(AssetsEditorPanel::TextureDragDropID, &textureHandle, sizeof(textureHandle));
                    ImGui::EndDragDropSource();
                }

            } else if(std::regex_match(entry.path().extension().c_str(), meshRegex)) {

                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                if(ImGui::BeginDragDropSource(flags)) {
                    
                    auto meshHandle = AssetHandle<Mesh>(AssetManager::QueueOrGetMesh(entry.path()));

                    ImGui::SetDragDropPayload(AssetsEditorPanel::MeshDragDropID, &meshHandle, sizeof(meshHandle));
                    ImGui::EndDragDropSource();
                }

            }
        }

        ImGui::PopStyleVar(); // Align Button Text
        ImGui::PopStyleColor();
    }
}

void AssetsEditorPanel::OnCreate(Scene* scene) {
    m_ScenePtr = scene;
}

void AssetsEditorPanel::OnUpdate() {

    // static ImGuiIO& io = ImGui::GetIO();

    // ImGui::ShowDemoWindow();

    // if(difftime(time(0), s_Timer) == AssetUpdateSeconds) {
    //     s_Timer = time(0);
    //     UpdateFilesystem();
    // }

    ImGui::Begin("Asset Dock");

    for(const auto& entry : std::filesystem::directory_iterator(s_CurrentPath)) {
        LoadFile(AssetsPath, entry);
    }

    ImGui::End();

}