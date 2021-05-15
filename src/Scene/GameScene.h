#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"
#include "../Core/Display.h"
#include "../Render/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace HarmonyEngine;

class GameScene : public Scene {

    Shader s_Shader;

public:

    void OnCreate() override {
        s_Shader = Shader("assets/shaders/Shader2D.vert.glsl", "assets/shaders/Shader2D.frag.glsl");
        s_Shader.Create();

        // int width, height;

        // Display::GetWidth(&width);
        // Display::GetHeight(&height);

        // s_Projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
        // s_View = glm::lookAt(
        //     glm::vec3(4, 3, 3),
        //     glm::vec3(0, 0, 0),
        //     glm::vec3(0, 1, 0)
        // );

        // glm::mat4 model = glm::mat4(1.0f);
        // s_MVP = s_Projection * s_View * model;

    }


    void Update(float deltaTime) override {
        
    }

};