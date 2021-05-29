#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"
#include "../Core/Display.h"
#include "../Render/Renderer2D.h"
#include "../Render/Camera.h"
#include "../Render/Renderer.h"

using namespace HarmonyEngine;

class GameScene : public Scene {

    PerspectiveCamera m_Camera;
    Mesh m_Mesh;
    Mesh m_Mesh2;
    // Quad m_Quad;

public:

    void OnCreate() override {
        m_Camera = PerspectiveCamera();

        // Renderer2D::OnCreate(&m_Camera);        
        Renderer::OnCreate(&m_Camera);

        // m_Quad = Quad({7, 0, 4}, {1, 1}, {0, 0, 1, 1});

        Renderer::LoadOBJFile("assets/objects/stall.obj", &m_Mesh);
        Renderer::LoadOBJFile("assets/objects/monkey.obj", &m_Mesh2);
    }


    void Update(float deltaTime) override {

        const float moveSpeed = 3.5f;
        const float mouseSensitivity = 4.5f;

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

        if(Input::IsKey(HARMONY_KEY_SPACE)) {
            m_Camera.Move(moveSpeed * deltaTime * m_Camera.GetCameraUp());
        }

         if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT)) {
            m_Camera.Move(-moveSpeed * deltaTime * m_Camera.GetCameraUp());
        }

        const glm::vec2 zero = {0, 0};

        if(Input::GetDeltaMousePosition() != zero) {
            m_Camera.Rotate(Input::GetDeltaMousePosition() * mouseSensitivity * deltaTime); 
        }

        // Renderer2D::StartBatch();
        // Renderer2D::DrawQuad(m_Quad);
        // Renderer2D::EndBatch();

        Renderer::StartBatch();
        Renderer::DrawMesh(m_Mesh);
        Renderer::DrawMesh(m_Mesh2);
        Renderer::EndBatch();

    }

    void OnDestroy() override {
        // Renderer2D::OnDestroy();
        Renderer::OnDestroy();
    }

};