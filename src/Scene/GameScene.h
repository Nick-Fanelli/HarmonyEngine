#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"
#include "../Core/Display.h"
#include "../Render/Renderer2D.h"
#include "../Render/Camera.h"
#include "../Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace HarmonyEngine;

class GameScene : public Scene {

    Shader m_Shader;
    PerspectiveCamera m_Camera;
    // Quad m_Quad;

    GLuint m_VaoID;
    GLuint m_VboID;
    GLuint m_IboID;
    int indexCount;

public:

    void OnCreate() override {
        m_Shader = Shader("assets/shaders/DefaultShader.vert.glsl", "assets/shaders/DefaultShader.frag.glsl");
        m_Shader.Create();

        // Texture texture = Texture("assets/textures/grass.jpeg");
        // texture.Create();

        m_Camera = PerspectiveCamera();
        
        Renderer::OnCreate(&m_Camera, &m_Shader);

        // Renderer2D::OnCreate(&m_Camera, &m_Shader);

        // auto textureID = Renderer2D::AddTexture(texture);

        // static const std::array<glm::vec4, 4> colorArray = {
        //     glm::vec4(1, 0, 0, 1),
        //     glm::vec4(0, 1, 0, 1),
        //     glm::vec4(0, 0, 1, 1),
        //     glm::vec4(1, 1, 0, 1)
        // };

        // m_Quad = Quad({-0.5, -0.5, 0}, {1, 1}, {1, 1, 1, 1}, textureID);

        GLfloat vertices[] = {
            // front
            -1.0, -1.0,  1.0,
            1.0, -1.0,  1.0,
            1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            // back
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0
        };

        GLuint indices[] = {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3
        };

        indexCount = sizeof(indices) / sizeof(unsigned int);

        glGenVertexArrays(1, &m_VaoID);
        glBindVertexArray(m_VaoID);

        glGenBuffers(1, &m_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &m_IboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
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

        // Renderer2D::StartBatch();
        // Renderer2D::DrawQuad(m_Quad);
        // Renderer2D::EndBatch();

        Renderer::Render(m_VaoID, m_IboID, indexCount);
    }

    void OnDestroy() override {
        // Renderer2D::OnDestroy();
    }

};