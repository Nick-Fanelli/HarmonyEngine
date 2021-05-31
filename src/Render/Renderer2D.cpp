#include "Renderer2D.h"

#include "Shader.h"

using namespace HarmonyEngine;

static const size_t MaxQuadCount = 20000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;

static RenderBatch2D s_Batch;
static Shader s_Shader;

Camera* Renderer2D::s_Camera = nullptr;

void Renderer2D::Render() {
    s_Shader.Bind();
    s_Shader.AddUniformMat4("uViewProjectionMatrix", s_Camera->GetProjectViewMatrix());
    s_Shader.AddUniformIntArray("uTextures", s_Batch.TextureIndex, s_Batch.Textures);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID); // Bind the EBO

    glDrawElements(GL_TRIANGLES, s_Batch.IndexCount, GL_UNSIGNED_INT, 0); // Draw the Elements

#ifdef HARMONY_DEBUG_UNBIND
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the EBO

    // Disable al the Vertex Attrib Pointers
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glBindVertexArray(0); // Unbind the VAO

    s_Shader.Unbind(); // Unbind the Shader
#endif
}

void Renderer2D::UpdateBatchVertexData() {
    GLsizeiptr size = (uint8_t*) s_Batch.VertexPtr - (uint8_t*) s_Batch.Vertices;

    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Batch.Vertices);
}

void Renderer2D::AllocateVertices(int amount) {
    uint32_t vertexCount = s_Batch.VertexPtr - s_Batch.Vertices;

    // If there isn't enough space, start a new render batch!
    if(vertexCount + amount >= MaxVertexCount) {
        Renderer2D::EndBatch(); // End the current batch
        Renderer2D::StartBatch(); // Start a new batch
    }
}

void Renderer2D::OnCreate(Camera* camera) {
    // Check to make sure that OnCreate method wasn't already called
    if(s_Batch.Vertices != nullptr) {
        Log::Error("Vertices array was not equal to nullptr, exiting Renderer2D::OnCreate()");
        return;
    }

    s_Camera = camera;

    auto maxTextureCount = OpenGLUtils::GetGUPMaxTextureSlots();

    std::unordered_map<std::string, std::string> replacements;
    replacements["MAX_TEXTURE_COUNT"] = std::to_string(maxTextureCount);

    s_Shader = Shader("assets/shaders/DefaultShader2D.vert.glsl", "assets/shaders/DefaultShader2D.frag.glsl", replacements);
    s_Shader.Create();

    s_Batch = RenderBatch2D();
    s_Batch.Vertices = new Vertex2D[MaxVertexCount];
    s_Batch.VertexPtr = s_Batch.Vertices;

    s_Batch.Textures = new int[maxTextureCount];

    // Bind the VAO
    glGenVertexArrays(1, &s_Batch.VaoID);
    glBindVertexArray(s_Batch.VaoID); // Bind Vertex Array

    // Bind the Vertices
    glGenBuffers(1, &s_Batch.VboID);
    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);

    // Push Data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex2D), (void*) offsetof(Vertex2D, Position));
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex2D), (void*) offsetof(Vertex2D, Color));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex2D), (void*) offsetof(Vertex2D, TextureCoord));
    glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(Vertex2D), (void*) offsetof(Vertex2D, TextureID));

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
    glGenBuffers(1, &s_Batch.IboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID); // Bind the indices buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

#ifdef HARMONY_DEBUG_UNBIND
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind Buffer
    glBindVertexArray(0); // Unbind Vertex Array
#endif
}

void Renderer2D::StartBatch() {
    s_Batch.IndexCount = 0;
    s_Batch.VertexPtr = s_Batch.Vertices;
}

void Renderer2D::EndBatch() {
    Renderer2D::UpdateBatchVertexData();
    Renderer2D::Render();
}

void Renderer2D::OnDestroy() {
    if(s_Batch.Vertices == nullptr) {
        Log::Warn("Vertices array was already nullptr, exiting Renderer2D::OnDestroy()");
        return;
    }

    glDeleteVertexArrays(1, &s_Batch.VaoID);
    glDeleteBuffers(1, &s_Batch.VboID);
    glDeleteBuffers(1, &s_Batch.IboID);

    delete[] s_Batch.Vertices;
    delete[] s_Batch.Textures;

    s_Batch.Vertices = nullptr;
    s_Batch.Textures = nullptr;
}

int Renderer2D::AddTexture(const Texture& texture) {
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

void Renderer2D::DrawQuad(Quad& quad) {
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
