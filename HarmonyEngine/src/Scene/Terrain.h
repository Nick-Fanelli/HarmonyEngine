#pragma once

#include "harmonypch.h"

#include "../Render/Renderer.h"

namespace HarmonyEngine {

    class Terrain {

        static constexpr int VertexCount = 128;
        static constexpr float Amplitude = 10.0f;
        static constexpr int Octaves = 3;
        static constexpr float Roughness = 0.3f;

        int m_Seed;

        Mesh m_Mesh;

        float GetNoise(int x, int z) {
            srand(x * 49632 + z * 325176 + m_Seed);
            return -1 + static_cast <float> (rand() / (static_cast<float> (RAND_MAX/2)));
        }

        float Interpolate(float a, float b, float blend) {
            double theta = blend * M_PI;
            float f = (float) (1.0f - cos(theta)) * 0.5f;
            return a * (1.0f - f) + b * f;
        }

        float GetInterpolatedNoise(float x, float z) {
            int intX = (int) x;
            int intZ = (int) z;
            float fracX = x - intX;
            float fracZ = z - intZ;

            float v1 = GetSmoothNoise(intX, intZ);
            float v2 = GetSmoothNoise(intX + 1, intZ);
            float v3 = GetSmoothNoise(intX, intZ + 1);
            float v4 = GetSmoothNoise(intX + 1, intZ + 1);

            float i1 = Interpolate(v1, v2, fracX);
            float i2 = Interpolate(v3, v4, fracX);
            return Interpolate(i1, i2, fracZ);
        }

        float GetSmoothNoise(int x, int z) {
            float corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / 16.0f;
            float sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / 8.0f;
            float center = GetNoise(x, z) / 4.0f;
            return corners + sides + center;
        }

        float GetHeight(int x, int z) {
            float total = 0;
            float d = (float) pow(2, Octaves - 1);
            
            for(int i = 0; i < Octaves; i++) {
                float freq = (float) (pow(2, i) / d);
                float amp = (float) pow(Roughness, i) * Amplitude;
                total += GetInterpolatedNoise(x * freq, z * freq) * amp;
            }
            
            return total; 
        }

        glm::vec3 CalculateNormal(int x, int z) {
            float heightL = GetHeight(x - 1, z);
            float heightR = GetHeight(x + 1, z);
            float heightD = GetHeight(x, z - 1);
            float heightU = GetHeight(x, z + 1);

            glm::vec3 normal{heightL - heightR, 2.0f, heightD - heightU};
            return glm::normalize(normal);
        }

    public:
        Terrain(float size = 128, float textureID = 0) {
            srand((unsigned) time(0));
            m_Seed = rand() % 1000000000;

            for(int i = 0; i < VertexCount; i++){
                for(int j = 0; j < VertexCount; j++){
                    m_Mesh.Vertices.push_back(Vertex({
                        (float) j / (VertexCount - 1) * size,
                        GetHeight(j, i),
                        (float) i / (VertexCount - 1) * size
                    }, CalculateNormal(j, i), {
                        (float) j / (VertexCount - 1),
                        (float) i / (VertexCount - 1)
                    }, textureID));
                }
            }

            for(int gz = 0; gz < VertexCount - 1; gz++){
                for(int gx = 0; gx < VertexCount - 1; gx++){
                    int topLeft = (gz * VertexCount) + gx;
                    int topRight = topLeft + 1;
                    int bottomLeft = ((gz + 1) * VertexCount) + gx;
                    int bottomRight = bottomLeft + 1;

                    m_Mesh.Indices.push_back(topLeft);
                    m_Mesh.Indices.push_back(bottomLeft);
                    m_Mesh.Indices.push_back(topRight);
                    m_Mesh.Indices.push_back(topRight);
                    m_Mesh.Indices.push_back(bottomLeft);
                    m_Mesh.Indices.push_back(bottomRight);
                }
            }
        }

        Mesh& GetMesh() { return m_Mesh; }

    };

}