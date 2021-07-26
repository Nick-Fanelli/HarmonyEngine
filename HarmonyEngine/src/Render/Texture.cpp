#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace HarmonyEngine;

static GLuint s_NullTextureID = -1;
static bool s_Setup = false;

void Texture::Create() {
    m_Created = true;

    if(!FileUtils::FileExists(m_Filepath.c_str())) {
        Log::FormatError("Can not load texture at: %s\n\tDoesn't Exist!\n\tStatus: Assigning null texture", m_Filepath.c_str());

        if(s_NullTextureID == -1) {
            glGenTextures(1, &s_NullTextureID);
            glBindTexture(GL_TEXTURE_2D, s_NullTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            uint32_t color = 0xff00ff;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &color);
        }

        m_TextureID = s_NullTextureID;

        return;
    }

    // Generate Texture on GPU
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Setup Image Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Setup STB_Image
    if(!s_Setup) { 
        stbi_set_flip_vertically_on_load(true);
        s_Setup = true;
    }

    int channels;

    unsigned char* data = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &channels, 0); // Load the image

    if (data != nullptr) {
        if (channels == 3) { // RGB Channels
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (channels == 4) { // RBGA Channels
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            Log::Error(std::string("Could not load image ") + m_Filepath.c_str() +
                    std::string(", with unknown number of channels: ") + std::to_string(channels));
        }

        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    if(m_TextureID == s_NullTextureID)
        return;
    glDeleteTextures(1, &m_TextureID);
}