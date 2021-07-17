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
    };
}