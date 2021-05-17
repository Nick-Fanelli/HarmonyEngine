#version 400 core

layout(location = 0) in vec3 position;

uniform mat4 uViewProjectionMatrix;
uniform vec3 uPositionOffset;

void main() {
    gl_Position = uViewProjectionMatrix * vec4(position + uPositionOffset, 1.0);
}