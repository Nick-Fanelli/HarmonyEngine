#include "Display.h"

#include "Application.h"

using namespace HarmonyEngine;

Display::Display(const std::string& title, const glm::vec2& displaySize) 
: m_DisplayTitle(title), m_InitialDisplaySize(displaySize) {

}

void Display::CreateDisplay() { 

    HARMONY_PROFILE_FUNCTION();

    Log::Trace("Initializing GLFW...");
    if(!glfwInit()) {
        // TODO: Fatal Error
        Log::Error("Could not initialize GLFW!");
        return;
    }

    Log::Trace("Intialized GLFW!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    Log::Trace("Create the GLFW Window...");
    m_WindowPtr = glfwCreateWindow(m_InitialDisplaySize.x, m_InitialDisplaySize.y, m_DisplayTitle.c_str(), nullptr, nullptr);
    glfwSwapInterval(1); // Enable V-Sync 0 to disable

    if(m_WindowPtr == nullptr) {
        // TODO: Fatal Error
        Log::Error("Could not create the GLFW window!");
        glfwTerminate();
        return;
    }
    
    Log::Trace("Successfully Created the GLFW Window!");

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(m_WindowPtr, &screenWidth, &screenHeight);
    glfwMakeContextCurrent(m_WindowPtr);

    // TODO: Handle Input

    glfwWindowHint(GLFW_SAMPLES, 4);

    Log::Trace("Initializing GLAD...");
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        // TODO: Fatal Error
        Log::Error("Failed to initialize GLAD!");
        return;
    }
    Log::Trace("Initalized GLAD!");

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    Log::Trace("Successfully Created the GLFW Display!");

    glfwFocusWindow(m_WindowPtr);

    Log::Info("Created Window");
}

void Display::StartGameLoop(Application& application) {

    Log::Trace("Starting Game Loop...");

    float endTime, startTime = (float) glfwGetTime();
    float deltaTime = 0.0f;
    float accumulativeDeltaTime = 0.0f;

    uint32_t frameCount = 0;

    while(!glfwWindowShouldClose(m_WindowPtr)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(deltaTime >= 0) {
            application.OnUpdate(deltaTime);

            // TODO: Update Input
        }

        glfwSwapBuffers(m_WindowPtr);
        glfwPollEvents();

        endTime = (float) glfwGetTime();
        deltaTime = endTime - startTime;
        startTime = endTime;

        accumulativeDeltaTime += deltaTime;
        if(accumulativeDeltaTime >= 1.0f) {
            m_CurrentFps = frameCount;
            frameCount = 0;
            accumulativeDeltaTime = 0.0f;
        }

        frameCount++;
    }

    Log::Trace("Exiting Game Loop...");

    CleanUp();

}

void Display::CleanUp() {
    Log::Trace("Cleaning Up Display...");

    glfwDestroyWindow(m_WindowPtr);
    glfwTerminate();

    Log::Trace("Display Cleaned Up!");
}