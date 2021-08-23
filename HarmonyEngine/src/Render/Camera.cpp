#include "Camera.h"

using namespace HarmonyEngine;

// Orthographic Camera
OrthographicCamera::OrthographicCamera(const glm::vec3& position) {
        m_Position = position;

        int displayWidth, displayHeight;

        Display::GetWidth(&displayWidth);
        Display::GetHeight(&displayHeight);

        const float aspectRatio = (float) displayWidth / (float) displayHeight;

        m_Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
        m_View = glm::mat4(1.0f);
        
        RecalculateViewMatrix();
    }

void OrthographicCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_View = glm::inverse(transform);
}

void OrthographicCamera::Move(const glm::vec3& deltaPosition) {
    m_Position += deltaPosition;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
}

// Perspective Camera
PerspectiveCamera::PerspectiveCamera() {
    UpdateView();
    UpdateProjection(); 
}

void PerspectiveCamera::UpdateProjection() {
    m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void PerspectiveCamera::UpdateView() {
    m_Position = CalculatePosition();

    glm::quat orientation = GetOrientation();
    m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_View = glm::inverse(m_View);
}

std::pair<float, float> PerspectiveCamera::PanSpeed() const {
    float x = std::min(m_ViewportWidth / 10000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_ViewportHeight / 10000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { xFactor, yFactor };
}

float PerspectiveCamera::RotationSpeed() const {
    return 0.8f;
}

float PerspectiveCamera::ZoomSpeed() const {
    float distance = m_Distance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 100.0f); // max speed = 100
    return speed;
}

void PerspectiveCamera::MousePan(const glm::vec2& delta) {
    auto [xSpeed, ySpeed] = PanSpeed();
    m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
    m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
}

void PerspectiveCamera::MouseRotate(const glm::vec2& delta) {
    float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_Yaw += yawSign * delta.x * RotationSpeed();
    m_Pitch += delta.y * RotationSpeed();
}

void PerspectiveCamera::MouseZoom(float delta) {
    delta *= ZoomSpeed();

    constexpr float maxMoveSpeed = 45.0f;

    std::clamp(delta, -maxMoveSpeed, maxMoveSpeed);

    m_Distance -= delta;
    if (m_Distance < 1.0f) {
        m_FocalPoint += GetForwardDirection();
        m_Distance = 1.0f;
    }
}

glm::vec3 PerspectiveCamera::GetUpDirection() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
glm::vec3 PerspectiveCamera::GetRightDirection() const { return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
glm::vec3 PerspectiveCamera::GetForwardDirection() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f)); }
glm::vec3 PerspectiveCamera::CalculatePosition() const { return m_FocalPoint - GetForwardDirection() * m_Distance; }
glm::quat PerspectiveCamera::GetOrientation() const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); }
