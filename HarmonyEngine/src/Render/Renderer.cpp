#include "Renderer.h"

#include "Shader.h"
#include "Renderer2D.h"
#include "../Scene/Component.h"

#include "MasterRenderer.h"

using namespace HarmonyEngine;

static const size_t MaxObjectCount = 245; // 245
static const size_t MaxVertexCount = 80000; // 80,000
static const size_t MaxIndexCount = 120000; // 120,000

static RenderBatch s_Batch;
static Shader s_Shader;

static uint32_t s_MaxTextureCount;

static GLuint s_WhiteTexture;

Scene* Renderer::s_ScenePtr = nullptr;

static int* s_TextureSlots;

static glm::vec3 s_LightPosition = { 0, 15, 0 };
static glm::vec3 s_LightColor = { 1, 1, 1 };
static float s_AmbientStrength = 0.2f;

void Renderer::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    if(s_Batch.Vertices != nullptr) {
        Log::Error("Vertices array was not equal to nullptr, exiting Renderer::OnCreate()");
        return;
    }

    s_MaxTextureCount = OpenGLUtils::GetGUPMaxTextureSlots();

    std::unordered_map<std::string, std::string> replacements;
    replacements["MAX_TEXTURE_COUNT"] = std::to_string(s_MaxTextureCount);
    replacements["MAX_OBJECT_COUNT"] = std::to_string(MaxObjectCount);

    s_Shader = Shader("engineAssets/shaders/DefaultShader.vert.glsl", "engineAssets/shaders/DefaultShader.frag.glsl", replacements);
    s_Shader.Create();

    s_Batch = RenderBatch();

    s_Batch.Vertices = new Vertex[MaxVertexCount];
    s_Batch.VertexPtr = s_Batch.Vertices;

    s_Batch.Indices = new uint32_t[MaxIndexCount];
    s_Batch.IndexPtr = s_Batch.Indices;

    s_Batch.Offsets = new glm::mat4[MaxObjectCount];
    s_Batch.OffsetPtr = s_Batch.Offsets;

    s_Batch.Textures = new int[s_MaxTextureCount];

    glGenTextures(1, &s_WhiteTexture);
    glBindTexture(GL_TEXTURE_2D, s_WhiteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    s_Batch.Textures[0] = s_WhiteTexture;

    s_TextureSlots = new int[s_MaxTextureCount];

    for(uint32_t i = 0; i < s_MaxTextureCount; i++) {
        s_TextureSlots[i] = i; 
    }

    // Bind the VAO
    glGenVertexArrays(1, &s_Batch.VaoID);
    glBindVertexArray(s_Batch.VaoID);

    // Bind the Vertices
    glGenBuffers(1, &s_Batch.VboID);
    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);

    // Push data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Position));
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Normal));
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, Color));
    glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, TextureCoord));
    glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, TextureID));
    glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof(Vertex), (void*) offsetof(Vertex, RenderID));

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the Buffer

    // Bind the Indices
    glGenBuffers(1, &s_Batch.IboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxIndexCount, nullptr, GL_DYNAMIC_DRAW);

#if HARMONY_DEBUG
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind Index Buffer
    glBindVertexArray(0); // Unbind Vertex Array
#endif
}

void Renderer::UpdateBatchData() {
    HARMONY_PROFILE_FUNCTION();

    size_t vertexSize = (s_Batch.VertexPtr - s_Batch.Vertices) * sizeof(Vertex);
    size_t indexSize = s_Batch.IndexCount * sizeof(uint32_t);

    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, s_Batch.Vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, s_Batch.Indices);
}

void Renderer::Render() {

    HARMONY_PROFILE_FUNCTION();

    size_t objectCount = s_Batch.OffsetPtr - s_Batch.Offsets;

    s_Shader.Bind();
    s_Shader.AddUniformMat4("uViewProjectionMatrix", MasterRenderer::CameraPtr->GetProjectViewMatrix());
    s_Shader.AddUniformIntArray("uTextures", s_Batch.TextureIndex, s_TextureSlots);
    s_Shader.AddUniformMat4Array("uTransformations", objectCount, s_Batch.Offsets);

    // TODO: Remove s_LightPosition, s_LightColor and s_AmbientStrength

    // Bind the textures
    for(int i = 0; i < s_Batch.TextureIndex; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, s_Batch.Textures[i]);
    }

    // Bind the lights
    int lightCount = 0;

    if(s_ScenePtr != nullptr) {

        s_ScenePtr->ForEachEntityWithTransform<PointLightComponent>([&](TransformComponent& transform, PointLightComponent& lightComponent) {
            std::string positionCapture = "uPointLights[" + std::to_string(lightCount) + "].Position";
            std::string colorCapture = "uPointLights[" + std::to_string(lightCount) + "].Color";

            s_Shader.AddUniformVec3(positionCapture.c_str(), transform.Transform.Position);
            s_Shader.AddUniformVec3(colorCapture.c_str(), lightComponent.Hue);

            lightCount++;
        });
    }

    s_Shader.AddUnformInt("uPointLightCount", lightCount);
    s_Shader.AddUniformVec3("uViewDirection", MasterRenderer::CameraPtr->GetPosition());

    s_Shader.AddUniformFloat("uAmbientIntensity", s_ScenePtr->GetAmbientIntensity());

    glBindVertexArray(s_Batch.VaoID); // Bind the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID); // Bind the indices
    glDrawElements(GL_TRIANGLES, s_Batch.IndexCount, GL_UNSIGNED_INT, 0); // Draw the elements

