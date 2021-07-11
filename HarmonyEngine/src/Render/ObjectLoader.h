#pragma once

#include "harmonypch.h"

#include <filesystem>
#include <fbxsdk/fbxsdk.h>

#include "Render/Mesh.h"

namespace HarmonyEngine {

namespace OBJLoader {

    void LoadOBJFile(const std::filesystem::path& filepath, Mesh* mesh, float textureID = 0);

}

namespace FBXLoader {

    void LoadFBXFile(const std::filesystem::path& filepath);

}

};