#pragma once

#include "harmonypch.h"

#include <GL/glew.h>

namespace HarmonyEngine {

    class Texture {

        GLuint m_TextureID = -1;

        const char* m_Filepath = nullptr;
        int m_Width, m_Height;

        bool m_Created = false;

    public:

        Texture() = default;
        Texture(const char* filepath) : m_Filepath(filepath) {}

        void Create();
        void Delete();

        void Create(const char* filepath) {
            m_Filepath = filepath;

            Texture::Create();
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