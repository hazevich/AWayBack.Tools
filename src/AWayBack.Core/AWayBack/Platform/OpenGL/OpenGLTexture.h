#pragma once

#include "AWayBack/Graphics/Texture.h"

namespace AWayBack
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& file);

        uint32_t GetTextureId() override { return _textureId; }
        uint32_t GetWidth() override { return _width; }
        uint32_t GetHeight() override { return _height; }
    private:
        uint32_t _textureId;
        uint32_t _width, _height;
    };
}