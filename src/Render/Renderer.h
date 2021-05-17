#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Renderer2D.h"
#include "Camera.h"

namespace HarmonyEngine {

    struct Mesh {
        GLuint VaoID;
        GLuint IboID;
        size_t IndexCount;

        Mesh() = default;
        Mesh(const Mesh&) = default;

        Mesh(GLfloat* vertices, int verticesSize, GLuint* indices, int indicesSize) {
            IndexCount = indicesSize / sizeof(GLuint);

            glGenVertexArrays(1, &VaoID);
            glBindVertexArray(VaoID);

            GLuint vboID;

            glGenBuffers(1, &vboID);
            glBindBuffer(GL_ARRAY_BUFFER, vboID);

            glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*) 0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &IboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IboID); 
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

            glBindVertexArray(0);
        }
    };

    namespace Renderer {

        static Shader* s_Shader;
        static Camera* s_Camera;

        void OnCreate(Camera* camera, Shader* shader) {
            s_Camera = camera;
            s_Shader = shader;
        }

        void Render(const Mesh& mesh, const glm::vec3& positionOffset = {0, 0, 0}) {
            s_Shader->Bind();
            s_Shader->AddUniformMat4("uViewProjectionMatrix", s_Camera->GetProjectViewMatrix());
            s_Shader->AddUniformVec3("uPositionOffset", positionOffset);

            // Log::Info(mesh.IndexCount);

            glBindVertexArray(mesh.VaoID);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IboID);
            glDrawElements(GL_TRIANGLES, mesh.IndexCount, GL_UNSIGNED_INT, 0);
        }
    }

}