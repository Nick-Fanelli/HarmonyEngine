#include "Application.h"

using namespace HarmonyEngine;

static bool s_LogInitialized = false;

Application::Application(const std::string& applicationName) : m_ApplicationName(applicationName) {
    if(!s_LogInitialized) {
        Log::Initialize();
        s_LogInitialized = true;
    }

    m_Display = std::make_shared<Display>(applicationName);

}

void Application::OnUpdate(float deltaTime) {

}

void Application::StartApplication() {

#ifdef DEBUG
    Log::Info("Running Application \"{}\" In [Debug] Mode!", m_ApplicationName);
#else
    Log::Info("Running Application \"{}\" In [Release] Mode!", m_ApplicationName);
#endif

    m_Display->CreateDisplay();
    m_Display->StartGameLoop(*this);

    Log::Trace("Cleaning Up Application...");

    CleanUp();

    Log::Info("Application Successfully Cleaned!");
}

void Application::CleanUp()  {

}