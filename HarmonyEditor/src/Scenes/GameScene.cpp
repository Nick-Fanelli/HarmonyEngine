#include "GameScene.h"

static PerspectiveCamera s_Camera;
static AssetHandle<Mesh> s_Mesh;

void GameScene::OnCreate() {
    s_Camera = PerspectiveCamera();
    
    // Renderer2D::OnCreate(&m_Camera);
    Renderer::OnCreate(&s_Camera);

    s_Mesh = AssetManager::QueueMesh("assets/objects/stall.obj");

    AssetManager::CreateAll();
}

void GameScene::OnUpdate(float deltaTime) {

    constexpr float moveSpeed = 3.5f;
    constexpr float mouseSensitivity = 4.5f;

    if(Input::IsKey(HARMONY_KEY_W)) {
        s_Camera.Move(glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
    }

    if(Input::IsKey(HARMONY_KEY_S)) {
        s_Camera.Move(glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
    }

    if(Input::IsKey(HARMONY_KEY_A)) {
        s_Camera.Move(glm::normalize(glm::cross(s_Camera.GetCameraFront(), s_Camera.GetCameraUp())) * moveSpeed * deltaTime);
    }

    if(Input::IsKey(HARMONY_KEY_D)) {
        s_Camera.Move(glm::normalize(glm::cross(s_Camera.GetCameraFront(), s_Camera.GetCameraUp())) * -moveSpeed * deltaTime);
    }

    if(Input::IsKey(HARMONY_KEY_SPACE)) {
        s_Camera.Move(moveSpeed * deltaTime * s_Camera.GetCameraUp());
    }

    if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT)) {
        s_Camera.Move(-moveSpeed * deltaTime * s_Camera.GetCameraUp());
    }

    const glm::vec2 zero = {0, 0};

    if(Input::GetDeltaMousePosition() != zero) {
        s_Camera.Rotate(Input::GetDeltaMousePosition() * mouseSensitivity * deltaTime); 
    }

    Renderer::StartBatch();

    Renderer::DrawMesh(s_Mesh);

    Renderer::EndBatch();
}

void GameScene::OnDestroy() {
    Renderer::OnDestroy();
    // Renderer2D::OnDestroy();
    m_Registry.clear<>();
    AssetManager::DestroyAll();
}