#include "harmonypch.h"

#include "Core/Display.h"
#include "Scene/GameScene.h"

using namespace HarmonyEngine;

int main() {

    GameScene gameScene = GameScene();

    SceneManager::SetActiveScene(&gameScene);

    Display::CreateDisplay("Harmony Engine");
    Display::StartGameLoop();

}