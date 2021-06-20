#pragma once

#include "harmonypch.h"

#include "../Render/Renderer.h"

namespace HarmonyEngine {

    struct TagComponent {

        std::string Name;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        TagComponent(const std::string& name) : Name(name) {}
    };

    struct TransformComponent {

        glm::vec3 Position;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const glm::vec3& position) : Position(position) {}

    };

    struct MeshRendererComponent {

        AssetHandle<Mesh> MeshHandle;
        AssetHandle<Texture> TextureHandle;

        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent&) = default;

        MeshRendererComponent(const AssetHandle<Mesh>& mesh) : MeshHandle(mesh) {}
        MeshRendererComponent(const AssetHandle<Mesh>& mesh, const AssetHandle<Texture>& texture) : MeshHandle(mesh), TextureHandle(texture) {}

    };

    struct QuadRendererComponent {

        glm::vec2 Scale;
        glm::vec4 Color;

        QuadRendererComponent() = default;
        QuadRendererComponent(const QuadRendererComponent&) = default;

        QuadRendererComponent(const glm::vec2& scale, const glm::vec4& color) : Scale(scale), Color(color) {}
    };

}