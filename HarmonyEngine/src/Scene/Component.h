#pragma once

#include "harmonypch.h"

#include "../Render/Renderer.h"

namespace HarmonyEngine {

    struct EntityTag {

        std::string Name;

        EntityTag() = default;
        EntityTag(const EntityTag&) = default;

        EntityTag(const std::string& name) : Name(name) {}
    };

    struct Transform {

        glm::vec3 Position;

        Transform() = default;
        Transform(const Transform&) = default;

        Transform(const glm::vec3& position) : Position(position) {}

    };

    struct MeshRenderer {

        AssetHandle<Mesh> MeshHandle;
        AssetHandle<Texture> TextureHandle;

        MeshRenderer() = default;
        MeshRenderer(const MeshRenderer&) = default;

        MeshRenderer(const AssetHandle<Mesh>& mesh) : MeshHandle(mesh) {}
        MeshRenderer(const AssetHandle<Mesh>& mesh, const AssetHandle<Texture>& texture) : MeshHandle(mesh), TextureHandle(texture) {}

    };

}