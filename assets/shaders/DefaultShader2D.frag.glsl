#version 410 core

in vec4 vColor;
in vec2 vTextureCoord;
in float vTextureID;

// The &KEYWORD means that something will be replaced at runtime!
uniform sampler2D[&MAX_TEXTURE_COUNT] uTextures;

out vec4 out_Color;

void main() {

    vec4 finalColor = vColor * texture(uTextures[int(vTextureID)], vTextureCoord);

    out_Color = finalColor;

}