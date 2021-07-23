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

        void SerializeYAML();
        void SerializerBinary();

        void DeserializeYAML();
        void DeserializeBinary();

        operator bool() const { return m_ScenePtr != nullptr; }
        bool IsAssigned() const { return m_ScenePtr != nullptr; }

    private:
        Scene* m_ScenePtr = nullptr;
        std::string m_Filepath;
    };

};