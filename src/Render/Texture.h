#pragma once

#include "harmonypch.h"

#include <GL/glew.h>
#include <GLUT/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

        void Create() {
            if (m_Filepath == nullptr) {
                Log::Warn("Can not create a texture without parameters!");
                return;
            }

            if(!FileUtils::FileExists(m_Filepath)) {
                Log::Error(std::string("Can not load texture at: ") + m_Filepath);
                return;
            }

            // Generate Texture on GPU
            glGenTextures(1, &m_TextureID);
            glBindTexture(GL_TEXTURE_2D, m_TextureID);

            // Setup Image Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_FILTER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_FILTER);

            // Setup STB_Image
            stbi_set_flip_vertically_on_load(true);

            int channels;

            stbi_uc* image = stbi_load(m_Filepath, &m_Width, &m_Height, &channels, 0); // Load the image

            if (image != nullptr) {
                if (channels == 3) { // RGB Channels
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                } else if (channels == 4) { // RBGA Channels
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                } else {
                    Log::Error(std::string("Could not load image ") + m_Filepath +
                            std::string(", with unknown number of channels: ") + std::to_string(channels));
                }

                stbi_image_free(image);
            }

            Unbind();
        }

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