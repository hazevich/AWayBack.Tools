#pragma once
#include "SpriteEditorController.h"

namespace AWayBack
{
    struct ToolbarWindow
    {
        ToolbarWindow(SpriteEditorController& controller, UndoRedoHistory& undoRedoHistory)
            : _controller(controller), _undoRedoHistory(undoRedoHistory)
        {
        }

        void Render();

    private:
        SpriteEditorController& _controller;
        UndoRedoHistory& _undoRedoHistory;
        bool _isUniformCellSizeControl = true;
    };
}
