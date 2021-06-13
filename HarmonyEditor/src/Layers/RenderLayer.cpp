#include "RenderLayer.h"

#include <Render/Renderer2D.h>

void RenderLayer::OnCreate(Camera* camera) {
    m_CameraPtr = camera;

    Renderer2D::OnCreate(m_CameraPtr);
}

void RenderLayer::OnUpdate(float deltaTime) {
    Renderer2D::StartBatch();

    Renderer2D::DrawQuad({0, 0, 0}, {1, 1});

    Renderer2D::EndBatch();
}

void RenderLayer::OnDestroy() {
    Renderer2D::OnDestroy();
}