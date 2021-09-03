#include "Display.h"

#include "Input.h"

using namespace HarmonyEngine;

GLFWwindow* Display::s_Window = nullptr;

int Display::s_DisplayWidth = 1600;
int Display::s_DisplayHeight = 900;
int Display::s_CurrentFps = -1;

void Display::StartGameLoop() {

    Log::Info("Starting Game Loop...");

    float endTime, startTime = (float) glfwGetTime();
    float deltaTime = -1.0f;
    float accumulativeDeltatime = 0.0f;

    uint32_t frameCount = 0;

    while(!glfwWindowShouldClose(s_Window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(deltaTime >= 0) {
            SceneManager::Update(deltaTime);

            Input::Update();
        }

        glfwSwapBuffers(s_Window);
        glfwPollEvents();

        endTime = (float) glfwGetTime();
        deltaTime = endTime - startTime;
        startTime = endTime;

        accumulativeDeltatime += deltaTime;
        if(accumulativeDeltatime >= 1.0f) {
            s_CurrentFps = frameCount;
            frameCount = 0;
            accumulativeDeltatime = 0.0f;
        }

        frameCount++;
    }

}

void Display::CreateDisplay(const char* title, const glm::vec2& displaySize) {
        
    HARMONY_PROFILE_FUNCTION();

    Log::Info("Creating the display...");

    s_DisplayWidth = displaySize.x;    
    s_DisplayHeight = displaySize.y;    

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
    glfwSwapInterval(1); // Enable V-Sync
    // glfwSwapInterval(0); // Disable V-Sync

    int width, height;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    width = mode->width;
    height = mode->height;

    int newWidth = (int) ((float) (900 * width) / (float) height);
    int newHeight = (int) ((float) (1600 * height) / (float) width);

    glfwSetWindowSize(s_Window, newWidth, newHeight);

    if(s_Window == nullptr) {
        glfwTerminate();
        Log::Error("Could not create the GLFW Window");
        return;
    }

    int screenWidth, screenHeight;

    glfwGetFramebufferSize(s_Window, &screenWidth, &screenHeight);

    glfwMakeContextCurrent(s_Window);

    // Handle Input
    glfwSetKeyCallback(s_Window, Input::KeyCallback);
    glfwSetMouseButtonCallback(s_Window, Input::MouseButtonCallback);
    glfwSetCursorPosCallback(s_Window, Input::MousePositionCallback);
    glfwSetScrollCallback(s_Window, Input::MouseScrollCallback);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        Log::Error("Failed to initialize GLAD");
        return;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);

    glfwSwapInterval(1); // Enable V-Sync

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