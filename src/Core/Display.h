#include "harmonypch.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLUT/glut.h>
#define GLFW_DLL

#include <GLFW/glfw3.h>

namespace HarmonyEngine::Display {

    static GLFWwindow* s_Window = nullptr;

    static int s_DisplayWidth = 1280;
    static int s_DisplayHeight = 720;

    static int s_CurrentFps = -1;

    void CreateDisplay(const char* title) {
        
        Log::Info("Creating the display...");

        if(!glfwInit()) {
            Log::Error("Could not initialize GLFW!");
            return;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        s_Window = glfwCreateWindow(s_DisplayWidth, s_DisplayHeight, title, nullptr, nullptr);

        if(s_Window == nullptr) {
            glfwTerminate();
            Log::Error("Could not create the GLFW Window");
            return;
        }

        int screenWidth, screenHeight;

        glfwGetFramebufferSize(s_Window, &screenWidth, &screenHeight);

        glfwMakeContextCurrent(s_Window);
        glewExperimental = true;

        if(glewInit() != GLEW_OK) {
            Log::Error("Could not initialize GLEW");
            return;
        }

        glViewport(0, 0, screenWidth, screenHeight);

        Log::Success("Created the GLFW Display!");

        glfwFocusWindow(s_Window);
    }

    static void CleanUp() {
        Log::Info("Cleaning up the program...");

        glfwDestroyWindow(s_Window);
        glfwTerminate();

        Log::Success("Progam successfully exited!");
    }

    void StartGameLoop() {
        Log::Info("Starting Game Loop...");

        float endTime, startTime = (float) glfwGetTime();
        float deltaTime = -1.0f;

        double lastTime = glfwGetTime();
        double currentTime;
        int frameCount = 0;

        while(!glfwWindowShouldClose(s_Window)) {

            glClearColor(0, 0, 0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if(deltaTime >= 0) {
                // Update the game
            }

            glfwSwapBuffers(s_Window);
            glfwPollEvents();

            currentTime = glfwGetTime();
            frameCount++;

            if(currentTime - lastTime >= 1.0f) {
                s_CurrentFps = 1000 / frameCount;
                frameCount = 0;
                lastTime = glfwGetTime();
            }

            endTime = (float) glfwGetTime();
            deltaTime = endTime - startTime;
            startTime = endTime;

        }

        CleanUp();
    
    }

    void CloseDisplay() {

    }

}