#include "MasterRenderer.h"

#include "Renderer.h"
#include "Renderer2D.h"

using namespace HarmonyEngine;

static bool s_UseFramebuffer = false;
static Framebuffer s_Framebuffer;

Framebuffer* MasterRenderer::GetFramebuffer() { return &s_Framebuffer; }

void MasterRenderer::OnCreate(Camera* cameraPtr) {
    Renderer::OnCreate(cameraPtr);
    Renderer2D::OnCreate(cameraPtr);
}

void MasterRenderer::OnDestroy() {
    Renderer::OnDestroy();
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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::End() {
    if(s_UseFramebuffer)
        s_Framebuffer.Unbind();

    RendererStats::End();
}