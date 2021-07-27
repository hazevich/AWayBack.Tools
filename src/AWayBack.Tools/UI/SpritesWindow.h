#pragma once
#include "SpriteEditorController.h"
#include "UndoRedo.h"

namespace AWayBack
{
    struct SpritesWindow
    {
        SpritesWindow(SpriteEditorController& controller, UndoRedoHistory& undoRedoHistory)
            : _controller(controller), _undoRedoHistory(undoRedoHistory)
        {
            
        }

        void Render();

    private:
        SpriteEditorController& _controller;
        UndoRedoHistory& _undoRedoHistory;
    };
}
