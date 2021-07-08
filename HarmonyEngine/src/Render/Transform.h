#pragma once

#include "harmonypch.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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

        glm::mat4 GetTransformationMatrix() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
                        * rotation
                        * glm::scale(glm::mat4(1.0f), Scale);
        }

        void GetTransformationMatrix(glm::mat4* out) const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            *out = glm::translate(glm::mat4(1.0f), Position)
                        * rotation
                        * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

}