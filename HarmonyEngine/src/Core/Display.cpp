#include "Display.h"

#include "Input.h"

using namespace HarmonyEngine;

GLFWwindow* Display::s_Window = nullptr;

int Display::s_DisplayWidth = 1280;
int Display::s_DisplayHeight = 720;
int Display::s_CurrentFps = -1;

void Display::StartGameLoop(Scene* scenePtr) {
    Log::Info("Starting Game Loop...");

    SceneManager::SetActiveScene(scenePtr);

    float endTime, startTime = (float) glfwGetTime();
    float deltaTime = -1.0f;

    double lastTime = glfwGetTime();
    double currentTime;
    int frameCount = 0;

    while(!glfwWindowShouldClose(s_Window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(deltaTime >= 0) {
            SceneManager::Update(deltaTime);

            Input::Update();
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

void Display::CreateDisplay(const char* title) {
        
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

    // Handle Input
    glfwSetKeyCallback(s_Window, Input::KeyCallback);
    glfwSetMouseButtonCallback(s_Window, Input::MouseButtonCallback);
    glfwSetCursorPosCallback(s_Window, Input::MousePositionCallback);
    glfwSetScrollCallback(s_Window, Input::MouseScrollCallback);

    if(glewInit() != GLEW_OK) {
        Log::Error("Could not initialize GLEW");
        return;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);

    // glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Log::Success("Created the GLFW Display!");

    glfwFocusWindow(s_Window);
}

void Display::CleanUp() {
    Log::Info("Cleaning up the program...");

    SceneManager::OnDestroy();

    glfwDestroyWindow(s_Window);
    glfwTerminate();

    Log::Success("Progam successfully exited!");
}

void Display::CloseDisplay() {
    
}