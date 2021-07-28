#pragma once

#include "SpriteEditorController.h"

namespace AWayBack
{
    enum OriginPlacement
    {
        Center = 0,
        TopLeft = 1,
        TopCenter = 2,
        TopRight = 3,
        CenterRight = 4,
        BottomRight = 5,
        BottomCenter = 6,
        BottomLeft = 7,
        LeftCenter = 8,
        Custom = 9
    };

    class SelectedSpriteWindow
    {
    public:
        SelectedSpriteWindow(SpriteEditorController& controller);

        void Render();

    private:
        SpriteEditorController& _controller;

        bool _isGridVisible;
        bool _isUniformCellSizeControl = true;

        ImVec2i _cellSize = ImVec2i(32, 32);

        OriginPlacement _originPlacement = Custom;

        static const int32_t SpriteNameMaxSize = 1024;
        char _nameBuffer[SpriteNameMaxSize] = { 0 };
        bool _isEditingName = false;

        std::optional<int32_t> _selectedSpriteId;

        void RenderControls();
    };
}