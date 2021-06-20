#pragma once
#include <vector>

#include "AWayBack/Graphics/Texture.h"

namespace AWayBack
{
    struct Vector2
    {
        float X, Y;
    };

    struct Sprite
    {
        char* Name;
        Vector2 Min;
        Vector2 Max;
        Vector2 Origin;
    };

    struct SpriteAtlas
    {
        std::string Name = "";
        std::string TextureName = "";
        std::vector<Sprite> Sprites;
    };

    class SpriteEditor
    {
    public:
        void SetTexture(Texture2D* texture);
        void Render();
    private:
        SpriteAtlas _spriteAtlas{};
        Texture2D* _texture = nullptr;

        void RenderCanvas();
        void RenderControls();
        void RenderSpriteAtlasHeader();
    };
}
