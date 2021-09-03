#include "Assets.h"

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

void AssetManager::CreateAll() {
    // Textures
    for(auto& texture : m_AssetRegistry<Texture>) {
        if(!texture.GetRawAsset()->IsCreated())
            texture.GetRawAsset()->Create();
    }
}

void AssetManager::DestroyAll() {
    for(auto& texture : m_AssetRegistry<Texture>) {
        if(texture.GetRawAsset()->IsCreated())
            texture.GetRawAsset()->Delete();
    }

    m_AssetRegistry<Texture>.clear(); // Clear all textures
}