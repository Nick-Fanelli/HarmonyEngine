#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "../Core/Assets.h"

namespace HarmonyEngine {

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
        static void AllocateTexture();

    public:

        static void OnCreate(Camera* camera, GLuint* textureID);
        static void OnDestroy();

        static void StartBatch();
        static void EndBatch();

        // Draw Methods
        static void DrawMesh(AssetHandle<Mesh>& mesh, const glm::vec3& positionOffset = {0, 0, 0});
        static void DrawMesh(AssetHandle<Mesh>& mesh, AssetHandle<Texture>& texture, const glm::vec3& positionOffset = {0, 0, 0});

        // Utility Functions
        static void LoadOBJFile(const char* filepath, Mesh* mesh, float textureID = 0);

    };
}