#include "harmonypch.h"

#include "Core/Display.h"
#include "Scene/SceneManager.h"
#include "Scenes/EditorScene.h"

using namespace HarmonyEngine;

int main() {

#if HARMONY_DEBUG
    Log::Info("Running Harmony Engine in DEBUG mode!");
#else
    Log::Info("Running Harmony Engine in RELEASE mode!");
#endif

    // Startup
    HARMONY_PROFILE_BEGIN_SESSION("Startup", "HarmonyProfile-Startup.json");

    EditorScene launchScene = EditorScene();
    Display::CreateDisplay("Harmony Engine");
    SceneManager::SetActiveScene(&launchScene);

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