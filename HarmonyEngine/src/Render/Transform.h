#pragma once

#include "harmonypch.h"

namespace HarmonyEngine {

    struct Transform {
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        Transform()
        : Position({0.0f, 0.0f, 0.0f}), Rotation({0.0f, 0.0f, 0.0f}), Scale({1.0f, 1.0f, 1.0f}) {}

        Transform(const glm::vec3& position)
        : Position(position), Rotation({0.0f, 0.0f, 0.0f}), Scale({1.0f, 1.0f, 1.0f}) {}

        Transform(const glm::vec3& position, const glm::vec3& rotation)
        : Position(position), Rotation(rotation), Scale({1.0f, 1.0f, 1.0f}) {}

        Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        : Position(position), Rotation(rotation), Scale(scale) {}

        bool operator==(const Transform& other) { return Position == other.Position && Rotation == other.Rotation && Scale == other.Scale; }
        bool operator!=(const Transform& other) { return !operator==(other); }
    };

}