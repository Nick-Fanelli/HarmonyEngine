#include "AssetsEditorPanel.h"

#include <Core/Assets.h>

using namespace HarmonyEngine;

const char* AssetsEditorPanel::TextureDragDropID = "ASSET_TEXTURE";

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

    ImGui::BeginChild("MainAssetWindow", size);
    {
        if(m_AssetType == AssetType::AssetTypeTexture) {
            for(auto& texture : AssetManager::GetTextureRegistry()) {
                ImGui::Button(texture->GetFilepath());
                if(ImGui::IsItemActive()) {
                    ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_DockingPreview), 4.0f);

                    static constexpr ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip;
                    if(ImGui::BeginDragDropSource(flags)) {

                        auto textureHandle = AssetHandle<Texture>(&texture);

                        ImGui::SetDragDropPayload(TextureDragDropID, &textureHandle, sizeof(texture));
                        ImGui::EndDragDropSource();
                    }
                }
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();

}