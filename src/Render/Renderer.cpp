#include "Renderer.h"

#include "Shader.h"

using namespace HarmonyEngine;

static const size_t MaxObjectCount = 500; // 500
static const size_t MaxVertexCount = 80000; // 80,000
static const size_t MaxIndexCount = 120000; // 120,000

static RenderBatch s_Batch;
static Shader s_Shader;

Camera* Renderer::s_Camera = nullptr;

void Renderer::OnCreate(Camera* camera) {
    if(s_Batch.Vertices != nullptr) {
        Log::Error("Vertices array was not equal to nullptr, exiting Renderer::OnCreate()");
        return;
    }

    s_Camera = camera;

    auto maxTextureCount = OpenGLUtils::GetGUPMaxTextureSlots();

    std::unordered_map<std::string, std::string> replacements;
    replacements["MAX_TEXTURE_COUNT"] = std::to_string(maxTextureCount);

    s_Shader = Shader("assets/shaders/DefaultShader.vert.glsl", "assets/shaders/DefaultShader.frag.glsl", replacements);
    s_Shader.Create();

    s_Batch = RenderBatch();

    s_Batch.Vertices = new Vertex[MaxVertexCount];
    s_Batch.VertexPtr = s_Batch.Vertices;

    s_Batch.Indices = new uint32_t[MaxIndexCount];
    s_Batch.IndexPtr = s_Batch.Indices;

    s_Batch.Offsets = new glm::vec3[MaxObjectCount];
    s_Batch.OffsetPtr = s_Batch.Offsets;

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

    glGenBuffers(1, &s_Batch.OboID);
    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.OboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MaxObjectCount, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(5, 1);

#ifdef HARMONY_DEBUG_ENABLED
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind Index Buffer
    glBindVertexArray(0); // Unbind Vertex Array
#endif
}

void Renderer::UpdateBatchData() {
    size_t vertexSize = (s_Batch.VertexPtr - s_Batch.Vertices) * sizeof(Vertex);
    size_t offsetSize = (s_Batch.OffsetPtr - s_Batch.Offsets) * sizeof(glm::vec3);
    size_t indexSize = s_Batch.IndexCount * sizeof(uint32_t);

    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.VboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, s_Batch.Vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, s_Batch.Indices);

    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.OboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, offsetSize, s_Batch.Offsets);
}

void Renderer::Render() {
    s_Shader.Bind();
    s_Shader.AddUniformMat4("uViewProjectionMatrix", s_Camera->GetProjectViewMatrix());
    s_Shader.AddUniformIntArray("uTextures", s_Batch.TextureIndex, s_Batch.Textures);
    s_Shader.AddUniformVec3("uLightPosition", {0, 20, -20});

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Batch.IboID); // Bind the indices
    glBindBuffer(GL_ARRAY_BUFFER, s_Batch.OboID);

    glDrawElementsInstanced(GL_TRIANGLES, s_Batch.IndexCount, GL_UNSIGNED_INT, 0, s_Batch.OffsetPtr - s_Batch.Offsets); // Draw the elements

#ifdef HARMONY_DEBUG_ENABLED
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the Ibo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Disable all the Vertex Attrib Pointers
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);

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
    if(s_Batch.Vertices == nullptr && s_Batch.Indices == nullptr) {
        Log::Warn("Vertices array was already nullptr, exiting Renderer::OnDestroy()");
        return;
    }

    glDeleteVertexArrays(1, &s_Batch.VaoID);
    glDeleteBuffers(1, &s_Batch.VboID);
    glDeleteBuffers(1, &s_Batch.IboID);
    glDeleteBuffers(1, &s_Batch.OboID);

    delete[] s_Batch.Vertices;
    delete[] s_Batch.Indices;
    delete[] s_Batch.Textures;
    delete[] s_Batch.Offsets;

    s_Batch.Vertices = nullptr;
    s_Batch.Indices = nullptr;
    s_Batch.Textures = nullptr;
    s_Batch.Offsets = nullptr;
}

int Renderer::AddTexture(const Texture& texture) {
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

void Renderer::DrawMesh(Mesh& mesh, const glm::vec3 positionOffset) {
    size_t indexOffset = s_Batch.VertexPtr - s_Batch.Vertices;

    AllocateVertices(mesh.Vertices.size());
    AllocateIndices(mesh.Indices.size());
    AllocateObject();

    for(auto& vertex : mesh.Vertices) {
        *s_Batch.VertexPtr = vertex;
        s_Batch.VertexPtr++;
    }

    for(auto index : mesh.Indices) {
        *s_Batch.IndexPtr = index + indexOffset;
        s_Batch.IndexPtr++;
    }

    *s_Batch.OffsetPtr = positionOffset;
    s_Batch.OffsetPtr++;
}

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
                Log::Error("Could not load obj file format!");
                return;
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