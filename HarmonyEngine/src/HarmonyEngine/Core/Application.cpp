#include "Application.h"

using namespace HarmonyEngine;

static bool s_LogInitialized = false;

Application Application::CreateApplication(const std::string& message) {
    if(!s_LogInitialized) {
        Log::Initialize();
        s_LogInitialized = true;
    }

    return Application(message);
}

void Application::StartApplication() {
    
}