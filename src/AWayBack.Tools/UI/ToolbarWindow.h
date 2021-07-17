#pragma once
#include "SpriteEditorController.h"

namespace AWayBack
{
    struct ToolbarWindow
    {
        ToolbarWindow(SpriteEditorController& controller)
            : _controller(controller)
        {
        }

        void Render();

    private:
        SpriteEditorController& _controller;
        bool _isUniformCellSizeControl = true;
    };
}
