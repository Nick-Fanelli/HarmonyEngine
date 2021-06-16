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
    m_Position = { 0, 0, -3 };

    m_Projection = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.1f, 100.0f);

    RecalculateViewMatrix();
}

void PerspectiveCamera::RecalculateViewMatrix() {
    m_View = glm::lookAt(m_Position, m_Position - cameraFront, cameraUp);
}

void PerspectiveCamera::Move(const glm::vec3& deltaPosition) {
    m_Position += deltaPosition;
}

void PerspectiveCamera::Rotate(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    pitch += pitchOffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
    cameraFront = glm::normalize(front);

    RecalculateViewMatrix();
}

void PerspectiveCamera::Rotate(const glm::vec2& rotation) {
    yaw += rotation.x;
    pitch += rotation.y;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
    cameraFront = glm::normalize(front);

    RecalculateViewMatrix();
}