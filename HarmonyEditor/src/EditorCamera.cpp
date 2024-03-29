#include "EditorCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Input.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Settings.h"

using namespace HarmonyEditor;

const char* EditorCamera::InputStyleFields[] = { "Default", "Reversed", "Modern" };
const uint32_t EditorCamera::InputStyleCount = sizeof(EditorCamera::InputStyleFields) / sizeof(EditorCamera::InputStyleFields[0]);

void EditorCamera::OnUpdate(float deltaTime) {

    HARMONY_PROFILE_FUNCTION();

    switch(Settings::EditorInputStyle.CurrentValue) {
        case InputStyleDefault:
            // Rotate on default pan on shift
            if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;

                if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT)) {
                    MousePan(delta);
                } else {
                    MouseRotate(delta * Settings::EditorMovementSensitivity.CurrentValue);
                }

                m_IsInOperation = true;
                UpdateView();
            } else if(Input::IsKey(HARMONY_KEY_LEFT_ALT)) {
                const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;
                
                if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_LEFT)) {
                    if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT))
                        MousePan(delta);
                    else
                        MouseRotate(delta * Settings::EditorMovementSensitivity.CurrentValue);
                }

                m_IsInOperation = true;
                UpdateView();
            } else if(Input::IsMouseButtonUp(HARMONY_MOUSE_BUTTON_MIDDLE) || Input::IsMouseButtonUp(HARMONY_MOUSE_BUTTON_LEFT)) {
                m_IsInOperation = false;
            }
            break;

        case InputStyleReversed:
            // Pan as default rotate on shift
            if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;

                if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT)) {
                    MouseRotate(delta * Settings::EditorMovementSensitivity.CurrentValue);
                } else {
                    MousePan(delta);
                }

                m_IsInOperation = true;
                UpdateView();
            } else if(Input::IsMouseButtonUp(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                m_IsInOperation = false;
            }
            break;
        case InputStyleModern:
            // Rotate on right click pan on middle mouse
            if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_RIGHT)) {
                const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;

                MouseRotate(delta * Settings::EditorMovementSensitivity.CurrentValue);

                m_IsInOperation = true; 
                UpdateView();
            } else if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;

                if(Input::IsKey(HARMONY_KEY_LEFT_ALT)) {
                    MouseZoom(delta.y);
                } else {
                    MousePan(delta);
                }

                m_IsInOperation = true; 
                UpdateView();
            } else if(Input::IsMouseButtonUp(HARMONY_MOUSE_BUTTON_RIGHT) || Input::IsMouseButtonUp(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                m_IsInOperation = false;
            }
            break;
    }

    if(Input::GetScrollPosition().y != 0.0f) {
        MouseZoom(Input::GetScrollPosition().y * 0.1f);
        UpdateView();
    }

    if(Input::IsKeyDown(HARMONY_KEY_0))
        ResetView();
}

void EditorCamera::UpdateProjection() {
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void EditorCamera::UpdateView() {
    m_Position = CalculatePosition();

    glm::quat orientation = GetOrientation();
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

std::pair<float, float> EditorCamera::PanSpeed() const {
    float x = std::min(m_AspectRatio / 10000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_AspectRatio / 10000.0f, 2.4f); // max = 2.4f
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
