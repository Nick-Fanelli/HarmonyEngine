#pragma once

#include "harmonypch.h"

#include <yaml-cpp/yaml.h>

#include "../Render/Transform.h"
#include "../Render/Renderer.h"

namespace HarmonyEngine {

    class Component {

    public:
        virtual void Serialize(YAML::Emitter& out) {}
        virtual void Deserialize(YAML::Node& node) {}

        virtual ~Component() {}
    };

    struct TagComponent : public Component {

        std::string Name;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        TagComponent(const std::string& name) : Name(name) {}

        void Serialize(YAML::Emitter& out) override;
        void Deserialize(YAML::Node& node) override;
    };

    struct TransformComponent : public Component {

        Transform Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const struct Transform& transform) : Transform(transform) {}

        void Serialize(YAML::Emitter& out) override;
        void Deserialize(YAML::Node& node) override;
    };

    struct MeshRendererComponent : public Component {

        AssetHandle<Mesh> MeshHandle;
        AssetHandle<Texture> TextureHandle;
        glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent&) = default;

        MeshRendererComponent(const AssetHandle<Mesh>& mesh) : MeshHandle(mesh) {}
        MeshRendererComponent(const AssetHandle<Mesh>& mesh, const AssetHandle<Texture>& texture) : MeshHandle(mesh), TextureHandle(texture) {}
        MeshRendererComponent(const AssetHandle<Mesh>& mesh, const AssetHandle<Texture>& texture, const glm::vec4& color) : MeshHandle(mesh), TextureHandle(texture), Color(color) {}

        void Serialize(YAML::Emitter& out) override;
        void Deserialize(YAML::Node& node) override;

    };

    struct QuadRendererComponent : public Component {

        glm::vec4 Color;
        AssetHandle<Texture> TextureHandle;

        QuadRendererComponent() : Color(1.0f) {};

        QuadRendererComponent(const QuadRendererComponent&) = default;

        QuadRendererComponent(const glm::vec4& color) : Color(color) {}
        QuadRendererComponent(const glm::vec4& color, const AssetHandle<Texture>& texture) : Color(color), TextureHandle(texture) {}

        void Serialize(YAML::Emitter& out) override;
        void Deserialize(YAML::Node& node) override;
    };

    struct SpriteRendererComponent : public Component {

        glm::vec4 Color;
        AssetHandle<Texture> TextureHandle;
        glm::vec2 TopLeftCoord = { 0.0f, 0.0f };
        glm::vec2 BottomRightCoord = { 1.0f, 1.0f }; 

        SpriteRendererComponent() : Color(1.0f) {};
        SpriteRendererComponent(const SpriteRendererComponent&) = default;

        SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
        SpriteRendererComponent(const glm::vec4& color, AssetHandle<Texture> textureHandle) : Color(color), TextureHandle(textureHandle) {}
        SpriteRendererComponent(const glm::vec4& color, AssetHandle<Texture> textureHandle, const glm::vec2& topLeft, const glm::vec2& bottomRight) 
            : Color(color), TextureHandle(textureHandle), TopLeftCoord(topLeft), BottomRightCoord(bottomRight) {}

        void Serialize(YAML::Emitter& out) override;
        void Deserialize(YAML::Node& node) override;
        
    };
}