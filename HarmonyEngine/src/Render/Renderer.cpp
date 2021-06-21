#include "Renderer.h"

#include "Shader.h"

using namespace HarmonyEngine;

static const size_t MaxObjectCount = 500; // 500
static const size_t MaxVertexCount = 80000; // 80,000
static const size_t MaxIndexCount = 120000; // 120,000

static RenderBatch s_Batch;
static Shader s_Shader;

static uint32_t s_MaxTextureCount;

static GLuint s_WhiteTexture;

Camera* Renderer::s_Camera = nullptr;

static int* s_TextureSlots;

void Renderer::OnCreate(Camera* camera) {
    HARMONY_PROFILE_FUNCTION();

    if(s_Batch.Vertices != nullptr) {
        Log::Error("Vertices array was not equal to nullptr, exiting Renderer::OnCreate()");
        return;
    }

    s_Camera = camera;

    s_MaxTextureCount = OpenGLUtils::GetGUPMaxTextureSlots();

    std::unordered_map<std::string, std::string> replacements;
    replacements["MAX_TEXTURE_COUNT"] = std::to_string(s_MaxTextureCount);

    s_Shader = Shader("assets/shaders/DefaultShader.vert.glsl", "assets/shaders/DefaultShader.frag.glsl", replacements);
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

    glGenBuffers(1, &s_Batch.OboID);
    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.OboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MaxObjectCount, nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) sizeof(glm::vec4));
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (sizeof(glm::vec4) * 2));
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (sizeof(glm::vec4) * 3));
    
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Unbind Framebuffer

#if HARMONY_DEBUG
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind Index Buffer
    glBindVertexArray(0); // Unbind Vertex Array
#endif
}

void Renderer::UpdateBatchData() {
    HARMONY_PROFILE_FUNCTION();

    size_t vertexSize = (s_Batch.VertexPtr - s_Batch.Vertices) * sizeof(Vertex);
    size_t offsetSize = (s_Batch.OffsetPtr - s_Batch.Offsets) * sizeof(glm::mat4);
    size_t indexSize = s_Batch.IndexCount * sizeof(uint32_t);

    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, s_Batch.Vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, s_Batch.Indices);

    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.OboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, offsetSize, s_Batch.Offsets);
}

void Renderer::Render() {

    HARMONY_PROFILE_FUNCTION();

    s_Shader.Bind();
    s_Shader.AddUniformMat4("uViewProjectionMatrix", s_Camera->GetProjectViewMatrix());
    s_Shader.AddUniformIntArray("uTextures", s_Batch.TextureIndex, s_TextureSlots);
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
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);
    glEnableVertexAttribArray(8);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID); // Bind the indices
    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.OboID);

    glDrawElementsInstanced(GL_TRIANGLES, s_Batch.IndexCount, GL_UNSIGNED_INT, 0, s_Batch.OffsetPtr - s_Batch.Offsets); // Draw the elements

#if HARMONY_DEBUG
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the Ibo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Disable all the Vertex Attrib Pointers
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glDisableVertexAttribArray(7);
    glDisableVertexAttribArray(8);

    glBindVertexArray(0);
#endif
}

