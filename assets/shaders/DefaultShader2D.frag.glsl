#version 400 core

in vec4 vColor;
in vec2 vTextureCoord;
in float vTextureID;

uniform sampler2D[16] uTextures;

out vec4 out_Color;

void main() {

    int textureID = int(vTextureID);

    if(textureID == 0) {
        out_Color = vColor;
    } else {
        out_Color * texture(uTextures[textureID], vTextureCoord);
    }

}