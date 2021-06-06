#pragma once

#include "harmonypch.h"

#include "../Render/Texture.h"

namespace HarmonyEngine {

    // ====================================================================================
    // Asset 
    // ====================================================================================
    template<typename T>
    class Asset {

        T* m_AssetPtr;

    public:

        Asset() = default;
        Asset(T* assetPtr) : m_AssetPtr(assetPtr) {}

        T* Get() { return m_AssetPtr; }
    };

    // ====================================================================================
    // Asset Manager
    // ====================================================================================
    struct AssetManager {

        static Asset<Texture> LoadTexture(const char* filepath);

        static void Flush();

        // Asset<Shader> LoadShader(std::string& vertexFilepath, std::string& fragmentFilepath);
        // Asset<Shader> LoadShader(std::string& vertexFilepath, std::string& fragmentFilepath, std::unordered_map<std::string, std::string>& replacements);

    };
}