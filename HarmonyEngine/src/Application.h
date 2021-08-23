#pragma once

#include "harmonypch.h"

#include "Scene/Scene.h"

namespace HarmonyEngine {

    class Application {

    public:
        Application();
        Application(const std::string& applicationName);

        Application(const Application&) = delete;
        
        ~Application();

        void StartApplication(Scene& scene);

    private:
        std::string m_ApplicationName = "Harmony Project";
        
        bool m_ShouldEndShutdown = false;
    };

}