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
    PerspectiveCamera m_Camera;
    Quad m_Quad;

public:

    void OnCreate() override {
        m_Shader = Shader("assets/shaders/Shader2D.vert.glsl", "assets/shaders/Shader2D.frag.glsl");
        m_Shader.Create();

        // Texture texture = Texture("assets/textures/grass.jpeg");
        // texture.Create();

        m_Camera = PerspectiveCamera();

        Renderer2D::OnCreate(&m_Camera, &m_Shader);

        // auto textureID = Renderer2D::AddTexture(texture);

        static const std::array<glm::vec4, 4> colorArray = {
            glm::vec4(1, 0, 0, 1),
            glm::vec4(0, 1, 0, 1),
            glm::vec4(0, 0, 1, 1),
            glm::vec4(1, 1, 0, 1)
        };

        m_Quad = Quad({-0.5, -0.5, 0}, {1, 1}, colorArray);
    }


    void Update(float deltaTime) override {

        const float moveSpeed = 2.0f;
        const float mouseSensitivity = 4.0f;

        if(Input::IsKey(HARMONY_KEY_W)) {
            m_Camera.Move(-moveSpeed * deltaTime * m_Camera.GetCameraFront());
        }

        if(Input::IsKey(HARMONY_KEY_S)) {
            m_Camera.Move(moveSpeed * deltaTime * m_Camera.GetCameraFront());
        }

        if(Input::IsKey(HARMONY_KEY_A)) {
            m_Camera.Move(glm::normalize(glm::cross(m_Camera.GetCameraFront(), m_Camera.GetCameraUp())) * moveSpeed * deltaTime);
        }

        if(Input::IsKey(HARMONY_KEY_D)) {
            m_Camera.Move(glm::normalize(glm::cross(m_Camera.GetCameraFront(), m_Camera.GetCameraUp())) * -moveSpeed * deltaTime);
        }

        const glm::vec2 zero = {0, 0};

        if(Input::GetDeltaMousePosition() != zero) {
            m_Camera.Rotate(Input::GetDeltaMousePosition() * mouseSensitivity * deltaTime); 
        }

        Renderer2D::StartBatch();
        Renderer2D::DrawQuad(m_Quad);
        Renderer2D::EndBatch();
    }

    void OnDestroy() override {
        Renderer2D::OnDestroy();
    }

};