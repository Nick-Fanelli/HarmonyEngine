#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"

using namespace HarmonyEngine;

class GameScene : public Scene {

public:
    void OnCreate() override {
        Shader shader = Shader("assets/shaders/DefaultShader.vert.glsl", "assets/shaders/DefaultShader.frag.glsl");
        shader.Create();
    }

    void Update(float deltaTime) override {
        
    }

};