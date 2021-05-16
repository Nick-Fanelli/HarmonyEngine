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
        glm::mat4 m_ProjectionViewMatrix;

    public:
        const glm::mat4& GetProjctionMatrix() const { return m_Projection; }
        const glm::mat4& GetViewMatrix() const { return m_View; }
        const glm::mat4& GetProjectViewMatrix() const { return m_ProjectionViewMatrix; }

        const glm::vec3& GetPosition() const { return m_Position; }

        Camera() = default;
    };

    class OrthographicCamera : public Camera {

        float m_Rotation = 0;

        void RecalculateViewMatrix() {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                                 glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

            m_View = glm::inverse(transform);
            CalculateProjectionViewMatrix();
        }

        void CalculateProjectionViewMatrix() {
            m_ProjectionViewMatrix = m_Projection * m_View;
        }

    public:

        OrthographicCamera(const glm::vec3& position) {
            m_Position = position;

            int displayWidth, displayHeight;

            Display::GetWidth(&displayWidth);
            Display::GetHeight(&displayHeight);

            const float aspectRatio = (float) displayWidth / (float) displayHeight;

            m_Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
            m_View = glm::mat4(1.0f);
            
            RecalculateViewMatrix();
        }

        void Move(const glm::vec3& deltaPosition) {
            m_Position += deltaPosition;
            RecalculateViewMatrix();
        }

        void SetPosition(const glm::vec3& position) {
            m_Position = position;
            RecalculateViewMatrix();
        }

        void SetRotation(float rotation) {
            m_Rotation = rotation;
            RecalculateViewMatrix();
        }

        const float GetRotation() const { return m_Rotation; }
    };

    class PerspectiveCamera : public Camera {

        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float yaw = -90.0f;
        float pitch = 0.0f;
        float fov = 45.0f;

        void CalculateProjectionViewMatrix() {
            m_ProjectionViewMatrix = m_Projection * m_View;
        }

        void RecalculateViewMatrix() {
            m_View = glm::lookAt(m_Position, m_Position - cameraFront, cameraUp);
            CalculateProjectionViewMatrix();
        }

    public:
        PerspectiveCamera() {
            m_Position = { 0, 0, -3 };

            m_Projection = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.1f, 100.0f);

            RecalculateViewMatrix();
        }

        void Move(const glm::vec3& deltaPosition) {
            m_Position += deltaPosition;
            RecalculateViewMatrix();
        }

        void Rotate(float yawOffset, float pitchOffset) {
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

        void Rotate(const glm::vec2& rotation) {
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

        const glm::vec3& GetCameraFront() const { return cameraFront; }
        const glm::vec3& GetCameraUp() const { return cameraUp; }
 
    };
}