#include "ObjectLoader.h"

using namespace HarmonyEngine;

// OBJ Loader
void OBJLoader::LoadOBJFile(const std::filesystem::path& filepath, Mesh* mesh, float textureID) {
    std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUvs;
    std::vector<glm::vec3> tempNormals;

    FILE* file = fopen(filepath.c_str(), "r");
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
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if(matches != 9) {
                int newMatches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);

                if(newMatches != 0) {
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