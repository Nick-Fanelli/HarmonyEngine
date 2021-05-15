#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"
#include "../Core/Display.h"
#include "../Render/Renderer2D.h"
#include "../Render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace HarmonyEngine;

class GameScene : public Scene {

    Shader m_Shader;
    OrthographicCamera m_Camera;
    Quad m_Quad;

public:

    void OnCreate() override {
        m_Shader = Shader("assets/shaders/Shader2D.vert.glsl", "assets/shaders/Shader2D.frag.glsl");
        m_Shader.Create();

        m_Camera = OrthographicCamera({0.5, 0.5, 0});

        Renderer2D::OnCreate(&m_Camera, &m_Shader);

        m_Quad = Quad({0, 0, 0});
    }


    void Update(float deltaTime) override {
        Renderer2D::StartBatch();
        
        Renderer2D::DrawQuad(m_Quad);

        Renderer2D::EndBatch();
    }

    void OnDestroy() override {
        Renderer2D::OnDestroy();
    }

};