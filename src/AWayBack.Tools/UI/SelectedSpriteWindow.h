#pragma once

#include "SpriteAtlasEditorController.h"

namespace AWayBack
{
    class SelectedSpriteWindow
    {
    public:
        SelectedSpriteWindow(SpriteAtlasEditorController& controller);

        void Render();

    private:
        SpriteAtlasEditorController& _controller;

        bool _isGridVisible;
        bool _isUniformCellSizeControl = true;

        ImVec2i _cellSize = ImVec2i(32, 32);

        std::optional<int32_t> _selectedSpriteId;

        void RenderProperties();
    };
}