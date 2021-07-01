#pragma once

#include "harmonypch.h"

#include <filesystem>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace HarmonyEngine {

    namespace FileUtils {

        std::string ReadFile(const char* filepath);
        std::string ReadFile(const std::string& filepath);

        bool FileExists(const char* filepath);
        bool FileExists(const std::string& filepath);
        bool FileExists(const std::filesystem::path& filepath);

        void CreateFile(const char* filepath);
        void CreateFile(const std::string& filepath);
        void CreateFile(const std::filesystem::path& filepath);
    }

    namespace OpenGLUtils {

        int GetGUPMaxTextureSlots();

    }

}