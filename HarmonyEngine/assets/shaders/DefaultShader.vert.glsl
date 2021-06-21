#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 textureCoord;
layout(location = 4) in float textureID;
layout(location = 5) in float objectID;

uniform mat4 uViewProjectionMatrix;
uniform mat4 uTransformations[&MAX_OBJECT_COUNT];

out vec3 vPosition;
out vec3 vNormal; 
out vec4 vColor;
out vec2 vTextureCoord;
out float vTextureID;
out mat4 vModel;

void main() {
    int modelID = int(objectID);

    vPosition = position;
    vNormal = normal;
    vColor = color;
    vTextureCoord = textureCoord;
    vTextureID = textureID;
    vModel = uTransformations[modelID];

    gl_Position = uViewProjectionMatrix * uTransformations[modelID] * vec4(position, 1.0);
}