#include "MasterRenderer.h"

#include "Renderer2D.h"

using namespace HarmonyEngine;

Camera* MasterRenderer::CameraPtr = nullptr;

static bool s_UseFramebuffer = false;
static Framebuffer s_Framebuffer;

static glm::vec4 s_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

Framebuffer* MasterRenderer::GetFramebuffer() { return &s_Framebuffer; }

void MasterRenderer::SetClearColor(const glm::vec4& color) {
    s_ClearColor = color;
}

void MasterRenderer::OnCreate(Camera* cameraPtr) {

    CameraPtr = cameraPtr;

    Renderer2D::OnCreate();
}

void MasterRenderer::OnDestroy() {
    Renderer2D::OnDestroy();
}

void MasterRenderer::SetUseFramebuffer(bool value) {
    s_UseFramebuffer = value;
    if(s_UseFramebuffer) s_Framebuffer.OnCreate();
}

bool MasterRenderer::GetUseFramebuffer() { return s_UseFramebuffer; }

void MasterRenderer::Begin() {
    if(s_UseFramebuffer)
        s_Framebuffer.Bind();

    RendererStats::Begin();

    // Clear Code
    glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::End() {
    if(s_UseFramebuffer)
        s_Framebuffer.Unbind();

    RendererStats::End();
}