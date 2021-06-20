#pragma once

#include "Application.h"
#include "OpenSpriteAtlasModal.h"
#include "AWayBack/Graphics/Texture.h";

namespace AWayBack
{
    class ToolsApplication : public Application
    {
    protected:
        ~ToolsApplication() override;

        void Render() override;
        void Initialize() override;

    private:
        void UpdateTextures();
        void RenderDockSpace();
        void RenderMainMenuBar();
        void RenderCanvas();
        void RenderOpenSpriteAtlasPopup();

        Texture2D* _texture = nullptr;
        Texture2D* _newTexture = nullptr;
        OpenSpriteAtlasModal _openSpriteAtlasModal{};
    };
}