#if HARMONY_DEBUG
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the Ibo
    glBindVertexArray(0);

    s_Shader.Unbind();
#endif
}

void Renderer::AllocateVertices(uint32_t amount) {
    RendererStats::CurrentVertexCount += amount;

    uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

    if(vertexCount + amount > MaxVertexCount) {
        Renderer::EndBatch();
        Renderer::StartBatch();
    }
}

void Renderer::AllocateIndices(uint32_t amount) {
    if(s_Batch.IndexCount + amount > MaxIndexCount) {
        Renderer::EndBatch();
        Renderer::StartBatch();
    }

    s_Batch.IndexCount += amount;
    RendererStats::CurrentIndexCount += amount;
}

void Renderer::AllocateObject() {
    uint32_t offsetCount = s_Batch.OffsetPtr - s_Batch.Offsets;

    if(offsetCount + 1 > MaxObjectCount) {
        Renderer::EndBatch();
        Renderer::StartBatch();
    }
}

void Renderer::AllocateTexture() {
    if(s_Batch.TextureIndex >= s_MaxTextureCount) {
        Renderer::EndBatch();
        Renderer::StartBatch();
    }
}

void Renderer::StartBatch() {
    s_Batch.IndexCount = 0;

    s_Batch.VertexPtr = s_Batch.Vertices;
    s_Batch.IndexPtr = s_Batch.Indices;
    s_Batch.OffsetPtr = s_Batch.Offsets;

    s_Batch.TextureIndex = 1;

    RendererStats::CurrentBatchCount++;
}

void Renderer::EndBatch() {
    Renderer::UpdateBatchData();
    Renderer::Render();
}

void Renderer::OnDestroy() {

    HARMONY_PROFILE_FUNCTION();

    if(s_Batch.Vertices == nullptr && s_Batch.Indices == nullptr) {
        Log::Warn("Vertices array was already nullptr, exiting Renderer::OnDestroy()");
        return;
    }

    glDeleteVertexArrays(1, &s_Batch.VaoID);
    glDeleteBuffers(1, &s_Batch.VboID);
    glDeleteBuffers(1, &s_Batch.IboID);
    
    glDeleteTextures(1, &s_WhiteTexture);

    delete[] s_Batch.Vertices;
    delete[] s_Batch.Indices;
    delete[] s_Batch.Textures;
    delete[] s_Batch.Offsets;

    delete[] s_TextureSlots;

    s_Batch.Vertices = nullptr;
    s_Batch.Indices = nullptr;
    s_Batch.Textures = nullptr;
    s_Batch.Offsets = nullptr;
}

void Renderer::DrawImGuiEnvironmentLighting(bool* open) {
    ImGui::Begin("Environment", open);

    ImGui::DragFloat3("Light Position", glm::value_ptr(s_LightPosition));
    ImGui::ColorEdit3("Light Color", glm::value_ptr(s_LightColor));
    ImGui::DragFloat("Ambient Strength", &s_AmbientStrength, 0.1f, 0, 1);

    ImGui::End();
}

void Renderer::DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh) {

    HARMONY_PROFILE_FUNCTION();

    uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

    AllocateVertices(mesh->Vertices.size());
    AllocateIndices(mesh->Indices.size());
    AllocateObject();

    int offsetID = s_Batch.OffsetPtr - s_Batch.Offsets;

    std::memcpy(s_Batch.VertexPtr, mesh->Vertices.data(), sizeof(*mesh->Vertices.begin()) * mesh->Vertices.size());

    std::for_each(s_Batch.VertexPtr, s_Batch.VertexPtr + mesh->Vertices.size(), [&](Vertex& vertex) {
        vertex.RenderID = offsetID;
    });
    s_Batch.VertexPtr += mesh->Vertices.size();

    for(auto& index : mesh->Indices) {
        *s_Batch.IndexPtr = index + vertexCount;
        s_Batch.IndexPtr++;
    }

    *s_Batch.OffsetPtr = transform.GetTransformationMatrix();
    s_Batch.OffsetPtr++;
}

