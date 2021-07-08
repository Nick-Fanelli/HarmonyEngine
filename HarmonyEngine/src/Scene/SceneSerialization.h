#pragma once

#include <harmonypch.h>

#include <filesystem>

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

        void DeserializeYAML(const std::filesystem::path& filepath = "");
        void DeserializeBinary();

    private:
        Scene* m_ScenePtr;
        std::string m_Filepath; 

    };

};