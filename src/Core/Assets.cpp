#include "Assets.h"

using namespace HarmonyEngine;

// ==========================================================================================
// Asset Manager
// ==========================================================================================
std::list<Asset<Texture>> AssetManager::s_TextureRegistry;

void AssetManager::CreateAll() {
    // Keep all texture loading on one thread for OpenGL
    for(auto& texture : s_TextureRegistry)
        texture->Create();
}

void AssetManager::DestroyAll() {
    for(auto& texture : s_TextureRegistry)
        texture->Delete();
    s_TextureRegistry.clear();
}