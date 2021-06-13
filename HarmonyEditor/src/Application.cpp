#include "harmonypch.h"

#include "Core/Display.h"
#include "Scenes/EditorScene.h"

using namespace HarmonyEngine;

int main() {

#ifdef HARMONY_DEBUG_ENABLED
    Log::Info("Running Harmony Engine in DEBUG mode!");
#else
    Log::Info("Running Harmony Engine in RELEASE mode!");
#endif

    EditorScene launchScene = EditorScene();

    Display::CreateDisplay("Harmony Engine");
    Display::StartGameLoop(&launchScene);

}