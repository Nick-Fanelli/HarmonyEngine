#include "Assets.h"

using namespace HarmonyEngine;

// ==========================================================================================
// Asset Manager
// ==========================================================================================
std::list<Asset<Texture>> AssetManager::s_TextureRegistry;
std::list<Asset<Mesh>> AssetManager::s_MeshRegistry;

void AssetManager::CreateAll() {
    // Keep all texture loading on one thread for OpenGL
    for(auto& texture : s_TextureRegistry)
        texture->Create();

    for(auto& mesh : s_MeshRegistry)
        Renderer::LoadOBJFile(mesh->Filepath, mesh.GetPtr());
}

void AssetManager::DestroyAll() {
    for(auto& texture : s_TextureRegistry)
        texture->Delete();
    s_TextureRegistry.clear();

    s_MeshRegistry.clear();
}