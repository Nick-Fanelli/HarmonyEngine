#pragma once

#include <harmonypch.h>

#include <Render/Camera.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorCamera : public Camera {

    public:
        EditorCamera() {
            UpdateView();
            UpdateProjection();
            m_FarClip = 2000.0f;
        }

        inline bool IsInOperation() const { return m_IsInOperation; }

        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }

        inline void ResetView() {
            m_FocalPoint = GetForwardDirection();
            m_Yaw = 0;
            m_Pitch = 0;
            m_Distance = 20.0f;

            UpdateView();
        }

        void OnUpdate(float deltaTime);

        enum InputStyle {
            InputStyleDefault = 0, InputStyleReversed = 1, InputStyleModern = 2
        };

        static const char* InputStyleFields[];
        static const uint32_t InputStyleCount;

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const { return m_Position; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }

        void UpdateProjection() override;
        void UpdateView() override;
    protected:

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;
    private:
        float m_FOV = 45.0f;
        float m_Distance = 10.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

        bool m_IsInOperation = false;
    };
}