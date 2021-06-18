#pragma once

#include "harmonypch.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace HarmonyEngine {

    namespace FileUtils {

        std::string ReadFile(const char* filepath);
        std::string ReadFile(const std::string& filepath);

        bool FileExists(const char* filepath);
        bool FileExists(const std::string& filepath);
    }

    namespace OpenGLUtils {

        int GetGUPMaxTextureSlots();

    }

}