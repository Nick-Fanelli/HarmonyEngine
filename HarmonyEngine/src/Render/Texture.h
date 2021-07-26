#pragma once

#include "harmonypch.h"

#include <glad/glad.h>

namespace HarmonyEngine {

    class Texture {

        GLuint m_TextureID = 0;

    protected:
        std::filesystem::path m_Filepath = "";
        int m_Width, m_Height;

        bool m_Created = false;

    public:

        Texture() = default;
        Texture(const std::filesystem::path& filepath) : m_Filepath(filepath) {}

        void Create();
        void Delete();

        void Create(const std::filesystem::path& filepath) {
            m_Filepath = filepath;

            Texture::Create();
        }

        void Bind() {
            if(m_TextureID == 0) {
                  Log::Warn("Texture ID of image '" + std::string(m_Filepath) + "' holds a textureID pointer of -1!\n"
                      "\tThis may result in the texture not binding or an OpenGL error!!\n"
                      "\tThe textureID pointer should hold the memory location of the texture!");
            }

            glBindTexture(GL_TEXTURE_2D, m_TextureID);
        }

        const GLuint& GetTextureID() const { return m_TextureID; }
        const std::filesystem::path& GetFilepath() const { return m_Filepath; }

        bool IsCreated() { return m_TextureID != 0; }

        const int GetWidth() const { return m_Width; }
        const int GetHeight() const { return m_Height; }
    };
}