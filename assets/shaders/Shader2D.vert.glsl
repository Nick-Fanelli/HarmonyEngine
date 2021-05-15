#version 400 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in float textureID;

uniform mat4 cameraViewProjectionMatrix;

out vec4 vColor;
out vec2 vTextureCoord;
out float vTextureID;

void main() {
    vColor = color;
    vTextureCoord = textureCoord;
    vTextureID = textureID;

    gl_Position = cameraViewProjectionMatrix * vec4(position, 1.0, 1.0);
}