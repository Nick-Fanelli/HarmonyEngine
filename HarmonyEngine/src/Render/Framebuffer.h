#include "harmonypch.h"

namespace HarmonyEngine {

    class Framebuffer {

        GLuint m_Framebuffer = 0;
        GLuint m_TextureID = 0;
        GLuint m_Renderbuffer = 0;

    public:

        Framebuffer() = default;
        Framebuffer(const Framebuffer& other) 
        : m_Framebuffer(other.m_Framebuffer), m_TextureID(other.m_TextureID), m_Renderbuffer(other.m_Renderbuffer) {}

        ~Framebuffer() { OnDestroy(); }

        void OnCreate();
        void OnDestroy();

        void Bind();
        void Unbind();

    };

}