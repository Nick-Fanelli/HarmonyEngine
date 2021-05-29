#pragma once

#include "harmonypch.h"

#include "../Render/Renderer.h"

namespace HarmonyEngine {

    class Terrain {

        static constexpr int VertexCount = 128;

        Mesh m_Mesh;

    public:
        Terrain(float size = 10) {
            for(int i = 0; i < VertexCount; i++){
                for(int j = 0; j < VertexCount; j++){
                    m_Mesh.Vertices.push_back(Vertex({
                        (float) j / (VertexCount - 1) * size,
                        0,
                        (float) i / (VertexCount - 1) * size
                    }, {
                        0, 1, 0
                    }, {
                        (float) j / (VertexCount - 1),
                        (float) i / (VertexCount - 1)
                    }));
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