#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 textureCoord;
layout(location = 4) in float textureID;

uniform mat4 uViewProjectionMatrix;

out vec3 vPosition;
out vec3 vNormal;
out vec4 vColor;
out vec2 vTextureCoord;
out float vTextureID;

void main() {
    vPosition = position;
    vNormal = normal;
    vColor = color;
    vTextureCoord = textureCoord;
    vTextureID = textureID;

    gl_Position = uViewProjectionMatrix * vec4(position, 1.0);
}