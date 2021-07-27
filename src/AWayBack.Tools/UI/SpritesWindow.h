#pragma once
#include "SpriteEditorController.h"
#include "UndoRedo.h"

namespace AWayBack
{
    struct SpritesWindow
    {
        SpritesWindow(SpriteEditorController& controller)
            : _controller(controller)
        {
            
        }

        void Render();

    private:
        SpriteEditorController& _controller;
    };
}
