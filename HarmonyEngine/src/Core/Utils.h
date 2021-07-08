#pragma once

#include "harmonypch.h"

#include <filesystem>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../Render/Transform.h"

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

    namespace MathUtils {

        bool DecomposeMatrix(const glm::mat4& matrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

    }

}