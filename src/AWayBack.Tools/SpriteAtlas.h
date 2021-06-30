#pragma once
#include <vector>
#include <string>

namespace AWayBack
{
    struct Vector2
    {
        Vector2() = default;
        Vector2(float x, float y)
            : X(x), Y(y)
        {
            
        }

        float X, Y;
    };

    struct Sprite
    {
        std::string Name;
        Vector2 Min;
        Vector2 Max;
        Vector2 Origin;
    };

    struct SpriteAtlas
    {
        std::string Path;
        std::string Name;
        std::string TextureName;
        std::vector<Sprite> Sprites;
    };
}
