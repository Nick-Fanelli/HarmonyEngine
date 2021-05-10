#pragma once

#include "harmonypch.h"

#include "Scene.h"

namespace HarmonyEngine {

    class GameScene : public Scene {

    public:
        void Update(float deltaTime) override {
            Log::Info("this is fun");
        }

    };

}