#pragma once

#include "harmonypch.h"

#include <entt/entt.hpp>

#include "Component.h"
#include "Scene.h"

namespace HarmonyEngine {

    class Entity {

        friend class Scene;

    private:
        Scene* m_ScenePtr = nullptr;
        entt::entity m_EntityID;

    public:
        Entity() = default;
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
                HARMONY_ASSERT_MESSAGE(true, "Entity does not contain the requested component!")
            }

            return m_ScenePtr->GetRegistry().get<T>(m_EntityID);
        }

        template<typename T>
        void RemoveComponent() {
            if(!ContainsComponent<T>()) {
                Log::Warn("Entity does not contain the requested component to be removed!\n\tStatus: Returning");
                return;
            }

            m_ScenePtr->GetRegistry().remove<T>(m_EntityID);
        }

        bool IsCreated() { return m_ScenePtr != nullptr; }

        operator uint32_t() const { return (uint32_t) m_EntityID; }
        operator bool() const { return m_ScenePtr != nullptr; }

        bool operator==(const Entity& other) const { return m_EntityID == other.m_EntityID && m_ScenePtr == other.m_ScenePtr; }
        bool operator!=(const Entity& other) const { return !operator==(other); }

    };

}