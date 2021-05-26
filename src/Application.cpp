#include "harmonypch.h"

#include "Core/Display.h"
#include "Scene/GameScene.h"

using namespace HarmonyEngine;

int main() {

#ifdef HARMONY_DEBUG_ENABLED
    Log::Info("Running Harmony Engine in DEBUG mode!");
#else
    Log::Info("Running Harmony Engine in RELEASE mode!");
#endif

    GameScene gameScene = GameScene();

    Display::CreateDisplay("Harmony Engine");
    Display::StartGameLoop(&gameScene);

}