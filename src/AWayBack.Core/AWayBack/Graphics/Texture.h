#pragma once

#include <cstdint>
#include <string>

namespace AWayBack
{
    class Texture2D
    {
    public:
        virtual ~Texture2D() = default;

        virtual uint32_t GetTextureId() = 0;
        virtual uint32_t GetWidth() = 0;
        virtual uint32_t GetHeight() = 0;

        static Texture2D* FromFile(const std::string& file);
    };
}
