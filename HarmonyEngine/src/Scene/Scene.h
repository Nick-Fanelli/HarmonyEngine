#pragma once

#include "harmonypch.h"

// #include "Component.h"

#include "../Render/Transform.h"

#include <entt/entt.hpp>

namespace HarmonyEngine {

    class SceneSerializer;
    class Entity;

    struct TransformComponent;

    class Scene {

        friend class SceneSerializer;
        friend class Entity;
        
    protected:
        std::string m_SceneName;
        entt::registry m_Registry;

    public:
        const std::string& GetSceneName() const { return m_SceneName; }

        void SetSceneName(const std::string& name) { m_SceneName = name; }

        Scene() = default;
        Scene(const std::string& sceneName) : m_SceneName(std::move(sceneName)) {}

        virtual void OnCreate() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnDestroy() { m_Registry.clear<>(); }

        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const std::string& name, const Transform& transform);

        void DeleteEntity(Entity& entity);

        template<typename ComponentType, typename Function>
        void ForEachEntityWithTransform(Function function) {
            auto group = m_Registry.group<ComponentType>(entt::get<TransformComponent>);
            for(auto& entity : group) {
                auto[component, transform] = group.template get<ComponentType, TransformComponent>(entity);
                function(transform, component);
            }            
        }

        template<typename Function>
        void ForEachEntity(Function function) {
            m_Registry.each([&](auto entityID) {
                function(entityID);
            });
        }

        void ClearRegistry();

    };

}