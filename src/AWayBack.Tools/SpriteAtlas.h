#pragma once
#include <vector>
#include <string>
#include <filesystem>

#include "AWayBack/Graphics/Texture.h"

namespace AWayBack
{
    const inline char* SpriteAtlasExtension = ".atlas";

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

        Vector2 operator-(Vector2 other) const
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
        SpriteAtlas(std::string folder, std::string name, std::vector<Sprite> sprites, Texture2D* texture);
        SpriteAtlas(const SpriteAtlas& spriteAtlas) = delete;
        ~SpriteAtlas();

        SpriteAtlas& operator=(const SpriteAtlas&) = delete;

        std::string Folder;
        std::string Name;
        std::vector<Sprite> Sprites;
        Texture2D* Texture;
    };

    struct SpriteAtlasData
    {
        std::string Name;
        std::string Folder;
        std::string TexturePath;
    };

    inline std::string getTextureName(const SpriteAtlasData& spriteAtlasData)
    {
        return std::filesystem::path(spriteAtlasData.TexturePath).filename().string();
    }

    inline std::filesystem::path getTexturePath(const SpriteAtlasData& spriteAtlasData)
    {
        auto textureName = std::filesystem::path(spriteAtlasData.TexturePath).filename();
        return spriteAtlasData.Folder / textureName;
    }

    template<typename TAtlas>
    std::filesystem::path getFullPath(const TAtlas& atlas)
    {
        return std::filesystem::path(atlas.Folder) / atlas.Name;
    }

    SpriteAtlas* LoadSpriteAtlasFromFile(const std::string& spriteAtlasPath);
    void SaveSpriteAtlas(const SpriteAtlas& spriteAtlas);
}
