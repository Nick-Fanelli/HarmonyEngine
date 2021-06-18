#pragma once

#include "harmonypch.h"
#include "imguipch.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

namespace HarmonyEngine {

    class Display {

        static constexpr glm::vec2 DisplayAspect = { 1920.0f, 1080.0f };

        static GLFWwindow* s_Window;

        static int s_DisplayWidth;
        static int s_DisplayHeight;

        static int s_CurrentFps;

    public:

        static void CreateDisplay(const char* title);
        static void CleanUp();
        static void StartGameLoop(Scene* scenePtr = nullptr);
        static void CloseDisplay();

        static void GetWidth(int* result) { glfwGetWindowSize(s_Window, result, nullptr); }
        static void GetHeight(int* result) { glfwGetWindowSize(s_Window, nullptr, result); }

        static const float GetAspectWidth() { return DisplayAspect.x; }
        static const float GetAspectHeight() { return DisplayAspect.y; }
        static const glm::vec2& GetRawDisplayAspect() { return DisplayAspect; }

        static const float GetAspectRatio() {
            static float aspectRatio = DisplayAspect.x / DisplayAspect.y;
            return aspectRatio;
        }
 
        static bool IsRunning() { return s_Window != nullptr; }

        static void GetImGuiSize(ImVec2* outVec2) {
            int x, y;
            glfwGetWindowSize(s_Window, &x, &y);

            outVec2->x = (float) x;
            outVec2->y = (float) y;
        }

        static GLFWwindow* GetWindowPtr() { return s_Window; }

    };

}