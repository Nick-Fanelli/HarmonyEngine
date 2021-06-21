#pragma once

#include "harmonypch.h"

#include "../Render/Transform.h"
#include "../Render/Renderer.h"

namespace HarmonyEngine {

    struct TagComponent {

        std::string Name;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        TagComponent(const std::string& name) : Name(name) {}
    };

    struct TransformComponent {

        Transform Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const struct Transform& transform) : Transform(transform) {}

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

        glm::vec4 Color;

        QuadRendererComponent() = default;
        QuadRendererComponent(const QuadRendererComponent&) = default;

        QuadRendererComponent(const glm::vec4& color) : Color(color) {}
    };

}