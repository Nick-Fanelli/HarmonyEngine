#pragma once

#include "harmonypch.h"

#include <entt/entt.hpp>

#include "Scene.h"
#include "Component.h"

namespace HarmonyEngine {

    class Entity {

    private:
        Scene* m_ScenePtr;
        entt::entity m_EntityID;

    public:
        Entity() = default;
        Entity(const Entity&) = default;

        Entity(Scene* scene) : m_ScenePtr(scene), m_EntityID(scene->CreateEntity()) {}
        Entity(Scene* scene, entt::entity entityID) : m_ScenePtr(scene), m_EntityID(entityID) {}

        const Scene* GetScenePtr() const { return m_ScenePtr; }
        const entt::entity& GetEntityID() const { return m_EntityID; }
        entt::entity& GetEntityID() { return m_EntityID; }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return m_ScenePtr->GetRegistry().emplace<T>(m_EntityID, std::forward<Args>(args)...);
        }

        template<typename T>
        bool ContainsComponent() {
            return m_ScenePtr->GetRegistry().has<T>(m_EntityID);
        }

        template<typename T>
        T& GetComponent() {
            if(!ContainsComponent<T>()) {
                Log::Warn("Entity does not contain the requested component!");
            }

            return m_ScenePtr->GetRegistry().get<T>(m_EntityID);
        }

        template<typename T>
        void RemoveComponenet() {
            if(!ContainsComponent<T>()) {
                Log::Warn("Entity does not contain the requested component to be removed!");
            }

            m_ScenePtr->GetRegistry().remove<T>(m_EntityID);
        }

    };

}