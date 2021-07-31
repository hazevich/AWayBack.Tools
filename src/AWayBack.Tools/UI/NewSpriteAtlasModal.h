#pragma once

#include "SpriteAtlasEditorController.h"

namespace AWayBack
{
    class NewSpriteAtlasModal
    {
    public:
        NewSpriteAtlasModal(SpriteAtlasEditorController& controller)
            : _controller(controller)
        {
            
        }

        void Open();
        void Render();

    private:
        SpriteAtlasEditorController& _controller;

        SpriteAtlasData _spriteAtlasData;

        bool _isOpenRequested;
        bool _isOpen;
    };
}