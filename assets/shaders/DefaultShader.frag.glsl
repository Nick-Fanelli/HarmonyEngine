#version 400 core

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec2 vTextureCoord;
in float vTextureID;

uniform vec3 uLightPosition;

// The $KEYWORD$ means that something will be replaced at runtime!
uniform sampler2D[16] uTextures;

out vec4 out_Color;

void main() {

    float ambientStength = 0.2;
    vec3 lightColor = vec3(1, 1, 1);
    vec3 ambient = ambientStength * lightColor;

    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vPosition);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vPosition;
    vNormal;

    int textureID = int(vTextureID);

    if(textureID == 0) {
        out_Color = vColor * vec4((ambient + diffuse), 1.0);
    } else {
        out_Color = vColor * vec4((ambient + diffuse), 1.0) * texture(uTextures[textureID], vTextureCoord);
    }
}
