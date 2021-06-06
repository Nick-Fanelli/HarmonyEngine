#pragma once

#include "harmonypch.h"

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
    const char* Filepath;
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;

    Mesh() = default;

    Mesh(const char* filepath) : Filepath(filepath) {}

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) :
    Filepath(""), Vertices(vertices), Indices(indices) {}
};