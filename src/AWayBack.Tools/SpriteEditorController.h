#pragma once
#include "SpriteAtlas.h"
#include "ImGuiExt.h"

#include "optional"
#include "UndoRedo.h"

#include "AWayBack/Graphics/Texture.h"

namespace AWayBack
{
    enum struct SlicingType
    {
        GridSequence = 0,
        GridSelection = 1,
        Freehand = 2
    };

    struct SelectedRegion
    {
        ImVec2 Min, Max;
    };

    class SpriteEditorController
    {
    public:
        SpriteEditorController(UndoRedoHistory& undoRedoHistory);
        ~SpriteEditorController();

        int32_t SliceStart, SliceEnd;
        int32_t GridWidth, GridHeight;

        std::optional<int32_t> SelectedSpriteId = std::nullopt;

        SlicingType SlicingType;

        std::vector<int32_t> SelectedCells = std::vector<int32_t>();

        SelectedRegion SelectedRegion;

        ImVec2i GetCellSize() { return _cellSize; }

        SpriteAtlas& GetSpriteAtlas() { return *_spriteAtlas; }

        Texture2D* GetTexture() { return _texture; }
        void LoadSpriteAtlas(const std::string& spriteAtlasPath);
        void CreateNewSpriteAtlas(SpriteAtlasData& spriteAtlasData);

        void Slice();
        void Save();

        void SetCellSize(const ImVec2i& cellSize);
        void RemoveSprite(int32_t spriteIndex);
        void ClearSprites();

    private:
        UndoRedoHistory& _undoRedoHistory;

        SpriteAtlas* _spriteAtlas = new SpriteAtlas();

        Texture2D* _texture = nullptr;

        ImVec2i _cellSize = ImVec2i(32, 32);

        void CalculateGridSize();

        void SliceGridSequence();
        void SliceGridSelection();
        void SliceFreehand();
    };

    int32_t GetCellFromPosition(ImVec2 position, ImVec2i cellSize, int32_t gridWidth);
    ImVec2 GetPositonFromCell(int32_t cell, ImVec2i cellSize, int32_t gridWidth);
}
