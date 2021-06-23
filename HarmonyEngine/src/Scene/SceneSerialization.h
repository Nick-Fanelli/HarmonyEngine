#pragma once

#include <harmonypch.h>

#include "Scene.h"

namespace HarmonyEngine {

    // ======================================================================================================
    // Scene Serialization
    // ======================================================================================================
    class SceneSerializer {

    public:

        SceneSerializer() = default;
        SceneSerializer(const SceneSerializer&) = default;

        SceneSerializer(Scene* scenePtr, const std::string& filepath);

        void LoadDefault() {
#if HARMONY_DEBUG
            LoadYAML();
#else
            LoadBinary();
#endif
        }

        void LoadYAML();
        void LoadBinary();

        void SerializeDefault() {
#if HARMONY_DEBUG
            SerializeYAML();
#else
            SerializeBinary();
#endif
        }

        void SerializeYAML();
        void SerializeBinary();

    private:
        Scene* m_ScenePtr;
        std::string m_Filepath;

    };

};