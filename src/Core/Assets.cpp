#include "Assets.h"

using namespace HarmonyEngine;

// ==========================================================================================
// Asset Manager
// ==========================================================================================
std::list<Asset<Texture>> AssetManager::s_TextureRegistry;

void AssetManager::CreateAll() {
    // TODO: Multithread
    for(auto& texture : s_TextureRegistry)
        texture->Create();
}

void AssetManager::DestroyAll() {
    for(auto& texture : s_TextureRegistry)
        texture->Delete();
    s_TextureRegistry.clear();
}