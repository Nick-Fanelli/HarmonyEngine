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

using namespace HarmonyEngine;

class GameScene : public Scene {

    PerspectiveCamera m_Camera;
    std::array<Texture, 16> m_Textures;
    Texture m_Texture;
    // Entity m_Entity = Entity(this);
    // Mesh m_Mesh;

    // Terrain m_Terrain;

public:

    void OnCreate() override {
        m_Camera = PerspectiveCamera();
        
        // Renderer::OnCreate(&m_Camera);
        Renderer2D::OnCreate(&m_Camera);

        for(int i = 0; i < m_Textures.size(); i++) {
            m_Textures[i] = Texture("assets/textures/grass.jpeg");
            m_Textures[i].Create(); 
        }

        m_Texture = Texture("assets/textures/grass.jpeg");
        m_Texture.Create();

        // auto textureID = Renderer::AddTexture(texture);
        // m_Terrain = Terrain(128, 0.0f);

        // Renderer::LoadOBJFile("assets/objects/stall.obj", &m_Mesh, textureID);

        // m_Entity.AddComponent<Transform>(glm::vec3(0, 0, 0));
        // m_Entity.AddComponent<MeshRenderer>(&m_Mesh);

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

        // Renderer::StartBatch();

        // Renderer::DrawMesh(m_Terrain.GetMesh());

        // auto meshRendererGroup = m_Registry.group<MeshRenderer>(entt::get<Transform>);
        // for(auto& entity : meshRendererGroup) {
        //     auto[meshRenderer, transform] = meshRendererGroup.get<MeshRenderer, Transform>(entity);
        //     Renderer::DrawMesh(*meshRenderer.MeshPtr, transform.Position);
        // }

        // Renderer::EndBatch();

        RendererStats2D::Start();
        Renderer2D::StartBatch();

        // for(int i = 0; i < 16; i++) {
            Renderer2D::DrawQuad({0, 0, 0}, {1, 1}, {1, 1, 1, 1}, m_Texture);
        // }

        Renderer2D::EndBatch();

        // Renderer2D::StartBatch();
        // Renderer2D::DrawQuad({0, 16, 0}, {1, 1}, {1, 1, 1, 1}, m_Textures[15]);

        // Renderer2D::EndBatch();

        RendererStats2D::End();

        // Log::Info(RendererStats2D::BatchCount);

    }

    void OnDestroy() override {
        // Renderer::OnDestroy();
        Renderer2D::OnDestroy();
        m_Registry.clear<>();
    }

};