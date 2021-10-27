#pragma once

#include "harmonypch.h"

#include "Display.h"

namespace HarmonyEngine {

    class Application {

        friend class Display;

    public:
        Application() = default;
        Application(const std::string& applicationName);

    public:
        void StartApplication();

    private:
        void OnUpdate(float deltaTime);
        void CleanUp();

    private:
        std::string m_ApplicationName = "Harmony Application";
        std::shared_ptr<Display> m_Display;
    };

}