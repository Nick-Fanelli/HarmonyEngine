#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Renderer2D.h"
#include "Camera.h"

namespace HarmonyEngine::Renderer {

    static Shader* s_Shader;
    static Camera* s_Camera;

    void OnCreate(Camera* camera, Shader* shader) {
        s_Camera = camera;
        s_Shader = shader;
    }

    void Render(GLuint& vao, GLuint& ibo, int indexCount) {
        s_Shader->Bind();
        s_Shader->AddUniformMat4("uViewProjectionMatrix", s_Camera->GetProjectViewMatrix());

        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

}