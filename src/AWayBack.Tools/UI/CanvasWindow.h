#pragma once

#include "SpriteEditorController.h"

namespace AWayBack
{
    struct CanvasWindow
    {
        CanvasWindow(SpriteEditorController& controller)
            : _controller(controller)
        {
            
        }

        void Render();

    private:
        SpriteEditorController& _controller;
    };
}
