﻿#pragma once
#include <vector>

#include "ImGuiExt.h"
#include "AWayBack/Graphics/Texture.h"
#include "SpriteAtlas.h"
#include "SpriteEditorController.h"

namespace AWayBack
{
    class SpriteEditor
    {
    public:
        ~SpriteEditor();

        void LoadTexture(const std::string& texturePath);
        void LoadSpriteAtlas(const std::string& spriteAtlasPath);
        void CreateNewSpriteAtlas();
        void Render();
    private:
        SpriteEditorController _controller;

        bool _isNewSpriteAtlasRequested;
        SpriteAtlasData _spriteAtlasData;

        bool _isUniformCellSizeControl = true;

        void RenderCanvas();

        void RenderControls();

        void RenderSpriteAtlasHeader();

        void RenderSlicingControls();
        void RenderGridSequenceSlicingControls();

        void RenderSprites();

        void RenderNewSpriteAtlasModal();

        void RenderSelectedSprite();
    };
}
