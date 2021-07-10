#pragma once

#include "harmonypch.h"

#include <list>

#include "Render/Mesh.h"
#include "Render/Texture.h"

namespace HarmonyEngine {

    // ==========================================================================================
    // Asset
    // ==========================================================================================

    template<typename T>
    struct Asset {

        T RawAsset;
        std::string AssetName;

        Asset() = default;

        template<typename... Args>
        Asset(const std::string& assetName, Args&&... args) : RawAsset(std::forward<Args>(args)...), AssetName(assetName) {}

        operator T*() const { return &RawAsset; }

        const T* operator->() const { return &RawAsset; }
        T* operator->() { return &RawAsset; }
        T* GetRawAsset() { return &RawAsset; }
    };

    // ==========================================================================================
    // Asset Handle
    // ==========================================================================================
    template<typename T>
    class AssetHandle {

        Asset<T>* m_Asset = nullptr;

    public:

        AssetHandle() = default;
        AssetHandle(Asset<T>* assetPtr) : m_Asset(assetPtr) {}
        AssetHandle(T* assetPtr) : m_Asset(Asset<T>(assetPtr)) {}
        AssetHandle(T* assetPtr, const std::string& assetName) : m_Asset(Asset<T>(assetPtr, assetName)) {}

        const T* operator->() const { return m_Asset->GetRawAsset(); }
        T* operator->() { return m_Asset->GetRawAsset(); }

        T* GetAsset() { return m_Asset->GetRawAsset(); }

        const Asset<T>* GetAssetBinding() const { return m_Asset; }
        Asset<T>* GetAssetBinding() { return m_Asset; }
 
        bool IsAssigned() { return m_Asset != nullptr; }
    };

    // ==========================================================================================
    // Asset Manager
    // ==========================================================================================
    class AssetManager {

        static std::list<Asset<Texture>> s_TextureRegistry;
        static std::list<Asset<Mesh>> s_MeshRegistry;

    public:

        static AssetHandle<Texture> QueueTexture(const std::string& filepath);
        static AssetHandle<Mesh> QueueMesh(const std::string& filepath);

        static AssetHandle<Texture> QueueTexture(const std::string& filepath, const std::string& assetName);
        static AssetHandle<Mesh> QueueMesh(const std::string& filepath, const std::string& assetName);

        static AssetHandle<Texture> QueueOrGetTexture(const std::string& filepath);
        static AssetHandle<Mesh> QueueOrGetMesh(const std::string& filepath);

        static AssetHandle<Texture> QueueOrGetTexture(const std::string& filepath, const std::string& assetName);
        static AssetHandle<Mesh> QueueOrGetMesh(const std::string& filepath, const std::string& assetName);

        static void UpdateTextureRegistry(const std::string& filepath);
        static void UpdateMeshRegistry(const std::string& filepath);

        static void CreateAll();
        static void DestroyAll();

        static std::list<Asset<Texture>>& GetTextureRegistry() { return s_TextureRegistry; }
        static std::list<Asset<Mesh>>& GetMeshRegistry() { return s_MeshRegistry; }
    };

}