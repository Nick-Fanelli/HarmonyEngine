#pragma once

#include "harmonypch.h"

#include "Scene.h"
#include "../Render/Shader.h"
#include "../Core/Display.h"
#include "../Render/Renderer2D.h"
#include "../Render/Camera.h"
#include "../Render/Renderer.h"
#include "Entity.h"
#include "Component.h"
#include "Terrain.h"
#include "../Core/Input.h"
#include "../Core/Assets.h"

using namespace HarmonyEngine;

class GameScene : public Scene {

    PerspectiveCamera m_Camera;
    AssetHandle<Mesh> m_Mesh;

public:

    void OnCreate() override {
        m_Camera = PerspectiveCamera();
        
        // Renderer2D::OnCreate(&m_Camera);
        Renderer::OnCreate(&m_Camera);

        m_Mesh = AssetManager::QueueMesh("assets/objects/low-polly-tree.obj");

        AssetManager::CreateAll();
    }

    void Update(float deltaTime) override {

        constexpr float moveSpeed = 3.5f;
        constexpr float mouseSensitivity = 4.5f;

        if(Input::IsKey(HARMONY_KEY_W)) {
            m_Camera.Move(glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
        }

        if(Input::IsKey(HARMONY_KEY_S)) {
            m_Camera.Move(glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
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

        Renderer::StartBatch();

        Renderer::DrawMesh(m_Mesh);

        Renderer::EndBatch();
    }

    void OnDestroy() override {
        Renderer::OnDestroy();
        // Renderer2D::OnDestroy();
        m_Registry.clear<>();
        AssetManager::DestroyAll();
    }

};