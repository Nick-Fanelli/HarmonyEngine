#pragma once

#include "harmonypch.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

namespace HarmonyEngine {

    class Display {

        static GLFWwindow* s_Window;

        static int s_DisplayWidth;
        static int s_DisplayHeight;

        static int s_CurrentFps;

    public:

        static void CreateDisplay(const char* title);
        static void CleanUp();
        static void StartGameLoop(Scene* scenePtr = nullptr);

        static void GetWidth(int* result) { glfwGetWindowSize(s_Window, result, nullptr); }
        static void GetHeight(int* result) { glfwGetWindowSize(s_Window, nullptr, result); }

        static bool IsRunning() { return s_Window != nullptr; }

        static void CloseDisplay();
    };

}