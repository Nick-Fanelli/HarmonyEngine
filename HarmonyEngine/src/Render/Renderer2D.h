#pragma once

#include "harmonypch.h"

#include <glm/glm.hpp>

#include "Texture.h"
#include "Camera.h"
#include "Framebuffer.h"
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

    struct RendererStats {

        static void Begin() {
            CurrentBatchCount = 0;
            CurrentVertexCount = 0;
            CurrentIndexCount = 0;
            CurrentTextureCount = 0;
        }

        static void End() {
            BatchCount = CurrentBatchCount;
            VertexCount = CurrentVertexCount;
            IndexCount = CurrentIndexCount;
            TextureCount = CurrentTextureCount;
        }

        static void DrawImGUIStats(bool* open) {
            ImGui::Begin("Renderer Stats", open);

            static const GLubyte* openglVersion = glGetString(GL_VERSION);
            static const GLubyte* graphicsVendor = glGetString(GL_VENDOR);
            static const GLubyte* graphicsRenderer = glGetString(GL_RENDERER);

            ImGui::Text("Rendering Type: OpenGL");
            ImGui::Text("OpenGL Version: %s", openglVersion);

            ImGui::Separator();

            ImGui::Text("Graphics Vendor: %s", graphicsVendor);
            ImGui::Text("Graphics Card: %s", graphicsRenderer);

            ImGui::Separator();

            ImGui::Text("V-Sync Enabled: %s", Display::IsVSyncEnabled() ? "true" : "false");
            ImGui::Text("Current FPS: %d", Display::GetFps());

            ImGui::Separator();

            ImGui::Text("Texture Allocations: %zu", Texture::GetNumTextureAllocations());

            ImGui::Separator();

            ImGui::Text("Batch Count: %zu", BatchCount);
            ImGui::Text("Vertex Count: %zu", VertexCount);
            ImGui::Text("Index Count: %zu", IndexCount);
            ImGui::Text("Texture Count: %zu", TextureCount);

            ImGui::End();
        }

    private:

        friend class Renderer2D;

        static size_t BatchCount;
        static size_t CurrentBatchCount;

        static size_t VertexCount;
        static size_t CurrentVertexCount;

        static size_t IndexCount;
        static size_t CurrentIndexCount;

        static size_t TextureCount;
        static size_t CurrentTextureCount;
    };

    class Renderer2D {

        static void Render();
        static void UpdateBatchVertexData();
        static void AllocateVertices(int amount);
        static void AllocateTexture();

    public:

        static void OnCreate();
        static void OnDestroy();

        static void StartBatch();
        static void EndBatch();

        static void DrawQuad(Transform& transform, const glm::vec4& color);
        static void DrawQuad(Transform& transform, const glm::vec4& color, AssetHandle<Texture>& texture);
        static void DrawQuad(Transform& transform, const glm::vec4& color, AssetHandle<Texture>& texture, const glm::vec2& topLeftCoord, const glm::vec2& bottomRightCoord);
    };

}