#pragma once

#include "harmonypch.h"

#include "Texture.h"

namespace HarmonyEngine {

    class Renderer2D {

        static void Render();
        static void UpdateBatchVertexData();
        static void AllocateVertices(int amount);

    public:

        static const int AddTexture(const Texture& texture);

        static void OnCreate();
        static void StartBatch();
        static void EndBatch();
        static void OnDestroy();

    };

}