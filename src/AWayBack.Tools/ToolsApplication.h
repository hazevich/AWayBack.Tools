#pragma once

#include "Application.h"
#include "OpenSpriteAtlasModal.h"
#include "SpriteEditor.h"

namespace AWayBack
{
    class ToolsApplication : public Application
    {
    protected:
        void Render() override;
        void Initialize() override;

    private:
        OpenSpriteAtlasModal _openSpriteAtlasModal{};
        SpriteEditor _spriteEditor{};

        void RenderDockSpace();
        void RenderMainMenuBar();
        void RenderSpriteEditor();
        void RenderOpenSpriteAtlasPopup();        
    };
}