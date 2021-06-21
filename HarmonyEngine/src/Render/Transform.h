#pragma once

#include "harmonypch.h"

#include <glm/gtc/matrix_transform.hpp>

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

        glm::mat4 GetTransformationMatrix() {
            return glm::translate(glm::mat4(1.0f), Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1.0f, 0.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });
        }

        void GetTransformationMatrix(glm::mat4* out) {
            *out = glm::translate(glm::mat4(1.0f), Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1.0f, 0.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });
        }
    };

}