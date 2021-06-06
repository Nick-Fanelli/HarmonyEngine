#pragma once

#include "harmonypch.h"

#include <list>

#include "../Render/Texture.h"

namespace HarmonyEngine {

    // ==========================================================================================
    // Asset 
    // ==========================================================================================
    template<typename T>
    class Asset {

        T m_Data;

    public:
        template<typename... Args>
        Asset(Args&&... args) {
            m_Data = T(std::forward<Args>(args)...);
        }

        T* operator->() {return &m_Data; }
    };

    // ==========================================================================================
    // Asset Manager
    // ==========================================================================================
    class AssetManager {

        static std::list<Asset<Texture>> s_TextureRegistry;

    public:

        static Asset<Texture>& QueueTexture(const char* filepath) {
            return s_TextureRegistry.emplace_back(filepath);
        }

        static void CreateAll();
        static void DestroyAll();

    };

}