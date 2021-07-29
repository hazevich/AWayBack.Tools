#pragma once

#include "SpriteEditorController.h"

namespace AWayBack
{
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
        
        Vector2 _originBuffer;
        bool _isEditingOrigin = false;

        std::optional<int32_t> _selectedSpriteId;

        void RenderProperties();
    };
}