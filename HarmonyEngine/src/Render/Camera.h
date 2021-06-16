#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Display.h"

namespace HarmonyEngine {

    class Camera {

    protected:
        glm::vec3 m_Position; 

        glm::mat4 m_Projection;
        glm::mat4 m_View;

    public:
        const glm::mat4& GetProjctionMatrix() const { return m_Projection; }
        const glm::mat4& GetViewMatrix() const { return m_View; }

        glm::mat4 GetProjectViewMatrix() { return m_Projection * m_View; }

        const glm::vec3& GetPosition() const { return m_Position; }

        Camera() = default;
    };

    class OrthographicCamera : public Camera {

        float m_Rotation = 0;

        void RecalculateViewMatrix();
        
    public:

        OrthographicCamera(const glm::vec3& position);

        void Move(const glm::vec3& deltaPosition);
        void SetPosition(const glm::vec3& position);
        void SetRotation(float rotation);

        const float GetRotation() const { return m_Rotation; }

    };

    class PerspectiveCamera : public Camera {

        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float yaw = -90.0f;
        float pitch = 0.0f;
        float fov = 70.0f;

    protected:
        void RecalculateViewMatrix();

    public:
        PerspectiveCamera();

        void Move(const glm::vec3& deltaPosition);
        void Rotate(float yawOffset, float pitchOffset);
        void Rotate(const glm::vec2& rotation);

        const glm::vec3& GetCameraFront() const { return cameraFront; }
        const glm::vec3& GetCameraUp() const { return cameraUp; }
 
    };
}