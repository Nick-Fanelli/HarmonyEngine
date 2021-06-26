#version 410 core

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec2 vTextureCoord;
in float vTextureID;
in mat4 vModel;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uAmbientStrength;

// The &KEYWORD means that something will be replaced at runtime!
uniform sampler2D[&MAX_TEXTURE_COUNT] uTextures;

out vec4 out_Color;

void main() {

    vec3 ambient = uAmbientStrength * uLightColor;

    vec3 norm = mat3(transpose(inverse(vModel))) * vNormal;
    vec3 lightDirection = normalize(uLightPosition - vPosition);

    float diff = max(dot(norm, lightDirection), 0);
    vec3 diffuse = diff * uLightColor;

    vTextureCoord;
    vTextureID;

    out_Color = vColor * vec4((ambient + diffuse), 1.0) * texture(uTextures[int(vTextureID)], vTextureCoord);
}
