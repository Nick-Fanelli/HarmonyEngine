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

        SceneSerializer(Scene* scenePtr, const std::filesystem::path& filepath);

        void SerializeYAML(const std::filesystem::path& mask = "");
        void SerializerBinary();

        void DeserializeYAML(const std::filesystem::path& mask = "");
        void DeserializeBinary();

        operator bool() const { return m_ScenePtr != nullptr; }
        bool IsAssigned() const { return m_ScenePtr != nullptr; }

    private:
        Scene* m_ScenePtr = nullptr;
        std::filesystem::path m_Filepath;
    };

};