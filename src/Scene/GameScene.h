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
    AssetHandle<Texture> m_Texture;
    AssetHandle<Texture> m_Texture2;
    // AssetHandle<Mesh> m_Mesh;

public:

    void OnCreate() override {
        m_Camera = PerspectiveCamera();
        
        Renderer2D::OnCreate(&m_Camera);
        m_Texture = AssetManager::QueueTexture("assets/textures/grass.jpeg");
        m_Texture2 = AssetManager::QueueTexture("assets/textures/grass.jpeg");

        // for(int i = 0; i < 32; i++) {
            // m_Textures[i] = AssetManager::QueueTexture("assets/textures/grass.jpeg");
            // toggle = !toggle;
        // }

        AssetManager::CreateAll();

        // Log::Info(m_Textures[0].GetPtr());
        // Log::Info(m_Textures[1].GetPtr());
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


        RendererStats2D::Start();
        Renderer2D::StartBatch();

        Renderer2D::DrawQuad({0, 0, 0}, {1, 1}, {1, 1, 1, 1}, m_Texture2);

        Renderer2D::EndBatch();
        RendererStats2D::End();

    }

    void OnDestroy() override {
        Renderer2D::OnDestroy();
        m_Registry.clear<>();
        AssetManager::DestroyAll();
    }

};