#pragma once

#include "harmonypch.h"

#include <filesystem>

#include "Render/Mesh.h"

namespace HarmonyEngine::OBJLoader {

    void LoadOBJFile(const std::filesystem::path& filepath, Mesh* mesh, float textureID = 0);

}