#include "Camera.h"

using namespace HarmonyEngine;

// Orthographic Camera
OrthographicCamera::OrthographicCamera() {
    UpdateProjection();
    UpdateView();
}

void OrthographicCamera::UpdateProjection() {
    m_ProjectionMatrix = glm::ortho(-m_AspectRatio + m_Zoom, m_AspectRatio - m_Zoom, -1.0f + m_Zoom, 1.0f - m_Zoom, m_NearClip, m_FarClip);
}

void OrthographicCamera::UpdateView() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);

    m_ViewMatrix = glm::inverse(transform);
}