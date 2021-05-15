#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Display.h"

namespace HarmonyEngine {

    class OrthographicCamera {

        glm::mat4 m_Projection;
        glm::mat4 m_View;
        glm::mat4 m_ProjectionViewMatrix;

        glm::vec3 m_Position;
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

        OrthographicCamera(const glm::vec3& position = {0, 0, 0}) {
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

        const glm::mat4& GetProjctionMatrix() const { return m_Projection; }
        const glm::mat4& GetViewMatrix() const { return m_View; }
        const glm::mat4& GetProjectViewMatrix() const { return m_ProjectionViewMatrix; }

        const glm::vec3& GetPosition() const { return m_Position; }
        const float GetRotation() const { return m_Rotation; }
    };
}