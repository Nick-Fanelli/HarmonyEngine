#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Texture.h"

namespace HarmonyEngine {

    static const std::array<glm::vec4, 4> DefaultColor = {
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1)
    };

    struct Vertex {

        glm::vec3 Position;
        glm::vec4 Color; 
        glm::vec2 TextureCoord;
        float TextureID;

        Vertex() = default;
        Vertex(const Vertex&) = default;
        Vertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoord, float textureID) 
        : Position(position), Color(color), TextureCoord(textureCoord), TextureID(textureID) {}

    };

    struct Quad {

        Vertex V0;
        Vertex V1;
        Vertex V2;
        Vertex V3;

        Quad() = default;

        Quad(const glm::vec3& position, const glm::vec2& scale = { 1, 1 }, const std::array<glm::vec4, 4>& colorArray = DefaultColor, int textureID = 0) : 
        V0(position, colorArray[0], {0, 0}, textureID),
        V1(glm::vec3(position.x, position.y + scale.y, position.z), colorArray[1], {0, 1}, textureID),
        V2(glm::vec3(position.x + scale.x, position.y + scale.y, position.z), colorArray[2], {1, 1}, textureID),
        V3(glm::vec3(position.x + scale.x, position.y, position.z), colorArray[3], {1, 0}, textureID)
        {} 

    };

    struct RenderBatch {
        GLuint VaoID = 0;
        GLuint VboID = 0;
        GLuint EboID = 0;

        size_t IndexCount = 0;
        size_t TextureIndex = 1;

        Vertex* Vertices = nullptr;
        Vertex* VertexPtr = nullptr; 

        int* Textures = nullptr;
    };

    namespace Renderer2D {

        static const size_t MaxQuadCount = 20000;
        static const size_t MaxVertexCount = MaxQuadCount * 4;
        static const size_t MaxIndexCount = MaxQuadCount * 6;

        static Shader* s_Shader;
        static RenderBatch s_Batch;

        static void Render() {
            s_Shader->Bind();
            // s_Shader->AddUniformMat4("cameraViewProjectionMatrix", nullptr);
            s_Shader->AddUniformIntArray("uTextures", s_Batch.TextureIndex, s_Batch.Textures);

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

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.EboID); // Bind the EBO

            glDrawElements(GL_TRIANGLES, s_Batch.IndexCount, GL_UNSIGNED_INT, 0); // Draw the Elements

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the EBO
            
            // Disable al the Vertex Attrib Pointers
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);

            glBindVertexArray(0); // Unbind the VAO

        }

        static void UpdateBatchVertexData() {
            GLsizeiptr size = (uint8_t*) s_Batch.VertexPtr - (uint8_t*) s_Batch.Vertices;

            glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Batch.Vertices);
        }

        void OnCreate(Shader* shader) {
            // Check to make sure that OnCreate method wasn't already called
            if(s_Batch.Vertices != nullptr) {
                Log::Error("Vertices array was not equal to nullptr, exiting Renderer::OnCreate()");
                return;
            }

            int maxTextureCount = OpenGLUtils::GetGUPMaxTextureSlots();

            s_Batch = RenderBatch();
            s_Batch.Vertices = new Vertex[MaxVertexCount];
            s_Batch.VertexPtr = s_Batch.Vertices;

            s_Batch.Textures = new int[maxTextureCount];
            s_Shader = shader;

            // Bind the VAO
            glGenVertexArrays(1, &s_Batch.VaoID);
            glBindVertexArray(s_Batch.VaoID); // Bind Vertex Array

            // Bind the Vertices
            glGenBuffers(1, &s_Batch.VboID);
            glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);

            // Push Data to GPU
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Position));
            glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Color));
            glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, TextureCoord));
            glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, TextureID));

            glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind Buffer

            uint32_t indices[MaxIndexCount];
            uint32_t offset = 0;

            for(size_t i = 0; i < MaxIndexCount; i += 6) {
                // First Triangle
                indices[i] = offset;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;

                // Second Triangle
                indices[i + 3] = offset + 2;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset;

                offset += 4; // Four Vertices Per Quad
            }

            // Bind the indices
            glGenBuffers(1, &s_Batch.EboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.EboID); // Bind the indices buffer
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind Buffer
            glBindVertexArray(0); // Unbind Vertex Array
        }

        void StartBatch() {
            s_Batch.IndexCount = 0;
            s_Batch.VertexPtr = s_Batch.Vertices;
        }

        void EndBatch() {
            Renderer2D::UpdateBatchVertexData();
            Renderer2D::Render();
        }

        static void AllocateVertices(int amount) {
            uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

            // If there isn't enough space, start a new render batch!
            if(vertexCount + amount >= MaxVertexCount) {
                Renderer2D::EndBatch(); // End the current batch
                Renderer2D::StartBatch(); // Start a new batch
            }
        }

        void OnDestroy() {
            if(s_Batch.Vertices == nullptr) {
                Log::Warn("Vertices array was already nullptr, exiting Renderer::OnDestroy()");
                return;
            }

            glDeleteVertexArrays(1, &s_Batch.VaoID);
            glDeleteBuffers(1, &s_Batch.VboID);
            glDeleteBuffers(1, &s_Batch.EboID);

            delete[] s_Batch.Vertices;
            delete[] s_Batch.Textures;

            s_Batch.Vertices = nullptr;
            s_Batch.Textures = nullptr;
        }

        int AddTexture(const Texture& texture) {
            // TODO: Check to make sure that we aren't exceeding the max texture amount
            if(texture.GetTextureID() == -1) {
                Log::Error("Texture : " + std::string(texture.GetFilepath()) +
                       " has not been initialized and can not be added to the render batch!");
                return 0;
            }

            s_Batch.Textures[s_Batch.TextureIndex] = texture.GetTextureID();
            s_Batch.TextureIndex++;

            return s_Batch.Textures[s_Batch.TextureIndex - 1];
        }

        void DrawQuad(Quad& quad) {
            Renderer2D::AllocateVertices(4);

            *s_Batch.VertexPtr = quad.V0;
            s_Batch.VertexPtr++;

            *s_Batch.VertexPtr = quad.V1;
            s_Batch.VertexPtr++;

            *s_Batch.VertexPtr = quad.V2;
            s_Batch.VertexPtr++;

            *s_Batch.VertexPtr = quad.V3;
            s_Batch.VertexPtr++;

            s_Batch.IndexCount += 6; // Six indices per quad
        }

    }

}