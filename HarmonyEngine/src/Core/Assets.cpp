#include "Assets.h"

#include "Render/Renderer.h"
#include "Render/ObjectLoader.h"

using namespace HarmonyEngine;

// ==========================================================================================
// Asset Manager
// ==========================================================================================
template<typename T>
std::list<Asset<T>> AssetManager::m_AssetRegistry;

AssetHandle<Texture> AssetManager::GetTexture(const std::filesystem::path& filepath) {
    for(auto& asset : m_AssetRegistry<Texture>) {
        if(asset.GetFilepath() == filepath)
            return AssetHandle<Texture>(&asset);
    }

    return AssetHandle<Texture>(&m_AssetRegistry<Texture>.emplace_back(filepath, filepath));
}

AssetHandle<Texture> AssetManager::CreateTexture(const std::filesystem::path& filepath) {
    for(auto& asset : m_AssetRegistry<Texture>) {
        if(asset.GetFilepath() == filepath) {
            if(!asset.GetRawAsset()->IsCreated())
                asset.GetRawAsset()->Create();
            return AssetHandle<Texture>(&asset);
        }
    }

    auto& texture = m_AssetRegistry<Texture>.emplace_back(filepath, filepath);
    texture.GetRawAsset()->Create();
    return AssetHandle<Texture>(&texture);
}


AssetHandle<Mesh> AssetManager::GetMesh(const std::filesystem::path& filepath) {
    for(auto& asset : m_AssetRegistry<Mesh>) {
        if(asset.GetFilepath() == filepath)
            return AssetHandle<Mesh>(&asset);
    }

    return AssetHandle<Mesh>(&m_AssetRegistry<Mesh>.emplace_back(filepath, filepath));
}

AssetHandle<Mesh> AssetManager::CreateMesh(const std::filesystem::path& filepath) {
    for(auto& asset : m_AssetRegistry<Mesh>) {
        if(asset.GetFilepath() == filepath) {
            if(!asset.GetRawAsset()->IsCreated())
                OBJLoader::LoadOBJFile(filepath, asset.GetRawAsset());
            return AssetHandle<Mesh>(&asset);
        }
    }

    auto& mesh = m_AssetRegistry<Mesh>.emplace_back(filepath, filepath);
    OBJLoader::LoadOBJFile(filepath, mesh.GetRawAsset());
    return AssetHandle<Mesh>(&mesh);
}

void AssetManager::CreateAll() {
    // Textures
    for(auto& texture : m_AssetRegistry<Texture>) {
        if(!texture.GetRawAsset()->IsCreated())
            texture.GetRawAsset()->Create();
    }

    // Meshes
    for(auto& mesh : m_AssetRegistry<Mesh>) {
        if(!mesh.GetRawAsset()->IsCreated())
            OBJLoader::LoadOBJFile(mesh.GetFilepath(), mesh.GetRawAsset());
    }
}

void AssetManager::DestroyAll() {
    for(auto& texture : m_AssetRegistry<Texture>) {
        if(texture.GetRawAsset()->IsCreated())
            texture.GetRawAsset()->Delete();
    }

    m_AssetRegistry<Texture>.clear(); // Clear all textures
    m_AssetRegistry<Mesh>.clear(); // Clear and Destroy Mesh Data
}