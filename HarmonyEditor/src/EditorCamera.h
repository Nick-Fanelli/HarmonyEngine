#pragma once

#include <harmonypch.h>

#include <Render/Camera.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorCamera : public PerspectiveCamera {

    public:
        EditorCamera();

        void OnUpdate(float deltaTime);

        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }

        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

        inline void ResetView() {
             m_FocalPoint = GetForwardDirection();
            m_Yaw = 0;
            m_Pitch = 0;
            m_Distance = 20.0f;

            UpdateView();
        }

        const glm::mat4& GetViewMatrix() const { return m_View; }
        glm::mat4 GetViewProjection() const { return m_Projection * m_View; }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const { return m_Position; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }

        enum InputStyle {
            InputStyleDefault, InputStyleReversed, InputStyleModern
        };

    private:
        void UpdateProjection();
        void UpdateView();

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;
    private:
        float m_ViewportWidth = 1280, m_ViewportHeight = 720;
        float m_FOV = 45.0f, m_AspectRatio = m_ViewportWidth / m_ViewportHeight, m_NearClip = 0.1f, m_FarClip = 2500.0f;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

        float m_Distance = 10.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;
    };
}