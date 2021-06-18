#include "MasterRenderer.h"

using namespace HarmonyEngine;

static bool s_UseFramebuffer = false;
static Framebuffer s_Framebuffer;

Framebuffer* MasterRenderer::GetFramebuffer() { return &s_Framebuffer; }

void MasterRenderer::SetUseFramebuffer(bool value) {
    s_UseFramebuffer = value;
    if(s_UseFramebuffer) s_Framebuffer.OnCreate();
}

bool MasterRenderer::GetUseFramebuffer() { return s_UseFramebuffer; }

void MasterRenderer::Begin() {
    if(s_UseFramebuffer)
        s_Framebuffer.Bind();

    // Clear Code
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
}

void MasterRenderer::End() {
    if(s_UseFramebuffer)
        s_Framebuffer.Unbind();
}