void Renderer::AllocateVertices(uint32_t amount) {
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
    glDeleteBuffers(1, &s_Batch.OboID);
    
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

void Renderer::DrawMesh(Transform& transform, AssetHandle<Mesh>& mesh) {

    HARMONY_PROFILE_FUNCTION();

    AllocateVertices(mesh->Vertices.size());
    AllocateIndices(mesh->Indices.size());
    AllocateObject();

    std::memcpy(s_Batch.VertexPtr, mesh->Vertices.data(), sizeof(*mesh->Vertices.begin()) * mesh->Vertices.size());
    s_Batch.VertexPtr += mesh->Vertices.size();

    std::memcpy(s_Batch.IndexPtr, mesh->Indices.data(), sizeof(*mesh->Indices.begin()) * mesh->Indices.size());
    s_Batch.IndexCount += mesh->Indices.size();

    *s_Batch.OffsetPtr = transform.GetTransformationMatrix();
    s_Batch.OffsetPtr++;
}

// void Renderer::DrawMesh(AssetHandle<Mesh>& mesh, const glm::vec3& positionOffset) {
//     size_t indexOffset = s_Batch.VertexPtr - s_Batch.Vertices;

//     AllocateVertices(mesh->Vertices.size());
//     AllocateIndices(mesh->Indices.size());
//     AllocateObject();

//     for(auto& vertex : mesh->Vertices) {
//         *s_Batch.VertexPtr = vertex;
//         s_Batch.VertexPtr++;
//     }

//     for(auto index : mesh->Indices) {
//         *s_Batch.IndexPtr = index + indexOffset;
//         s_Batch.IndexPtr++;
//     }

//     *s_Batch.OffsetPtr = positionOffset;
//     s_Batch.OffsetPtr++;
// }

// void Renderer::DrawMesh(AssetHandle<Mesh>& mesh, AssetHandle<Texture>& texture, const glm::vec3& positionOffset) {
//     if(texture->GetTextureID() == 0) {
//         DrawMesh(mesh, positionOffset);
//         return;
//     }

//     size_t indexOffset = s_Batch.VertexPtr - s_Batch.Vertices;

//     AllocateVertices(mesh->Vertices.size());
//     AllocateIndices(mesh->Indices.size());
//     AllocateObject();
//     AllocateTexture();

//     float textureIndex = 0.0f;

//     for(uint32_t i = 1; i < s_Batch.TextureIndex; i++) {
//         if(s_Batch.Textures[i] == texture->GetTextureID()) {
//             textureIndex = (float) i;
//             break;
//         }
//     }

//     if(textureIndex == 0.0f) {
//         textureIndex = (float) s_Batch.TextureIndex;
//         s_Batch.Textures[s_Batch.TextureIndex] = texture->GetTextureID();
//         s_Batch.TextureIndex++;
//     }

//     for(auto& vertex : mesh->Vertices) {
//         // Assign new vertex with the textureID
//         Vertex v = Vertex(vertex);
//         v.TextureID = textureIndex;

//         *s_Batch.VertexPtr = v;
//         s_Batch.VertexPtr++;
//     }

//     for(auto index : mesh->Indices) {
//         *s_Batch.IndexPtr = index + indexOffset;
//         s_Batch.IndexPtr++;
//     }

//     *s_Batch.OffsetPtr = positionOffset;
//     s_Batch.OffsetPtr++;
// }

// Utility Functions
void Renderer::LoadOBJFile(const char* filepath, Mesh* mesh, float textureID) {
    std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUvs;
    std::vector<glm::vec3> tempNormals;

    FILE* file = fopen(filepath, "r");
    if(file == NULL) {
        Log::Error("Could not find and load obj file!");
        return;
    }

    while(true) {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF)
            break;

        if(strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            tempVertices.push_back(vertex);
        } else if(strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            tempUvs.push_back(uv);
        } else if(strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        } else if(strcmp(lineHeader, "f") == 0) {
            uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if(matches != 9) {
                Log::Info(vertexIndex[0]);
                int newMatches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                Log::Info(vertexIndex[0]);

                if(newMatches != 9) {
                    Log::Error("Could not load obj file format!");
                    return;
                }
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    uint32_t duplicateOffset = 0;

    bool isDuplicate = false;
    uint32_t duplicatePosition = 0;

    for(uint32_t i = 0; i < vertexIndices.size(); i++) {
        uint32_t vertexIndex = vertexIndices[i];
        uint32_t normalIndex = normalIndices[i];
        uint32_t uvIndex = uvIndices[i];

        glm::vec3 vertexPosition = tempVertices[vertexIndex - 1];
        glm::vec3 vertexNormal = tempNormals[normalIndex - 1];
        glm::vec2 vertexUv = tempUvs[uvIndex - 1];

        for(uint32_t i = 0; i < mesh->Vertices.size(); i++) {
            if(mesh->Vertices[i].Equals(vertexPosition, vertexNormal, vertexUv, textureID)) {
                duplicateOffset++;
                duplicatePosition = i;
                isDuplicate = true; 
                break;
            }
        }

        if(!isDuplicate) {
            mesh->Vertices.push_back(Vertex(vertexPosition, vertexNormal, vertexUv, textureID));
            mesh->Indices.push_back(i - duplicateOffset);
        } else {
            mesh->Indices.push_back(duplicatePosition);
            isDuplicate = false;
        }
    }
}