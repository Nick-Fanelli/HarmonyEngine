#include "Scene.h"

#include "Entity.h"

#include "Component.h"

using namespace HarmonyEngine;

Entity Scene::CreateEntity() {
    Entity entity = Entity(this, m_Registry.create());
    entity.AddComponent<TagComponent>("Untitled Entity");
    entity.AddComponent<TransformComponent>();
    return entity;
}

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

void Scene::OnCreate() {
    if(m_GlobalLuaScript.IsAssigned())
        m_GlobalLuaScript.OnCreate();
}

void Scene::OnUpdate(float deltaTime) {
    m_GlobalLuaScript.OnUpdate(deltaTime);
}

void Scene::OnDestroy() {
    if(m_GlobalLuaScript.IsAssigned())
        m_GlobalLuaScript.OnDestroy();
}

void Scene::DeleteEntity(Entity& entity) {
    m_Registry.destroy(entity.GetEntityID());

    entity.m_ScenePtr = nullptr;
}

bool Scene::IsValid(const Entity& entity) {
    if(entity.m_ScenePtr != this)
        return false;
    return m_Registry.valid(entity.GetEntityID());    
} 

void Scene::ClearRegistry() {
    m_Registry.clear<>();
}