#include "Application.h"

#include "Core/Display.h"
#include "Scene/SceneManager.h"

using namespace HarmonyEngine;

Application::Application() {
    HARMONY_PROFILE_BEGIN_SESSION("Startup", "HarmonyProfile-Startup.json");
}

Application::Application(const std::string& applicationName) : m_ApplicationName(applicationName) {
    HARMONY_PROFILE_BEGIN_SESSION("Startup", "HarmonyProfile-Startup.json");
}

Application::~Application() {
    if(m_ShouldEndShutdown)
        HARMONY_PROFILE_END_SESSION(); // Shutdown
}

void Application::CreateDisplay() {
    Display::CreateDisplay(m_ApplicationName.c_str());
}

void Application::StartApplication(Scene& scene) {
    // ========================================================================
    // Create Display and Set Active Scene
    // ========================================================================

    if(Display::GetWindowPtr() == nullptr)
        Display::CreateDisplay(m_ApplicationName.c_str());

    SceneManager::SetActiveScene(&scene);

    HARMONY_PROFILE_END_SESSION(); // End Startup

    // ========================================================================
    // Start Game Loop
    // ========================================================================
    HARMONY_PROFILE_BEGIN_SESSION("Runtime", "HarmonyProfile-Runtime.json");

    Display::StartGameLoop();

    HARMONY_PROFILE_END_SESSION();

    // ========================================================================
    // Shutdown
    // ========================================================================
    HARMONY_PROFILE_BEGIN_SESSION("Shutdown", "HarmonyProfile-Shutdown.json");
    m_ShouldEndShutdown = true;

    Display::CleanUp();
}