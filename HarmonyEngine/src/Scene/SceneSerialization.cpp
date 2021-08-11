#include "SceneSerialization.h"

#include <yaml-cpp/yaml.h>

#include "Core/Assets.h"

#include "Scene/Component.h"
#include "Scene/Entity.h"

using namespace HarmonyEngine;

static const char* TagComponentID = "TagComponent";
static const char* TransformComponentID = "TransformComponent";
static const char* QuadRendererComponentID = "QuadRendererComponent";
static const char* MeshRendererComponentID = "MeshRendererComponent";
static const char* SpriteRendererComponentID = "SpriteRendererComponent";
static const char* LuaScriptComponentID = "LuaScriptComponent";

// ======================================================================================================
// Scene Serialization
// ======================================================================================================

SceneSerializer::SceneSerializer(Scene* scenePtr, const std::string& filepath) : m_ScenePtr(scenePtr), m_Filepath(filepath) {

}

template<typename ComponentType>
static void SerializeComponenetYAML(YAML::Emitter& out, Entity& entity, const char* componentName) {
    if(entity.ContainsComponent<ComponentType>()) {
        out << YAML::Key << componentName;
        out << YAML::BeginMap; // Component Map

        auto& rawComponent = entity.GetComponent<ComponentType>();

        if(std::is_base_of<Component, ComponentType>()) {
            auto component = dynamic_cast<Component*>(&rawComponent);
            component->Serialize(out);
        }

        out << YAML::EndMap; // Component Map   
    }
}

static void SerializeEntityYAML(YAML::Emitter& out, Entity& entity) {
    // Begin Entity Map
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << (uint32_t) entity.GetEntityID();
    
    SerializeComponenetYAML<TagComponent>(out, entity, TagComponentID);
    SerializeComponenetYAML<TransformComponent>(out, entity, TransformComponentID);
    SerializeComponenetYAML<QuadRendererComponent>(out, entity, QuadRendererComponentID);
    SerializeComponenetYAML<MeshRendererComponent>(out, entity, MeshRendererComponentID);
    SerializeComponenetYAML<SpriteRendererComponent>(out, entity, SpriteRendererComponentID);
    SerializeComponenetYAML<LuaScriptComponent>(out, entity, LuaScriptComponentID);

    // End Entity Map
    out << YAML::EndMap;
}

template<typename ComponenetType>
static bool DeserializeComponentYAML(YAML::detail::iterator_value& entityNode, Entity& entity, const char* componentName) {
    auto componentNode = entityNode[componentName];

    if(componentNode) {
        auto& rawComponent = entity.ContainsComponent<ComponenetType>() ? entity.GetComponent<ComponenetType>() : entity.AddComponent<ComponenetType>();

        if(std::is_base_of<Component, ComponenetType>()) {
            auto component = dynamic_cast<Component*>(&rawComponent);
            component->Deserialize(componentNode);
            return true;
        }

    }

    return false;
}

static void DeserializeEntityYAML(YAML::detail::iterator_value& entityNode, Entity& entity, const std::string& name) {

    if(!DeserializeComponentYAML<TransformComponent>(entityNode, entity, TransformComponentID)) {
        entity.RemoveComponent<TransformComponent>();
        return;
    }

    DeserializeComponentYAML<QuadRendererComponent>(entityNode, entity, QuadRendererComponentID);
    DeserializeComponentYAML<SpriteRendererComponent>(entityNode, entity, SpriteRendererComponentID);
    DeserializeComponentYAML<MeshRendererComponent>(entityNode, entity, MeshRendererComponentID);
    DeserializeComponentYAML<QuadRendererComponent>(entityNode, entity, QuadRendererComponentID);
    DeserializeComponentYAML<LuaScriptComponent>(entityNode, entity, LuaScriptComponentID);
}

void SceneSerializer::SerializeYAML() {
    YAML::Emitter out;

    out << YAML::BeginMap; 
    out << YAML::Key << "Scene" << YAML::Value << m_ScenePtr->m_SceneName;
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    std::vector<Entity> entities;

    m_ScenePtr->m_Registry.each([&](auto entityID) {
        entities.emplace_back(m_ScenePtr, entityID);
    });

    for(uint32_t i = entities.size(); i --> 0;) {
        SerializeEntityYAML(out, entities[i]);
    }

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
    m_ScenePtr->m_SceneName = sceneName;
    Log::FormatInfo("Deserializing scene '%s'", sceneName.c_str());

    m_ScenePtr->m_Registry.clear();

    auto entities = root["Entities"];
    if(entities) {
        for(auto entity : entities) {
            std::string name;
            auto tagComponent = entity[TagComponentID];
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