#pragma once
#include "SpriteAtlasEditorController.h"
#include "UndoRedo.h"

namespace AWayBack
{
    struct SpritesWindow
    {
        SpritesWindow(SpriteAtlasEditorController& controller)
            : _controller(controller)
        {
            
        }

        void Render();

    private:
        SpriteAtlasEditorController& _controller;
    };
}
