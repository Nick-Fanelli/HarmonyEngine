#include <harmonypch.h>

#include <Scene/SceneManager.h>
#include <Core/Display.h>

#include "EditorScene.h"

using namespace HarmonyEngine;
using namespace HarmonyEditor;

int main() {

#if HARMONY_DEBUG
    Log::Info("Running Harmony Engine in DEBUG mode!");
#else
    Log::Info("Running Harmony Engine in RELEASE mode!");
#endif

    Log::SetLogLevel(Log::LogLevel::LogLevelWarn);

    // Startup
    HARMONY_PROFILE_BEGIN_SESSION("Startup", "HarmonyProfile-Startup.json");
    EditorScene editorScene{};
    Display::CreateDisplay("Harmony Engine");
    SceneManager::SetActiveScene(&editorScene);

    HARMONY_PROFILE_END_SESSION();

    // Runtime
    HARMONY_PROFILE_BEGIN_SESSION("Runtime", "HarmonyProfile-Runtime.json");
    Display::StartGameLoop();
    HARMONY_PROFILE_END_SESSION();

    // Shutdown
    HARMONY_PROFILE_BEGIN_SESSION("Shutdown", "HarmonyProfile-Shutdown.json");
    Display::CleanUp();
    HARMONY_PROFILE_END_SESSION();
}