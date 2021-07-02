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

        Vector2 operator+(Vector2 other)
        {
            return Vector2(X + other.X, Y + other.Y);
        }

        Vector2 operator-(Vector2 other)
        {
            return Vector2(X - other.X, Y - other.Y);
        }
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
        std::string Folder;
        std::string Name;
        std::string TextureName;
        std::vector<Sprite> Sprites;
    };

    struct SpriteAtlasData
    {
        std::string Name;
        std::string Folder;
        std::string TexturePath;
    };
}
