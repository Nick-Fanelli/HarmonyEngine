#pragma once

#include "harmonypch.h"

#include <GL/glew.h>

namespace HarmonyEngine {

    static const int MIN_FILTER = GL_LINEAR;
    static const int MAG_FILTER = GL_LINEAR;

    class Texture {

        GLuint m_TextureID = -1;

        const char* m_Filepath = nullptr;
        int m_Width, m_Height;

    public:

        Texture() = default;
        Texture(const Texture&) = default;
        Texture(const char* filepath) : m_Filepath(filepath) {}

        void Create();

        void Create(const char* filepath) {
            m_Filepath = filepath;

            Texture::Create();
        }

        static void Unbind() {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Bind() {
            if(m_TextureID == -1) {
                  Log::Warn("Texture ID of image '" + std::string(m_Filepath) + "' holds a textureID pointer of -1!\n"
                      "\tThis may result in the texture not binding or an OpenGL error!!\n"
                      "\tThe textureID pointer should hold the memory location of the texture!");
            }

            glBindTexture(GL_TEXTURE_2D, m_TextureID);
        }

        const GLuint& GetTextureID() const { return m_TextureID; }
        const char* GetFilepath() const { return m_Filepath; }

        const int GetWidth() const { return m_Width; }
        const int GetHeight() const { return m_Height; }
    };

}