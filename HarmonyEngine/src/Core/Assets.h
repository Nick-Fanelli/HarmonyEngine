#pragma once

#include "harmonypch.h"

#include <list>

#include "Render/Texture.h"

namespace HarmonyEngine {

    // ==========================================================================================
    // Asset
    // ==========================================================================================
    template<typename T>
    struct Asset {

        Asset() = default;

        template<typename... Args>
        Asset(const std::filesystem::path& filepath, Args&&... args) : m_Filepath(filepath) {
            m_RawAsset = new T(std::forward<Args>(args)...);
        }

        ~Asset() {
            delete m_RawAsset;
        }

        T* GetRawAsset() { return m_RawAsset; }
        const T* GetRawAsset() const { return m_RawAsset; }

        const std::filesystem::path& GetFilepath() const { return m_Filepath; }

        const std::string& GetAssetName() const {
            if(m_AssetName == "")
                m_AssetName = m_Filepath.filename();
            return m_AssetName;
        }

    private:
        T* m_RawAsset = nullptr;
        std::filesystem::path m_Filepath = "";

        mutable std::string m_AssetName = "";
    };


    // ==========================================================================================
    // Asset Handle
    // ==========================================================================================
    template<typename T>
    struct AssetHandle {

        AssetHandle() = default;
        AssetHandle(Asset<T>* rawAsset) : m_AssetStruct(rawAsset) {}

        bool IsAssigned() const { return m_AssetStruct != nullptr; }

        T* operator->() { return m_AssetStruct->GetRawAsset(); }
        const T* operator->() const { return m_AssetStruct->GetRawAsset(); }

        // Asset<T>* GetAssetBinding() { return m_AssetStruct; }
        const Asset<T>* GetAssetBinding() const { return m_AssetStruct; }

    private:
        Asset<T>* m_AssetStruct = nullptr;

    };

    // ==========================================================================================
    // Asset Manager
    // ==========================================================================================
    class AssetManager {

        template<typename T>
        static std::list<Asset<T>> m_AssetRegistry;

    public:

        static AssetHandle<Texture> GetTexture(const std::filesystem::path& filepath);
        static AssetHandle<Texture> CreateTexture(const std::filesystem::path& filepath);

        static void CreateAll();
        static void DestroyAll();
    };

}