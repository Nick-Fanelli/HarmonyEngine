#pragma once

#include "harmonypch.h"

#include <yaml-cpp/yaml.h>

#include "../Render/Transform.h"
#include "../Render/Renderer.h"

namespace HarmonyEngine {

    static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        return out;
    }

    static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return out;
    }

    static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
        return out;
    }

    class Component {

    protected:
        void SerializeTexture(YAML::Emitter& out, AssetHandle<Texture>& textureHandle) {
            if(textureHandle.IsAssigned())
                out << YAML::Key << "Texture" << YAML::Value << textureHandle->GetFilepath();
        }

        void SerializeMesh(YAML::Emitter& out, AssetHandle<Mesh>& meshHandle) {
            if(meshHandle.IsAssigned())
                out << YAML::Key << "Mesh" << YAML::Value << meshHandle->Filepath;
        }

    public:
        virtual void Serialize(YAML::Emitter& out) {}
        virtual ~Component() {}
    };

    struct TagComponent : public Component {

        std::string Name;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        TagComponent(const std::string& name) : Name(name) {}

        void Serialize(YAML::Emitter& out) override {
            out << YAML::Key << "Name" << YAML::Value << Name;
        }
    };

    struct TransformComponent : public Component {

        Transform Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const struct Transform& transform) : Transform(transform) {}

        void Serialize(YAML::Emitter& out) override {
            out << YAML::Key << "Position" << YAML::Value << Transform.Position;
            out << YAML::Key << "Rotation" << YAML::Value << Transform.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << Transform.Scale;
        }
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

        void Serialize(YAML::Emitter& out) override {
            out << YAML::Key << "Color" << YAML::Value << Color;
            SerializeTexture(out, TextureHandle);
            SerializeMesh(out, MeshHandle);
        }

    };

    struct QuadRendererComponent : public Component {

        glm::vec4 Color;
        AssetHandle<Texture> TextureHandle;

        QuadRendererComponent() : Color(1.0f) {};

        QuadRendererComponent(const QuadRendererComponent&) = default;

        QuadRendererComponent(const glm::vec4& color) : Color(color) {}
        QuadRendererComponent(const glm::vec4& color, const AssetHandle<Texture>& texture) : Color(color), TextureHandle(texture) {}

        void Serialize(YAML::Emitter& out) override {
            out << YAML::Key << "Color" << YAML::Value << Color;
            SerializeTexture(out, TextureHandle);
        }
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

        void Serialize(YAML::Emitter& out) override {
        out << YAML::Key << "Color" << YAML::Value << Color;
        SerializeTexture(out, TextureHandle);
        out << YAML::Key << "TopLeftCoord" << YAML::Value << TopLeftCoord;
        out << YAML::Key << "BottomRightCoord" << YAML::Value << BottomRightCoord;
        }
    };
}