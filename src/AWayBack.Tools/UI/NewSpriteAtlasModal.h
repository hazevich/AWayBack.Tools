#pragma once

#include "SpriteEditorController.h"

namespace AWayBack
{
    class NewSpriteAtlasModal
    {
    public:
        NewSpriteAtlasModal(SpriteEditorController& controller)
            : _controller(controller)
        {
            
        }

        void Open();
        void Render();

    private:
        SpriteEditorController& _controller;

        SpriteAtlasData _spriteAtlasData;

        bool _isOpenRequested;
        bool _isOpen;
    };
}