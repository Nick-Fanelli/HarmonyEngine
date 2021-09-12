#include "DebugPanels.h"

#include <Core/Assets.h>

#include "../ImGuiDefaults.h"

using namespace HarmonyEditor;

void DebugPanels::OnImGuiRender() {
    DebugPanels::AssetsDebugPanel::OnImGuiRender();
}

void DebugPanels::AssetsDebugPanel::OnImGuiRender() {
    // TODO: Add option to not draw this!
    ImGui::Begin("Assets Debug");

    auto& textureRegistry = AssetManager::GetAssetRegistry<Texture>();

    ImGui::Text("Texture Count: %zu", textureRegistry.size());

    ImGui::Separator();

    if(ImGui::TreeNodeEx("Loaded Assets", ImGuiTreeNodeFlags_DefaultOpen)) {

        if(ImGui::TreeNodeEx("Textures", ImGuiTreeNodeFlags_DefaultOpen)) {

            for(auto& texture : textureRegistry) {
                ImGui::TreeNodeEx(texture.GetFilepath().string().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}