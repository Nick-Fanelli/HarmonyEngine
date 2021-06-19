#include "Scene.h"

#include "Entity.h"

using namespace HarmonyEngine;

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity = Entity(this, m_Registry.create());
    entity.AddComponent<EntityTag>(name);
    entity.AddComponent<Transform>();
    return entity;
}

Entity Scene::CreateEntity(const std::string& name, const glm::vec3& position) {
    Entity entity = Entity(this, m_Registry.create());
    entity.AddComponent<EntityTag>(name);
    entity.AddComponent<Transform>(position);
    return entity;
}