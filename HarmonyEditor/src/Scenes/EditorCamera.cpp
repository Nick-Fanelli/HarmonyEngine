#include "EditorCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Input.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

EditorCamera::EditorCamera() {
    UpdateView();
}

void EditorCamera::UpdateProjection() {
    m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void EditorCamera::UpdateView() {
    m_Position = CalculatePosition();

    glm::quat orientation = GetOrientation();
    m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_View = glm::inverse(m_View);
}

std::pair<float, float> EditorCamera::PanSpeed() const {
    float x = std::min(m_ViewportWidth / 10000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_ViewportHeight / 10000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { xFactor, yFactor };
}

float EditorCamera::RotationSpeed() const {
    return 0.8f;
}

float EditorCamera::ZoomSpeed() const {
    float distance = m_Distance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 100.0f); // max speed = 100
    return speed;
}

void EditorCamera::OnUpdate(float deltaTime) {

    HARMONY_PROFILE_FUNCTION();

    if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_MIDDLE)) {
        const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;

        if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT)) {
            MousePan(delta);
        } else if(Input::IsKey(HARMONY_KEY_LEFT_ALT)) {
            MouseZoom(delta.y);
        } else {
            MouseRotate(delta);
        }

        UpdateView();
    }

    if(Input::GetScrollPosition().y != 0.0f) {
        MouseZoom(Input::GetScrollPosition().y * 0.1f);
        UpdateView();
    }

}

void EditorCamera::MousePan(const glm::vec2& delta) {
    auto [xSpeed, ySpeed] = PanSpeed();
    m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
    m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
}

void EditorCamera::MouseRotate(const glm::vec2& delta) {
    float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_Yaw += yawSign * delta.x * RotationSpeed();
    m_Pitch += delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta) {
    delta *= ZoomSpeed();

    constexpr float maxMoveSpeed = 45.0f;

    std::clamp(delta, -maxMoveSpeed, maxMoveSpeed);

    m_Distance -= delta;
    if (m_Distance < 1.0f) {
        m_FocalPoint += GetForwardDirection();
        m_Distance = 1.0f;
    }
}

glm::vec3 EditorCamera::GetUpDirection() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
glm::vec3 EditorCamera::GetRightDirection() const { return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
glm::vec3 EditorCamera::GetForwardDirection() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f)); }
glm::vec3 EditorCamera::CalculatePosition() const { return m_FocalPoint - GetForwardDirection() * m_Distance; }
glm::quat EditorCamera::GetOrientation() const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); }