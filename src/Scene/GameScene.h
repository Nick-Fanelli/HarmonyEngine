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

        Texture texture = Texture("assets/textures/grass.jpeg");
        texture.Create();

        m_Camera = OrthographicCamera({0.5, 0.5, 0});

        Renderer2D::OnCreate(&m_Camera, &m_Shader);

        auto textureID = Renderer2D::AddTexture(texture);

        static const std::array<glm::vec4, 4> colorArray = {
            glm::vec4(1, 0, 0, 1),
            glm::vec4(0, 1, 0, 1),
            glm::vec4(0, 0, 1, 1),
            glm::vec4(1, 1, 0, 1)
        };

        m_Quad = Quad({0, 0, 0}, {1, 1}, colorArray, textureID);
    }


    void Update(float deltaTime) override {

        const float moveSpeed = 2.0f;

        if(Input::IsKey(HARMONY_KEY_LEFT)) {
            m_Camera.Move({-moveSpeed * deltaTime, 0, 0});
        }

        if(Input::IsKey(HARMONY_KEY_RIGHT)) {
            m_Camera.Move({moveSpeed * deltaTime, 0, 0});
        }

        if(Input::IsKey(HARMONY_KEY_UP)) {
            m_Camera.Move({0, moveSpeed * deltaTime, 0});
        }

        if(Input::IsKey(HARMONY_KEY_DOWN)) {
            m_Camera.Move({0, -moveSpeed * deltaTime, 0});
        }

        Renderer2D::StartBatch();
        Renderer2D::DrawQuad(m_Quad);
        Renderer2D::EndBatch();
    }

    void OnDestroy() override {
        Renderer2D::OnDestroy();
    }

};