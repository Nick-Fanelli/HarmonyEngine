#pragma once

#include "harmonypch.h"

namespace HarmonyEngine {

    class Application {

    public:
        static Application CreateApplication(const std::string& message);

    public:
        void StartApplication();

    private:
        Application() = default;
        Application(const std::string& applicationName) : m_ApplicationName(applicationName) {}

    private:
        std::string m_ApplicationName = "Harmony Application";
    };

}