#version 400 core

layout(location = 0) in vec3 position;

uniform mat4 uViewProjectionMatrix;

void main() {
    gl_Position = uViewProjectionMatrix * vec4(position, 1.0);
}