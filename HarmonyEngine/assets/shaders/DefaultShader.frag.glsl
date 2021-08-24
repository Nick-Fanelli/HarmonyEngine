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
    float Intensity;

};

uniform PointLight[MAX_LIGHT_COUNT] uPointLights;
uniform int uPointLightCount;

uniform float uAmbientIntensity;

uniform vec3 uViewDirection;

out vec4 out_Color;

void main() {

    // vec3 ambient = vec3(1.0, 1.0, 1.0) * uAmbientIntensity;
    // vec3 diffuse = vec3(1.0, 1.0, 1.0);
    // vec3 specular = vec3(1.0, 1.0, 1.0);

    // for(int i = 0; i < uPointLightCount; i++) {
    //     PointLight light = uPointLights[i];

    //     // Ambient
    //     ambient += light.Color;

    //     // Diffuse
    //     vec3 norm = normalize(vNormal);
    //     vec3 lightDir = normalize(light.Position - vPosition);
    //     float diff = max(dot(norm, lightDir), 0.0);

    // //     vec3 diffuse = max(dot(vNormal, lightDir), 0.0) * vColor.rgb * uPointLights[i].Color;


    //     diffuse += diff * vColor * light.Color;

    //     // Specular
    //     float specularStrength = 0.5;
    //     vec3 viewDir = normalize(uViewDirection - vPosition);
    //     vec3 reflectDir = reflect(-lightDir, norm);
    //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //     // specular += specularStrength * spec * light.Color;
    // }

    vec3 lightingResult = vColor.rgb * uAmbientIntensity;

    for(int i = 0; i < uPointLightCount; i++) {
        // float dist = length(uPointLights[i].Position - vPosition);

        // Normalize the Normal
        vec3 normal = normalize(vNormal);
        vec3 adjustedColor = uPointLights[i].Color * uPointLights[i].Intensity;

        // if(distance > 250.0f) { // TODO: Radius

        // Diffuse
        vec3 lightDir = normalize(uPointLights[i].Position - vPosition);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * vColor.rgb * adjustedColor;

        // Specular
        vec3 halfwayDir = normalize(lightDir * uViewDirection);
        float spec = pow(max(dot(normalize(normal), halfwayDir), 0.0), 16.0);
        vec3 specular = adjustedColor * spec;

        // Attenuation
        // float attenuation = 1.0 / (1.0 + LINEAR * dist + QUADRATIC * dist * dist);

        // diffuse *= attenuation;
        // specular *= attenuation;
        lightingResult += diffuse + specular;

    }

    out_Color = vec4(lightingResult, 1.0) * texture(uTextures[int(vTextureID)], vTextureCoord);
}
