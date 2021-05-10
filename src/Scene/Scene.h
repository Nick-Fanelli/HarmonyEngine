#pragma once

#include "harmonypch.h"

namespace HarmonyEngine {

    class Scene {
        
    protected:
        const std::string m_SceneName;

    public:
        const std::string& GetSceneName() const { return m_SceneName; }

        Scene() = default;
        Scene(const Scene&) = default;
        Scene(const std::string& sceneName) : m_SceneName(std::move(sceneName)) {}

        virtual void OnCreate() {}
        virtual void Update(float deltaTime) {}
        virtual void OnDestroy() {}

    };

}