void Renderer::DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh, const glm::vec4& color) {

    HARMONY_PROFILE_FUNCTION();

    uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

    AllocateVertices(mesh->Vertices.size());
    AllocateIndices(mesh->Indices.size());
    AllocateObject();

    int offsetID = s_Batch.OffsetPtr - s_Batch.Offsets;

    std::memcpy(s_Batch.VertexPtr, mesh->Vertices.data(), sizeof(*mesh->Vertices.begin()) * mesh->Vertices.size());

    std::for_each(s_Batch.VertexPtr, s_Batch.VertexPtr + mesh->Vertices.size(), [&](Vertex& vertex) {
        vertex.RenderID = offsetID;
        vertex.Color = color;
    });
    s_Batch.VertexPtr += mesh->Vertices.size();

    for(auto& index : mesh->Indices) {
        *s_Batch.IndexPtr = index + vertexCount;
        s_Batch.IndexPtr++;
    }

    *s_Batch.OffsetPtr = transform.GetTransformationMatrix();
    s_Batch.OffsetPtr++;
}

void Renderer::DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh, AssetHandle<Texture>& texture) {

    HARMONY_PROFILE_FUNCTION();

    if(texture->GetTextureID() == 0.0f) {
        DrawMesh(transform, mesh);
        return;
    }

    uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

    AllocateVertices(mesh->Vertices.size());
    AllocateIndices(mesh->Indices.size());
    AllocateObject();
    AllocateTexture();

    float textureIndex = 0.0f;

    for(uint32_t i = 1; i < s_Batch.TextureIndex; i++) {
        if(s_Batch.Textures[i] == texture->GetTextureID()) {
            textureIndex = (float) i;
            break;
        }
    }

    if(textureIndex == 0.0f) {
        textureIndex = (float) s_Batch.TextureIndex;
        s_Batch.Textures[s_Batch.TextureIndex] = texture->GetTextureID();
        s_Batch.TextureIndex++;
    }

    int offsetID = s_Batch.OffsetPtr - s_Batch.Offsets;

    std::memcpy(s_Batch.VertexPtr, mesh->Vertices.data(), sizeof(*mesh->Vertices.begin()) * mesh->Vertices.size());

    std::for_each(s_Batch.VertexPtr, s_Batch.VertexPtr + mesh->Vertices.size(), [&](Vertex& vertex) {
        vertex.RenderID = offsetID;
        vertex.TextureID = textureIndex;
    });
    s_Batch.VertexPtr += mesh->Vertices.size();

    for(auto& index : mesh->Indices) {
        *s_Batch.IndexPtr = index + vertexCount;
        s_Batch.IndexPtr++;
    }

    *s_Batch.OffsetPtr = transform.GetTransformationMatrix();
    s_Batch.OffsetPtr++;
}

void Renderer::DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh, AssetHandle<Texture>& texture, const glm::vec4& color) {

    HARMONY_PROFILE_FUNCTION();

    if(texture->GetTextureID() == 0.0f) {
        DrawMesh(transform, mesh);
        return;
    }

    uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

    AllocateVertices(mesh->Vertices.size());
    AllocateIndices(mesh->Indices.size());
    AllocateObject();
    AllocateTexture();

    float textureIndex = 0.0f;

    for(uint32_t i = 1; i < s_Batch.TextureIndex; i++) {
        if(s_Batch.Textures[i] == texture->GetTextureID()) {
            textureIndex = (float) i;
            break;
        }
    }

    if(textureIndex == 0.0f) {
        textureIndex = (float) s_Batch.TextureIndex;
        s_Batch.Textures[s_Batch.TextureIndex] = texture->GetTextureID();
        s_Batch.TextureIndex++;
    }

    int offsetID = s_Batch.OffsetPtr - s_Batch.Offsets;

    std::memcpy(s_Batch.VertexPtr, mesh->Vertices.data(), sizeof(*mesh->Vertices.begin()) * mesh->Vertices.size());

    std::for_each(s_Batch.VertexPtr, s_Batch.VertexPtr + mesh->Vertices.size(), [&](Vertex& vertex) {
        vertex.RenderID = offsetID;
        vertex.Color = color;
        vertex.TextureID = textureIndex;
    });
    s_Batch.VertexPtr += mesh->Vertices.size();

    for(auto& index : mesh->Indices) {
        *s_Batch.IndexPtr = index + vertexCount;
        s_Batch.IndexPtr++;
    }

    *s_Batch.OffsetPtr = transform.GetTransformationMatrix();
    s_Batch.OffsetPtr++;
}