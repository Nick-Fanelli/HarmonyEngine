#include "Scene.h"

#include "Entity.h"

using namespace HarmonyEngine;

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity = Entity(this, m_Registry.create());
    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<TransformComponent>();
    return entity;
}

Entity Scene::CreateEntity(const std::string& name, const Transform& transform) {
    Entity entity = Entity(this, m_Registry.create());
    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<TransformComponent>(transform);
    return entity;
}