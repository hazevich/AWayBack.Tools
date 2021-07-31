#pragma once
#include "SpriteAtlasEditorController.h"

namespace AWayBack
{
    struct ToolbarWindow
    {
        ToolbarWindow(SpriteAtlasEditorController& controller, UndoRedoHistory& undoRedoHistory)
            : _controller(controller), _undoRedoHistory(undoRedoHistory)
        {
        }

        void Render();

    private:
        SpriteAtlasEditorController& _controller;
        UndoRedoHistory& _undoRedoHistory;
        bool _isUniformCellSizeControl = true;
    };
}
