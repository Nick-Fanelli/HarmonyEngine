#pragma once

#include "harmonypch.h"

#include "../Render/Renderer.h"

namespace HarmonyEngine {

    struct Transform {

        glm::vec3 Position;

        Transform() = default;
        Transform(const Transform&) = default;

        Transform(const glm::vec3& position) : Position(position) {}

    };

    struct MeshRenderer {

        Mesh* MeshPtr;

        MeshRenderer() = default;
        MeshRenderer(const MeshRenderer&) = default;

        MeshRenderer(Mesh* mesh) : MeshPtr(mesh) {}

    };

}