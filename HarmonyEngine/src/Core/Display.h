#pragma once

#include "harmonypch.h"
#include "imguipch.h"

#include <glad/glad.h>
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

        static void CreateDisplay(const char* title, const glm::vec2& displaySize = { 1600, 900 });
        static void CleanUp();
        static void StartGameLoop();
        static void CloseDisplay();

        static void GetWidth(int* result) { glfwGetWindowSize(s_Window, result, nullptr); }
        static void GetHeight(int* result) { glfwGetWindowSize(s_Window, nullptr, result); }

        static int GetWidth() { 
            int width;
            glfwGetWindowSize(s_Window, &width, nullptr);
            return width;
        }

        static int GetHeight() { 
            int height;
            glfwGetWindowSize(s_Window, nullptr, &height);
            return height;
        }

        static glm::vec2 GetWindowSize() {
            int width, height;
            glfwGetWindowSize(s_Window, &width, &height);
            return { width, height };
        }

        static glm::vec2 GetWindowPosition() {
            int x, y;
            glfwGetWindowPos(s_Window, &x, &y);
            return { x, y };
        }

        static void SetVSync(bool value) {
            if(s_Window == nullptr) {
                Log::Error("Can not set V-Sync before the display is created!\n\tStatus Returning!");
                return;
            }

            glfwSwapInterval(value);
        }

        static const float GetAspectWidth() { return DisplayAspect.x; }
        static const float GetAspectHeight() { return DisplayAspect.y; }
        static const glm::vec2& GetRawDisplayAspect() { return DisplayAspect; }

        static int GetFps() { return s_CurrentFps; }

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

        static ImVec2 GetImGuiSize() {
            int x, y;
            glfwGetWindowSize(s_Window, &x, &y);

            return { (float) x, (float) y };
        }

        static GLFWwindow* GetWindowPtr() { return s_Window; }

    };

}