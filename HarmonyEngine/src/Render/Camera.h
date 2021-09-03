#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Display.h"

namespace HarmonyEngine {

    class Camera {

    public:

        Camera() = default;

        void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; UpdateProjection(); }
        void SetPosition(const glm::vec3& position) { m_Position = position; UpdateView(); }

        glm::mat4 GetProjectionViewMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

        inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

        void SetNearClip(float nearClip) { m_NearClip = nearClip; }
        void SetFarClip(float farClip) { m_FarClip = farClip; }

        inline float GetNearClip() const { return m_NearClip; }
        inline float GetFarClip() const { return m_FarClip; }

        float& GetNearClip() { return m_NearClip; }
        float& GetFarClip() { return m_FarClip; }

    protected:
        virtual void UpdateProjection() {}
        virtual void UpdateView() {}

    protected:
        glm::mat4 m_ProjectionMatrix, m_ViewMatrix;
        glm::vec3 m_Position, m_Rotation;

        float m_AspectRatio = 1920.0f / 1080.0f;
        float m_NearClip = 0.1f, m_FarClip = 100.0f;

    };

    class OrthographicCamera : public Camera {

    public:
        OrthographicCamera();

    protected:
        void UpdateProjection() override;
        void UpdateView() override;

    };
}