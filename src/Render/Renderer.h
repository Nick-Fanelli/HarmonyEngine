#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Renderer2D.h"
#include "Camera.h"

namespace HarmonyEngine {

    struct Vertex {

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Color; 
        glm::vec2 TextureCoord;
        float TextureID;

        Vertex() = default;
        Vertex(const Vertex&) = default;

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec4& color, const glm::vec2& textureCoord, float textureID) 
        : Position(position), Normal(normal), Color(color), TextureCoord(textureCoord), TextureID(textureID) {}

        Vertex(const glm::vec3& position) : Position(position), Normal({0, 0, 0}), Color({1, 1, 1, 1}), TextureCoord({0, 0}), TextureID(0) {}

        Vertex(const glm::vec3& position, const glm::vec3& normal) : Position(position), Normal(normal), Color({1, 1, 1, 1}), TextureCoord({0, 0}), TextureID(0) {}
    };

    struct Mesh {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

        Mesh() = default;

        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) :
        Vertices(vertices), Indices(indices) {}
    };

    struct RenderBatch {
        GLuint VaoID = 0;
        GLuint VboID = 0;
        GLuint IboID = 0;

        size_t IndexCount = 0;
        size_t TextureIndex = 1;

        Vertex* Vertices = nullptr;
        Vertex* VertexPtr = nullptr;

        uint32_t* Indices = nullptr;
        uint32_t* IndexPtr = nullptr;

        int* Textures = nullptr;
    };

    namespace Renderer {

        static const size_t MaxVertexCount = 80000; // 80,000
        static const size_t MaxIndexCount = 120000; // 120,000

        static Shader s_Shader;
        static Camera* s_Camera;

        static RenderBatch s_Batch;

        static void Render() {
            s_Shader.Bind();
            s_Shader.AddUniformMat4("uViewProjectionMatrix", s_Camera->GetProjectViewMatrix());
            s_Shader.AddUniformIntArray("uTextures", s_Batch.TextureIndex, s_Batch.Textures);
            s_Shader.AddUniformVec3("uLightPosition", {0, 20, 20});

            for(int i = 0; i < s_Batch.TextureIndex; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, s_Batch.Textures[i]);
            }

            glBindVertexArray(s_Batch.VaoID); // Bind the VAO

            // Enable all the Vertex Attrib Pointers
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID); // Bind the indices

            glDrawElements(GL_TRIANGLES, s_Batch.IndexCount, GL_UNSIGNED_INT, 0); // Draw the elements

#ifdef HARMONY_DEBUG_UNBIND
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the Ibo

            // Disable all the Vertex Attrib Pointers
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);

            glBindVertexArray(0);
#endif
        }

        static void UpdateBatchData() {
            size_t vertexSize = (s_Batch.VertexPtr - s_Batch.Vertices) * sizeof(Vertex);
            size_t indexSize = s_Batch.IndexCount * sizeof(uint32_t);

            glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, s_Batch.Vertices);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, s_Batch.Indices);
        }

        void OnCreate(Camera* camera) {
            if(s_Batch.Vertices != nullptr) {
                Log::Error("Vertices array was not equal to nullptr, exiting Renderer::OnCreate()");
                return;
            }

            s_Camera = camera;
            s_Shader = Shader("assets/shaders/DefaultShader.vert.glsl", "assets/shaders/DefaultShader.frag.glsl");
            s_Shader.Create();

            auto maxTextureCount = OpenGLUtils::GetGUPMaxTextureSlots();

            s_Batch = RenderBatch();

            s_Batch.Vertices = new Vertex[MaxVertexCount];
            s_Batch.VertexPtr = s_Batch.Vertices;

            s_Batch.Indices = new uint32_t[MaxIndexCount];
            s_Batch.IndexPtr = s_Batch.Indices;

            s_Batch.Textures = new int[maxTextureCount];

            // Bind the VAO
            glGenVertexArrays(1, &s_Batch.VaoID);
            glBindVertexArray(s_Batch.VaoID);

            // Bind the Vertices
            glGenBuffers(1, &s_Batch.VboID);
            glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);

            // Push data to GPU
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Position));
            glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Normal));
            glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Color));
            glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, TextureCoord));
            glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, TextureID));

            glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the Buffer

            // Bind the Indices
            glGenBuffers(1, &s_Batch.IboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxIndexCount, nullptr, GL_DYNAMIC_DRAW);

#ifdef HARMONY_DEBUG_UNBIND
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind Index Buffer
            glBindVertexArray(0); // Unbind Vertex Array
#endif
        }

        void StartBatch() {
            s_Batch.IndexCount = 0;
            
            s_Batch.VertexPtr = s_Batch.Vertices;
            s_Batch.IndexPtr = s_Batch.Indices;
        }

        void EndBatch() {
            Renderer::UpdateBatchData();
            Renderer::Render();
        }

        static void AllocateVertices(size_t amount) {
            size_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

            if(vertexCount + amount >= MaxVertexCount) {
                Renderer::EndBatch();
                Renderer::StartBatch();
            }
        }

        static void AllocateIndices(size_t amount) {
            if(s_Batch.IndexCount + amount >= MaxIndexCount) {
                Renderer::EndBatch();
                Renderer::StartBatch();
            } else {
                s_Batch.IndexCount += amount;
            }
        }

        void OnDestroy() {
            if(s_Batch.Vertices == nullptr && s_Batch.Indices == nullptr) {
                Log::Warn("Vertices array was already nullptr, exiting Renderer::OnDestroy()");
                return;
            }

            glDeleteVertexArrays(1, &s_Batch.VaoID);
            glDeleteBuffers(1, &s_Batch.VboID);
            glDeleteBuffers(1, &s_Batch.IboID);

            delete[] s_Batch.Vertices;
            delete[] s_Batch.Indices;
            delete[] s_Batch.Textures;

            s_Batch.Vertices = nullptr;
            s_Batch.Indices = nullptr;
            s_Batch.Textures = nullptr;
        }
 
        void DrawMesh(Mesh& mesh) {
            size_t indexOffset = s_Batch.VertexPtr - s_Batch.Vertices;

            AllocateVertices(mesh.Vertices.size());
            AllocateIndices(mesh.Indices.size());

            for(auto& vertex : mesh.Vertices) {
                *s_Batch.VertexPtr = vertex;
                s_Batch.VertexPtr++;
            }

            for(auto index : mesh.Indices) {
                *s_Batch.IndexPtr = index + indexOffset;
                s_Batch.IndexPtr++;
            }
        }

        // Utility Functions

        void LoadOBJFile(const char* filepath, Mesh* mesh) {
            
            
        }
    }


}