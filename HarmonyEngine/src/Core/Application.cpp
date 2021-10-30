#include "Application.h"

using namespace HarmonyEngine;

Application::Application(const std::string& applicationName) : m_ApplicationName(applicationName) {
    m_Display = std::make_shared<Display>(applicationName);

}

void Application::OnUpdate(float deltaTime) {

}

void Application::StartApplication() {

#ifdef DEBUG
    Log::FormatInfo("Running Application \"%s\" In [Debug] Mode!", m_ApplicationName.c_str());
#else
    Log::FormatInfo("Running Application \"%s\" In [Release] Mode!", m_ApplicationName.c_str());
#endif

    m_Display->CreateDisplay();
    m_Display->StartGameLoop(*this);

    Log::Trace("Cleaning Up Application...");

    CleanUp();

    Log::Info("Application Successfully Cleaned!");
}

void Application::CleanUp()  {

}