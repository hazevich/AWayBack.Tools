#pragma once

#include "SpriteAtlasEditorController.h"

namespace AWayBack
{
    struct CanvasWindow
    {
        CanvasWindow(SpriteAtlasEditorController& controller)
            : _controller(controller)
        {
            
        }

        void Render();

    private:
        SpriteAtlasEditorController& _controller;
    };
}
