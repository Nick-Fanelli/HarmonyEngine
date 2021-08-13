#include "Component.h"

#include "../Core/Assets.h"

using namespace HarmonyEngine;

// YAML Overloads
namespace YAML {
    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if(!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if(!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if(!node.IsSequence() || node.size() != 4) 
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

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

static void SerializeTexture(YAML::Emitter& out, AssetHandle<Texture>& textureHandle, const std::filesystem::path& mask) {
    if(textureHandle.IsAssigned())
        out << YAML::Key << "Texture" << YAML::Value << std::filesystem::relative(textureHandle->GetFilepath(), mask);
}

static void SerializeMesh(YAML::Emitter& out, AssetHandle<Mesh>& meshHandle, const std::filesystem::path& mask) {
    if(meshHandle.IsAssigned())
        out << YAML::Key << "Mesh" << YAML::Value << std::filesystem::relative(meshHandle->Filepath, mask);
}

static void DeserializeTexture(YAML::Node& node, AssetHandle<Texture>& textureHandle, const std::filesystem::path& mask) {
    if(auto textureNode = node["Texture"]) {
        auto textureFilepath = mask / (textureNode.as<std::string>());
        textureHandle = AssetManager::GetTexture(textureFilepath);
    }
}

static void DeserializeMesh(YAML::Node& node, AssetHandle<Mesh>& meshHandle, const std::filesystem::path& mask) {
    if(auto meshNode = node["Mesh"]) {
        auto meshFilepath = mask / (meshNode.as<std::string>());
        meshHandle = AssetManager::GetMesh(meshFilepath);
    }
}

// Tag Component
void TagComponent::Serialize(YAML::Emitter& out, const std::filesystem::path& mask) {
    out << YAML::Key << "Name" << YAML::Value << Name;
}

void TagComponent::Deserialize(YAML::Node& node, const std::filesystem::path& mask) {
    Name = node["Name"].as<std::string>();
}

// Transform Component
void TransformComponent::Serialize(YAML::Emitter& out, const std::filesystem::path& mask) {
    out << YAML::Key << "Position" << YAML::Value << Transform.Position;
    out << YAML::Key << "Rotation" << YAML::Value << Transform.Rotation;
    out << YAML::Key << "Scale" << YAML::Value << Transform.Scale;
}

void TransformComponent::Deserialize(YAML::Node& node, const std::filesystem::path& mask) {
    Transform.Position = node["Position"].as<glm::vec3>();
    Transform.Rotation = node["Rotation"].as<glm::vec3>();
    Transform.Scale = node["Scale"].as<glm::vec3>();            
}

// Mesh Renderer Component
void MeshRendererComponent::Serialize(YAML::Emitter& out, const std::filesystem::path& mask) {
    out << YAML::Key << "Color" << YAML::Value << Color;
    SerializeTexture(out, TextureHandle, mask);
    SerializeMesh(out, MeshHandle, mask);
}

void MeshRendererComponent::Deserialize(YAML::Node& node, const std::filesystem::path& mask) {
    Color = node["Color"].as<glm::vec4>();
    DeserializeTexture(node, TextureHandle, mask);
    DeserializeMesh(node, MeshHandle, mask);
}

// Quad Renderer Component
void QuadRendererComponent::Serialize(YAML::Emitter& out, const std::filesystem::path& mask) {
    out << YAML::Key << "Color" << YAML::Value << Color;
    SerializeTexture(out, TextureHandle, mask);
}

void QuadRendererComponent::Deserialize(YAML::Node& node, const std::filesystem::path& mask) {
    Color = node["Color"].as<glm::vec4>();
    DeserializeTexture(node, TextureHandle, mask);
}

// Sprite Renderer Component
void SpriteRendererComponent::Serialize(YAML::Emitter& out, const std::filesystem::path& mask) {
    out << YAML::Key << "Color" << YAML::Value << Color;
    SerializeTexture(out, TextureHandle, mask);
    out << YAML::Key << "TopLeftCoord" << YAML::Value << TopLeftCoord;
    out << YAML::Key << "BottomRightCoord" << YAML::Value << BottomRightCoord;
}

void SpriteRendererComponent::Deserialize(YAML::Node& node, const std::filesystem::path& mask) {
    Color = node["Color"].as<glm::vec4>();
    TopLeftCoord = node["TopLeftCoord"].as<glm::vec2>();
    BottomRightCoord = node["BottomRightCoord"].as<glm::vec2>();
    DeserializeTexture(node, TextureHandle, mask);
}

// Lua Script Component
// TODO: update the following two methods to use wide strings instead.
void LuaScriptComponent::Serialize(YAML::Emitter& out, const std::filesystem::path& mask) {
    // if(Script)
    //     out << YAML::Key << "ScriptPath" << YAML::Value << Script.GetFilepath().string();
}

void LuaScriptComponent::Deserialize(YAML::Node& node, const std::filesystem::path& mask) {
    // if(node["ScriptPath"]) {
    //     auto path = node["ScriptPath"].as<std::string>();
    //     if(!std::filesystem::is_empty(path) && std::filesystem::exists(path)) {
    //         Script = { path };
    //     }
    // }
}