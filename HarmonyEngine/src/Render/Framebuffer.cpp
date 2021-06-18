#include "Framebuffer.h"

#include "../Core/Display.h"

using namespace HarmonyEngine;

void Framebuffer::OnCreate() {

    // Create the framebuffer
    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer);

    // Create the texture
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Display::GetAspectWidth(), Display::GetAspectHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // Bind the texture to the framebuffer
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

    // Create the renderbuffer
    glGenRenderbuffers(1, &m_Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Display::GetAspectWidth(), Display::GetAspectHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);
}

void Framebuffer::OnDestroy() {
    if(m_Framebuffer == 0) // Already Destroyed
        return;

    glDeleteFramebuffers(1, &m_Framebuffer);
    glDeleteRenderbuffers(1, &m_Renderbuffer);
    glDeleteTextures(1, &m_TextureID);

    // Set the framebuffer to zero so Framebuffer::OnDestroy() doesn't get called twice
    m_Framebuffer = 0; 
}

void Framebuffer::Bind() {
    static const glm::vec2& displayAspect = Display::GetRawDisplayAspect();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer);
    glViewport(0, 0, displayAspect.x, displayAspect.y);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}