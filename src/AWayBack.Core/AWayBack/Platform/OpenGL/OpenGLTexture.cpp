#include "OpenGLTexture.h"

#include "stb_image.h"
#include "glad/glad.h"
#include "filesystem"

namespace AWayBack
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& file)
    {
        std::filesystem::path filePath(file);
        _name = filePath.filename().string();
        _fullPath = file;

        int32_t width, height;
        uint8_t* data = stbi_load(file.c_str(), &width, &height, nullptr, 4);
       
        _width = width;
        _height = height;

        glGenTextures(1, &_textureId);

        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &_textureId);
    }
}
