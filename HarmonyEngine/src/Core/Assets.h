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

        T* AssetPtr = nullptr;
        std::string AssetName;

        Asset() = default;
        Asset(T* assetPtr) : AssetPtr(assetPtr) {} 
        Asset(T* assetPtr, const std::string& assetName) : AssetPtr(assetPtr), AssetName(assetName) {}

        operator T*() const { return AssetPtr; }
    };

    // ==========================================================================================
    // Asset Handle
    // ==========================================================================================
    template<typename T>
    class AssetHandle {

        Asset<T> m_Asset;

    public:

        AssetHandle() = default;
        AssetHandle(const Asset<T>& assetPtr) : m_Asset(assetPtr) {}
        AssetHandle(T* assetPtr) : m_Asset(Asset<T>(assetPtr)) {}
        AssetHandle(T* assetPtr, const std::string& assetName) : m_Asset(Asset<T>(assetPtr, assetName)) {}

        const T* operator->() const { return m_Asset; }
        T* operator->() { return m_Asset; }

        T* GetAsset() const { return m_Asset; }

        const Asset<T>& GetAssetBinding() const { return m_Asset; }
        Asset<T>& GetAssetBinding() { return m_Asset; }
 
        bool IsAssigned() { return m_Asset != nullptr; }
    };

    // ==========================================================================================
    // Asset Manager
    // ==========================================================================================
    class AssetManager {

        static std::list<Texture> s_TextureRegistry;
        static std::list<Mesh> s_MeshRegistry;

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

        static std::list<Texture>& GetTextureRegistry() { return s_TextureRegistry; }
        static std::list<Mesh>& GetMeshRegistry() { return s_MeshRegistry; }
    };

}