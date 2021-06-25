#include "SceneSerialization.h"

#include <yaml-cpp/yaml.h>

#include "Core/Assets.h"

#include "Scene/Component.h"
#include "Scene/Entity.h"

using namespace HarmonyEngine;

// YAML Overloads
namespace YAML {
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

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec) {
    out << YAML::Flow;
    out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec) {
    out << YAML::Flow;
    out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
    return out;
}

// ======================================================================================================
// Scene Serialization
// ======================================================================================================

SceneSerializer::SceneSerializer(Scene* scenePtr, const std::string& filepath) : m_ScenePtr(scenePtr), m_Filepath(filepath) {

}

template<typename ComponentType, typename SerializeFunction>
static void SerializeComponenetYAML(YAML::Emitter& out, Entity& entity, const char* componentName, SerializeFunction function) {
    if(entity.ContainsComponent<ComponentType>()) {
        out << YAML::Key << componentName;
        out << YAML::BeginMap; // Component Map

        auto& component = entity.GetComponent<ComponentType>();

        function(component);

        out << YAML::EndMap; // Component Map
    }
}

static void SerializeEntityYAML(YAML::Emitter& out, Entity& entity) {
    // Begin Entity Map
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << entity;
    
    SerializeComponenetYAML<TagComponent>(out, entity, "TagComponent", [&](TagComponent& component) {
        out << YAML::Key << "Name" << YAML::Value << component.Name;
    });

    SerializeComponenetYAML<TransformComponent>(out, entity, "TransformComponent", [&](TransformComponent& component) {
        out << YAML::Key << "Position" << YAML::Value << component.Transform.Position;
        out << YAML::Key << "Rotation" << YAML::Value << component.Transform.Rotation;
        out << YAML::Key << "Scale" << YAML::Value << component.Transform.Scale;
    });

    SerializeComponenetYAML<QuadRendererComponent>(out, entity, "QuadRendererComponent", [&](QuadRendererComponent& component) {
        out << YAML::Key << "Color" << YAML::Value << component.Color;
        if(component.TextureHandle.IsAssigned())
            out << YAML::Key << "Texture" << YAML::Value << component.TextureHandle->GetFilepath();
    });

    SerializeComponenetYAML<MeshRendererComponent>(out, entity, "MeshRendererComponent", [&](MeshRendererComponent& component) {
        out << YAML::Key << "Color" << YAML::Value << component.Color;
        if(component.TextureHandle.IsAssigned())
            out << YAML::Key << "Texture" << YAML::Value << component.TextureHandle->GetFilepath();
        if(component.MeshHandle.IsAssigned())
            out << YAML::Key << "Mesh" << YAML::Value << component.MeshHandle->Filepath;
    });

    // End Entity Map
    out << YAML::EndMap;
}

template<typename ComponenetType, typename DeserializeFunction>
static void DeserializeComponentYAML(YAML::detail::iterator_value& entityNode, Entity& entity, const char* componentName, DeserializeFunction function) {
    auto componentNode = entityNode[componentName];

    if(componentNode) {
        auto& component = entity.AddComponent<ComponenetType>();
        function(component, componentNode);
    }
}

static void DeserializeEntityYAML(YAML::detail::iterator_value& entityNode, Entity& entity, const std::string& name) {

    auto transformComponentNode = entityNode["TransformComponent"];

    if(transformComponentNode) {
        auto& transformComponent = entity.GetComponent<TransformComponent>();
        transformComponent.Transform.Position = transformComponentNode["Position"].as<glm::vec3>();
        transformComponent.Transform.Rotation = transformComponentNode["Rotation"].as<glm::vec3>();
        transformComponent.Transform.Scale = transformComponentNode["Scale"].as<glm::vec3>();
    } else {
        entity.RemoveComponenet<TransformComponent>();
    }

    DeserializeComponentYAML<QuadRendererComponent>(entityNode, entity, "QuadRendererComponent", [&](QuadRendererComponent& component, YAML::Node& node) {
        component.Color = node["Color"].as<glm::vec4>();

        if(auto textureNode = node["Texture"]) {
            auto textureFilepath = textureNode.as<std::string>();
            component.TextureHandle = AssetManager::QueueOrGetTexture(textureFilepath);;
        }
    });

    DeserializeComponentYAML<MeshRendererComponent>(entityNode, entity, "MeshRendererComponent", [&](MeshRendererComponent& component, YAML::Node& node) {
        component.Color = node["Color"].as<glm::vec4>();

        if(auto textureNode = node["Texture"]) {
            auto textureFilepath = textureNode.as<std::string>();
            component.TextureHandle = AssetManager::QueueOrGetTexture(textureFilepath);;
        }

        if(auto meshNode = node["Mesh"]) {
            auto meshFilepath = meshNode.as<std::string>();
            component.MeshHandle = AssetManager::QueueOrGetMesh(meshFilepath);
        }
    });
}

void SceneSerializer::SerializeYAML() {
    YAML::Emitter out;

    out << YAML::BeginMap; 
    out << YAML::Key << "Scene" << YAML::Value << m_ScenePtr->m_SceneName;
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    m_ScenePtr->m_Registry.each([&](auto entityID) {
        Entity entity = { m_ScenePtr, entityID };
        SerializeEntityYAML(out, entity);
    });

    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream outStream(m_Filepath);
    outStream << out.c_str();
}

void SceneSerializer::SerializerBinary() {
    HARMONY_ASSERT(true);
}

void SceneSerializer::DeserializeYAML() {

    std::ifstream in(m_Filepath);
    std::stringstream stream;
    stream << in.rdbuf();

    YAML::Node root = YAML::Load(stream.str());
    if(!root["Scene"]) {
        Log::Warn("Scene Deserialization File does not contian scene data!\n\tStatus: Returning");
        return;
    }

    std::string sceneName = root["Scene"].as<std::string>();
    Log::FormatInfo("Deserializing scene '%s'", sceneName.c_str());

    auto entities = root["Entities"];
    if(entities) {
        for(auto entity : entities) {
            std::string name;
            auto tagComponent = entity["TagComponent"];
            if(tagComponent)
                name = tagComponent["Name"].as<std::string>();
            else
                continue;

            Entity deserializedEntity = m_ScenePtr->CreateEntity(name);

            DeserializeEntityYAML(entity, deserializedEntity, name);
        }   
    }
}

void SceneSerializer::DeserializeBinary() {
    HARMONY_ASSERT(true);
}