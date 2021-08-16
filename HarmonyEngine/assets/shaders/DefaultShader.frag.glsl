#version 410 core

#define MAX_LIGHT_COUNT 32

const float QUADRATIC = 1.8;
const float LINEAR = 0.7;

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec2 vTextureCoord;
in float vTextureID;

uniform sampler2D[&MAX_TEXTURE_COUNT] uTextures;

struct PointLight {

    vec3 Position;
    vec3 Color;

};

uniform PointLight[MAX_LIGHT_COUNT] uPointLights;
uniform int uPointLightCount;

uniform vec3 uViewDirection;

out vec4 out_Color;

void main() {

    vec3 lighting = vColor.rgb * 0.1; // 0.1 is the hard-coded ambient value

    for(int i = 0; i < uPointLightCount; i++) {
        float dist = length(uPointLights[i].Position - vPosition);

        // if(distance > 250.0f) { // TODO: Radius
        vec3 lightDir = normalize(uPointLights[i].Position - vPosition);
        vec3 diffuse = max(dot(vNormal, lightDir), 0.0) * vColor.rgb * uPointLights[i].Color;

        vec3 halfwayDir = normalize(lightDir * uViewDirection);
        float spec = pow(max(dot(vNormal, halfwayDir), 0.0), 16.0);
        vec3 specular = uPointLights[i].Color * spec;

        // float attenuation = 1.0 / (1.0 + LINEAR * dist + QUADRATIC * dist * dist);

        // diffuse *= attenuation;
        // specular *= attenuation;
        lighting += diffuse + specular;

            // Attenuation
        // }

    }

    out_Color = vec4(lighting, 1.0) * texture(uTextures[int(vTextureID)], vTextureCoord);
}
