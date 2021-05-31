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

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoords) : Position(position), Normal(normal), Color({1, 1, 1, 1}), TextureCoord(textureCoords), TextureID(0) {}
        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoords, float textureID) : Position(position), Normal(normal), Color({1, 1, 1, 1}), TextureCoord(textureCoords), TextureID(textureID) {}

        bool operator==(const Vertex& v) {
            return (v.Position == Position) && (v.Normal == Normal) && (v.Color == Color) && (v.TextureCoord == TextureCoord) && (v.TextureID == TextureID);
        }

        bool Equals(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoord, float textureID) {
            return (Position == position) && (Normal == normal) && (TextureCoord == textureCoord) && (TextureID == textureID);
        } 
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
        GLuint OboID = 0;

        size_t IndexCount = 0;
        size_t TextureIndex = 1;

        Vertex* Vertices = nullptr;
        Vertex* VertexPtr = nullptr;

        uint32_t* Indices = nullptr;
        uint32_t* IndexPtr = nullptr;

        glm::vec3* Offsets = nullptr;
        glm::vec3* OffsetPtr = nullptr;

        int* Textures = nullptr;
    };

    class Renderer {

        static Camera* s_Camera;

        static void UpdateBatchData();
        static void Render();

        static void AllocateVertices(uint32_t amount);
        static void AllocateIndices(uint32_t amount);
        static void AllocateObject();

    public:

        static void OnCreate(Camera* camera);
        static void OnDestroy();

        static void StartBatch();
        static void EndBatch();

        // TODO: To be replaced/removed
        static int AddTexture(const Texture& texture);

        // Draw Methods
 
        static void DrawMesh(Mesh& mesh, const glm::vec3 positionOffset = {0, 0, 0});

        // Utility Functions
        static void LoadOBJFile(const char* filepath, Mesh* mesh, float textureID = 0);
    };
}