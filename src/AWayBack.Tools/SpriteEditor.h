﻿#pragma once
#include <vector>

#include "ImGuiExt.h"
#include "AWayBack/Graphics/Texture.h"

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
        std::string Name;
        std::string TextureName;
        std::vector<Sprite> Sprites;
    };

    class SpriteEditor
    {
    public:
        ~SpriteEditor();

        void LoadTexture(const std::string& texturePath);
        void LoadSpriteAtlas(const std::string& spriteAtlasPath);
        void Render();
    private:
        SpriteAtlas* _spriteAtlas = new SpriteAtlas();

        Texture2D* _newTexture = nullptr;
        Texture2D* _texture = nullptr;

        int32_t _sliceStart = 0, _sliceEnd = 1;
        int32_t _gridWidth = 0, _gridHeight = 0;

        bool _isUniformCellSizeControl = true;

        ImGui::ImVec2i _cellSize = ImGui::ImVec2i(32, 32);

        void RenderCanvas();

        void RenderControls();

        void RenderSpriteAtlasHeader();

        void RenderSlicingControls();
        void RenderGridSequenceSlicingControls();

        void RenderSprites();

        void CalculateGridSize();

        void SyncData();
    };
}
