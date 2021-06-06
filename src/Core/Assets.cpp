#include "Assets.h"

using namespace HarmonyEngine;

static std::vector<Texture> s_Textures;

// ====================================================================================
// Asset Manager
// ====================================================================================
Asset<Texture> AssetManager::LoadTexture(const char* filepath) {
    auto& texture = s_Textures.emplace_back(filepath);
    texture.Create();

    return Asset<Texture>(&texture);
}

void AssetManager::Flush() {
    for(auto& texture : s_Textures) { texture.Delete(); }
    s_Textures.clear();
}