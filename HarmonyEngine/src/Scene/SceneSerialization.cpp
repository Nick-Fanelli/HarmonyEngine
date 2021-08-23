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
static const char* PointLightComponentID = "PointLightComponent";
static const char* CameraComponentID = "CameraComponent";

// ======================================================================================================
// Scene Serialization
// ======================================================================================================

SceneSerializer::SceneSerializer(Scene* scenePtr, const std::filesystem::path& filepath) : m_ScenePtr(scenePtr), m_Filepath(filepath) {

}

template<typename ComponentType>
static void SerializeComponenetYAML(YAML::Emitter& out, Entity& entity, const char* componentName, const std::filesystem::path& mask) {
    if(entity.ContainsComponent<ComponentType>()) {
        out << YAML::Key << componentName;
        out << YAML::BeginMap; // Component Map

        auto& rawComponent = entity.GetComponent<ComponentType>();

        if(std::is_base_of<Component, ComponentType>()) {
            auto component = dynamic_cast<Component*>(&rawComponent);
            component->Serialize(out, mask);
        }

        out << YAML::EndMap; // Component Map   
    }
}

static void SerializeEntityYAML(YAML::Emitter& out, Entity& entity, const std::filesystem::path& mask) {
    // Begin Entity Map
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << (uint32_t) entity.GetEntityID();
    
    SerializeComponenetYAML<TagComponent>(out, entity, TagComponentID, mask);
    SerializeComponenetYAML<TransformComponent>(out, entity, TransformComponentID, mask);
    SerializeComponenetYAML<QuadRendererComponent>(out, entity, QuadRendererComponentID, mask);
    SerializeComponenetYAML<MeshRendererComponent>(out, entity, MeshRendererComponentID, mask);
    SerializeComponenetYAML<SpriteRendererComponent>(out, entity, SpriteRendererComponentID, mask);
    SerializeComponenetYAML<LuaScriptComponent>(out, entity, LuaScriptComponentID, mask);
    SerializeComponenetYAML<PointLightComponent>(out, entity, PointLightComponentID, mask);
    SerializeComponenetYAML<CameraComponent>(out, entity, CameraComponentID, mask);

    // End Entity Map
    out << YAML::EndMap;
}

template<typename ComponenetType>
static bool DeserializeComponentYAML(YAML::detail::iterator_value& entityNode, Entity& entity, const char* componentName, const std::filesystem::path& mask) {
    auto componentNode = entityNode[componentName];

    if(componentNode) {
        auto& rawComponent = entity.ContainsComponent<ComponenetType>() ? entity.GetComponent<ComponenetType>() : entity.AddComponent<ComponenetType>();

        if(std::is_base_of<Component, ComponenetType>()) {
            auto component = dynamic_cast<Component*>(&rawComponent);
            component->Deserialize(componentNode, mask);
            return true;
        }

    }

    return false;
}

static void DeserializeEntityYAML(YAML::detail::iterator_value& entityNode, Entity& entity, const std::string& name, const std::filesystem::path& mask) {

    if(!DeserializeComponentYAML<TransformComponent>(entityNode, entity, TransformComponentID, mask)) {
        entity.RemoveComponent<TransformComponent>();
        return;
    }

    DeserializeComponentYAML<QuadRendererComponent>(entityNode, entity, QuadRendererComponentID, mask);
    DeserializeComponentYAML<SpriteRendererComponent>(entityNode, entity, SpriteRendererComponentID, mask);
    DeserializeComponentYAML<MeshRendererComponent>(entityNode, entity, MeshRendererComponentID, mask);
    DeserializeComponentYAML<QuadRendererComponent>(entityNode, entity, QuadRendererComponentID, mask);
    DeserializeComponentYAML<LuaScriptComponent>(entityNode, entity, LuaScriptComponentID, mask);
    DeserializeComponentYAML<PointLightComponent>(entityNode, entity, PointLightComponentID, mask);
    DeserializeComponentYAML<CameraComponent>(entityNode, entity, CameraComponentID, mask);
}

void SceneSerializer::SerializeYAML(const std::filesystem::path& mask) {

    Log::FormatInfo("Serializing Scene '%s'", m_ScenePtr->m_SceneName.c_str());

    YAML::Emitter out;

    out << YAML::BeginMap; 
    out << YAML::Key << "Scene" << YAML::Value << m_ScenePtr->m_SceneName;
    out << YAML::Key << "AmbientIntensity" << YAML::Value << m_ScenePtr->m_AmbientIntensity;
    
    // TODO: Use wstring instead of string
    if(m_ScenePtr->m_GlobalLuaScript.IsAssigned())
        out << YAML::Key << "GlobalScript" << YAML::Value << std::filesystem::relative(m_ScenePtr->m_GlobalLuaScript.GetFilepath().string(), mask);

    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    std::vector<Entity> entities;

    m_ScenePtr->ForEachEntity([&](auto entityID) {
        entities.emplace_back(m_ScenePtr, entityID);
    });

    for(uint32_t i = entities.size(); i --> 0;) {
        SerializeEntityYAML(out, entities[i], mask);
    }

    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream outStream(m_Filepath);
    outStream << out.c_str();

    Log::Success("Successfully Serialized Scene!");
}

void SceneSerializer::SerializerBinary() {
    HARMONY_ASSERT(true);
}

void SceneSerializer::DeserializeYAML(const std::filesystem::path& mask) {

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

    if(root["AmbientIntensity"])
        m_ScenePtr->m_AmbientIntensity = root["AmbientIntensity"].as<float>();

    if(root["GlobalScript"])
        m_ScenePtr->m_GlobalLuaScript.LoadGlobalScript(mask / root["GlobalScript"].as<std::string>());

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

            DeserializeEntityYAML(entity, deserializedEntity, name, mask);
        }   
    }

    Log::Success("Successfully Deserialized Scene!");
}

void SceneSerializer::DeserializeBinary() {
    HARMONY_ASSERT(true);
}