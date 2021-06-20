#pragma once

#include "Application.h"
#include "OpenSpriteAtlasModal.h"
#include "AWayBack/Graphics/Texture.h"
#include "SpriteEditor.h"

namespace AWayBack
{
    class ToolsApplication : public Application
    {
    protected:
        ~ToolsApplication() override;

        void Render() override;
        void Initialize() override;

    private:
        Texture2D* _texture = nullptr;
        Texture2D* _newTexture = nullptr;
        OpenSpriteAtlasModal _openSpriteAtlasModal{};
        SpriteEditor _spriteEditor{};

        void UpdateTextures();
        void RenderDockSpace();
        void RenderMainMenuBar();
        void RenderSpriteEditor();
        void RenderOpenSpriteAtlasPopup();        
    };
}