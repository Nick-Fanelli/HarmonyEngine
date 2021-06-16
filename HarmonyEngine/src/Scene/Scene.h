#pragma once

#include "harmonypch.h"

#include <entt/entt.hpp>

namespace HarmonyEngine {

    class Scene {
        
    protected:
        std::string m_SceneName;
        entt::registry m_Registry;

    public:
        const std::string& GetSceneName() const { return m_SceneName; }

        Scene() = default;
        Scene(const std::string& sceneName) : m_SceneName(std::move(sceneName)) {}

        virtual void OnCreate() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnDestroy() { m_Registry.clear<>(); }

        entt::entity CreateEntity() {
            return m_Registry.create();
        }

        const entt::registry& GetRegistry() const { return m_Registry; }
        entt::registry& GetRegistry() { return m_Registry; }

    };

}