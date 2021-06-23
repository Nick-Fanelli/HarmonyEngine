#include "Assets.h"

#include "Render/Renderer.h"

using namespace HarmonyEngine;

// ==========================================================================================
// Asset Manager
// ==========================================================================================
std::list<Texture> AssetManager::s_TextureRegistry;
std::list<Mesh> AssetManager::s_MeshRegistry;

AssetHandle<Texture> AssetManager::QueueTexture(const std::string& filepath) {
    return AssetHandle<Texture>(&s_TextureRegistry.emplace_back(filepath));
}

AssetHandle<Mesh> AssetManager::QueueMesh(const std::string& filepath) {
    return AssetHandle<Mesh>(&s_MeshRegistry.emplace_back(filepath));
}

AssetHandle<Texture> AssetManager::QueueOrGetTexture(const std::string& filepath) {
    for(auto& texture : s_TextureRegistry) {
        if(texture.GetFilepath() == filepath)
            return AssetHandle<Texture>(&texture);
    }

    return AssetManager::QueueTexture(filepath);
}

AssetHandle<Mesh> AssetManager::QueueOrGetMesh(const std::string& filepath) {
    for(auto& mesh : s_MeshRegistry) {
        if(mesh.Filepath == filepath)
            return AssetHandle<Mesh>(&mesh);
    }

    return AssetManager::QueueMesh(filepath);
}

void AssetManager::CreateAll() {
    // Keep all texture loading on one thread for OpenGL
    for(auto& texture : s_TextureRegistry)
        texture.Create();

    for(auto& mesh : s_MeshRegistry)
        Renderer::LoadOBJFile(mesh.Filepath, &mesh);
}

void AssetManager::DestroyAll() {
    for(auto& texture : s_TextureRegistry)
        texture.Delete();
        
    s_TextureRegistry.clear();
    s_MeshRegistry.clear();
}