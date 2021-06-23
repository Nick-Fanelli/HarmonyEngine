#pragma once

#include "harmonypch.h"

#include <list>

#include "Render/Mesh.h"
#include "Render/Texture.h"

namespace HarmonyEngine {

    // ==========================================================================================
    // Asset Handle
    // ==========================================================================================
    template<typename T>
    class AssetHandle {

        T* m_Asset = nullptr;

    public:

        AssetHandle() = default;
        AssetHandle(T* assetPtr) : m_Asset(assetPtr) {}

        const T* operator->() const { return m_Asset; }
        T* operator->() { return m_Asset; }
 
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

        static AssetHandle<Texture> QueueOrGetTexture(const std::string& filepath);
        static AssetHandle<Mesh> QueueOrGetMesh(const std::string& filepath);

        static void DirectlyAddTexture(const std::string& filepath);
        static void DirectlyAddMesh(const std::string& filepath);

        static void UpdateTextureRegistry(const std::string& filepath);
        static void UpdateMeshRegistry(const std::string& filepath);

        static void CreateAll();
        static void DestroyAll();

        static std::list<Texture>& GetTextureRegistry() { return s_TextureRegistry; }
        static std::list<Mesh>& GetMeshRegistry() { return s_MeshRegistry; }
    };

}