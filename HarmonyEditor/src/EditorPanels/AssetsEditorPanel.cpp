#include "AssetsEditorPanel.h"

#include <time.h>
#include <filesystem>

#include <Core/Assets.h>

using namespace HarmonyEngine;

const char* AssetsEditorPanel::TextureDragDropID = "ASSET_TEXTURE";
const char* AssetsEditorPanel::MeshDragDropID = "ASSET_MESH";

static const std::string AssetsPath = "assets/";

static const uint32_t AssetUpdateSeconds = 2;

static void UpdateFile(const std::filesystem::directory_entry& entry) {
    if(entry.is_directory()) {
        for(const auto& file : std::filesystem::directory_iterator(entry.path())) {
            UpdateFile(file);
        }
    } else {

        static const std::regex imageRegex(".jpeg|.png|.jpg|.tga|.bmp|.psd|.gif|.hdr|.pic|.pnm");
        static const std::regex meshRegex(".obj");

        auto extension = entry.path().extension();
        if(std::regex_match(extension.c_str(), imageRegex)) {
            AssetManager::UpdateTextureRegistry(entry.path());
        } else if(std::regex_match(extension.c_str(), meshRegex)) {
            AssetManager::UpdateMeshRegistry(entry.path());
        }
    }
}

static void UpdateFilesystem() {
    for(const auto& entry : std::filesystem::directory_iterator(AssetsPath))
        UpdateFile(entry);
}

static void DrawAssetTextures(ImGuiIO& io) {
    for(auto& texture : AssetManager::GetTextureRegistry()) {
        ImGui::Button(texture.GetFilepath().c_str());
        if(ImGui::IsItemActive()) {
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

            static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
            if(ImGui::BeginDragDropSource(flags)) {

                auto textureHandle = AssetHandle<Texture>(&texture);

                ImGui::SetDragDropPayload(AssetsEditorPanel::TextureDragDropID, &textureHandle, sizeof(texture));
                ImGui::EndDragDropSource();
            }
        }
    }
}

static void DrawAssetMeshes(ImGuiIO& io) {
    for(auto& mesh : AssetManager::GetMeshRegistry()) {
        ImGui::Button(mesh.Filepath.c_str());
        if(ImGui::IsItemActive()) {
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);
            
            static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
            if(ImGui::BeginDragDropSource(flags)) {

                auto meshHandle = AssetHandle<Mesh>(&mesh);

                ImGui::SetDragDropPayload(AssetsEditorPanel::MeshDragDropID, &meshHandle, sizeof(mesh));
                ImGui::EndDragDropSource();
            }
        }
    }
}

static time_t s_Timer = time(0);

void AssetsEditorPanel::OnCreate(Scene* scene) {
    m_ScenePtr = scene;
    UpdateFilesystem();
}

void AssetsEditorPanel::OnUpdate() {

    static ImGuiIO& io = ImGui::GetIO();

    // ImGui::ShowDemoWindow();

    ImGui::Begin("Asset Dock");

    ImVec2 size = ImGui::GetWindowSize();
    size.x /= 2;

    ImGui::BeginChild("TypeSelector", size);
    {
        static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoAutoOpenOnLog |
                                            ImGuiTreeNodeFlags_NoTreePushOnOpen;

        ImGui::TreeNodeEx("Textures", flags);
        if(ImGui::IsItemActive())
            m_AssetType = AssetType::AssetTypeTexture;

        ImGui::TreeNodeEx("Meshes", flags);
        if(ImGui::IsItemActive())
            m_AssetType = AssetType::AssetTypeMesh;

    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    if(difftime(time(0), s_Timer) == AssetUpdateSeconds) {
        s_Timer = time(0);
        UpdateFilesystem();
    }

    ImGui::BeginChild("MainAssetWindow", size);
    {
        switch(m_AssetType) {
            case AssetType::AssetTypeNone:
                break;
            case AssetType::AssetTypeTexture:
                DrawAssetTextures(io);
                break;
            case AssetType::AssetTypeMesh:
                DrawAssetMeshes(io);
                break;
        }
    }
    ImGui::EndChild();

    ImGui::End();

}