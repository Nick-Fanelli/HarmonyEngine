#pragma once

#include "harmonypch.h"

namespace HarmonyEngine {

    class Application;

    class Display {

    public:
        Display() = default;
        Display(const std::string& title, const glm::vec2& displaySize = { 1600, 900 });

        void CreateDisplay();
        void StartGameLoop(Application& application);

        uint32_t GetCurrentFPS() const { return m_CurrentFps; }

    private:
        void CleanUp();

    private:
        std::string m_DisplayTitle = "Harmony Application";
        glm::vec2 m_InitialDisplaySize = { 1600, 900 };

        GLFWwindow* m_WindowPtr = nullptr;

        uint32_t m_CurrentFps = 0;
    };

}