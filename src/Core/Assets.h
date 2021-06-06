#pragma once

#include "harmonypch.h"

#include <list>

#include "../Render/Mesh.h"
#include "../Render/Texture.h"

namespace HarmonyEngine {

    // ==========================================================================================
    // Asset 
    // ==========================================================================================
    template<typename T>
    class Asset {

        T m_Data;

    public:
        Asset(const Asset& other) {
            m_Data = other.m_Data;
        }

        template<typename... Args>
        Asset(Args&&... args) {
            m_Data = T(std::forward<Args>(args)...);
        }

        T* operator->() { return &m_Data; }
        T* GetPtr() { return &m_Data; }

        T& GetData() { return m_Data; }
    };

    // ==========================================================================================
    // Asset Handle
    // ==========================================================================================
    template<typename T>
    class AssetHandle {

        Asset<T>* m_Asset;

    public:

        AssetHandle() = default;
        AssetHandle(const AssetHandle& other) { m_Asset = other.m_Asset; } 
        AssetHandle(Asset<T>* assetPtr) : m_Asset(assetPtr) {}

        T* operator->() {
            static auto* data = m_Asset->GetPtr();
            return data;
        }

    };

    // ==========================================================================================
    // Asset Manager
    // ==========================================================================================
    class AssetManager {

        static std::list<Asset<Texture>> s_TextureRegistry;
        static std::list<Asset<Mesh>> s_MeshRegistry;

    public:

        static AssetHandle<Texture> QueueTexture(const char* filepath) {
            return AssetHandle<Texture>(&s_TextureRegistry.emplace_back(filepath));
        }

        static AssetHandle<Mesh> QueueMesh(const char* filepath) {
            return AssetHandle<Mesh>(&s_MeshRegistry.emplace_back(filepath));
        }

        static void CreateAll();
        static void DestroyAll();

    };

}