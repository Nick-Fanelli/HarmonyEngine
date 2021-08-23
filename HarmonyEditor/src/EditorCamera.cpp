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
                } else if(Input::IsKey(HARMONY_KEY_LEFT_ALT)) {
                    MouseZoom(delta.y);
                } else {
                    MouseRotate(delta * Settings::EditorMovementSensitivity.CurrentValue);
                }

                m_IsInOperation = true;
                UpdateView();
            } else if(Input::IsMouseButtonUp(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                m_IsInOperation = false;
            }
            break;

        case InputStyleReversed:
            // Pan as default rotate on shift
            if(Input::IsMouseButton(HARMONY_MOUSE_BUTTON_MIDDLE)) {
                const glm::vec2& delta = Input::GetDeltaMousePosition() * 0.003f;

                if(Input::IsKey(HARMONY_KEY_LEFT_SHIFT)) {
                    MouseRotate(delta * Settings::EditorMovementSensitivity.CurrentValue);
                } else if(Input::IsKey(HARMONY_KEY_LEFT_ALT)) {
                    MouseZoom(delta.y);
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