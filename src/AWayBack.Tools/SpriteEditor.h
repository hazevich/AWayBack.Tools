#pragma once
#include <vector>

#include "ImGuiExt.h"
#include "AWayBack/Graphics/Texture.h"
#include "SpriteAtlas.h"

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
        SpriteAtlas* _spriteAtlas = new SpriteAtlas();
        SpriteAtlas* _newSpriteAtlas = new SpriteAtlas();

        Texture2D* _newTexture = nullptr;
        Texture2D* _texture = nullptr;

        int32_t _sliceStart = 0, _sliceEnd = 1;
        int32_t _gridWidth = 0, _gridHeight = 0;

        bool _isUniformCellSizeControl = true;
        bool _isNewSpriteAtlasRequested = false;

        ImGui::ImVec2i _cellSize = ImGui::ImVec2i(32, 32);

        void RenderCanvas();

        void RenderControls();

        void RenderSpriteAtlasHeader();

        void RenderSlicingControls();
        void RenderGridSequenceSlicingControls();

        void RenderSprites();

        void RenderNewSpriteAtlasModal();

        void CalculateGridSize();

        void SyncData();
    };
}
