#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Texture.h"
#include "Camera.h"
#include "../Core/Assets.h"

namespace HarmonyEngine {

    static const std::array<glm::vec4, 4> DefaultColor = {
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1)
    };

    struct Vertex2D {

        glm::vec3 Position;
        glm::vec4 Color; 
        glm::vec2 TextureCoord;
        float TextureID;

        Vertex2D() = default;
        Vertex2D(const Vertex2D&) = default;
        Vertex2D(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoord, float textureID) 
        : Position(position), Color(color), TextureCoord(textureCoord), TextureID(textureID) {}
        Vertex2D(const glm::vec3& position) : Position(position), Color({1, 1, 1, 1}) {}

    };

    struct Quad {

        Vertex2D V0;
        Vertex2D V1;
        Vertex2D V2;
        Vertex2D V3;

        Quad() = default;

        Quad(const glm::vec3& position, const glm::vec2& scale = { 1, 1 }, const std::array<glm::vec4, 4>& colorArray = DefaultColor, int textureID = 0) : 
        V0(position, colorArray[0], {0, 0}, textureID),
        V1(glm::vec3(position.x, position.y + scale.y, position.z), colorArray[1], {0, 1}, textureID),
        V2(glm::vec3(position.x + scale.x, position.y + scale.y, position.z), colorArray[2], {1, 1}, textureID),
        V3(glm::vec3(position.x + scale.x, position.y, position.z), colorArray[3], {1, 0}, textureID)
        {} 

        Quad(const glm::vec3& position, const glm::vec2& scale = { 1, 1 }, const glm::vec4& color = { 1, 1, 1, 1}, int textureID = 0) :
        V0(position, color, {0, 0}, textureID),
        V1(glm::vec3(position.x, position.y + scale.y, position.z), color, {0, 1}, textureID),
        V2(glm::vec3(position.x + scale.x, position.y + scale.y, position.z), color, {1, 1}, textureID),
        V3(glm::vec3(position.x + scale.x, position.y, position.z), color, {1, 0}, textureID)
        {}

    };

    struct RenderBatch2D {
        GLuint VaoID = 0;
        GLuint VboID = 0;
        GLuint IboID = 0;

        size_t IndexCount = 0;
        size_t TextureIndex = 1;

        Vertex2D* Vertices = nullptr;
        Vertex2D* VertexPtr = nullptr; 

        uint32_t* Textures = nullptr;
    };

    struct RendererStats2D {

        friend class Renderer2D;

        static size_t BatchCount;

        static void Start() {
            CurrentBatchCount = 0;
        }

        static void End() {
            BatchCount = CurrentBatchCount;
        }

    private:
        static size_t CurrentBatchCount;

    };

    class Renderer2D {

        static Camera* s_Camera;

        static void Render();
        static void UpdateBatchVertexData();

    public:

        static void OnCreate(Camera* camera);
        static void OnDestroy();

        static void StartBatch();
        static void EndBatch();

        static void DrawQuad(const glm::vec3& position, const glm::vec2& scale);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, AssetHandle<Texture>& texture);
    };

}