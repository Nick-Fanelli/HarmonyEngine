#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"

namespace HarmonyEngine {

    class GameScene : public Scene {

    public:
        void OnCreate() override {
            Log::Info("On Create!");
        }

        void Update(float deltaTime) override {
            
        }

    };

}