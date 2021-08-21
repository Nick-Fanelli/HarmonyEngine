#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Framebuffer.h"

#include "../Core/Assets.h"

namespace HarmonyEngine {

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

        glm::mat4* Offsets = nullptr;
        glm::mat4* OffsetPtr = nullptr;

        int* Textures = nullptr;
    };

    class Renderer {

        static Scene* s_ScenePtr;

        static void UpdateBatchData();
        static void Render();

        static void AllocateVertices(uint32_t amount);
        static void AllocateIndices(uint32_t amount);
        static void AllocateObject();
        static void AllocateTexture();

    public:

        static void OnCreate();
        static void OnDestroy();

        static void StartBatch();
        static void EndBatch();

        static void SetScenePtr(Scene* scenePtr) { s_ScenePtr = scenePtr; }

        // TODO: Remove
        static void DrawImGuiEnvironmentLighting(bool* open);

        // Draw Methods
        static void DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh);
        static void DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh, const glm::vec4& color);
        static void DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh, AssetHandle<Texture>& texture);
        static void DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh, AssetHandle<Texture>& texture, const glm::vec4& color);
    };
}